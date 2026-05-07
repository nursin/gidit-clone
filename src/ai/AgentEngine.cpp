#include "ai/AgentEngine.h"
#include <QRegularExpression>

namespace Gidit {

AgentEngine::AgentEngine(QObject *parent) : QObject(parent) { initResponses(); }

void AgentEngine::initResponses() {
    m_responses["schedule"] = {"You have 5 events today. Next free slot is 11:30-12:45.", "calendar_tool → 5 events"};
    m_responses["meeting"] = {"Thursday 2:00 PM works for the whole team. Send invites?", "calendar_tool.find_slot() → Thu 2:00 PM"};
    m_responses["budget"] = {"$3,247 / $4,500 budget. $1,253 remaining. Groceries $892, Dining $412.", "finance_tool.summary() → 47 txns"};
    m_responses["health"] = {"Sleep: 7h 12m. Steps: 6,482. Resting HR: 64 bpm. Water: 4/8 glasses.", "health_tool.metrics() → 4 tracked"};
    m_responses["forget"] = {"Mom's birthday in 4 days — she wants a Kindle. Groceries before Wed dinner.", "vector_store.search(\"reminders\") → 3 items"};
    m_responses["summarize"] = {"5 events, 6 tasks (2 high priority), 12-day streak. Sleep solid at 7h 12m.", "calendar + tasks + health → summary"};
    m_responses["deep work"] = {"9-11 AM: CRUSH parser. 1-3 PM: Qt migration. DND enabled for both blocks.", "calendar.block() + notifications.snooze()"};
}

QString AgentEngine::checkModeCommand(const QString &message) {
    QString lower = message.toLower().trimmed();
    if (lower.contains("manual control")) return "manual";
    if (lower.contains("autopilot"))      return "autopilot";
    return "";
}

AgentResponse AgentEngine::processMessage(const QString &message) {
    QString lower = message.toLower();

    for (auto it = m_responses.constBegin(); it != m_responses.constEnd(); ++it) {
        if (lower.contains(it.key())) return it.value();
    }

    // Default RAG-style response
    return {
        QString("Searched your knowledge base for \"%1\". Found relevant context.").arg(message.left(40)),
        QString("vector_store.search(\"%1\") → 3 chunks").arg(message.left(20))
    };
}

} // namespace Gidit
