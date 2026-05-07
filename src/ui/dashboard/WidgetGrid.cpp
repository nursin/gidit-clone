// ══════════════════════════════════════════════════════════════
//  Gidit — Widget Grid Implementation
// ══════════════════════════════════════════════════════════════
#include "ui/dashboard/WidgetGrid.h"
#include "ui/dashboard/widgets/BaseWidget.h"
#include "ui/dashboard/widgets/QuadrantWidget.h"
#include "ui/dashboard/widgets/StatsWidget.h"
#include "ui/dashboard/widgets/TimeBlockWidget.h"
#include "ui/dashboard/widgets/TodoWidget.h"
#include "ui/dashboard/widgets/StreaksWidget.h"
#include "ui/dashboard/widgets/StickyNotesWidget.h"
#include "ui/dashboard/widgets/HealthWidget.h"
#include "utils/Theme.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

namespace Gidit {

WidgetGrid::WidgetGrid(QWidget *parent) : QWidget(parent) {
    setAcceptDrops(true);

    m_grid = new QGridLayout(this);
    m_grid->setContentsMargins(0, 0, 0, 0);
    m_grid->setSpacing(16);

    // Column stretch: all equal
    for (int c = 0; c < COLUMNS; ++c) {
        m_grid->setColumnStretch(c, 1);
    }

    loadDefaultWidgets();
}

void WidgetGrid::addWidget(BaseWidget *widget, int row, int col, int rowSpan, int colSpan) {
    m_grid->addWidget(widget, row, col, rowSpan, colSpan);
    m_widgets.append(widget);
}

void WidgetGrid::loadDefaultWidgets() {
    // Row 0: Eisenhower Matrix (spans 2 cols) + Stats
    auto *quadrant = new QuadrantWidget(this);
    addWidget(quadrant, 0, 0, 1, 2);

    auto *stats = new StatsWidget(this);
    addWidget(stats, 0, 2);

    // Row 1: Time Blocks + To-Do + Streaks
    auto *timeblock = new TimeBlockWidget(this);
    addWidget(timeblock, 1, 0);

    auto *todo = new TodoWidget(this);
    todo->addItem("Port Builder.cpp drag engine", "high");
    todo->addItem("Integrate ONNX Runtime", "med");
    todo->addItem("Design QSS theme", "med");
    todo->addItem("Test FAISS perf", "low");
    addWidget(todo, 1, 1);

    auto *streaks = new StreaksWidget(this);
    addWidget(streaks, 1, 2);

    // Row 2: Sticky Notes (spans 2 cols) + Health
    auto *sticky = new StickyNotesWidget(this);
    addWidget(sticky, 2, 0, 1, 2);

    auto *health = new HealthWidget(this);
    addWidget(health, 2, 2);
}

void WidgetGrid::setBuilderMode(bool on) {
    m_builderMode = on;
    for (auto *w : m_widgets) {
        if (on) {
            w->setStyleSheet(w->styleSheet() +
                " BaseWidget { outline: 2px dashed " + QString(Colors::PrimaryMed) + ";"
                " outline-offset: 2px; }");
        } else {
            // Reset to normal style
            w->setStyleSheet(QString(
                "BaseWidget { background: %1; border: 1px solid %2; border-radius: %3px; }"
            ).arg(Colors::Surface, Colors::BorderLight).arg(Spacing::Radius));
        }
    }
}

void WidgetGrid::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-gidit-widget")) {
        event->acceptProposedAction();
    }
}

void WidgetGrid::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void WidgetGrid::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasFormat("application/x-gidit-widget")) return;

    // Find which grid cell the drop landed in
    QPoint dropPos = event->position().toPoint();
    int targetRow = -1, targetCol = -1;

    for (int r = 0; r < m_grid->rowCount(); ++r) {
        for (int c = 0; c < m_grid->columnCount(); ++c) {
            auto *item = m_grid->itemAtPosition(r, c);
            if (item && item->widget() && item->widget()->geometry().contains(dropPos)) {
                targetRow = r;
                targetCol = c;
                break;
            }
        }
        if (targetRow >= 0) break;
    }

    // Find the dragged widget
    auto *source = qobject_cast<BaseWidget *>(event->source());
    if (!source || targetRow < 0) return;

    // Find source position
    int srcRow = -1, srcCol = -1, srcRowSpan = 1, srcColSpan = 1;
    int idx = m_grid->indexOf(source);
    if (idx >= 0) {
        m_grid->getItemPosition(idx, &srcRow, &srcCol, &srcRowSpan, &srcColSpan);
    }

    // Swap the widgets in the grid
    auto *targetItem = m_grid->itemAtPosition(targetRow, targetCol);
    if (!targetItem || !targetItem->widget()) return;
    auto *targetWidget = targetItem->widget();

    int tgtRowSpan = 1, tgtColSpan = 1;
    int tgtIdx = m_grid->indexOf(targetWidget);
    if (tgtIdx >= 0) {
        int tr, tc;
        m_grid->getItemPosition(tgtIdx, &tr, &tc, &tgtRowSpan, &tgtColSpan);
    }

    // Remove both from layout and re-add swapped
    m_grid->removeWidget(source);
    m_grid->removeWidget(targetWidget);
    m_grid->addWidget(source, targetRow, targetCol, tgtRowSpan, tgtColSpan);
    m_grid->addWidget(targetWidget, srcRow, srcCol, srcRowSpan, srcColSpan);

    event->acceptProposedAction();
}

} // namespace Gidit
