// ══════════════════════════════════════════════════════════════
//  Gidit — Emotion Engine Implementation
// ══════════════════════════════════════════════════════════════
#include "ui/face/EmotionEngine.h"
#include <QDateTime>
#include <QRegularExpression>
#include <QRandomGenerator>

namespace Gidit {

EmotionEngine::EmotionEngine(QObject *parent) : QObject(parent) {
    initEmotions();

    // Main animation tick (~60fps)
    m_tickTimer = new QTimer(this);
    m_tickTimer->setInterval(16);
    connect(m_tickTimer, &QTimer::timeout, this, &EmotionEngine::tick);
    m_tickTimer->start();

    // Auto emotion cycling
    m_autoTimer = new QTimer(this);
    m_autoTimer->setInterval(5000 + QRandomGenerator::global()->bounded(5000));
    connect(m_autoTimer, &QTimer::timeout, this, &EmotionEngine::autoEmotionTick);
    m_autoTimer->start();

    // Blink end timer (single-shot)
    m_blinkTimer = new QTimer(this);
    m_blinkTimer->setSingleShot(true);
    m_blinkTimer->setInterval(130);
    connect(m_blinkTimer, &QTimer::timeout, this, &EmotionEngine::endBlink);

    // Emotion restore timer (returns to auto after manual set)
    m_emotionRestore = new QTimer(this);
    m_emotionRestore->setSingleShot(true);
    m_emotionRestore->setInterval(6000);
    connect(m_emotionRestore, &QTimer::timeout, this, [this](){
        m_autoEnabled = true;
    });

    m_lastBlinkMs = QDateTime::currentMSecsSinceEpoch();
}

void EmotionEngine::initEmotions() {
    // Comprehensive emotion set
    auto add = [&](const QString &name, double mc, double mw, double mo,
                   double asy, double le, double re, double eb, double inten) {
        Emotion e;
        e.name = name; e.mouthCurve = mc; e.mouthWidth = mw;
        e.mouthOpen = mo; e.asymmetry = asy;
        e.leftEye = le; e.rightEye = re;
        e.eyebrowRaise = eb; e.intensity = inten;
        m_emotionIndex[name.toLower()] = m_emotions.size();
        m_emotions.append(e);
    };
    //              name            mc    mw    mo    asy   le    re    eb    int
    add("happy",           0.5,  0.7,  0.0,  0.0,  0.9,  0.9,  0.1,  1.0);
    add("ecstatic",        0.7,  0.9,  0.5,  0.0,  1.1,  1.1,  0.3,  1.6);
    add("delighted",       0.55, 0.75, 0.0,  0.0,  1.1,  1.1,  0.25, 1.3);
    add("amused",          0.45, 0.6,  0.0,  0.0,  0.85, 0.85, 0.1,  1.0);
    add("content",         0.3,  0.5,  0.0,  0.0,  0.8,  0.8,  0.0,  0.9);
    add("joyful",          0.6,  0.75, 0.0,  0.0,  1.0,  1.0,  0.2,  1.3);
    add("serene",          0.25, 0.45, 0.0,  0.0,  0.75, 0.75, 0.0,  0.8);
    add("playful",         0.5,  0.6,  0.0,  0.2,  1.0,  0.8,  0.15, 1.2);
    add("loving",          0.45, 0.6,  0.0,  0.0,  0.85, 0.85, 0.12, 1.05);
    add("grateful",        0.42, 0.58, 0.0,  0.0,  0.88, 0.88, 0.13, 1.05);
    add("sad",            -0.5,  0.4,  0.0,  0.0,  0.7,  0.7, -0.2,  0.8);
    add("melancholic",    -0.35, 0.4,  0.0,  0.0,  0.65, 0.65, 0.1,  0.75);
    add("disappointed",   -0.4,  0.35, 0.0,  0.0,  0.6,  0.6, -0.15, 0.85);
    add("hurt",           -0.4,  0.38, 0.0,  0.0,  0.65, 0.65, 0.1,  0.82);
    add("angry",          -0.3,  0.3,  0.0,  0.0,  0.5,  0.5, -0.4,  1.2);
    add("frustrated",     -0.35, 0.4,  0.0,  0.0,  0.7,  0.7, -0.3,  1.2);
    add("furious",        -0.4,  0.35, 0.0,  0.0,  0.4,  0.4, -0.45, 1.4);
    add("surprised",      -0.1,  0.6,  0.5,  0.0,  1.4,  1.4,  0.4,  1.3);
    add("shocked",         0.0,  0.7,  0.7,  0.0,  1.5,  1.5,  0.5,  1.5);
    add("fearful",        -0.2,  0.5,  0.3,  0.0,  1.3,  1.3,  0.3,  1.1);
    add("anxious",        -0.2,  0.35, 0.0,  0.0,  1.1,  1.1,  0.25, 1.1);
    add("worried",        -0.3,  0.4,  0.0,  0.0,  0.9,  0.9,  0.25, 1.0);
    add("confused",        0.1,  0.3,  0.0,  0.3,  0.8,  1.1,  0.2,  0.9);
    add("skeptical",       0.2,  0.35, 0.0,  0.4,  0.6,  1.0, -0.1,  0.85);
    add("mischievous",     0.4,  0.5,  0.0,  0.2,  0.7,  1.0,  0.15, 1.1);
    add("contemplative",   0.05, 0.3,  0.0,  0.0,  0.7,  0.9,  0.1,  0.7);
    add("curious",         0.15, 0.35, 0.0,  0.0,  1.2,  1.2,  0.3,  1.1);
    add("excited",         0.6,  0.8,  0.4,  0.0,  1.2,  1.2,  0.3,  1.4);
    add("relaxed",         0.2,  0.45, 0.0,  0.0,  0.7,  0.7,  0.0,  0.75);
    add("proud",           0.4,  0.5,  0.0,  0.0,  0.9,  0.9,  0.2,  1.2);
    add("confident",       0.35, 0.5,  0.0,  0.0,  1.0,  1.0,  0.05, 1.2);
    add("determined",      0.1,  0.4,  0.0,  0.0,  1.0,  1.0, -0.2,  1.3);
    add("hopeful",         0.3,  0.5,  0.0,  0.0,  1.0,  1.0,  0.2,  1.0);
    add("inspired",        0.4,  0.55, 0.0,  0.0,  1.15, 1.15, 0.25, 1.25);
    add("bored",          -0.1,  0.3,  0.0,  0.0,  0.6,  0.6, -0.1,  0.6);
    add("disgusted",      -0.4,  0.3,  0.0,  0.0,  0.5,  0.5, -0.3,  1.1);
    add("nostalgic",       0.2,  0.4,  0.0,  0.0,  0.75, 0.75, 0.05, 0.85);
    add("embarrassed",     0.2,  0.35, 0.0,  0.0,  0.5,  0.5,  0.2,  0.8);
}

void EmotionEngine::setEmotion(const QString &name) {
    const QString key = name.toLower();
    if (!m_emotionIndex.contains(key)) return;

    const Emotion &e = m_emotions[m_emotionIndex[key]];
    m_targetMouthCurve = e.mouthCurve;
    m_targetMouthWidth = e.mouthWidth;
    m_targetMouthOpen  = e.mouthOpen;
    m_targetAsymmetry  = e.asymmetry;
    m_targetLeftEye    = e.leftEye;
    m_targetRightEye   = e.rightEye;
    m_targetEyebrow    = e.eyebrowRaise;
    m_targetIntensity  = e.intensity;

    // Pause auto-cycling, restore after 6 sec
    m_autoEnabled = false;
    m_emotionRestore->start();

    emit emotionChanged(name);
}

void EmotionEngine::setAutoEmotions(bool enabled) {
    m_autoEnabled = enabled;
}

void EmotionEngine::setFaceTrackingData(double x, double y, double rotation) {
    if (!m_trackingEnabled) return;
    m_targetFaceX     = x;
    m_targetFaceY     = y;
    m_targetRotation  = rotation;
}

void EmotionEngine::triggerBlink() {
    m_preBlinkLeftEye  = m_targetLeftEye;
    m_preBlinkRightEye = m_targetRightEye;
    m_targetLeftEye    = 0.0;
    m_targetRightEye   = 0.0;
    m_blinkTimer->start();
}

void EmotionEngine::endBlink() {
    m_targetLeftEye  = m_preBlinkLeftEye;
    m_targetRightEye = m_preBlinkRightEye;
}

void EmotionEngine::tick() {
    constexpr double s = 0.08;
    constexpr double eyeS = 0.15;

    m_state.faceX         = lerp(m_state.faceX,         m_targetFaceX,     s);
    m_state.faceY         = lerp(m_state.faceY,         m_targetFaceY,     s);
    m_state.faceRotation  = lerp(m_state.faceRotation,  m_targetRotation,  s);
    m_state.leftEyeOpen   = lerp(m_state.leftEyeOpen,   m_targetLeftEye,   eyeS);
    m_state.rightEyeOpen  = lerp(m_state.rightEyeOpen,  m_targetRightEye,  eyeS);
    m_state.mouthCurve    = lerp(m_state.mouthCurve,    m_targetMouthCurve, s);
    m_state.mouthWidth    = lerp(m_state.mouthWidth,    m_targetMouthWidth, s);
    m_state.mouthOpenness = lerp(m_state.mouthOpenness, m_targetMouthOpen,  s);
    m_state.mouthAsymmetry= lerp(m_state.mouthAsymmetry,m_targetAsymmetry,  s);
    m_state.eyebrowRaise  = lerp(m_state.eyebrowRaise,  m_targetEyebrow,    s);
    m_state.intensity     = lerp(m_state.intensity,      m_targetIntensity,  s);

    // Auto blink
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - m_lastBlinkMs > m_blinkIntervalMs) {
        triggerBlink();
        m_lastBlinkMs = now;
        m_blinkIntervalMs = 2000 + QRandomGenerator::global()->bounded(4000);
    }

