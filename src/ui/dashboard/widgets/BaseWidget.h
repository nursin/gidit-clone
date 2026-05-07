#pragma once
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace Gidit {

class BaseWidget : public QFrame {
    Q_OBJECT
public:
    explicit BaseWidget(const QString &title, const QString &icon, const QColor &iconBg, QWidget *parent = nullptr);

    void setWidgetBody(QWidget *body);

    // For drag-and-drop
    QString widgetType() const { return m_type; }
    void setWidgetType(const QString &t) { m_type = t; }

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QVBoxLayout *m_mainLayout = nullptr;
    QWidget     *m_header = nullptr;
    QWidget     *m_body = nullptr;
    QLabel      *m_titleLabel = nullptr;
    QString      m_type;
    QPoint       m_dragStartPos;
};

} // namespace Gidit
