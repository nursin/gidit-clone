#include "ui/panel/AIPanel.h"
#include "ui/face/FaceCanvas.h"
#include "ui/face/EmotionEngine.h"
#include "ui/panel/ChatWidget.h"
#include "utils/Theme.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Gidit {

AIPanel::AIPanel(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    setFixedWidth(Layout::PanelWidth);
    setStyleSheet(QString("background: %1; border-left: 1px solid %2;")
                  .arg(Colors::Surface, Colors::Border));

    auto *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // Header
    auto *header = new QWidget;
    header->setStyleSheet(QString("border-bottom: 1px solid %1;").arg(Colors::BorderLight));
    auto *hl = new QHBoxLayout(header);
    hl->setContentsMargins(16, 14, 16, 14);
    hl->setSpacing(10);

    auto *iconW = new QLabel("✦", header);
    iconW->setFixedSize(28, 28);
    iconW->setAlignment(Qt::AlignCenter);
    iconW->setStyleSheet(QString(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 %1, stop:1 %2); "
        "border-radius: 8px; color: white; font-size: 14px; font-weight: 800;"
    ).arg(Colors::Primary, Colors::Accent));
    hl->addWidget(iconW);

    auto *title = new QLabel("Gidit AI", header);
    title->setStyleSheet(QString("font-weight: 700; font-size: 14px; color: %1;").arg(Colors::Text));
    hl->addWidget(title);
    hl->addStretch();

    // Status dot
    auto *statusDot = new QWidget(header);
    statusDot->setFixedSize(6, 6);
    statusDot->setStyleSheet(QString("background: %1; border-radius: 3px;").arg(Colors::Success));
    hl->addWidget(statusDot);
    auto *statusLabel = new QLabel("Local", header);
    statusLabel->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 600;").arg(Colors::Success));
    hl->addWidget(statusLabel);
    vl->addWidget(header);

    // Mini face canvas
    auto *faceWrapper = new QWidget;
    faceWrapper->setFixedHeight(180);
    faceWrapper->setStyleSheet(QString(
        "background: black; border: 1px solid %1; border-radius: %2px; margin: 12px 16px;"
    ).arg(Colors::BorderLight).arg(Spacing::Radius));
    auto *fwl = new QVBoxLayout(faceWrapper);
    fwl->setContentsMargins(0, 0, 0, 0);

    m_miniCanvas = new FaceCanvas(engine, faceWrapper);
    m_miniCanvas->setMiniMode(true);
    fwl->addWidget(m_miniCanvas);

    // "AUTOPILOT" label overlay
    auto *miniLabel = new QLabel("AUTOPILOT", faceWrapper);
    miniLabel->setStyleSheet("color: rgba(255,255,255,0.4); font-size: 9px; font-weight: 500; letter-spacing: 1px;");
    miniLabel->setFont(Theme::monoFont(7));
    miniLabel->move(8, faceWrapper->height() - 20);
    vl->addWidget(faceWrapper);

    // Tabs
    auto *tabs = new QWidget;
    tabs->setStyleSheet(QString("border-bottom: 1px solid %1;").arg(Colors::BorderLight));
    auto *tl = new QHBoxLayout(tabs);
    tl->setContentsMargins(0, 0, 0, 0);
    tl->setSpacing(0);

    for (const auto &tabName : {"Chat", "Insights", "Actions"}) {
        auto *tab = new QPushButton(tabName, tabs);
        tab->setFlat(true);
        bool active = (QString(tabName) == "Chat");
        tab->setStyleSheet(QString(
            "QPushButton { padding: 10px; font-size: 11px; font-weight: 600; color: %1; "
            "border: none; border-bottom: 2px solid %2; }"
        ).arg(active ? Colors::Primary : Colors::TextTertiary,
              active ? Colors::Primary : "transparent"));
        tl->addWidget(tab);
    }
    vl->addWidget(tabs);

    // Chat widget
    m_chat = new ChatWidget(this);
    vl->addWidget(m_chat, 1);

    // Connect chat for autopilot mode switching
    connect(m_chat, &ChatWidget::messageSent, this, [this](const QString &text) {
        if (text.toLower().contains("autopilot")) {
            emit requestAutopilotMode();
        }
    });
}

} // namespace Gidit
