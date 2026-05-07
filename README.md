# Gidit — Your Life, Organized

Native cross-platform desktop app for personal life management. Built with C++17 and Qt 6.

**Dual-mode interface:**
- **Autopilot** — Animated face agent with voice commands, camera tracking, emotion engine
- **Manual Control** — Full dashboard with drag-and-drop widgets, AI sidebar, builder mode

Switch between modes with `Ctrl+Tab`, the floating mode button, or by typing/saying "manual control" / "autopilot".

---

## Prerequisites

| Dependency | Minimum | Notes |
|---|---|---|
| **CMake** | 3.21+ | [cmake.org/download](https://cmake.org/download/) |
| **C++ Compiler** | C++17 | GCC 9+, Clang 10+, MSVC 2019+ |
| **Qt** | 6.5+ | Core, Widgets, Gui, Multimedia, Sql, Svg, Network |

Optional: Qt WebEngine (browser widget), ONNX Runtime (AI inference).

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake \
    qt6-base-dev qt6-multimedia-dev libqt6sql6-sqlite \
    libqt6svg6-dev libqt6network6-dev
```

Fedora:
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qtmultimedia-devel \
    qt6-qtsvg-devel
```

Arch:
```bash
sudo pacman -S base-devel cmake qt6-base qt6-multimedia qt6-svg
```

### macOS

```bash
brew install cmake qt@6
export CMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

### Windows

1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) (Desktop C++ workload)
2. Install [CMake](https://cmake.org/download/)
3. Install [Qt 6](https://www.qt.io/download-qt-installer) (select MSVC 2022 64-bit)
4. Set environment:
   ```powershell
   $env:CMAKE_PREFIX_PATH = "C:\Qt\6.7.0\msvc2022_64"
   ```

---

## Build

### Linux / macOS

```bash
# Release build
./scripts/build.sh release

# Debug build (with AddressSanitizer)
./scripts/build.sh debug

# Clean
./scripts/build.sh clean

# Package (DEB/RPM/AppImage on Linux, DMG on macOS)
./scripts/build.sh package
```

### Windows (PowerShell)

```powershell
# Release build
.\scripts\build.ps1 -Config Release

# Debug build
.\scripts\build.ps1 -Config Debug

# Clean
.\scripts\build.ps1 -Clean

# Package (NSIS installer)
.\scripts\build.ps1 -Config Release -Package
```

### Manual CMake

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j$(nproc)
./build/Gidit
```

---

## Project Structure

```
gidit-desktop/
├── CMakeLists.txt              # Build system (Linux/macOS/Windows)
├── cmake/
│   └── Info.plist.in           # macOS bundle metadata
├── scripts/
│   ├── build.sh                # Linux/macOS build script
│   └── build.ps1               # Windows PowerShell build script
├── resources/
│   ├── gidit.qrc               # Qt resource file
│   ├── gidit.desktop           # Linux desktop entry
│   └── icons/
│       └── gidit-logo.png      # App icon
├── src/
│   ├── main.cpp                # Entry point (cross-platform)
│   ├── app/
│   │   ├── App.h/cpp           # Application singleton, font/theme init
│   │   └── Settings.h/cpp      # QSettings persistence (geometry, mode, prefs)
│   ├── ui/
│   │   ├── MainWindow.h/cpp    # Mode switching (Autopilot ↔ Manual) via QStackedWidget
│   │   ├── face/
│   │   │   ├── FaceCanvas.h/cpp      # QPainter animated face renderer (dual-canvas)
│   │   │   └── EmotionEngine.h/cpp   # 38 emotions, lerp interpolation, blink, sentiment
│   │   ├── autopilot/
│   │   │   ├── AutopilotView.h/cpp   # Fullscreen face + overlaid sidebar/chatbar
│   │   │   ├── FaceSidebar.h/cpp     # Slide-in emotion panel
│   │   │   └── ChatBar.h/cpp         # Voice/text input, mode commands
│   │   ├── dashboard/
│   │   │   ├── DashboardView.h/cpp   # 3-column layout: sidebar | grid | AI panel
│   │   │   ├── WidgetGrid.h/cpp      # Drag-and-drop QGridLayout
│   │   │   └── widgets/
│   │   │       ├── BaseWidget.h/cpp        # Draggable card base class
│   │   │       ├── QuadrantWidget.h/cpp    # Eisenhower matrix
│   │   │       ├── TodoWidget.h/cpp        # Checklist with priorities
│   │   │       ├── TimeBlockWidget.h/cpp   # Day schedule with color blocks
│   │   │       ├── StatsWidget.h/cpp       # Weekly metrics grid
│   │   │       ├── StreaksWidget.h/cpp      # Habit tracking with dot visualization
│   │   │       ├── StickyNotesWidget.h/cpp  # Color-coded note grid
│   │   │       └── HealthWidget.h/cpp       # Sleep/steps/HR progress bars
│   │   ├── sidebar/
│   │   │   ├── Sidebar.h/cpp          # Dashboard nav + module list
│   │   │   └── BuilderPalette.h/cpp   # Draggable widget palette
│   │   └── panel/
│   │       ├── AIPanel.h/cpp           # Right panel: mini face + chat + actions
│   │       └── ChatWidget.h/cpp        # AI conversation with tool-call badges
│   ├── ai/
│   │   └── AgentEngine.h/cpp    # Message routing, mode commands, simulated RAG
│   ├── services/
│   │   └── Database.h/cpp       # SQLite via QSql (tasks, health, streaks, notes)
│   └── utils/
│       └── Theme.h/cpp          # Colors, fonts, QSS generation, QPalette
├── .clang-format
└── .gitignore
```

---

## Architecture

### Mode Switching

`MainWindow` uses a `QStackedWidget` to toggle between `AutopilotView` and `DashboardView`. Both share a single `EmotionEngine` instance, so the animated face stays synchronized across modes.

```
MainWindow (QStackedWidget)
├── AutopilotView          ← Fullscreen FaceCanvas + overlaid UI
│   ├── FaceCanvas (main)
│   ├── FaceSidebar
│   └── ChatBar
└── DashboardView          ← 3-column layout
    ├── Sidebar
    ├── WidgetGrid
    │   └── BaseWidget subclasses (draggable)
    └── AIPanel
        ├── FaceCanvas (mini)
        └── ChatWidget
```

### Emotion Engine

The `EmotionEngine` is the shared brain. It:
- Defines 38 emotions with 8 facial parameters each
- Runs a 60fps tick loop smoothing all values via lerp
- Auto-cycles through random emotions (toggleable)
- Handles blink timing with randomized intervals
- Provides sentiment analysis for text → emotion mapping
- Accepts face-tracking data from camera (future MediaPipe/dlib integration)

### Face Rendering

`FaceCanvas` uses `QPainter` with `QPainterPath` for resolution-independent bezier curves. The same engine renders to:
- **Main canvas** (fullscreen in Autopilot) — full tracking offset
- **Mini canvas** (sidebar in Manual) — 30% tracking offset to stay within bounds

### Widget System

All dashboard widgets extend `BaseWidget`, which provides:
- Consistent card chrome (icon, title, menu button)
- Drag-and-drop via `QDrag` + `QMimeData`
- Hover/focus styling
- Builder mode outline toggle

`WidgetGrid` manages a `QGridLayout` with drop handling for reordering.

---

## Keyboard Shortcuts

| Shortcut | Action |
|---|---|
| `Ctrl+Tab` | Toggle Autopilot ↔ Manual |
| `Ctrl+1` | Switch to Autopilot |
| `Ctrl+2` | Switch to Manual |
| `F11` | Toggle fullscreen |
| `Escape` | Exit fullscreen |

---

## Extending

### Add a New Widget

1. Create `src/ui/dashboard/widgets/MyWidget.h` inheriting `BaseWidget`
2. Implement constructor calling `BaseWidget("Title", "🎯", QColor("#EFF6FF"), parent)`
3. Build your widget body and call `setWidgetBody(body)`
4. Register in `WidgetGrid::loadDefaultWidgets()` and `CMakeLists.txt`
5. Add to `BuilderPalette` for drag-from-sidebar support

### Add a New Emotion

Add a line in `EmotionEngine::initEmotions()`:
```cpp
add("blissful", 0.6, 0.8, 0.2, 0.0, 1.0, 1.0, 0.2, 1.3);
//   name       mc   mw   mo   asy  le   re   eb   intensity
```

The engine auto-registers it for text analysis, the emotion panel, and auto-cycling.

---

## License

MIT
# gidit-desktop-tauri
