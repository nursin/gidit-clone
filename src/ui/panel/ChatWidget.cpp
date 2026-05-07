#include "ui/panel/ChatWidget.h"
#include "utils/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

namespace Gidit {

ChatWidget::ChatWidget(QWidget *parent) : QWidget(parent) {
    auto *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // Chat messages scroll
    m_scroll = new QScrollArea;
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setStyleSheet("background: transparent;");

    m_chatArea = new QWidget;
    m_chatLayout = new QVBoxLayout(m_chatArea);
    m_chatLayout->setContentsMargins(16, 16, 16, 16);
    m_chatLayout->setSpacing(10);
    m_chatLayout->addStretch();
    m_scroll->setWidget(m_chatArea);
    vl->addWidget(m_scroll, 1);

    // Quick actions
    auto *quickActions = new QWidget;
    quickActions->setStyleSheet(QString("border-top: 1px solid %1;").arg(Colors::BorderLight));
    auto *qal = new QHBoxLayout(quickActions);
    qal->setContentsMargins(12, 12, 12, 12);
    qal->setSpacing(6);

    for (const auto &chip : QVector<QPair<QString,QString>>{
        {"📋", "Summarize my day"}, {"🧠", "Plan deep work"},
        {"💰", "Budget check"}, {"🤔", "Forgetting?"}
    }) {
        auto *btn = new QPushButton(chip.first + " " + chip.second, quickActions);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton { padding: 5px 10px; border-radius: 20px; border: 1px solid %1; "
            "background: %2; font-size: 11px; font-weight: 500; color: %3; }"
            "QPushButton:hover { border-color: %4; background: %5; color: %4; }"
        ).arg(Colors::Border, Colors::Surface, Colors::TextSecondary, Colors::Primary, Colors::PrimarySoft));
        connect(btn, &QPushButton::clicked, this, [this, text = chip.second]() { emit messageSent(text); });
        qal->addWidget(btn);
    }
    vl->addWidget(quickActions);

    // Input area
    auto *inputArea = new QWidget;
    inputArea->setStyleSheet(QString("border-top: 1px solid %1;").arg(Colors::BorderLight));
    auto *ial = new QHBoxLayout(inputArea);
    ial->setContentsMargins(12, 12, 12, 12);
    ial->setSpacing(8);

    m_input = new QLineEdit;
    m_input->setPlaceholderText("Ask Gidit AI...");
    m_input->setStyleSheet(QString(
        "QLineEdit { padding: 10px 14px; border-radius: 10px; border: 1px solid %1; "
        "background: %2; font-size: 13px; }"
        "QLineEdit:focus { border-color: %3; background: %4; }"
    ).arg(Colors::Border, Colors::SurfaceSunken, Colors::Primary, Colors::Surface));
    connect(m_input, &QLineEdit::returnPressed, this, [this]() {
        QString text = m_input->text().trimmed();
        if (!text.isEmpty()) { emit messageSent(text); m_input->clear(); }
    });
    ial->addWidget(m_input, 1);

    auto *sendBtn = new QPushButton("↑");
    sendBtn->setFixedSize(40, 40);
    sendBtn->setCursor(Qt::PointingHandCursor);
    sendBtn->setStyleSheet(QString(
        "QPushButton { border-radius: 10px; border: none; background: %1; color: white; font-size: 16px; }"
        "QPushButton:hover { background: %2; }"
    ).arg(Colors::Primary, Colors::PrimaryDark));
    connect(sendBtn, &QPushButton::clicked, this, [this]() {
        QString text = m_input->text().trimmed();
        if (!text.isEmpty()) { emit messageSent(text); m_input->clear(); }
    });
    ial->addWidget(sendBtn);
    vl->addWidget(inputArea);

    // Initial welcome message
    addAIMessage("Good morning! <b>3 urgent tasks</b> need attention. Your <b>12-day coding streak</b> is your best this month. Sprint planning at 11:00.",
                 "calendar → 5 events · health → 7h sleep · tasks → 3 urgent");
}

void ChatWidget::addAIMessage(const QString &text, const QString &toolBadge) {
    auto *msg = new QWidget(m_chatArea);
    msg->setMaximumWidth(300);
    msg->setStyleSheet(QString(
        "background: %1; border: 1px solid %2; border-radius: 12px; border-bottom-left-radius: 4px;"
    ).arg(Colors::PrimarySoft, Colors::PrimaryMed));

    auto *ml = new QVBoxLayout(msg);
    ml->setContentsMargins(10, 10, 10, 10);
    ml->setSpacing(4);

    auto *sender = new QLabel("GIDIT AI");
    sender->setFont(Theme::labelFont(7));
    sender->setStyleSheet(QString("color: %1; font-weight: 700;").arg(Colors::Primary));
    ml->addWidget(sender);

    auto *body = new QLabel(text);
    body->setWordWrap(true);
    body->setTextFormat(Qt::RichText);
    body->setStyleSheet(QString("color: %1; font-size: 13px; line-height: 1.55;").arg(Colors::Text));
    ml->addWidget(body);

    if (!toolBadge.isEmpty()) {
        auto *badge = new QLabel("⚡ " + toolBadge);
        badge->setFont(Theme::monoFont(7));
        badge->setStyleSheet(QString(
            "background: %1; border: 1px solid rgba(51,243,205,0.25); border-radius: 4px; "
            "padding: 3px 8px; color: %2; font-size: 10px;"
        ).arg(Colors::AccentSoft, Colors::AccentDark));
        badge->setWordWrap(true);
        ml->addWidget(badge);
    }

    m_chatLayout->insertWidget(m_chatLayout->count() - 1, msg, 0, Qt::AlignLeft);
    QTimer::singleShot(50, this, [this]() { m_scroll->verticalScrollBar()->setValue(m_scroll->verticalScrollBar()->maximum()); });
}

void ChatWidget::addUserMessage(const QString &text) {
    auto *msg = new QLabel(text);
    msg->setWordWrap(true);
    msg->setMaximumWidth(260);
    msg->setStyleSheet(QString(
        "background: %1; color: %2; padding: 10px 14px; border-radius: 12px; "
        "border-bottom-right-radius: 4px; font-size: 13px;"
    ).arg(Colors::Text, Colors::TextInverse));
    m_chatLayout->insertWidget(m_chatLayout->count() - 1, msg, 0, Qt::AlignRight);
    QTimer::singleShot(50, this, [this]() { m_scroll->verticalScrollBar()->setValue(m_scroll->verticalScrollBar()->maximum()); });
}

} // namespace Gidit
