#!/usr/bin/env bash
# ══════════════════════════════════════════════════════════════
#  Gidit — Build Script (Linux / macOS)
#  Usage: ./scripts/build.sh [debug|release|clean|install|package]
# ══════════════════════════════════════════════════════════════
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"
BUILD_TYPE="${1:-release}"

RED='\033[0;31m'; GREEN='\033[0;32m'; CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'
info()  { echo -e "${CYAN}[gidit]${NC} $*"; }
ok()    { echo -e "${GREEN}[  ok ]${NC} $*"; }
fail()  { echo -e "${RED}[fail]${NC} $*"; exit 1; }

# ── Detect platform ──
OS="$(uname -s)"
case "$OS" in
    Linux*)  PLATFORM="Linux"  ;;
    Darwin*) PLATFORM="macOS"  ;;
    *)       fail "Unsupported platform: $OS" ;;
esac
info "Platform: $PLATFORM"

# ── Check dependencies ──
check_cmd() {
    if ! command -v "$1" &>/dev/null; then
        fail "$1 not found. $2"
    fi
    ok "$1 found: $(command -v "$1")"
}

info "Checking dependencies..."
check_cmd cmake    "Install: https://cmake.org/download/"
check_cmd g++      "Install: sudo apt install g++ (Linux) or xcode-select --install (macOS)"

# Check Qt6
if ! cmake --find-package -DNAME=Qt6 -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=EXIST 2>/dev/null; then
    # Fallback: check for qmake6
    if command -v qmake6 &>/dev/null; then
        QT_DIR="$(qmake6 -query QT_INSTALL_PREFIX)"
        export CMAKE_PREFIX_PATH="$QT_DIR:${CMAKE_PREFIX_PATH:-}"
        ok "Qt6 found via qmake6: $QT_DIR"
    elif [ -d "$HOME/Qt" ]; then
        # Common Qt installer location
        QT_CANDIDATE=$(find "$HOME/Qt" -maxdepth 3 -name "Qt6Config.cmake" -print -quit 2>/dev/null || true)
        if [ -n "$QT_CANDIDATE" ]; then
            QT_DIR="$(dirname "$(dirname "$QT_CANDIDATE")")"
            export CMAKE_PREFIX_PATH="$QT_DIR:${CMAKE_PREFIX_PATH:-}"
            ok "Qt6 found: $QT_DIR"
        else
            fail "Qt6 not found. Install Qt6: https://www.qt.io/download-qt-installer"
        fi
    else
        info "Qt6 cmake package not detected — CMake will attempt to find it."
        info "If build fails, set CMAKE_PREFIX_PATH to your Qt installation."
        info ""
        info "  Install Qt6:"
        if [ "$PLATFORM" = "Linux" ]; then
            info "    Ubuntu/Debian: sudo apt install qt6-base-dev qt6-multimedia-dev qt6-sql-dev libqt6svg6-dev"
            info "    Fedora:        sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel qt6-qtsvg-devel"
            info "    Arch:          sudo pacman -S qt6-base qt6-multimedia qt6-svg"
        else
            info "    brew install qt@6"
            info "    export CMAKE_PREFIX_PATH=\$(brew --prefix qt@6)"
        fi
    fi
fi

# ── Handle commands ──
case "$BUILD_TYPE" in
    clean)
        info "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
        ok "Clean complete"
        exit 0
        ;;

    debug)
        CMAKE_BUILD_TYPE="Debug"
        ;;

    release)
        CMAKE_BUILD_TYPE="Release"
        ;;

    install)
        info "Installing..."
        cmake --install "$BUILD_DIR" --prefix "${INSTALL_PREFIX:-/usr/local}"
        ok "Installed"
        exit 0
        ;;

    package)
        info "Packaging..."
        cd "$BUILD_DIR"
        cpack
        ok "Package created in $BUILD_DIR"
        exit 0
        ;;

    *)
        echo "Usage: $0 [debug|release|clean|install|package]"
        exit 1
        ;;
esac

# ── Configure ──
info "Configuring ($CMAKE_BUILD_TYPE)..."
cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    ${CMAKE_PREFIX_PATH:+-DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH"}

# ── Build ──
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
info "Building with $JOBS parallel jobs..."
cmake --build "$BUILD_DIR" --config "$CMAKE_BUILD_TYPE" -j "$JOBS"

ok "Build complete!"

# ── Post-build info ──
if [ "$PLATFORM" = "macOS" ]; then
    BINARY="$BUILD_DIR/Gidit.app"
else
    BINARY="$BUILD_DIR/Gidit"
fi

if [ -e "$BINARY" ]; then
    ok "Binary: $BINARY"
    echo ""
    info "Run with:  $BINARY"
fi
