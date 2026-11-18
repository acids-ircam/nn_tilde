#!/bin/bash
# Build the Max externals for Apple Silicon (arm64), as in CI.
#
#   install/macos_build.sh
#
# Uses its own build tree, arm64_build/build, with the official arm64 libtorch
# in arm64_build/torch (add_torch.cmake looks in <build dir>/../torch and
# downloads it if missing). Results go to src/externals and src/support.
#
# Requirements:
#   Homebrew LLVM (`brew install llvm`), as in CI: Apple clang's libc++ refuses
#   std::filesystem below macOS 10.15
#   conda env with python, tqdm, curl, linked as ./env  (ENV_DIR)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ENV_DIR="${ENV_DIR:-$HOME/miniconda3/envs/nn_tilde}"
SIGN_ID="${SIGN_ID:--}"
TARGETS="nn_tilde mc.nn_tilde mcs.nn_tilde nn.info"
TORCH_VERSION="${TORCH_VERSION:-2.8.0}"
WORK="$ROOT/arm64_build"   # matches *build in .gitignore

# arm64 Homebrew first: a non-interactive shell may find the Intel one (/usr/local) first in PATH
BREW="$(command -v /opt/homebrew/bin/brew || command -v brew || true)"
LLVM="${LLVM_PREFIX:-$([ -n "$BREW" ] && "$BREW" --prefix llvm 2>/dev/null || true)}"
[ -x "$LLVM/bin/clang++" ] || { echo "[error] Homebrew LLVM not found (brew install llvm)"; exit 1; }
[ -f "$ENV_DIR/lib/libcurl.dylib" ] || { echo "[error] no libcurl in $ENV_DIR"; exit 1; }
ln -sfn "$ENV_DIR" "$ROOT/env"

# reuse the official arm64 libtorch if already downloaded at the right version, else CMake downloads it
torch_version_of() { cat "$1/libtorch/build-version" 2>/dev/null | head -1 | sed 's/+.*//'; }
if [ -d "$WORK/torch/libtorch" ] && [ "$(torch_version_of "$WORK/torch")" != "$TORCH_VERSION" ]; then
  echo "removing libtorch $(torch_version_of "$WORK/torch") from $WORK/torch (expected $TORCH_VERSION)"
  rm -rf "$WORK/torch"
fi
if [ ! -d "$WORK/torch/libtorch" ] && [ "$(torch_version_of "$ROOT/torch")" = "$TORCH_VERSION" ]; then
  mkdir -p "$WORK/torch"
  cp -cR "$ROOT/torch/libtorch" "$WORK/torch/"
fi

# build (post-build steps share src/support: keep make serial)
rm -rf "$ROOT/src/externals" "$ROOT/src/support"
mkdir -p "$WORK/build"
cd "$WORK/build"
cmake "$ROOT/src" -DCMAKE_C_COMPILER="$LLVM/bin/clang" -DCMAKE_CXX_COMPILER="$LLVM/bin/clang++" \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 -DNO_PUREDATA=1 -Dtorch_version="$TORCH_VERSION" -DSIGN_ID="$SIGN_ID" > cmake.log
make -j1 $TARGETS > make.log 2>&1 || { echo "[error] build failed, see $WORK/build/make.log"; exit 1; }

echo "built:"
for mxo in "$ROOT/src/externals/"*.mxo; do
  bin="$mxo/Contents/MacOS/$(basename "$mxo" .mxo)"
  echo "  $(basename "$mxo"): $(lipo -archs "$bin")"
done
echo "  libtorch_cpu: $(lipo -archs "$ROOT/src/support/libtorch_cpu.dylib") (torch $(torch_version_of "$WORK/torch"))"
