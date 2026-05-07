// ══════════════════════════════════════════════════════════════
//  Gidit — Entry Point
//  Cross-platform: Linux / macOS / Windows
// ══════════════════════════════════════════════════════════════
#include "app/App.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
    // High-DPI scaling (Qt 6 enables this by default, but be explicit)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

#ifdef Q_OS_WIN
    // Ensure UTF-8 console on Windows
    SetConsoleOutputCP(CP_UTF8);
#endif

    return Gidit::App::instance().run(argc, argv);
}
