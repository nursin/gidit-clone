#include "ui/dashboard/widgets/StickyNotesWidget.h"
#include "utils/Theme.h"
#include <QGridLayout>
#include <QLabel>

namespace Gidit {

StickyNotesWidget::StickyNotesWidget(QWidget *parent)
    : BaseWidget("Sticky Notes", "▤", QColor("#FEF9C3"), parent)
{
    setWidgetType("sticky");

    struct Note { QString text; QString bg; QString fg; };
    QVector<Note> notes = {
        {"Qt signals/slots replace Zustand entirely",   "#FEF9C3", "#713F12"},
        {"FAISS C++ API — benchmark vs ChromaDB",       "#DBEAFE", "#1E3A5F"},
        {"Family dinner Wed — groceries before 5pm",    "#DCFCE7", "#14532D"},
        {"ONNX export: opset 17 for Gemma-3",          "#FCE7F3", "#831843"},
        {"Read \"Game Programming Patterns\" ch. 4-6", "#EDE9FE", "#3B0764"},
        {"Mom's birthday — she wants a Kindle",         "#FFEDD5", "#7C2D12"},
    };

    auto *body = new QWidget;
    auto *grid = new QGridLayout(body);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(8);

    for (int i = 0; i < notes.size(); ++i) {
        auto *note = new QLabel(notes[i].text);
        note->setWordWrap(true);
        note->setMinimumHeight(72);
        note->setStyleSheet(QString(
            "background: %1; color: %2; padding: 10px; border-radius: %3px; "
            "font-size: 11px; font-weight: 500; line-height: 1.5;"
        ).arg(notes[i].bg, notes[i].fg).arg(Spacing::RadiusSm));
        note->setCursor(Qt::PointingHandCursor);
        grid->addWidget(note, i / 3, i % 3);
    }

    setWidgetBody(body);
}

} // namespace Gidit