    emit stateUpdated();
}

void EmotionEngine::autoEmotionTick() {
    if (!m_autoEnabled || m_emotions.isEmpty()) return;

    int idx = QRandomGenerator::global()->bounded(m_emotions.size());
    const Emotion &e = m_emotions[idx];

    m_targetMouthCurve = e.mouthCurve;
    m_targetMouthWidth = e.mouthWidth;
    m_targetMouthOpen  = e.mouthOpen;
    m_targetAsymmetry  = e.asymmetry;
    m_targetLeftEye    = e.leftEye;
    m_targetRightEye   = e.rightEye;
    m_targetEyebrow    = e.eyebrowRaise;
    m_targetIntensity  = e.intensity;

    // Randomize next auto-cycle interval
    m_autoTimer->setInterval(3000 + QRandomGenerator::global()->bounded(7000));
}

QString EmotionEngine::analyzeText(const QString &text) {
    const QString lower = text.toLower();

    // Sentiment rules — returns emotion name
    struct Rule { QRegularExpression re; QString emotion; };
    static const QVector<Rule> rules = {
        { QRegularExpression(R"(\b(love|amazing|wonderful|great|awesome|fantastic|happy|joy)\b)"), "ecstatic" },
        { QRegularExpression(R"(\b(excited|wow|omg|incredible)\b|!{2,})"), "excited" },
        { QRegularExpression(R"(\b(haha|lol|funny|hilarious)\b)"), "amused" },
        { QRegularExpression(R"(\b(sad|sorry|awful|depressed)\b)"), "sad" },
        { QRegularExpression(R"(\b(angry|mad|furious|hate)\b)"), "furious" },
        { QRegularExpression(R"(\b(scared|afraid|terrified)\b)"), "fearful" },
        { QRegularExpression(R"(\b(worried|anxious|nervous)\b)"), "anxious" },
        { QRegularExpression(R"(\?{2,}|\b(confused|what|huh)\b)"), "confused" },
        { QRegularExpression(R"(\b(thinking|think|wonder|hmm)\b)"), "contemplative" },
        { QRegularExpression(R"(\b(calm|relax|chill|peace)\b)"), "serene" },
        { QRegularExpression(R"(\b(bored|boring|meh)\b)"), "bored" },
        { QRegularExpression(R"(\b(proud|achieved)\b)"), "proud" },
        { QRegularExpression(R"(\b(grateful|thank)\b)"), "grateful" },
        { QRegularExpression(R"(\b(inspired|motivated)\b)"), "inspired" },
        { QRegularExpression(R"(\b(surprised|no way)\b)"), "surprised" },
        { QRegularExpression(R"(\b(hopeful|hope)\b)"), "hopeful" },
    };

    for (const auto &rule : rules) {
        if (rule.re.match(lower).hasMatch()) {
            return rule.emotion;
        }
    }
    return "content";
}

} // namespace Gidit
