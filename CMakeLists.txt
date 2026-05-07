# ══════════════════════════════════════════════════════════════
#  Gidit — Native Cross-Platform Life Operating System
#  Build: CMake 3.21+ | Qt 6.5+ | C++17
#  Platforms: Linux, macOS, Windows
# ══════════════════════════════════════════════════════════════
cmake_minimum_required(VERSION 3.21)

project(Gidit
    VERSION 0.1.0
    DESCRIPTION "Your Life, Organized — Native Desktop AI Dashboard"
    LANGUAGES CXX
)

# ── C++ Standard ──
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Qt Auto Tools ──
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ── Build Type Default ──
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ── Platform Detection ──
if(APPLE)
    set(GIDIT_PLATFORM "macOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS version")
elseif(WIN32)
    set(GIDIT_PLATFORM "Windows")
elseif(UNIX)
    set(GIDIT_PLATFORM "Linux")
endif()
message(STATUS "Gidit v${PROJECT_VERSION} — Building for ${GIDIT_PLATFORM} (${CMAKE_BUILD_TYPE})")

# ══════════════════════════════════════
#  Dependencies
# ══════════════════════════════════════

# Qt 6 — Core, Widgets, Gui, Multimedia, Sql, Svg
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Multimedia
    Sql
    Svg
    Network
)

# Optional: Qt WebEngine (for embedded browser widget)
find_package(Qt6 COMPONENTS WebEngineWidgets QUIET)
if(Qt6WebEngineWidgets_FOUND)
    message(STATUS "  Qt WebEngine found — WebBrowser widget enabled")
    add_compile_definitions(GIDIT_HAS_WEBENGINE)
else()
    message(STATUS "  Qt WebEngine not found — WebBrowser widget disabled")
endif()

# Optional: ONNX Runtime (for AI inference)
find_package(onnxruntime QUIET)
if(onnxruntime_FOUND)
    message(STATUS "  ONNX Runtime found — AI inference enabled")
    add_compile_definitions(GIDIT_HAS_ONNX)
endif()

# ══════════════════════════════════════
#  Source Files
# ══════════════════════════════════════

set(GIDIT_SOURCES
    # Entry point
    src/main.cpp

    # App core
    src/app/App.cpp
    src/app/App.h
    src/app/Settings.cpp
    src/app/Settings.h

    # UI — Main window & mode management
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h

    # UI — Animated Face Engine
    src/ui/face/FaceCanvas.cpp
    src/ui/face/FaceCanvas.h
    src/ui/face/EmotionEngine.cpp
    src/ui/face/EmotionEngine.h

    # UI — Autopilot View
    src/ui/autopilot/AutopilotView.cpp
    src/ui/autopilot/AutopilotView.h
    src/ui/autopilot/FaceSidebar.cpp
    src/ui/autopilot/FaceSidebar.h
    src/ui/autopilot/ChatBar.cpp
    src/ui/autopilot/ChatBar.h

    # UI — Dashboard View
    src/ui/dashboard/DashboardView.cpp
    src/ui/dashboard/DashboardView.h
    src/ui/dashboard/WidgetGrid.cpp
    src/ui/dashboard/WidgetGrid.h

    # UI — Dashboard Widgets
    src/ui/dashboard/widgets/BaseWidget.cpp
    src/ui/dashboard/widgets/BaseWidget.h
    src/ui/dashboard/widgets/TodoWidget.cpp
    src/ui/dashboard/widgets/TodoWidget.h
    src/ui/dashboard/widgets/TimeBlockWidget.cpp
    src/ui/dashboard/widgets/TimeBlockWidget.h
    src/ui/dashboard/widgets/StatsWidget.cpp
    src/ui/dashboard/widgets/StatsWidget.h
    src/ui/dashboard/widgets/StreaksWidget.cpp
    src/ui/dashboard/widgets/StreaksWidget.h
    src/ui/dashboard/widgets/StickyNotesWidget.cpp
    src/ui/dashboard/widgets/StickyNotesWidget.h
    src/ui/dashboard/widgets/HealthWidget.cpp
    src/ui/dashboard/widgets/HealthWidget.h
    src/ui/dashboard/widgets/QuadrantWidget.cpp
    src/ui/dashboard/widgets/QuadrantWidget.h

    # UI — Sidebar & Panel
    src/ui/sidebar/Sidebar.cpp
    src/ui/sidebar/Sidebar.h
    src/ui/sidebar/BuilderPalette.cpp
    src/ui/sidebar/BuilderPalette.h
    src/ui/panel/AIPanel.cpp
    src/ui/panel/AIPanel.h
    src/ui/panel/ChatWidget.cpp
    src/ui/panel/ChatWidget.h

    # AI Engine
    src/ai/AgentEngine.cpp
    src/ai/AgentEngine.h

    # Services
    src/services/Database.cpp
    src/services/Database.h

    # Utils
    src/utils/Theme.cpp
    src/utils/Theme.h
)

