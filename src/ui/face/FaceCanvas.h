// ══════════════════════════════════════════════════════════════
//  Gidit — Face Canvas (Header)
//  QPainter-based animated face renderer
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QWidget>
#include <QPainter>
#include "ui/face/EmotionEngine.h"

namespace Gidit {

class FaceCanvas : public QWidget {
    Q_OBJECT

public:
    explicit FaceCanvas(EmotionEngine *engine, QWidget *parent = nullptr);

    // When true, reduces face-tracking offset for small canvas
    void setMiniMode(bool mini) { m_miniMode = mini; update(); }
    bool isMiniMode() const { return m_miniMode; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawFace(QPainter &p, int w, int h);
    void drawEye(QPainter &p, double x, double y, double openness, double scale);
    void drawMouth(QPainter &p, double x, double y, double scale);

    EmotionEngine *m_engine;
    bool m_miniMode = false;
};

} // namespace Gidit
