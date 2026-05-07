#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"
#include <QVector>
#include <QCheckBox>

namespace Gidit {

struct TodoItem {
    QString text;
    QString priority; // "high", "med", "low"
    bool completed = false;
};

class TodoWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit TodoWidget(QWidget *parent = nullptr);
    void addItem(const QString &text, const QString &priority);

signals:
    void itemToggled(int index, bool checked);

private:
    void rebuild();
    QVector<TodoItem> m_items;
    QWidget *m_listWidget = nullptr;
};

} // namespace Gidit
