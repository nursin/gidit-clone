#include "ui/sidebar/Sidebar.h"
#include "ui/sidebar/BuilderPalette.h"
#include "utils/Theme.h"
#include <QLabel>
#include <QScrollArea>
#include <QProgressBar>

namespace Gidit {

Sidebar::Sidebar(QWidget *parent) : QWidget(parent) {
    setFixedWidth(Layout::SidebarWidth);
    setStyleSheet(QString("background: %1; border-right: 1px solid %2;")
                  .arg(Colors::Surface, Colors::Border));

    auto *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(0);

    // Workspace section
    auto *workLabel = new QLabel("WORKSPACE");
    workLabel->setFont(Theme::labelFont(7));
    workLabel->setStyleSheet(QString("color: %1; padding: 16px 22px 8px;").arg(Colors::TextTertiary));
    vl->addWidget(workLabel);

    addNavItem(vl, "◉", "Dashboard", true, "3");
    addNavItem(vl, "◫", "Builder Mode");
    addNavItem(vl, "⊞", "Templates");

    // Modules section
    auto *modLabel = new QLabel("MODULES");
    modLabel->setFont(Theme::labelFont(7));
    modLabel->setStyleSheet(QString("color: %1; padding: 16px 22px 8px;").arg(Colors::TextTertiary));
    vl->addWidget(modLabel);

    addNavItem(vl, "☑", "Tasks");
    addNavItem(vl, "▦", "Calendar");
    addNavItem(vl, "♥", "Health");
    addNavItem(vl, "$", "Finances");
    addNavItem(vl, "≡", "Documents");

    // Add Widgets label
    auto *addLabel = new QLabel("ADD WIDGETS");
    addLabel->setFont(Theme::labelFont(7));
    addLabel->setStyleSheet(QString("color: %1; padding: 16px 22px 8px;").arg(Colors::TextTertiary));
    vl->addWidget(addLabel);

    // Builder palette (scrollable)
    m_palette = new BuilderPalette(this);
    auto *scroll = new QScrollArea;
    scroll->setWidget(m_palette);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent;");
    vl->addWidget(scroll, 1);

    // Footer with storage bar
    auto *footer = new QWidget;
    footer->setStyleSheet(QString("border-top: 1px solid %1;").arg(Colors::BorderLight));
    auto *fl = new QVBoxLayout(footer);
    fl->setContentsMargins(14, 12, 14, 12);

    auto *storageBar = new QProgressBar;
    storageBar->setRange(0, 100);
    storageBar->setValue(38);
    storageBar->setTextVisible(false);
    storageBar->setFixedHeight(4);
    storageBar->setStyleSheet(QString(
        "QProgressBar { background: %1; border: none; border-radius: 2px; }"
        "QProgressBar::chunk { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 %2, stop:1 %3); border-radius: 2px; }"
    ).arg(Colors::SurfaceSunken, Colors::Primary, Colors::Accent));
    fl->addWidget(storageBar);

    auto *storageText = new QLabel("Local storage: 1.2 GB / 3.2 GB used");
    storageText->setStyleSheet(QString("color: %1; font-size: 11px;").arg(Colors::TextTertiary));
    fl->addWidget(storageText);
    vl->addWidget(footer);
}

QPushButton *Sidebar::addNavItem(QVBoxLayout *layout, const QString &icon, const QString &label,
                                  bool active, const QString &badge) {
    auto *btn = new QPushButton(this);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedHeight(38);

    auto *hl = new QHBoxLayout(btn);
    hl->setContentsMargins(22, 0, 14, 0);
    hl->setSpacing(10);

    auto *iconLbl = new QLabel(icon, btn);
    iconLbl->setFixedWidth(20);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet("font-size: 15px;");
    hl->addWidget(iconLbl);

    auto *textLbl = new QLabel(label, btn);
    textLbl->setStyleSheet(QString("font-size: 13px; font-weight: 500; color: %1;")
                           .arg(active ? Colors::Primary : Colors::TextSecondary));
    hl->addWidget(textLbl, 1);

    if (!badge.isEmpty()) {
        auto *b = new QLabel(badge, btn);
        b->setFont(Theme::monoFont(7));
        b->setStyleSheet(QString("background: %1; color: white; font-size: 10px; font-weight: 700; "
                                 "padding: 1px 7px; border-radius: 10px;").arg(Colors::Primary));
        hl->addWidget(b);
    }

    QString activeCss = active ?
        QString("QPushButton { background: %1; border: none; border-radius: %2px; }")
        .arg(Colors::PrimarySoft).arg(Spacing::RadiusSm) :
        QString("QPushButton { background: transparent; border: none; border-radius: %1px; }"
                "QPushButton:hover { background: %2; }").arg(Spacing::RadiusSm).arg(Colors::SurfaceHover);
    btn->setStyleSheet(activeCss);

    connect(btn, &QPushButton::clicked, this, [this, label, btn]() {
        for (auto *b : m_navItems) b->setStyleSheet(
            QString("QPushButton { background: transparent; border: none; border-radius: %1px; }"
                    "QPushButton:hover { background: %2; }").arg(Spacing::RadiusSm).arg(Colors::SurfaceHover));
        btn->setStyleSheet(QString("QPushButton { background: %1; border: none; border-radius: %2px; }")
                           .arg(Colors::PrimarySoft).arg(Spacing::RadiusSm));
        emit navItemClicked(label);
    });

    m_navItems.append(btn);
    layout->addWidget(btn);
    return btn;
}

} // namespace Gidit
