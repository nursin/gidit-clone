#include "ui/dashboard/widgets/TodoWidget.h"
#include "utils/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Gidit {

TodoWidget::TodoWidget(QWidget *parent)
    : BaseWidget("To-Do", "☑", QColor("#FEF3C7"), parent)
{
    setWidgetType("todo");

    // Default items
    addItem("Set up CMake build", "high");
    addItem("Port Builder.cpp drag engine", "high");
    addItem("Integrate ONNX Runtime", "med");
    addItem("Design QSS theme", "med");
    addItem("Test FAISS perf", "low");

    m_items[0].completed = true;
    rebuild();
}

void TodoWidget::addItem(const QString &text, const QString &priority) {
    m_items.append({text, priority, false});
}

void TodoWidget::rebuild() {
    auto *body = new QWidget;
    auto *layout = new QVBoxLayout(body);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    for (int i = 0; i < m_items.size(); ++i) {
        const auto &item = m_items[i];

        auto *row = new QWidget;
        auto *rl = new QHBoxLayout(row);
        rl->setContentsMargins(0, 8, 0, 8);
        rl->setSpacing(10);

        auto *check = new QCheckBox(row);
        check->setChecked(item.completed);
        check->setStyleSheet(QString(
            "QCheckBox::indicator { width: 18px; height: 18px; border-radius: 9px; border: 2px solid %1; }"
            "QCheckBox::indicator:checked { background: %2; border-color: %2; }"
        ).arg(Colors::Border, Colors::Primary));

        auto *textLabel = new QLabel(item.text, row);
        textLabel->setStyleSheet(item.completed ?
            QString("text-decoration: line-through; color: %1;").arg(Colors::TextTertiary) :
            QString("color: %1;").arg(Colors::Text));
        textLabel->setFont(Theme::bodyFont(10));

        auto *badge = new QLabel(item.priority.toUpper(), row);
        QString badgeBg, badgeColor;
        if (item.priority == "high")     { badgeBg = "#FEE2E2"; badgeColor = "#DC2626"; }
        else if (item.priority == "med") { badgeBg = "#FEF3C7"; badgeColor = "#D97706"; }
        else                             { badgeBg = "#DBEAFE"; badgeColor = "#2563EB"; }
        badge->setStyleSheet(QString(
            "background: %1; color: %2; font-size: 9px; font-weight: 700; padding: 2px 6px; border-radius: 4px;"
        ).arg(badgeBg, badgeColor));
        badge->setFont(Theme::labelFont(7));

        rl->addWidget(check);
        rl->addWidget(textLabel, 1);
        rl->addWidget(badge);

        // Toggle handler
        connect(check, &QCheckBox::toggled, this, [this, i, textLabel](bool checked) {
            m_items[i].completed = checked;
            textLabel->setStyleSheet(checked ?
                QString("text-decoration: line-through; color: %1;").arg(Colors::TextTertiary) :
                QString("color: %1;").arg(Colors::Text));
            emit itemToggled(i, checked);
        });

        layout->addWidget(row);

        // Separator
        if (i < m_items.size() - 1) {
            auto *sep = new QFrame;
            sep->setFrameShape(QFrame::HLine);
            sep->setStyleSheet(QString("color: %1;").arg(Colors::BorderLight));
            sep->setFixedHeight(1);
            layout->addWidget(sep);
        }
    }

    setWidgetBody(body);
}

} // namespace Gidit
