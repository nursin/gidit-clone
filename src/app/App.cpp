#include "app/App.h"
#include "ui/MainWindow.h"
#include "ui/face/EmotionEngine.h"
#include "utils/Theme.h"
#include <QFontDatabase>

namespace Gidit {

App &App::instance() { static App app; return app; }

int App::run(int argc, char *argv[]) {
    QApplication qapp(argc, argv);
    qapp.setApplicationName("Gidit");
    qapp.setApplicationVersion("0.1.0");
    qapp.setOrganizationName("Gidit");

    initFonts();
    initTheme(qapp);

    m_emotionEngine = std::make_unique<EmotionEngine>();
    m_mainWindow = std::make_unique<MainWindow>(m_emotionEngine.get());
    m_mainWindow->show();

    return qapp.exec();
}

void App::initFonts() {
    // In production, bundle fonts and load them:
    // QFontDatabase::addApplicationFont(":/fonts/PlusJakartaSans.ttf");
    // QFontDatabase::addApplicationFont(":/fonts/DMMono-Regular.ttf");
}

void App::initTheme(QApplication &app) {
    app.setPalette(Theme::lightPalette());
    app.setStyleSheet(Theme::dashboardStyleSheet());
    app.setFont(Theme::bodyFont(10));
}

} // namespace Gidit
