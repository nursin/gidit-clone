#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <functional>

namespace Gidit {

struct AgentResponse {
    QString text;
    QString toolCall;   // e.g. "calendar_tool → 5 events"
};

class AgentEngine : public QObject {
    Q_OBJECT
public:
    explicit AgentEngine(QObject *parent = nullptr);

    // Process a user message and return a response
    AgentResponse processMessage(const QString &message);

    // Check if a message is a mode-switch command
    // Returns: "manual", "autopilot", or "" (not a command)
    QString checkModeCommand(const QString &message);

signals:
    void responseReady(const AgentResponse &response);

private:
    void initResponses();
    QMap<QString, AgentResponse> m_responses;
};

} // namespace Gidit
