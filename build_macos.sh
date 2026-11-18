#!/usr/bin/env bash
# Build nn~ for Max/MSP on macOS arm64 (Pd frontend disabled).
#
# Usage: ./build_macos.sh
# Re-running is safe: existing miniconda env is reused.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$REPO_ROOT"

ENV_DIR="$REPO_ROOT/env"
BUILD_DIR="$REPO_ROOT/build"
# Miniforge instead of Anaconda's miniconda: defaults to conda-forge, so we
# avoid the ToS gate Anaconda added to its main/r channels in 2025.
CONDA_INSTALLER_URL="https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-arm64.sh"

log()  { printf '\033[1;34m==>\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33m!!\033[0m  %s\n' "$*" >&2; }
die()  { printf '\033[1;31mxx\033[0m  %s\n' "$*" >&2; exit 1; }

# -----------------------------------------------------------------------------
# 1. Prerequisite checks
# -----------------------------------------------------------------------------
log "Checking prerequisites"

[[ "$(uname -s)" == "Darwin" ]] || die "This script is macOS-only (got $(uname -s))."
[[ "$(uname -m)" == "arm64"  ]] || die "This script targets arm64 (got $(uname -m))."

xcode-select -p >/dev/null 2>&1 \
  || die "Xcode command-line tools not found. Install with: xcode-select --install"

command -v brew >/dev/null 2>&1 \
  || die "Homebrew not found. Install from https://brew.sh/"

command -v cmake >/dev/null 2>&1 \
  || die "cmake not found. Install with: brew install cmake"

command -v curl >/dev/null 2>&1 \
  || die "curl not found (unexpected on macOS)."

if ! brew --prefix llvm >/dev/null 2>&1 || [[ ! -x "$(brew --prefix llvm)/bin/clang" ]]; then
  die "Homebrew llvm not installed. Install with: brew install llvm"
fi
LLVM_PREFIX="$(brew --prefix llvm)"

if [[ ! -f "$REPO_ROOT/src/frontend/maxmsp/min-api/script/min-pretarget.cmake" \
   || ! -f "$REPO_ROOT/src/json/CMakeLists.txt" ]]; then
  die "Git submodules missing. Run: git submodule update --init --recursive"
fi

log "Prerequisites OK"

# -----------------------------------------------------------------------------
# 2. Local miniconda environment
# -----------------------------------------------------------------------------
if [[ ! -x "$ENV_DIR/bin/python" ]]; then
  log "Installing Miniforge into $ENV_DIR"
  TMP_SH="$(mktemp -t miniforge.XXXXXX).sh"
  curl -fsSL "$CONDA_INSTALLER_URL" -o "$TMP_SH"
  bash "$TMP_SH" -b -u -p "$ENV_DIR"
  rm -f "$TMP_SH"
else
  log "Reusing existing conda env at $ENV_DIR"
fi

# shellcheck disable=SC1091
source "$ENV_DIR/bin/activate"

# torch==2.5 (pinned in requirements.txt) has no Python 3.13 wheel, but the
# "latest" miniconda installer ships 3.13. Pin the env's Python to 3.12.
PY_VERSION="$(python -c 'import sys; print(f"{sys.version_info.major}.{sys.version_info.minor}")')"
if [[ "$PY_VERSION" != "3.12" ]]; then
  log "Pinning env Python to 3.12 (was $PY_VERSION; torch==2.5 has no 3.13 wheels)"
  conda install -y -c conda-forge --override-channels python=3.12
fi

log "Installing Python requirements (torch, cached_conv)"
pip install --quiet --disable-pip-version-check -r requirements.txt

if [[ ! -x "$ENV_DIR/bin/curl" ]]; then
  log "Installing conda-forge curl (used at link time by the Max external)"
  conda install -y -c conda-forge --override-channels curl
else
  log "conda-forge curl already present"
fi

# Detect the site-packages dir that actually got torch (python version varies
# between miniconda releases, so don't hard-code 3.12).
TORCH_CMAKE_DIR="$(python -c 'import torch, os; print(os.path.dirname(torch.__file__))')"
[[ -d "$TORCH_CMAKE_DIR/share/cmake/Torch" ]] \
  || die "libtorch CMake config not found under $TORCH_CMAKE_DIR"
log "Using libtorch at $TORCH_CMAKE_DIR"

# -----------------------------------------------------------------------------
# 3. Patch hard-coded deployment target
# -----------------------------------------------------------------------------
# CMakeLists.txt files pin CMAKE_OSX_DEPLOYMENT_TARGET to 10.12 (2016). Homebrew
# libc++ 22+ marks std::to_chars(double) as unavailable below macOS 13.3, which
# breaks compilation of <format>. Bump to 13.3 in place. Idempotent.
DEPLOYMENT_TARGET_FILES=(
  "$REPO_ROOT/src/CMakeLists.txt"
  "$REPO_ROOT/src/frontend/maxmsp/nn.info/CMakeLists.txt"
  "$REPO_ROOT/src/frontend/maxmsp/nn_tilde/CMakeLists.txt"
  "$REPO_ROOT/src/frontend/maxmsp/mc.nn_tilde/CMakeLists.txt"
  "$REPO_ROOT/src/frontend/maxmsp/mcs.nn_tilde/CMakeLists.txt"
)
PATCHED=0
for f in "${DEPLOYMENT_TARGET_FILES[@]}"; do
  if grep -q 'CMAKE_OSX_DEPLOYMENT_TARGET "10.12"' "$f"; then
    sed -i.bak 's/CMAKE_OSX_DEPLOYMENT_TARGET "10\.12"/CMAKE_OSX_DEPLOYMENT_TARGET "13.3"/' "$f"
    PATCHED=$((PATCHED+1))
  fi
done
if [[ $PATCHED -gt 0 ]]; then
  log "Bumped deployment target 10.12 → 13.3 in $PATCHED CMakeLists.txt file(s) (.bak backups left)"
fi

# -----------------------------------------------------------------------------
# 4. Configure + build (Max only, Pd disabled via NO_PUREDATA=1)
# -----------------------------------------------------------------------------
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

log "Configuring CMake (Max only, arm64)"
cmake "$REPO_ROOT/src" \
  -DCMAKE_C_COMPILER="$LLVM_PREFIX/bin/clang" \
  -DCMAKE_CXX_COMPILER="$LLVM_PREFIX/bin/clang++" \
  -DCMAKE_PREFIX_PATH="$TORCH_CMAKE_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DNO_PUREDATA=1

log "Building (serial: post-build dylib_fix.py races on shared src/support/ dir)"
cmake --build . --config Release

log "Done. Max package is the repo root ($REPO_ROOT); externals are under src/externals/."
