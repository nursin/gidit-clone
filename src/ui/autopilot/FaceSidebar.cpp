#include "ui/autopilot/FaceSidebar.h"
#include "ui/face/EmotionEngine.h"
#include "utils/Theme.h"

#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QLabel>
#include <QScrollArea>
#include <QFlowLayout>

namespace Gidit {

FaceSidebar::FaceSidebar(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    setFixedWidth(Layout::FaceSidebarW);
    setStyleSheet("background: rgba(255,255,255,0.04); border-right: 1px solid rgba(255,255,255,0.18);");
    move(-Layout::FaceSidebarW, 0); // Start hidden

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 28, 10, 24);
    layout->setSpacing(6);

    // Emotion nav button
    auto *emoBtn = new QPushButton("◉", this);
    emoBtn->setObjectName("faceNavItem");
    emoBtn->setFixedSize(44, 44);
    emoBtn->setToolTip("Emotions");
    emoBtn->setStyleSheet(Theme::autopilotStyleSheet());
    connect(emoBtn, &QPushButton::clicked, this, &FaceSidebar::createEmotionPanel);
    layout->addWidget(emoBtn, 0, Qt::AlignHCenter);

    // Auto emotions toggle
    auto *autoBtn = new QPushButton("⟳", this);
    autoBtn->setObjectName("faceNavItem");
    autoBtn->setFixedSize(44, 44);
    autoBtn->setToolTip("Toggle Auto Emotions");
    autoBtn->setStyleSheet(Theme::autopilotStyleSheet());
    connect(autoBtn, &QPushButton::clicked, this, [this, autoBtn]() {
        bool on = !m_engine->autoEmotionsEnabled();
        m_engine->setAutoEmotions(on);
        autoBtn->setStyleSheet(Theme::autopilotStyleSheet() +
            QString("QPushButton#faceNavItem { opacity: %1; }").arg(on ? "1" : "0.3"));
    });
    layout->addWidget(autoBtn, 0, Qt::AlignHCenter);

    layout->addStretch();
}

void FaceSidebar::toggle() {
    m_visible = !m_visible;
    auto *anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(300);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->setStartValue(pos());
    anim->setEndValue(QPoint(m_visible ? 0 : -Layout::FaceSidebarW, 0));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void FaceSidebar::createEmotionPanel() {
    if (m_emotionPanel) {
        m_emotionPanel->setVisible(!m_emotionPanel->isVisible());
        return;
    }

    m_emotionPanel = new QWidget(parentWidget());
    m_emotionPanel->setFixedWidth(240);
    m_emotionPanel->setMaximumHeight(parentWidget()->height() * 7 / 10);
    m_emotionPanel->setStyleSheet(
        "background: rgba(10,10,10,0.96); border: 1px solid rgba(255,255,255,0.18); border-radius: 16px;");
    m_emotionPanel->move(Layout::FaceSidebarW + 16, parentWidget()->height() / 2 - 200);

    auto *scroll = new QScrollArea(m_emotionPanel);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("background: transparent; border: none;");

    auto *inner = new QWidget;
    auto *flow = new QVBoxLayout(inner);
    flow->setContentsMargins(14, 14, 14, 14);
    flow->setSpacing(5);

    // Title
    auto *title = new QLabel("EMOTIONS");
    title->setStyleSheet("color: rgba(255,255,255,0.45); font-size: 11px; font-weight: 700; letter-spacing: 1.5px; padding-bottom: 8px; border-bottom: 1px solid rgba(255,255,255,0.18);");
    flow->addWidget(title);

    // Emotion chips in a wrap layout
    auto *chipContainer = new QWidget;
    auto *chipLayout = new QVBoxLayout(chipContainer); // Simplified: vertical list
    chipLayout->setSpacing(4);
    chipLayout->setContentsMargins(0, 8, 0, 0);

    for (const auto &emo : m_engine->allEmotions()) {
        auto *chip = new QPushButton(emo.name, chipContainer);
        chip->setStyleSheet(
            "QPushButton { background: transparent; border: 1px solid rgba(255,255,255,0.18);"
            "border-radius: 14px; padding: 4px 10px; color: rgba(255,255,255,0.45); font-size: 12px; }"
            "QPushButton:hover { background: rgba(255,255,255,0.12); color: white; border-color: rgba(255,255,255,0.4); }");
        chip->setCursor(Qt::PointingHandCursor);
        connect(chip, &QPushButton::clicked, this, [this, name = emo.name]() {
            m_engine->setEmotion(name);
        });
        chipLayout->addWidget(chip);
    }
    flow->addWidget(chipContainer);

    scroll->setWidget(inner);
    auto *panelLayout = new QVBoxLayout(m_emotionPanel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->addWidget(scroll);

    m_emotionPanel->show();
    m_emotionPanel->raise();
}

} // namespace Gidit
