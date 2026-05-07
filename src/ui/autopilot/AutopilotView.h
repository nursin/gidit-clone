#pragma once
#include <QWidget>

namespace Gidit {
class EmotionEngine;
class FaceCanvas;
class FaceSidebar;
class ChatBar;

class AutopilotView : public QWidget {
    Q_OBJECT
public:
    explicit AutopilotView(EmotionEngine *engine, QWidget *parent = nullptr);

signals:
    void requestManualMode();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    EmotionEngine *m_engine;
    FaceCanvas    *m_faceCanvas = nullptr;
    FaceSidebar   *m_sidebar = nullptr;
    ChatBar       *m_chatBar = nullptr;
    QWidget       *m_modeLabel = nullptr;
};
} // namespace Gidit
