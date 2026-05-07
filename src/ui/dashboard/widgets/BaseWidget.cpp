#include "ui/dashboard/widgets/BaseWidget.h"
#include "utils/Theme.h"
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsDropShadowEffect>

namespace Gidit {

BaseWidget::BaseWidget(const QString &title, const QString &icon, const QColor &iconBg, QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::NoFrame);
    setStyleSheet(QString(
        "BaseWidget { background: %1; border: 1px solid %2; border-radius: %3px; }"
    ).arg(Colors::Surface, Colors::BorderLight).arg(Spacing::Radius));

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Header
    m_header = new QWidget(this);
    auto *headerLayout = new QHBoxLayout(m_header);
    headerLayout->setContentsMargins(16, 14, 16, 0);

    // Icon + Title
    auto *iconLabel = new QLabel(icon, m_header);
    iconLabel->setFixedSize(22, 22);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(QString(
        "background: %1; border-radius: 6px; font-size: 12px; color: %2;"
    ).arg(iconBg.name(QColor::HexArgb), iconBg.darker(120).name()));
    headerLayout->addWidget(iconLabel);

    m_titleLabel = new QLabel(title.toUpper(), m_header);
    m_titleLabel->setFont(Theme::labelFont(9));
    m_titleLabel->setStyleSheet(QString("color: %1; letter-spacing: 0.8px;").arg(Colors::TextTertiary));
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();

    auto *menuBtn = new QPushButton("⋯", m_header);
    menuBtn->setFixedSize(24, 24);
    menuBtn->setStyleSheet(
        "QPushButton { background: none; border: none; color: " + QString(Colors::TextTertiary) + "; font-size: 14px; border-radius: 6px; }"
        "QPushButton:hover { background: " + QString(Colors::SurfaceSunken) + "; }");
    headerLayout->addWidget(menuBtn);

    m_mainLayout->addWidget(m_header);
}

void BaseWidget::setWidgetBody(QWidget *body) {
    if (m_body) { m_mainLayout->removeWidget(m_body); delete m_body; }
    m_body = body;
    m_body->setContentsMargins(16, 14, 16, 16);
    m_mainLayout->addWidget(m_body);
}

void BaseWidget::enterEvent(QEnterEvent *) {
    setStyleSheet(QString(
        "BaseWidget { background: %1; border: 1px solid %2; border-radius: %3px; }"
    ).arg(Colors::Surface, Colors::Border).arg(Spacing::Radius));
}

void BaseWidget::leaveEvent(QEvent *) {
    setStyleSheet(QString(
        "BaseWidget { background: %1; border: 1px solid %2; border-radius: %3px; }"
    ).arg(Colors::Surface, Colors::BorderLight).arg(Spacing::Radius));
}

void BaseWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) m_dragStartPos = event->pos();
    QFrame::mousePressEvent(event);
}

void BaseWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - m_dragStartPos).manhattanLength() < 20) return;

    auto *drag = new QDrag(this);
    auto *mime = new QMimeData;
    mime->setData("application/x-gidit-widget", m_type.toUtf8());
    drag->setMimeData(mime);
    drag->exec(Qt::MoveAction);
}

} // namespace Gidit
