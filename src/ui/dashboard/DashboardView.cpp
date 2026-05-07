// ══════════════════════════════════════════════════════════════
//  Gidit — Dashboard View Implementation
// ══════════════════════════════════════════════════════════════
#include "ui/dashboard/DashboardView.h"
#include "ui/sidebar/Sidebar.h"
#include "ui/dashboard/WidgetGrid.h"
#include "ui/panel/AIPanel.h"
#include "ui/face/EmotionEngine.h"
#include "utils/Theme.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QLabel>
#include <QDateTime>
#include <QScrollArea>

namespace Gidit {

DashboardView::DashboardView(EmotionEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine)
{
    setStyleSheet(QString("background: %1;").arg(Colors::Background));

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    createHeader();
    root->addWidget(m_header);

    createMainArea();
}

// ═══════════════════════════════════════════
//  Header Bar
// ═══════════════════════════════════════════
void DashboardView::createHeader() {
    m_header = new QWidget(this);
    m_header->setFixedHeight(Layout::HeaderHeight);
    m_header->setStyleSheet(QString(
        "background: %1; border-bottom: 1px solid %2;"
    ).arg(Colors::Surface, Colors::Border));

    auto *hl = new QHBoxLayout(m_header);
    hl->setContentsMargins(20, 0, 20, 0);
    hl->setSpacing(16);

    // ── Logo ──
    auto *logoBox = new QWidget;
    logoBox->setFixedWidth(180);
    auto *logoLayout = new QHBoxLayout(logoBox);
    logoLayout->setContentsMargins(0, 0, 0, 0);
    logoLayout->setSpacing(10);

    auto *logoIcon = new QLabel("G");
    logoIcon->setFixedSize(32, 32);
    logoIcon->setAlignment(Qt::AlignCenter);
    logoIcon->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #3391F3, stop:1 #33F3CD);"
        "border-radius: 8px; color: white; font-weight: 800; font-size: 16px;");
    logoLayout->addWidget(logoIcon);

    auto *logoText = new QLabel("Gidit");
    logoText->setFont(Theme::headingFont(16));
    logoText->setStyleSheet("color: " + QString(Colors::Text) + "; background: transparent; border: none;");
    logoLayout->addWidget(logoText);
    logoLayout->addStretch();
    hl->addWidget(logoBox);

    // ── Search Bar ──
    m_searchInput = new QLineEdit;
    m_searchInput->setPlaceholderText("⌕  Search widgets, tasks, notes, anything...");
    m_searchInput->setMaximumWidth(480);
    m_searchInput->setMinimumWidth(200);
    hl->addWidget(m_searchInput, 1);

    // ── Shortcut hint ──
    auto *shortcut = new QLabel("⌘K");
    shortcut->setFont(Theme::monoFont(9));
    shortcut->setFixedSize(32, 20);
    shortcut->setAlignment(Qt::AlignCenter);
    shortcut->setStyleSheet(QString(
        "background: %1; border: 1px solid %2; border-radius: 4px; color: %3; font-size: 10px;"
    ).arg(Colors::Surface, Colors::Border, Colors::TextTertiary));
    hl->addWidget(shortcut);

    hl->addStretch();

    // ── Right actions ──
    auto makeHeaderBtn = [&](const QString &text, const QString &tooltip) -> QPushButton* {
        auto *btn = new QPushButton(text);
        btn->setFixedSize(36, 36);
        btn->setToolTip(tooltip);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton { background: transparent; border: none; border-radius: 8px;"
            "  color: %1; font-size: 16px; }"
            "QPushButton:hover { background: %2; color: %3; }"
        ).arg(Colors::TextSecondary, Colors::SurfaceSunken, Colors::Text));
        return btn;
    };

    hl->addWidget(makeHeaderBtn("🔔", "Notifications"));
    hl->addWidget(makeHeaderBtn("⚙", "Settings"));

    // Avatar
    auto *avatar = new QLabel("B");
    avatar->setFixedSize(32, 32);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #3391F3, stop:1 #33F3CD);"
        "border-radius: 16px; color: white; font-weight: 700; font-size: 12px;");
    avatar->setCursor(Qt::PointingHandCursor);
    hl->addWidget(avatar);
}

