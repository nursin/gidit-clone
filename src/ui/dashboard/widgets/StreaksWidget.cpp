#include "ui/dashboard/widgets/StreaksWidget.h"
#include "utils/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Gidit {

StreaksWidget::StreaksWidget(QWidget *parent)
    : BaseWidget("Streaks", "🔥", QColor("#FEF2F2"), parent)
{
    setWidgetType("streaks");

    struct Streak { QString icon; QString name; QString days; QVector<int> dots; };
    // dot: 0=filled, 1=missed, 2=pending, 3=today
    QVector<Streak> streaks = {
        {"💻", "Code daily",   "12 day streak", {0,0,0,0,0,0,3}},
        {"🏋️", "Exercise",     "5 day streak",  {0,1,0,0,0,0,3}},
        {"📖", "Read 30 min",  "8 day streak",  {0,0,0,0,0,0,2}},
        {"🧘", "Meditate",     "3 day streak",  {1,1,0,0,0,1,2}},
    };

    auto dotColor = [](int type) -> QString {
        switch (type) {
            case 0: return Colors::Success;
            case 1: return "rgba(239,68,68,0.4)";
            case 2: return Colors::SurfaceSunken;
            case 3: return Colors::Primary;
            default: return Colors::Border;
        }
    };

    auto *body = new QWidget;
    auto *vl = new QVBoxLayout(body);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    for (int s = 0; s < streaks.size(); ++s) {
        const auto &st = streaks[s];
        auto *row = new QWidget;
        auto *rl = new QHBoxLayout(row);
        rl->setContentsMargins(0, 8, 0, 8);
        rl->setSpacing(10);

        auto *icon = new QLabel(st.icon, row);
        icon->setFixedWidth(28);
        icon->setAlignment(Qt::AlignCenter);
        icon->setStyleSheet("font-size: 18px;");

        auto *info = new QWidget(row);
        auto *il = new QVBoxLayout(info);
        il->setContentsMargins(0, 0, 0, 0);
        il->setSpacing(1);
        auto *name = new QLabel(st.name, info);
        name->setStyleSheet(QString("font-size: 12px; font-weight: 600; color: %1;").arg(Colors::Text));
        auto *days = new QLabel(st.days, info);
        days->setFont(Theme::monoFont(8));
        days->setStyleSheet(QString("color: %1;").arg(Colors::TextTertiary));
        il->addWidget(name);
        il->addWidget(days);

        auto *dotsW = new QWidget(row);
        auto *dl = new QHBoxLayout(dotsW);
        dl->setContentsMargins(0, 0, 0, 0);
        dl->setSpacing(3);
        for (int d : st.dots) {
            auto *dot = new QWidget(dotsW);
            dot->setFixedSize(10, 10);
            QString extra = d == 3 ? " border: 2px solid " + QString(Colors::PrimaryMed) + ";" : "";
            if (d == 2) extra = " border: 1px solid " + QString(Colors::Border) + ";";
            dot->setStyleSheet(QString("background: %1; border-radius: 2px;%2").arg(dotColor(d), extra));
            dl->addWidget(dot);
        }

        rl->addWidget(icon);
        rl->addWidget(info, 1);
        rl->addWidget(dotsW);
        vl->addWidget(row);

        if (s < streaks.size() - 1) {
            auto *sep = new QFrame;
            sep->setFrameShape(QFrame::HLine);
            sep->setStyleSheet(QString("color: %1;").arg(Colors::BorderLight));
            sep->setFixedHeight(1);
            vl->addWidget(sep);
        }
    }

    setWidgetBody(body);
}

} // namespace Gidit
