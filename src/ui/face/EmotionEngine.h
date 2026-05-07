// ══════════════════════════════════════════════════════════════
//  Gidit — Emotion Engine
//  Manages emotion state, transitions, auto-cycling
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QObject>
#include <QTimer>
#include <QString>
#include <QMap>
#include <QVector>
#include <cmath>

namespace Gidit {

struct Emotion {
    QString name;
    double mouthCurve    = 0.3;
    double mouthWidth    = 0.5;
    double mouthOpen     = 0.0;
    double asymmetry     = 0.0;
    double leftEye       = 1.0;
    double rightEye      = 1.0;
    double eyebrowRaise  = 0.0;
    double intensity     = 1.0;
};

// Interpolated face state used for rendering
struct FaceState {
    double faceX         = 0.0;
    double faceY         = 0.0;
    double faceRotation  = 0.0;
    double leftEyeOpen   = 1.0;
    double rightEyeOpen  = 1.0;
    double mouthCurve    = 0.3;
    double mouthWidth    = 0.5;
    double mouthOpenness = 0.0;
    double mouthAsymmetry= 0.0;
    double eyebrowRaise  = 0.0;
    double intensity     = 1.0;
};

class EmotionEngine : public QObject {
    Q_OBJECT

public:
    explicit EmotionEngine(QObject *parent = nullptr);

    // Emotion control
    void setEmotion(const QString &name);
    void setAutoEmotions(bool enabled);
    bool autoEmotionsEnabled() const { return m_autoEnabled; }
    QVector<Emotion> allEmotions() const { return m_emotions; }

    // Face tracking input
    void setFaceTrackingData(double x, double y, double rotation);
    void setTrackingEnabled(bool on) { m_trackingEnabled = on; }

    // State output (smoothly interpolated)
    const FaceState &state() const { return m_state; }

    // Sentiment analysis
    QString analyzeText(const QString &text);

    // Blink
    void triggerBlink();

signals:
    void stateUpdated();
    void emotionChanged(const QString &name);

private slots:
    void tick();
    void autoEmotionTick();
    void endBlink();

private:
    void initEmotions();
    static double lerp(double a, double b, double t) { return a + (b - a) * t; }

    // Current interpolated state
    FaceState m_state;

    // Target values
    double m_targetFaceX = 0, m_targetFaceY = 0, m_targetRotation = 0;
    double m_targetLeftEye = 1, m_targetRightEye = 1;
    double m_targetMouthCurve = 0.3, m_targetMouthWidth = 0.5;
    double m_targetMouthOpen = 0, m_targetAsymmetry = 0;
    double m_targetEyebrow = 0, m_targetIntensity = 1;

    // Pre-blink values (restored after blink)
    double m_preBlinkLeftEye = 1, m_preBlinkRightEye = 1;

    // Emotion database
    QVector<Emotion>      m_emotions;
    QMap<QString, int>    m_emotionIndex;

    // Timers
    QTimer *m_tickTimer      = nullptr;
    QTimer *m_autoTimer      = nullptr;
    QTimer *m_blinkTimer     = nullptr;
    QTimer *m_emotionRestore = nullptr;

    bool m_autoEnabled      = true;
    bool m_trackingEnabled  = true;
    qint64 m_lastBlinkMs    = 0;
    int    m_blinkIntervalMs = 3500;
};

} // namespace Gidit