# Resources
set(GIDIT_RESOURCES
    resources/gidit.qrc
)

# ══════════════════════════════════════
#  Executable Target
# ══════════════════════════════════════

if(APPLE)
    # macOS: Build as .app bundle
    set(MACOSX_BUNDLE_ICON_FILE gidit.icns)
    set(MACOSX_BUNDLE_BUNDLE_NAME "Gidit")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.gidit.app")

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${GIDIT_SOURCES}
        ${GIDIT_RESOURCES}
    )
elseif(WIN32)
    # Windows: Build as GUI app (no console window)
    add_executable(${PROJECT_NAME} WIN32
        ${GIDIT_SOURCES}
        ${GIDIT_RESOURCES}
    )
else()
    # Linux: Standard executable
    add_executable(${PROJECT_NAME}
        ${GIDIT_SOURCES}
        ${GIDIT_RESOURCES}
    )
endif()

# ── Include Paths ──
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ── Link Libraries ──
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Sql
    Qt6::Svg
    Qt6::Network
)

if(Qt6WebEngineWidgets_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::WebEngineWidgets)
endif()

if(onnxruntime_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE onnxruntime::onnxruntime)
endif()

# ══════════════════════════════════════
#  Compiler Flags (per-platform)
# ══════════════════════════════════════

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4 /utf-8 /permissive-
        $<$<CONFIG:Release>:/O2 /GL>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Release>:-O3 -flto>
        $<$<CONFIG:Debug>:-g -fsanitize=address>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )
endif()

# ══════════════════════════════════════
#  Platform-specific Post-build
# ══════════════════════════════════════

if(APPLE)
    # Deploy Qt frameworks into .app bundle
    find_program(MACDEPLOYQT macdeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(MACDEPLOYQT)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${MACDEPLOYQT}" "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>" -always-overwrite
            COMMENT "Running macdeployqt..."
        )
    endif()

    # Code signing placeholder
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in"
    )
elseif(WIN32)
    # Deploy Qt DLLs alongside .exe
    find_program(WINDEPLOYQT windeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(WINDEPLOYQT)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT}" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt..."
        )
    endif()
endif()

# ══════════════════════════════════════
#  Install Rules
# ══════════════════════════════════════

include(GNUInstallDirs)

if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    # Desktop entry for Linux
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/gidit.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
    )
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/gidit-logo.png"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps"
        RENAME "gidit.png"
    )
elseif(APPLE)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
    )
elseif(WIN32)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION .
    )
endif()

# ══════════════════════════════════════
#  CPack Packaging
# ══════════════════════════════════════

set(CPACK_PACKAGE_NAME "Gidit")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Gidit")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Your Life, Organized")

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "Gidit")
elseif(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "Gidit")
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM;AppImage")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6sql6-sqlite")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Gidit Team")
endif()

include(CPack)

message(STATUS "──────────────────────────────────────")
message(STATUS "  Configure complete. Run:")
message(STATUS "    cmake --build build --config ${CMAKE_BUILD_TYPE}")
message(STATUS "──────────────────────────────────────")
