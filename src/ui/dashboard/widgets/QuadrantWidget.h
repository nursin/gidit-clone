#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"
#include <QVector>

namespace Gidit {

struct QuadrantTask { QString text; bool done = false; };

class QuadrantWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit QuadrantWidget(QWidget *parent = nullptr);

private:
    QWidget *buildQuadrant(const QString &label, const QString &icon, const QColor &bg,
                           const QColor &textColor, QVector<QuadrantTask> &tasks);
    QVector<QuadrantTask> m_urgent, m_schedule, m_delegate, m_eliminate;
};

} // namespace Gidit
