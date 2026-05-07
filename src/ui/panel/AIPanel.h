#pragma once
#include <QWidget>

namespace Gidit {
class EmotionEngine;
class FaceCanvas;
class ChatWidget;

class AIPanel : public QWidget {
    Q_OBJECT
public:
    explicit AIPanel(EmotionEngine *engine, QWidget *parent = nullptr);
    ChatWidget *chatWidget() const { return m_chat; }

signals:
    void requestAutopilotMode();

private:
    EmotionEngine *m_engine;
    FaceCanvas    *m_miniCanvas = nullptr;
    ChatWidget    *m_chat = nullptr;
};
} // namespace Gidit
