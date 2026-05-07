#include "ui/dashboard/widgets/StatsWidget.h"
#include "utils/Theme.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>

namespace Gidit {

StatsWidget::StatsWidget(QWidget *parent)
    : BaseWidget("This Week", "◩", QColor("#F0FDF4"), parent)
{
    setWidgetType("stats");

    struct Stat { QString label; QString value; QString change; QColor color; };
    QVector<Stat> stats = {
        {"TASKS DONE", "24",    "↑ 18%",  QColor(Colors::Primary)},
        {"FOCUS TIME", "16.5h", "↑ 2.3h", QColor("#7C3AED")},
        {"STREAK",     "12d",   "🔥 Best", QColor("#F59E0B")},
        {"MOOD",       "8.2",   "↑ Great", QColor("#EC4899")},
    };

    auto *body = new QWidget;
    auto *grid = new QGridLayout(body);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(10);

    for (int i = 0; i < stats.size(); ++i) {
        auto *card = new QWidget;
        card->setStyleSheet(QString("background: %1; border-radius: %2px; padding: 12px;")
                            .arg(Colors::SurfaceSunken).arg(Spacing::RadiusSm));
        auto *cl = new QVBoxLayout(card);
        cl->setContentsMargins(12, 12, 12, 12);
        cl->setSpacing(2);

        auto *lbl = new QLabel(stats[i].label, card);
        lbl->setFont(Theme::labelFont(7));
        lbl->setStyleSheet(QString("color: %1;").arg(Colors::TextTertiary));

        auto *val = new QLabel(stats[i].value, card);
        val->setFont(Theme::headingFont(16));
        val->setStyleSheet(QString("color: %1; font-size: 22px; font-weight: 800; letter-spacing: -1px;").arg(stats[i].color.name()));

        auto *chg = new QLabel(stats[i].change, card);
        chg->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 600;").arg(Colors::Success));

        cl->addWidget(lbl);
        cl->addWidget(val);
        cl->addWidget(chg);

        grid->addWidget(card, i / 2, i % 2);
    }

    setWidgetBody(body);
}

} // namespace Gidit
