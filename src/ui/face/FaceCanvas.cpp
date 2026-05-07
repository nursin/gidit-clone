// ══════════════════════════════════════════════════════════════
//  Gidit — Face Canvas Implementation
// ══════════════════════════════════════════════════════════════
#include "ui/face/FaceCanvas.h"
#include <QPainterPath>
#include <cmath>

namespace Gidit {

FaceCanvas::FaceCanvas(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(100, 60);

    // Repaint whenever the engine updates state (~60fps)
    connect(m_engine, &EmotionEngine::stateUpdated, this, QOverload<>::of(&QWidget::update));
}

void FaceCanvas::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    drawFace(p, width(), height());
}

void FaceCanvas::drawFace(QPainter &p, int w, int h) {
    // Black background
    p.fillRect(0, 0, w, h, Qt::black);

    const auto &st = m_engine->state();
    const double trackScale = m_miniMode ? 0.3 : 1.0;

    const double cx = w / 2.0 + st.faceX * trackScale;
    const double cy = h / 2.0 + st.faceY * trackScale;
    const double scale = std::min(w, h) / 800.0;

    p.save();
    p.translate(cx, cy);
    p.rotate(st.faceRotation * 180.0 / M_PI); // QPainter uses degrees

    const double eyeY = -80.0 * scale + st.eyebrowRaise * 30.0 * scale;
    drawEye(p, -120.0 * scale, eyeY, st.leftEyeOpen, scale);
    drawEye(p,  120.0 * scale, eyeY, st.rightEyeOpen, scale);
    drawMouth(p, 0.0, 120.0 * scale, scale);

    p.restore();
}

void FaceCanvas::drawEye(QPainter &p, double x, double y, double openness, double scale) {
    p.save();
    p.translate(x, y);

    QPen pen(Qt::white);
    pen.setWidthF(10.0 * scale * m_engine->state().intensity);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    if (openness > 0.1) {
        const double w = 80.0 * scale;
        const double h = 45.0 * scale * openness;
        QPainterPath path;
        path.moveTo(-w / 2.0, 0);
        path.quadTo(0, -h, w / 2.0, 0);
        p.drawPath(path);
    } else {
        p.drawLine(QPointF(-40.0 * scale, 0), QPointF(40.0 * scale, 0));
    }

    p.restore();
}

void FaceCanvas::drawMouth(QPainter &p, double x, double y, double scale) {
    p.save();
    p.translate(x, y);

    const auto &st = m_engine->state();

    QPen pen(Qt::white);
    pen.setWidthF(10.0 * scale * st.intensity);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    const double w   = 140.0 * scale * st.mouthWidth;
    const double c   = 60.0  * scale * st.mouthCurve;
    const double asy = 20.0  * scale * st.mouthAsymmetry;

    // Upper lip
    QPainterPath upper;
    upper.moveTo(-w / 2.0, asy);
    upper.quadTo(0, c, w / 2.0, -asy);
    p.drawPath(upper);

    // Lower lip (mouth open)
    if (st.mouthOpenness > 0.1) {
        const double oh = 40.0 * scale * st.mouthOpenness;
        QPainterPath lower;
        lower.moveTo(-w / 2.0, asy + 5.0);
        lower.quadTo(0, c + oh, w / 2.0, -asy + 5.0);
        p.drawPath(lower);
    }

    p.restore();
}

} // namespace Gidit
