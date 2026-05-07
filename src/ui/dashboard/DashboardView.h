// ══════════════════════════════════════════════════════════════
//  Gidit — Dashboard View (Manual Control Mode)
//  Three-column layout: Sidebar | Widget Grid | AI Panel
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Gidit {

class EmotionEngine;
class Sidebar;
class WidgetGrid;
class AIPanel;

class DashboardView : public QWidget {
    Q_OBJECT

public:
    explicit DashboardView(EmotionEngine *engine, QWidget *parent = nullptr);

signals:
    void requestAutopilotMode();

private:
    void createHeader();
    void createMainArea();

    EmotionEngine *m_engine;

    // Header
    QWidget    *m_header       = nullptr;
    QLineEdit  *m_searchInput  = nullptr;

    // Three columns
    Sidebar    *m_sidebar      = nullptr;
    WidgetGrid *m_widgetGrid   = nullptr;
    AIPanel    *m_aiPanel      = nullptr;

    // Builder mode
    bool        m_builderMode  = false;
    QPushButton *m_builderBtn  = nullptr;
};

} // namespace Gidit
