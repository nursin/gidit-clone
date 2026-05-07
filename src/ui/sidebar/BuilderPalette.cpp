#include "ui/sidebar/BuilderPalette.h"
#include "utils/Theme.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

namespace Gidit {

BuilderPalette::BuilderPalette(QWidget *parent) : QWidget(parent) {
    struct PaletteItem { QString icon; QString label; QString type; };
    QVector<PaletteItem> items = {
        {"☑", "To-Do",       "todo"},
        {"◫", "Quadrant",    "quadrant"},
        {"▥", "Time Block",  "timeblock"},
        {"▤", "Sticky Notes","sticky"},
        {"🔥","Streaks",     "streaks"},
        {"◩", "Stats",       "stats"},
        {"♥", "Health",      "health"},
        {"✦", "AI Chat",     "ai"},
    };

    auto *grid = new QGridLayout(this);
    grid->setContentsMargins(14, 8, 14, 16);
    grid->setSpacing(6);

    for (int i = 0; i < items.size(); ++i) {
        auto *btn = new QPushButton(this);
        btn->setFixedHeight(64);
        btn->setCursor(Qt::OpenHandCursor);

        auto *vl = new QVBoxLayout(btn);
        vl->setAlignment(Qt::AlignCenter);
        vl->setSpacing(4);

        auto *icon = new QLabel(items[i].icon, btn);
        icon->setAlignment(Qt::AlignCenter);
        icon->setStyleSheet("font-size: 20px;");
        auto *lbl = new QLabel(items[i].label, btn);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(QString("font-size: 10px; font-weight: 600; color: %1;").arg(Colors::TextSecondary));
        vl->addWidget(icon);
        vl->addWidget(lbl);

        btn->setStyleSheet(QString(
            "QPushButton { background: %1; border: 1px solid %2; border-radius: %3px; }"
            "QPushButton:hover { border-color: %4; background: %5; }"
        ).arg(Colors::Surface, Colors::BorderLight).arg(Spacing::RadiusSm)
         .arg(Colors::Primary, Colors::PrimarySoft));

        QString type = items[i].type;
        // Enable drag from palette
        btn->installEventFilter(this);
        btn->setProperty("widgetType", type);

        connect(btn, &QPushButton::pressed, this, [this, type]() {
            auto *drag = new QDrag(this);
            auto *mime = new QMimeData;
            mime->setData("application/x-gidit-widget", type.toUtf8());
            drag->setMimeData(mime);
            drag->exec(Qt::CopyAction);
            emit widgetDragged(type);
        });

        grid->addWidget(btn, i / 2, i % 2);
    }
}

} // namespace Gidit
