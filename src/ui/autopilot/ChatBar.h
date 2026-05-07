#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Gidit {
class EmotionEngine;

class ChatBar : public QWidget {
    Q_OBJECT
public:
    explicit ChatBar(EmotionEngine *engine, QWidget *parent = nullptr);
    void toggle();
    bool isOpen() const { return m_visible; }

signals:
    void messageSent(const QString &text);
    void requestManualMode();

private slots:
    void onSend();

private:
    EmotionEngine *m_engine;
    QLineEdit     *m_input = nullptr;
    QPushButton   *m_sendBtn = nullptr;
    QPushButton   *m_micBtn = nullptr;
    bool           m_visible = false;
};
} // namespace Gidit
