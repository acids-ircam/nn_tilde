# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

`nn_tilde` is a translation layer between Max/MSP and PureData and the libtorch C++ interface. It loads TorchScript (`.ts`) models for real-time audio ML inference. The repo ships both the C++ external (the DSP object) and a Python package (`python_tools`, published as `nn_tilde` on PyPI) used to export PyTorch models into the `.ts` format the external expects.

## Repository layout

- `src/backend/` — C++ backend wrapping a `torch::jit::script::Module` (see `backend.h`). Handles model load, inference, method dispatch, and attribute get/set. One shared library consumed by all frontends.
- `src/frontend/maxmsp/` — Max/MSP externals built with Cycling74's `min-api` (git submodule): `nn_tilde` (basic), `mc.nn_tilde` (multichannel over batch), `mcs.nn_tilde` (multichannel over dims), `nn.info` (model inspection).
- `src/frontend/puredata/nn_tilde/` — Pd external. Uses `m_pd.h` which CMake downloads at configure time unless `PUREDATA_INCLUDE_DIR` is set.
- `src/shared/` — headers shared by backend + frontends: `circular_buffer.h`, `static_buffer.h`, `model_download.h` (IRCAM Forum API model downloads via libcurl).
- `src/json/` — git submodule (nlohmann/json), linked by Max externals.
- `python_tools/` — Python API for exporting models. `Module` (subclasses `torch.nn.Module`) exposes `register_method(name, in_channels, in_ratio, out_channels, out_ratio, ...)`, `register_attribute(...)`, `register_buffer_attribute(...)`, and `export_to_ts(path)`. Methods must accept and return a single 3D tensor `[batch, channels, time]`.
- `python_tools/test/` — pytest tests that build and script `Module` subclasses (used to verify the exporter round-trip).
- `scripting/` — example export scripts (`effects.py`, `features.py`, `unmix.py`) showing common input/output shape patterns.
- `install/` — macOS/Windows post-build helpers (`dylib_fix.py` rewrites install names + codesigns on macOS; `macos_*_makeub.sh` make universal binaries; `patch_with_vst.sh`).
- `.github/workflows/build.yaml` — CI for releases; `python-publish.yaml` — PyPI publishing for `python_tools`.

## Build

Both Max and Pd externals build from a single top-level CMake project in `src/`. Submodules must be checked out: `git submodule update --init --recursive` (or clone `--recurse-submodules`).

macOS build (mirrors the README; creates a local conda env + libtorch from the `torch` pip wheel):

```bash
bash miniconda.sh -b -u -p ./env && source ./env/bin/activate
pip install -r requirements.txt
conda install -c conda-forge curl
mkdir -p build/puredata_include
curl -L https://raw.githubusercontent.com/pure-data/pure-data/master/src/m_pd.h -o build/puredata_include/m_pd.h
export CC=$(brew --prefix llvm)/bin/clang CXX=$(brew --prefix llvm)/bin/clang++
cd build && cmake ../src \
  -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
  -DCMAKE_PREFIX_PATH=../env/lib/python3.12/site-packages/torch \
  -DCMAKE_BUILD_TYPE=Release \
  -DPUREDATA_INCLUDE_DIR=../puredata_include \
  -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build . --config Release
```

CMake flags worth knowing:
- `-DNO_PUREDATA=1` skips the Pd frontend (fatal on non-Apple Linux; Max-only on macOS/Windows).
- `-DPUREDATA_INCLUDE_DIR=...` / `-DPUREDATA_BIN_DIR=...` (Windows needs both; link against `pd.lib`).
- `-DCMAKE_OSX_ARCHITECTURES=arm64|x86_64` — single-arch build; universal binaries are assembled by `install/macos_*_makeub.sh`.
- `-DSIGN_ID=<identity>` (or env var) — codesign identity used by the macOS post-build step; defaults to ad-hoc `-`.

Build outputs:
- Max package: `src/` itself (the repo becomes the Max package; `src/externals/` holds the bundles).
- Pd external: `build/frontend/puredata/Release/`.

On macOS, every Max external has a post-build `dylib_fix.py` step that rewrites `@rpath`/`@loader_path`, copies dylibs into `src/support/`, drops the SSL cert, and codesigns. If dylibs or cert.pem are missing from `support/` after a build, that script failed — check its output before chasing linker errors.

Windows requires `vcpkg` with curl installed and expects `libtorch` unzipped next to the repo; pass `-DTorch_DIR=../libtorch/share/cmake/Torch`.

## Testing

Python exporter tests use pytest:

```bash
cd python_tools/test && pytest
# single test
pytest test_attributes.py::test_attribute_foo
```

The C++ side has no unit-test suite. `src/frontend/maxmsp/nn_tilde/nn_tilde_test.cpp` exists but is not wired into CMake; manual verification is done by loading `.ts` models from the `scripting/` examples in the help patches (`src/help/`, `src/frontend/*/nn~.maxhelp` / `nn~-help.pd`).

## Model-export contract (python_tools → backend)

When editing `python_tools/module.py` or the backend's method/attribute parsing, both sides must agree:

- A registered method signature is `(self, x: Tensor) -> Tensor` with `x` shaped `[batch, in_channels, time]` and output `[batch, out_channels, time // (out_ratio / in_ratio) or similar]`. `register_method` runs a shape-check on export (unless `test_method=False`) and also exercises a batch-of-4 pass when `cached_conv.MAX_BATCH_SIZE > 1` — this is what validates `mc.nn~` compatibility.
- Each registered method emits a buffer `f"{name}_params"` holding `[in_channels, in_ratio, out_channels, out_ratio]`, plus `{name}_input_labels` / `{name}_output_labels`. The backend (`get_method_params`, `get_model_info`) reads these names directly — renaming on one side without the other breaks model loading silently.
- Attribute types are enumerated in two places that must stay in sync: `python_tools/__init__.py` `TYPE_HASH` (`bool=0, int=1, float=2, str=3, Tensor=4, Buffer=5`) and `backend.h` `Backend::id_to_string_hash`.
- `Buffer` attributes (Max `buffer~` / Pd arrays) are tracked via `_buffer_attributes` entries formatted `f"{attr_name}#{index}"`. The backend splits on `#` to route writes — don't change the separator.
- Reserved attribute names: `sr` (sample rate). `enable` is a backend-level special message, not a user attribute.

## Conventions

- C++ is C++20 with libtorch linked against the conda env's pip-installed torch (not a separate `libtorch` download on macOS). `CMAKE_PREFIX_PATH` points at `env/lib/pythonX.Y/site-packages/torch`.
- `python_tools` targets Python ≥ 3.11 and pins `torch==2.5` + `cached_conv>=2.5.0`. `cached_conv` is load-bearing (not optional): it controls batch-size behavior used by the method tester.
- The circular-buffer threading path is disabled on Windows because of pytorch/pytorch#24237 (memory leak when a TorchScript model is used on a separate thread). Don't re-enable it without verifying that upstream issue.
- Version is derived from `git describe --tags` at CMake configure time (baked into the Max external via `-DVERSION=...`), and from `NN_TILDE_VERSION` env var for the Python package (`setup.py`).
