#include "ui/dashboard/widgets/HealthWidget.h"
#include "utils/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

namespace Gidit {

HealthWidget::HealthWidget(QWidget *parent)
    : BaseWidget("Health", "♥", QColor("#FCE7F3"), parent)
{
    setWidgetType("health");

    struct Metric { QString icon; QString iconBg; QString label; QString value; QString suffix; int pct; QColor barColor; };
    QVector<Metric> metrics = {
        {"😴", "#DBEAFE", "Sleep",  "7h 12m", "",             85, QColor(Colors::Primary)},
        {"🚶", "#FEF3C7", "Steps",  "6,482",  " / 10k",      65, QColor(Colors::Warning)},
        {"💓", "#FCE7F3", "HR",     "64",     " bpm",         78, QColor("#EC4899")},
    };

    auto *body = new QWidget;
    auto *vl = new QVBoxLayout(body);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(12);

    for (const auto &m : metrics) {
        auto *row = new QWidget;
        auto *rl = new QHBoxLayout(row);
        rl->setContentsMargins(0, 0, 0, 0);
        rl->setSpacing(12);

        auto *icon = new QLabel(m.icon, row);
        icon->setFixedSize(36, 36);
        icon->setAlignment(Qt::AlignCenter);
        icon->setStyleSheet(QString("background: %1; border-radius: %2px; font-size: 16px;")
                            .arg(m.iconBg).arg(Spacing::RadiusSm));

        auto *info = new QWidget(row);
        auto *il = new QVBoxLayout(info);
        il->setContentsMargins(0, 0, 0, 0);
        il->setSpacing(2);

        auto *lbl = new QLabel(m.label, info);
        lbl->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 500;").arg(Colors::TextTertiary));

        auto *valRow = new QWidget(info);
        auto *vrl = new QHBoxLayout(valRow);
        vrl->setContentsMargins(0, 0, 0, 0);
        vrl->setSpacing(0);
        auto *val = new QLabel(m.value, valRow);
        val->setStyleSheet(QString("color: %1; font-size: 16px; font-weight: 700; letter-spacing: -0.3px;").arg(Colors::Text));
        vrl->addWidget(val);
        if (!m.suffix.isEmpty()) {
            auto *suf = new QLabel(m.suffix, valRow);
            suf->setStyleSheet(QString("color: %1; font-size: 11px;").arg(Colors::TextTertiary));
            vrl->addWidget(suf);
        }
        vrl->addStretch();

        auto *bar = new QProgressBar(info);
        bar->setRange(0, 100);
        bar->setValue(m.pct);
        bar->setTextVisible(false);
        bar->setFixedHeight(5);
        bar->setStyleSheet(QString(
            "QProgressBar { background: %1; border: none; border-radius: 3px; }"
            "QProgressBar::chunk { background: %2; border-radius: 3px; }"
        ).arg(Colors::SurfaceSunken, m.barColor.name()));

        il->addWidget(lbl);
        il->addWidget(valRow);
        il->addWidget(bar);

        rl->addWidget(icon);
        rl->addWidget(info, 1);
        vl->addWidget(row);
    }

    setWidgetBody(body);
}

} // namespace Gidit
