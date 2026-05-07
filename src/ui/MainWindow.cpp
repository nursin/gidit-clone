// ══════════════════════════════════════════════════════════════
//  Gidit — Main Window Implementation
// ══════════════════════════════════════════════════════════════
#include "ui/MainWindow.h"
#include "ui/autopilot/AutopilotView.h"
#include "ui/dashboard/DashboardView.h"
#include "ui/face/EmotionEngine.h"
#include "app/Settings.h"
#include "utils/Theme.h"

#include <QKeyEvent>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QShortcut>
#include <QScreen>
#include <QApplication>

namespace Gidit {

MainWindow::MainWindow(EmotionEngine *engine, QWidget *parent)
    : QMainWindow(parent), m_engine(engine)
{
    setWindowTitle("Gidit");
    setMinimumSize(1024, 680);

    // Restore window geometry
    QByteArray geo = Settings::instance().windowGeometry();
    if (!geo.isEmpty()) {
        restoreGeometry(geo);
    } else {
        // Default: 85% of primary screen
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect sr = screen->availableGeometry();
            int w = static_cast<int>(sr.width() * 0.85);
            int h = static_cast<int>(sr.height() * 0.85);
            resize(w, h);
            move((sr.width() - w) / 2, (sr.height() - h) / 2);
        }
    }

    setupUI();
    createModeSwitchButton();

    // Restore last mode
    QString lastMode = Settings::instance().lastMode();
    if (lastMode == "manual") switchMode(Manual);

    // Keyboard shortcuts
    new QShortcut(QKeySequence("Ctrl+Tab"), this, [this]() { toggleMode(); });
    new QShortcut(QKeySequence("Ctrl+1"), this,   [this]() { switchMode(Autopilot); });
    new QShortcut(QKeySequence("Ctrl+2"), this,   [this]() { switchMode(Manual); });
    new QShortcut(QKeySequence("F11"), this,       [this]() {
        if (isFullScreen()) showNormal(); else showFullScreen();
    });
}

void MainWindow::setupUI() {
    // Central stacked widget switches between views
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // Autopilot view (animated face fullscreen)
    m_autopilotView = new AutopilotView(m_engine, this);
    m_stack->addWidget(m_autopilotView);

    // Dashboard view (widgets, sidebar, AI panel)
    m_dashboardView = new DashboardView(m_engine, this);
    m_stack->addWidget(m_dashboardView);

    // Connect mode switch requests from child views
    connect(m_autopilotView, &AutopilotView::requestManualMode, this, [this]() {
        switchMode(Manual);
    });
    connect(m_dashboardView, &DashboardView::requestAutopilotMode, this, [this]() {
        switchMode(Autopilot);
    });

    m_stack->setCurrentIndex(0); // Start in Autopilot
}

void MainWindow::createModeSwitchButton() {
    // Floating button overlaid on top of the stack
    m_modeSwitchBtn = new QPushButton(this);
    m_modeSwitchBtn->setFixedSize(180, 38);
    m_modeSwitchBtn->setCursor(Qt::PointingHandCursor);
    m_modeSwitchBtn->raise();
    m_modeSwitchBtn->setText("⬡  Manual Control");

    m_modeSwitchBtn->setStyleSheet(R"(
        QPushButton {
            background: rgba(0,0,0,0.7);
            border: 1px solid rgba(255,255,255,0.18);
            border-radius: 19px;
            color: rgba(255,255,255,0.85);
            font-family: "Plus Jakarta Sans", sans-serif;
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.3px;
            padding: 0 16px;
        }
        QPushButton:hover {
            background: rgba(51,145,243,0.3);
            border-color: #3391F3;
            color: white;
        }
    )");

    connect(m_modeSwitchBtn, &QPushButton::clicked, this, &MainWindow::toggleMode);

    // Position bottom-right (updated on resize)
    auto reposition = [this]() {
        m_modeSwitchBtn->move(width() - m_modeSwitchBtn->width() - 20,
                              height() - m_modeSwitchBtn->height() - 20);
    };
    reposition();

    // Also reposition on resize
    connect(this, &QMainWindow::windowTitleChanged, this, reposition); // Proxy; see resizeEvent
}

void MainWindow::switchMode(Mode mode) {
    if (mode == m_currentMode) return;
    m_currentMode = mode;

    if (mode == Autopilot) {
        m_stack->setCurrentWidget(m_autopilotView);
        m_modeSwitchBtn->setText("⬡  Manual Control");
        m_modeSwitchBtn->setStyleSheet(R"(
            QPushButton {
                background: rgba(0,0,0,0.7);
                border: 1px solid rgba(255,255,255,0.18);
                border-radius: 19px;
                color: rgba(255,255,255,0.85);
                font-size: 12px; font-weight: 600;
                padding: 0 16px;
            }
            QPushButton:hover {
                background: rgba(51,145,243,0.3);
                border-color: #3391F3; color: white;
            }
        )");
    } else {
        m_stack->setCurrentWidget(m_dashboardView);
        m_modeSwitchBtn->setText("◉  Autopilot");
        m_modeSwitchBtn->setStyleSheet(R"(
            QPushButton {
                background: rgba(26,35,50,0.85);
                border: 1px solid rgba(51,145,243,0.4);
                border-radius: 19px;
                color: rgba(255,255,255,0.9);
                font-size: 12px; font-weight: 600;
                padding: 0 16px;
            }
            QPushButton:hover {
                background: rgba(51,145,243,0.4);
                border-color: #33F3CD; color: white;
            }
        )");
    }

    // Reposition button
    m_modeSwitchBtn->move(width() - m_modeSwitchBtn->width() - 20,
                          height() - m_modeSwitchBtn->height() - 20);
    m_modeSwitchBtn->raise();

    Settings::instance().setLastMode(mode == Autopilot ? "autopilot" : "manual");
    emit modeChanged(mode);
}

void MainWindow::toggleMode() {
    switchMode(m_currentMode == Autopilot ? Manual : Autopilot);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Escape exits fullscreen
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        showNormal();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    Settings::instance().setWindowGeometry(saveGeometry());
    event->accept();
}

} // namespace Gidit
