#pragma once
#include <QWidget>
#include <QPushButton>

namespace Gidit {
class EmotionEngine;

class FaceSidebar : public QWidget {
    Q_OBJECT
public:
    explicit FaceSidebar(EmotionEngine *engine, QWidget *parent = nullptr);
    void toggle();

private:
    void createEmotionPanel();
    EmotionEngine *m_engine;
    bool m_visible = false;
    QWidget *m_emotionPanel = nullptr;
};
} // namespace Gidit
