#include "ui/dashboard/widgets/TimeBlockWidget.h"
#include "utils/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Gidit {

TimeBlockWidget::TimeBlockWidget(QWidget *parent)
    : BaseWidget("Today's Blocks", "▥", QColor("#EDE9FE"), parent)
{
    setWidgetType("timeblock");

    m_blocks = {
        {"8:00",  "Morning routine",          "☀", "focus",    1},
        {"9:00",  "Deep work — CRUSH parser",  "🧠", "deep",     2},
        {"11:00", "Sprint planning",           "📞", "meeting",  1},
        {"12:00", "Lunch + walk",              "🌿", "break",    1},
        {"1:00",  "Gidit Qt migration",        "🧠", "deep",     2},
        {"3:00",  "Family time",               "👨‍👩‍👧", "personal", 1},
    };

    auto *body = new QWidget;
    auto *vl = new QVBoxLayout(body);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(4);

    auto typeStyle = [](const QString &type) -> QString {
        if (type == "focus")    return "background:#DBEAFE; color:#1E40AF; border-left:3px solid #3B82F6;";
        if (type == "deep")     return "background:#EDE9FE; color:#5B21B6; border-left:3px solid #7C3AED;";
        if (type == "meeting")  return "background:#FCE7F3; color:#9D174D; border-left:3px solid #EC4899;";
        if (type == "break")    return "background:#ECFDF5; color:#065F46; border-left:3px solid #10B981;";
        if (type == "personal") return "background:#FEF3C7; color:#92400E; border-left:3px solid #F59E0B;";
        return "";
    };

    for (const auto &b : m_blocks) {
        auto *row = new QWidget;
        auto *rl = new QHBoxLayout(row);
        rl->setContentsMargins(0, 0, 0, 0);
        rl->setSpacing(10);

        auto *time = new QLabel(b.time, row);
        time->setFont(Theme::monoFont(8));
        time->setFixedWidth(42);
        time->setStyleSheet(QString("color: %1;").arg(Colors::TextTertiary));

        auto *bar = new QLabel(b.icon + " " + b.label, row);
        bar->setMinimumHeight(b.heightScale > 1 ? 54 : 36);
        bar->setStyleSheet(typeStyle(b.type) +
            QString(" border-radius: %1px; padding: 7px 10px; font-size: 12px; font-weight: 500;").arg(Spacing::RadiusXs));

        rl->addWidget(time);
        rl->addWidget(bar, 1);
        vl->addWidget(row);
    }

    setWidgetBody(body);
}

} // namespace Gidit
