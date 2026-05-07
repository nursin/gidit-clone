#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLineEdit>

namespace Gidit {

class ChatWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    void addAIMessage(const QString &text, const QString &toolBadge = "");
    void addUserMessage(const QString &text);

signals:
    void messageSent(const QString &text);

private:
    QWidget     *m_chatArea = nullptr;
    QVBoxLayout *m_chatLayout = nullptr;
    QScrollArea *m_scroll = nullptr;
    QLineEdit   *m_input = nullptr;
};

} // namespace Gidit
