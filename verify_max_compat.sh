#!/usr/bin/env bash
# Verify that the externals built in src/externals/ are loadable by the
# installed Max.app on this machine. Runs static checks only — does not
# require copying anything into Documents/Max ../Packages/.
#
# Exits 0 if every check passes, 1 otherwise.

set -uo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXTERNALS_DIR="$REPO_ROOT/src/externals"
SUPPORT_DIR="$REPO_ROOT/src/support"
MAX_APP="${MAX_APP:-/Applications/Max.app}"

FAILS=0
ok()   { printf '  \033[1;32mOK\033[0m   %s\n' "$*"; }
fail() { printf '  \033[1;31mFAIL\033[0m %s\n' "$*"; FAILS=$((FAILS+1)); }
warn() { printf '  \033[1;33mWARN\033[0m %s\n' "$*"; }
hdr()  { printf '\n\033[1;34m== %s ==\033[0m\n' "$*"; }

# -----------------------------------------------------------------------------
hdr "Max.app"
# -----------------------------------------------------------------------------
if [[ ! -x "$MAX_APP/Contents/MacOS/Max" ]]; then
  fail "Max not found at $MAX_APP (override with MAX_APP=...)"
  exit 1
fi
MAX_BIN="$MAX_APP/Contents/MacOS/Max"
MAX_VER="$(defaults read "$MAX_APP/Contents/Info" CFBundleShortVersionString 2>/dev/null || echo unknown)"
MAX_ARCHS="$(lipo -info "$MAX_BIN" 2>/dev/null | sed 's/.*: //')"
ok "Max version $MAX_VER  ($MAX_BIN)"
ok "Max architectures: $MAX_ARCHS"

HOST_ARCH="$(uname -m)"
if [[ " $MAX_ARCHS " != *" $HOST_ARCH "* ]]; then
  warn "Max has no $HOST_ARCH slice — Max would launch under Rosetta"
fi

# -----------------------------------------------------------------------------
hdr "Externals in $EXTERNALS_DIR"
# -----------------------------------------------------------------------------
shopt -s nullglob
EXTERNALS=( "$EXTERNALS_DIR"/*.mxo )
if [[ ${#EXTERNALS[@]} -eq 0 ]]; then
  fail "No .mxo bundles found — run ./build_macos.sh first"
  exit 1
fi
ok "Found ${#EXTERNALS[@]} external(s): $(basename -a "${EXTERNALS[@]}" | tr '\n' ' ')"

for mxo in "${EXTERNALS[@]}"; do
  name="$(basename "$mxo")"
  binary="$mxo/Contents/MacOS/${name%.mxo}"
  hdr "$name"

  if [[ ! -x "$binary" ]]; then
    fail "binary missing: $binary"; continue
  fi

  # Architecture
  ext_archs="$(lipo -info "$binary" 2>/dev/null | sed 's/.*: //')"
  if [[ " $MAX_ARCHS " == *" $HOST_ARCH "* && " $ext_archs " == *" $HOST_ARCH "* ]]; then
    ok "arch: $ext_archs (matches Max's native $HOST_ARCH slice)"
  elif [[ " $MAX_ARCHS " == *" $ext_archs "* ]]; then
    ok "arch: $ext_archs (Max can run this slice)"
  else
    fail "arch mismatch: external=[$ext_archs] vs Max=[$MAX_ARCHS]"
  fi

  # Min macOS
  cur_macos="$(sw_vers -productVersion)"
  ext_minos="$(otool -l "$binary" 2>/dev/null | awk '/LC_BUILD_VERSION|LC_VERSION_MIN_MACOSX/{f=1; next} f && /minos|version/{print $2; f=0}' | head -1)"
  if [[ -n "$ext_minos" ]] \
     && [[ "$(printf '%s\n%s\n' "$ext_minos" "$cur_macos" | sort -V | head -1)" == "$ext_minos" ]]; then
    ok "min macOS $ext_minos ≤ host $cur_macos"
  else
    fail "min macOS $ext_minos > host $cur_macos"
  fi

  # Max framework refs (paths starting with @executable_path/../Frameworks/)
  while IFS= read -r fwk; do
    rel="${fwk#@executable_path/../}"
    abs="$MAX_APP/Contents/$rel"
    if [[ -f "$abs" ]]; then
      ok "framework ref resolved: $rel"
    else
      fail "framework ref MISSING: $rel  (looked in $abs)"
    fi
  done < <(otool -L "$binary" 2>/dev/null | awk '/^\t@executable_path/ {print $1}')

  # Support dylib refs (paths via @loader_path/../../../../support/)
  while IFS= read -r dep; do
    relpath="${dep#@loader_path/../../../../support/}"
    abs="$SUPPORT_DIR/$relpath"
    if [[ -f "$abs" ]]; then
      lib_archs="$(lipo -info "$abs" 2>/dev/null | sed 's/.*: //')"
      if [[ " $lib_archs " == *" $HOST_ARCH "* ]]; then
        ok "support dylib: $relpath  [$lib_archs]"
      else
        fail "support dylib $relpath has wrong arch: $lib_archs (need $HOST_ARCH)"
      fi
    else
      fail "support dylib MISSING: $relpath  (expected at $abs)"
    fi
  done < <(otool -L "$binary" 2>/dev/null | awk '/^\t@loader_path/ {print $1}')

  # Codesign
  if codesign --verify --deep --strict "$mxo" >/dev/null 2>&1; then
    sig="$(codesign -dv "$mxo" 2>&1 | awk -F= '/^Signature/{print $2}')"
    ok "codesign valid (${sig:-ad-hoc})"
  else
    fail "codesign verification failed"
  fi
done

# -----------------------------------------------------------------------------
hdr "Summary"
# -----------------------------------------------------------------------------
if [[ $FAILS -eq 0 ]]; then
  printf '  \033[1;32mAll checks passed.\033[0m  Externals are compatible with Max %s on %s.\n' "$MAX_VER" "$HOST_ARCH"
  printf '  Note: an undefined symbol like _arrayobj_findregistered_retain in `nm -u`\n'
  printf '  is normal — Max provides those at load time (see min-api/.../max-linker-flags.txt).\n'
  exit 0
else
  printf '  \033[1;31m%d check(s) failed.\033[0m\n' "$FAILS"
  exit 1
fi
