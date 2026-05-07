// ══════════════════════════════════════════════════════════════
//  Gidit — Main Window
//  Manages Autopilot (face) ↔ Manual (dashboard) mode switching
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QPropertyAnimation>

namespace Gidit {

class EmotionEngine;
class AutopilotView;
class DashboardView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(EmotionEngine *engine, QWidget *parent = nullptr);

    enum Mode { Autopilot, Manual };

public slots:
    void switchMode(Mode mode);
    void toggleMode();

signals:
    void modeChanged(Mode mode);

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    void createModeSwitchButton();

    EmotionEngine *m_engine;
    QStackedWidget *m_stack = nullptr;
    AutopilotView  *m_autopilotView = nullptr;
    DashboardView  *m_dashboardView = nullptr;
    QPushButton    *m_modeSwitchBtn = nullptr;
    Mode            m_currentMode = Autopilot;
};

} // namespace Gidit