// ═══════════════════════════════════════════
//  Main 3-Column Area
// ═══════════════════════════════════════════
void DashboardView::createMainArea() {
    auto *root = qobject_cast<QVBoxLayout *>(layout());

    auto *body = new QWidget(this);
    auto *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // ── Left Sidebar ──
    m_sidebar = new Sidebar(this);
    m_sidebar->setFixedWidth(Layout::SidebarWidth);
    bodyLayout->addWidget(m_sidebar);

    // ── Center: Canvas Wrapper (scrollable widget grid) ──
    auto *centerScroll = new QScrollArea(this);
    centerScroll->setWidgetResizable(true);
    centerScroll->setFrameShape(QFrame::NoFrame);
    centerScroll->setStyleSheet("background: transparent; border: none;");

    auto *centerContent = new QWidget;
    auto *centerLayout = new QVBoxLayout(centerContent);
    centerLayout->setContentsMargins(24, 24, 24, 24);
    centerLayout->setSpacing(24);

    // ── Canvas Header (greeting + controls) ──
    auto *canvasHeader = new QWidget;
    auto *chLayout = new QHBoxLayout(canvasHeader);
    chLayout->setContentsMargins(0, 0, 0, 0);

    auto *greetBox = new QWidget;
    auto *greetLayout = new QVBoxLayout(greetBox);
    greetLayout->setContentsMargins(0, 0, 0, 0);
    greetLayout->setSpacing(4);

    // Time-based greeting
    int hour = QTime::currentTime().hour();
    QString greeting = (hour < 12) ? "Good morning" : (hour < 17) ? "Good afternoon" : "Good evening";

    auto *greetLabel = new QLabel(QString("%1, <span style='color:%2;'>Bobby</span>")
                                  .arg(greeting, Colors::Primary));
    greetLabel->setFont(Theme::headingFont(20));
    greetLabel->setTextFormat(Qt::RichText);
    greetLabel->setStyleSheet("background: transparent; border: none;");
    greetLayout->addWidget(greetLabel);

    auto *dateLabel = new QLabel(QDate::currentDate().toString("dddd, MMMM d, yyyy"));
    dateLabel->setStyleSheet(QString("color: %1; font-size: 13px; font-weight: 500; background: transparent; border: none;")
                             .arg(Colors::TextTertiary));
    greetLayout->addWidget(dateLabel);
    chLayout->addWidget(greetBox);
    chLayout->addStretch();

    // Controls
    auto *ctrlBox = new QWidget;
    auto *ctrlLayout = new QHBoxLayout(ctrlBox);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(8);

    auto *layoutBtn = new QPushButton("⊞ Layout");
    layoutBtn->setCursor(Qt::PointingHandCursor);
    ctrlLayout->addWidget(layoutBtn);

    m_builderBtn = new QPushButton("✦ Builder Mode");
    m_builderBtn->setObjectName("primaryButton");
    m_builderBtn->setCursor(Qt::PointingHandCursor);
    connect(m_builderBtn, &QPushButton::clicked, this, [this]() {
        m_builderMode = !m_builderMode;
        m_builderBtn->setText(m_builderMode ? "✓ Exit Builder" : "✦ Builder Mode");
        if (m_builderMode) {
            m_builderBtn->setStyleSheet(
                "QPushButton { background: #1dd4ac; border: 1px solid #1dd4ac;"
                "  border-radius: 8px; color: white; font-weight: 600; padding: 7px 14px; }"
                "QPushButton:hover { background: #19b895; }");
        } else {
            m_builderBtn->setStyleSheet(""); // Reset to default primary style
            m_builderBtn->setObjectName("primaryButton");
        }
        m_widgetGrid->setBuilderMode(m_builderMode);
    });
    ctrlLayout->addWidget(m_builderBtn);
    chLayout->addWidget(ctrlBox);

    centerLayout->addWidget(canvasHeader);

    // ── Widget Grid ──
    m_widgetGrid = new WidgetGrid(this);
    centerLayout->addWidget(m_widgetGrid);
    centerLayout->addStretch();

    centerScroll->setWidget(centerContent);
    bodyLayout->addWidget(centerScroll, 1);

    // ── Right AI Panel ──
    m_aiPanel = new AIPanel(m_engine, this);
    m_aiPanel->setFixedWidth(Layout::PanelWidth);
    bodyLayout->addWidget(m_aiPanel);

    // Forward autopilot request from AI panel
    connect(m_aiPanel, &AIPanel::requestAutopilotMode, this, &DashboardView::requestAutopilotMode);

    root->addWidget(body, 1);
}

} // namespace Gidit
