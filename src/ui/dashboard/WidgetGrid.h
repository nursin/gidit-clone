// ══════════════════════════════════════════════════════════════
//  Gidit — Widget Grid
//  Drag-and-drop grid layout that hosts dashboard widgets
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QVector>

namespace Gidit {

class BaseWidget;

class WidgetGrid : public QWidget {
    Q_OBJECT

public:
    explicit WidgetGrid(QWidget *parent = nullptr);

    void addWidget(BaseWidget *widget, int row, int col, int rowSpan = 1, int colSpan = 1);
    void setBuilderMode(bool on);
    bool builderMode() const { return m_builderMode; }

    // Populate with default demo widgets
    void loadDefaultWidgets();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGridLayout          *m_grid = nullptr;
    QVector<BaseWidget *> m_widgets;
    bool                  m_builderMode = false;
    static constexpr int  COLUMNS = 3;
};

} // namespace Gidit
