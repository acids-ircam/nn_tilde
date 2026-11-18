# Installation

- [Requirements](#requirements)
- [Max/MSP](#maxmsp)
- [PureData](#puredata)
- [Getting models](#getting-models)
- [Python tools (for exporting models)](#python-tools-for-exporting-models)
- [Building from source](#building-from-source)

## Requirements

| Platform | Max/MSP | PureData |
|---|---|---|
| macOS, Apple Silicon (arm64) | Max 8 / Max 9 | yes |
| Windows, x64 | Max 8 / Max 9 | yes |
| Linux, x86_64 | | yes |

Intel Macs are not supported by current releases.

Multichannel features (`mc.nn~`, `mcs.nn~`, and the `-m` flag in Pd) need Max 8+ and Pd 0.54+.

## Max/MSP

1. Download `nn_max_msp_macOS_arm64.tar.gz` (Mac) or `nn_max_msp_windows_x64.tar.gz` (Windows) from the [latest release](https://github.com/acids-ircam/nn_tilde/releases/latest).
2. Unpack it into the Packages folder of your Max installation: `Documents/Max 9/Packages/` (or `Max 8`). You should get `Packages/nn_tilde/` with `externals/`, `support/`, `help/`, `models/`...
3. Restart Max. The console prints `nn~ <version> - torch <version>` when the external is first loaded.
4. Alt-click an `nn~` object to open its help patch, or open the *nn~ Overview* from the Extras menu.

Keep only **one** `nn_tilde` package in your Packages folders: with two versions installed, Max may load either.

### macOS: quarantine

Releases are code-signed and should open without warning. If macOS blocks them anyway, run in a terminal:

```bash
cd ~/Documents/Max\ 9/Packages/nn_tilde
xattr -r -d com.apple.quarantine .
codesign --force --sign - support/*.dylib
codesign --force --deep --sign - externals/*.mxo
```

### Windows

Install the [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/cpp/windows/latest-supported-vc-redist) if Max reports that the external cannot be loaded.

On Windows, `nn~` computes in the audio thread instead of a separate thread, because of a [memory leak in libtorch](https://github.com/pytorch/pytorch/issues/24237) when a TorchScript model runs in another thread. This needs more CPU headroom. See [Threads](description.md#threads-and-latency).

## PureData

1. Download the Pd archive for your platform (`nn_puredata_macOS_arm64.tar.gz`, `nn_puredata_windows_x64.tar.gz` or `nn_linux_puredata_x86_64.tar.gz`) from the [latest release](https://github.com/acids-ircam/nn_tilde/releases/latest).
2. Unpack it into your externals folder, e.g. `Documents/Pd/externals/`.
3. In Pd, add the `nn_tilde` folder in *File > Preferences > Path*.

If macOS blocks the external:

```bash
cd ~/Documents/Pd/externals/nn_tilde
xattr -r -d com.apple.quarantine .
codesign --force --sign - *.dylib nn~.pd_darwin
```

## Getting models

`nn~` needs **pretrained models**: TorchScript files with a `.ts` extension. There are several ways to get them:

- **Download from `nn~`.** Send `print_available_models` to an `nn~` object to list the models of the IRCAM Forum API, then `download <model>` (optionally `download <model> <name>` to rename it). Downloaded models go to the package's `models/` folder and can be removed with `delete <model>` (`remove <model>` in Pd). In Max, [`nn.info`](reference.md#nninfo) shows the same list as a dictionary.
- **Download from the web**: [RAVE models](https://acids-ircam.github.io/rave_models_download), [Intelligent Instruments Lab models](https://huggingface.co/Intelligent-Instruments-Lab/rave-models), [vschaos2 models](https://www.dropbox.com/sh/avdeiza7c6bn2of/AAAGZsnRo9ZVMa0iFhouCBL-a?dl=0).
- **Export your own** with the Python tools: see [Wrapping models](wrapping.md).

Put `.ts` files in `nn_tilde/models/`, or anywhere in the Max search path (*Options > File Preferences*) or the Pd path. Then refer to a model by its name, with or without `.ts`: `[nn~ isis forward]`.

## Python tools (for exporting models)

The `nn_tilde` Python package exports PyTorch models to `nn~`-compatible TorchScript files. It requires Python 3.11+.

```bash
pip install nn_tilde
```

Use the same PyTorch version as the libtorch shipped with `nn~` (printed in the Max console at startup), or an older one. A model exported with a newer PyTorch may not load.

## Building from source

### macOS (Apple Silicon)

Requirements: Xcode command line tools, [Homebrew](https://brew.sh) LLVM (`brew install llvm`), CMake, and conda.

```bash
git clone https://github.com/acids-ircam/nn_tilde --recurse-submodules
cd nn_tilde

# python + curl environment, linked as ./env (used for linking and packaging)
conda create -n nn_tilde -c conda-forge python=3.11 curl tqdm
ln -s "$(conda info --base)/envs/nn_tilde" env

./install/macos_build.sh
```

`install/macos_build.sh` downloads the official arm64 libtorch if needed, builds `nn~`, `mc.nn~`, `mcs.nn~` and `nn.info`, and copies the externals and their libraries into `src/externals` and `src/support`. `src/` is then a complete Max package: link it into your Packages folder to test it.

```bash
ln -s "$PWD/src" ~/Documents/Max\ 9/Packages/nn_tilde
```

To build by hand, or to build the Pd external too:

```bash
mkdir build && cd build
cmake ../src -DCMAKE_C_COMPILER=$(brew --prefix llvm)/bin/clang \
             -DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++ \
             -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
make -j1
```

Use `make -j1`: after linking, each external copies its libraries into the shared `src/support` folder, and parallel jobs would overwrite each other's files. Add `-DNO_PUREDATA=1` to skip the Pd external. The Pd external is produced in `build/frontend/puredata/nn_tilde`.

### Windows

Requirements: Visual Studio 2022, CMake, Git Bash.

```bash
git clone https://github.com/acids-ircam/nn_tilde --recurse-submodules
cd nn_tilde
# Pd headers and binaries (only for the Pd external)
mkdir pd && cd pd
curl -L https://msp.ucsd.edu/Software/pd-0.55-2.msw.zip -o pd.zip
unzip pd.zip && mv pd*/src . && mv pd*/bin . && cd ..

# curl
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat
./vcpkg/vcpkg.exe integrate install
./vcpkg/vcpkg.exe install curl

mkdir build && cd build
cmake ../src -G "Visual Studio 17 2022" -A x64 \
      -DPUREDATA_INCLUDE_DIR=../pd/src -DPUREDATA_BIN_DIR=../pd/bin
cmake --build . --config Release
```

CMake downloads libtorch (CPU) into `torch/` if it is not there yet. The Max package is produced in `src/`, the Pd external in `build/frontend/puredata/nn_tilde/Release`. Copy `torch/libtorch/lib/*` and the vcpkg DLLs (`vcpkg/installed/x64-windows/bin/*.dll`) into the package's `support/` folder.

### Linux (PureData)

```bash
git clone https://github.com/acids-ircam/nn_tilde --recurse-submodules
cd nn_tilde
mkdir build && cd build
cmake ../src -DCMAKE_BUILD_TYPE=Release
make
```

CMake downloads libtorch (CPU) into `torch/` if it is not there yet. The external is produced in `build/frontend/puredata/nn_tilde`.
