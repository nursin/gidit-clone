#include "ui/dashboard/widgets/QuadrantWidget.h"
#include "utils/Theme.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

namespace Gidit {

QuadrantWidget::QuadrantWidget(QWidget *parent)
    : BaseWidget("Eisenhower Matrix", "◫", QColor("#EFF6FF"), parent)
{
    setWidgetType("quadrant");

    m_urgent    = {{"Ship v0.2 build to testers"}, {"Fix auth crash on Linux"}, {"Deploy hotfix #47", true}};
    m_schedule  = {{"Architect plugin system"}, {"Write AI flows spec"}};
    m_delegate  = {{"Update dependencies"}, {"Vendor emails"}};
    m_eliminate = {{"Reorganize Figma files"}};

    auto *body = new QWidget;
    auto *grid = new QGridLayout(body);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(8);

    grid->addWidget(buildQuadrant("⚡ Do First",  "urgent",    QColor("#FEF2F2"), QColor("#DC2626"), m_urgent),    0, 0);
    grid->addWidget(buildQuadrant("📋 Schedule",   "schedule",  QColor("#F0F9FF"), QColor("#2563EB"), m_schedule),  0, 1);
    grid->addWidget(buildQuadrant("👤 Delegate",   "delegate",  QColor("#FFFBEB"), QColor("#D97706"), m_delegate),  1, 0);
    grid->addWidget(buildQuadrant("✕ Eliminate",   "eliminate",  QColor("#F0FDF4"), QColor("#16A34A"), m_eliminate), 1, 1);

    grid->setRowMinimumHeight(0, 120);
    grid->setRowMinimumHeight(1, 120);
    setWidgetBody(body);
}

QWidget *QuadrantWidget::buildQuadrant(const QString &label, const QString &, const QColor &bg,
                                        const QColor &textColor, QVector<QuadrantTask> &tasks) {
    auto *w = new QWidget;
    w->setStyleSheet(QString("background: %1; border-radius: %2px; padding: 10px;")
                     .arg(bg.name()).arg(Spacing::RadiusSm));
    auto *vl = new QVBoxLayout(w);
    vl->setContentsMargins(10, 10, 10, 10);
    vl->setSpacing(4);

    auto *lbl = new QLabel(label, w);
    lbl->setStyleSheet(QString("color: %1; font-size: 9px; font-weight: 700; letter-spacing: 1.2px;").arg(textColor.name()));
    lbl->setFont(Theme::labelFont(7));
    vl->addWidget(lbl);

    for (int i = 0; i < tasks.size(); ++i) {
        auto *row = new QWidget(w);
        auto *rl = new QHBoxLayout(row);
        rl->setContentsMargins(0, 2, 0, 2);
        rl->setSpacing(5);

        auto *check = new QCheckBox(row);
        check->setChecked(tasks[i].done);
        check->setStyleSheet(QString(
            "QCheckBox::indicator { width: 12px; height: 12px; border-radius: 3px; border: 1.5px solid %1; }"
            "QCheckBox::indicator:checked { background: %2; border-color: %2; }"
        ).arg(Colors::Border, Colors::Success));

        auto *txt = new QLabel(tasks[i].text, row);
        txt->setStyleSheet(QString("font-size: 11px; color: %1;").arg(Colors::TextSecondary));
        if (tasks[i].done) txt->setStyleSheet(txt->styleSheet() + " text-decoration: line-through; opacity: 0.5;");

        rl->addWidget(check);
        rl->addWidget(txt, 1);

        connect(check, &QCheckBox::toggled, this, [&tasks, i, txt](bool on) {
            tasks[i].done = on;
            txt->setStyleSheet(on ?
                QString("font-size: 11px; color: %1; text-decoration: line-through; opacity: 0.5;").arg(Colors::TextSecondary) :
                QString("font-size: 11px; color: %1;").arg(Colors::TextSecondary));
        });
        vl->addWidget(row);
    }
    vl->addStretch();
    return w;
}

} // namespace Gidit
