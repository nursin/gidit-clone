#include "ui/autopilot/AutopilotView.h"
#include "ui/face/FaceCanvas.h"
#include "ui/face/EmotionEngine.h"
#include "ui/autopilot/FaceSidebar.h"
#include "ui/autopilot/ChatBar.h"
#include "utils/Theme.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>

namespace Gidit {

AutopilotView::AutopilotView(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    // Full-screen face canvas (no layout — children float on top)
    m_faceCanvas = new FaceCanvas(engine, this);
    m_faceCanvas->lower();

    // Face sidebar (slide-in from left)
    m_sidebar = new FaceSidebar(engine, this);
    m_sidebar->move(0, 0);

    // Chat bar (slides up from bottom)
    m_chatBar = new ChatBar(engine, this);

    // "Autopilot" label top-right
    m_modeLabel = new QWidget(this);
    m_modeLabel->setFixedSize(140, 24);
    auto *ll = new QHBoxLayout(m_modeLabel);
    ll->setContentsMargins(0,0,0,0);
    ll->setSpacing(8);
    auto *dot = new QWidget(m_modeLabel);
    dot->setFixedSize(6, 6);
    dot->setStyleSheet("background: #33F3CD; border-radius: 3px;");
    auto *lbl = new QLabel("AUTOPILOT", m_modeLabel);
    lbl->setStyleSheet("color: rgba(255,255,255,0.25); font-size: 11px; font-weight: 600; letter-spacing: 1px;");
    lbl->setFont(Theme::monoFont(8));
    ll->addWidget(dot);
    ll->addWidget(lbl);

    // Forward mode switch requests from ChatBar
    connect(m_chatBar, &ChatBar::requestManualMode, this, &AutopilotView::requestManualMode);
}

void AutopilotView::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    m_faceCanvas->setGeometry(0, 0, width(), height());
    m_sidebar->setFixedHeight(height());
    m_chatBar->move(0, height() - m_chatBar->height());
    m_chatBar->setFixedWidth(width());
    m_modeLabel->move(width() - 160, 22);
}

} // namespace Gidit
