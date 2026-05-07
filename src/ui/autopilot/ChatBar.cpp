#include "ui/autopilot/ChatBar.h"
#include "ui/face/EmotionEngine.h"
#include "utils/Theme.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>

namespace Gidit {

ChatBar::ChatBar(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    setFixedHeight(72);
    setStyleSheet("background: transparent;");

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 16);
    layout->setSpacing(10);
    layout->addStretch();

    auto *inner = new QWidget;
    inner->setFixedWidth(640);
    auto *innerLayout = new QHBoxLayout(inner);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(10);

    m_micBtn = new QPushButton("🎤", inner);
    m_micBtn->setObjectName("micBtn");
    m_micBtn->setFixedSize(42, 42);
    m_micBtn->setStyleSheet(Theme::autopilotStyleSheet());
    innerLayout->addWidget(m_micBtn);

    m_input = new QLineEdit(inner);
    m_input->setObjectName("chatInput");
    m_input->setPlaceholderText("Type or speak… say 'manual control' for dashboard");
    m_input->setStyleSheet(Theme::autopilotStyleSheet());
    connect(m_input, &QLineEdit::returnPressed, this, &ChatBar::onSend);
    innerLayout->addWidget(m_input);

    m_sendBtn = new QPushButton("➤", inner);
    m_sendBtn->setObjectName("chatSend");
    m_sendBtn->setFixedSize(42, 42);
    m_sendBtn->setStyleSheet(Theme::autopilotStyleSheet());
    connect(m_sendBtn, &QPushButton::clicked, this, &ChatBar::onSend);
    innerLayout->addWidget(m_sendBtn);

    layout->addWidget(inner);
    layout->addStretch();

    // Start hidden below
    hide();
}

void ChatBar::toggle() {
    m_visible = !m_visible;
    if (m_visible) {
        show();
        m_input->setFocus();
    } else {
        hide();
    }
}

void ChatBar::onSend() {
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;

    // Check for mode switch
    if (text.toLower().contains("manual control")) {
        m_input->clear();
        emit requestManualMode();
        return;
    }

    // Analyze sentiment and set emotion
    QString emotion = m_engine->analyzeText(text);
    m_engine->setEmotion(emotion);

    emit messageSent(text);
    m_input->clear();
}

} // namespace Gidit
