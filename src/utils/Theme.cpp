// ══════════════════════════════════════════════════════════════
//  Gidit — Theme Implementation
// ══════════════════════════════════════════════════════════════
#include "utils/Theme.h"
#include <QFontDatabase>
#include <QApplication>

namespace Gidit {

QFont Theme::headingFont(int pointSize) {
    QFont f("Plus Jakarta Sans", pointSize, QFont::Bold);
    f.setLetterSpacing(QFont::AbsoluteSpacing, -0.5);
    return f;
}

QFont Theme::bodyFont(int pointSize) {
    QFont f("Plus Jakarta Sans", pointSize, QFont::Normal);
    return f;
}

QFont Theme::monoFont(int pointSize) {
    QFont f("DM Mono", pointSize, QFont::Normal);
    f.setStyleHint(QFont::Monospace);
    return f;
}

QFont Theme::labelFont(int pointSize) {
    QFont f("Plus Jakarta Sans", pointSize, QFont::DemiBold);
    f.setLetterSpacing(QFont::AbsoluteSpacing, 1.0);
    f.setCapitalization(QFont::AllUppercase);
    return f;
}

QPalette Theme::lightPalette() {
    QPalette p;
    p.setColor(QPalette::Window,          QColor(Colors::Background));
    p.setColor(QPalette::WindowText,      QColor(Colors::Text));
    p.setColor(QPalette::Base,            QColor(Colors::Surface));
    p.setColor(QPalette::AlternateBase,   QColor(Colors::SurfaceSunken));
    p.setColor(QPalette::ToolTipBase,     QColor(Colors::Surface));
    p.setColor(QPalette::ToolTipText,     QColor(Colors::Text));
    p.setColor(QPalette::Text,            QColor(Colors::Text));
    p.setColor(QPalette::PlaceholderText, QColor(Colors::TextTertiary));
    p.setColor(QPalette::Button,          QColor(Colors::Surface));
    p.setColor(QPalette::ButtonText,      QColor(Colors::Text));
    p.setColor(QPalette::Highlight,       QColor(Colors::Primary));
    p.setColor(QPalette::HighlightedText, QColor(Colors::TextInverse));
    p.setColor(QPalette::Mid,             QColor(Colors::Border));
    p.setColor(QPalette::Light,           QColor(Colors::BorderLight));
    return p;
}

QString Theme::dashboardStyleSheet() {
    return QStringLiteral(R"(
        /* ── Global ── */
        QWidget {
            font-family: "Plus Jakarta Sans", -apple-system, "Segoe UI", sans-serif;
            font-size: 13px;
            color: %1;
        }

        /* ── Scrollbar ── */
        QScrollBar:vertical {
            width: 5px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: %2;
            border-radius: 2px;
            min-height: 30px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
            height: 0;
        }
        QScrollBar:horizontal {
            height: 5px;
            background: transparent;
        }
        QScrollBar::handle:horizontal {
            background: %2;
            border-radius: 2px;
        }

        /* ── Push Buttons ── */
        QPushButton {
            background: %3;
            border: 1px solid %2;
            border-radius: 8px;
            padding: 7px 14px;
            font-weight: 600;
            font-size: 12px;
            color: %4;
        }
        QPushButton:hover {
            border-color: %5;
            color: %5;
        }
        QPushButton:pressed {
            background: %6;
        }
        QPushButton#primaryButton {
            background: %5;
            border-color: %5;
            color: white;
        }
        QPushButton#primaryButton:hover {
            background: %7;
        }

        /* ── Line Edits ── */
        QLineEdit {
            background: %8;
            border: 1px solid transparent;
            border-radius: 8px;
            padding: 9px 16px;
            font-size: 13px;
            color: %1;
            selection-background-color: %9;
        }
        QLineEdit:focus {
            background: %3;
            border-color: %5;
        }

        /* ── Tool Tips ── */
        QToolTip {
            background: %1;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 5px 10px;
            font-size: 12px;
        }

        /* ── Menu ── */
        QMenu {
            background: %3;
            border: 1px solid %2;
            border-radius: 8px;
            padding: 4px;
        }
        QMenu::item {
            padding: 6px 24px 6px 12px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background: %6;
        }
    )")
    .arg(Colors::Text)           // %1
    .arg(Colors::Border)         // %2
    .arg(Colors::Surface)        // %3
    .arg(Colors::TextSecondary)  // %4
    .arg(Colors::Primary)        // %5
    .arg(Colors::SurfaceSunken)  // %6
    .arg(Colors::PrimaryDark)    // %7
    .arg(Colors::SurfaceSunken)  // %8
    .arg(Colors::PrimaryMed);    // %9
}

QString Theme::autopilotStyleSheet() {
    return QStringLiteral(R"(
        QWidget#autopilotOverlay {
            background: transparent;
        }
        QPushButton#faceNavItem {
            background: transparent;
            border: 1px solid transparent;
            border-radius: 12px;
            color: rgba(255,255,255,0.45);
            font-size: 19px;
            padding: 10px;
        }
        QPushButton#faceNavItem:hover {
            background: rgba(255,255,255,0.12);
            border-color: rgba(255,255,255,0.18);
            color: white;
        }
        QLineEdit#chatInput {
            background: transparent;
            border: 1px solid rgba(255,255,255,0.18);
            border-radius: 22px;
            padding: 12px 18px;
            color: white;
            font-size: 14px;
        }
        QLineEdit#chatInput:focus {
            border-color: rgba(255,255,255,0.42);
            background: rgba(255,255,255,0.025);
        }
        QPushButton#chatSend, QPushButton#micBtn {
            background: transparent;
            border: 1px solid rgba(255,255,255,0.18);
            border-radius: 21px;
            color: rgba(255,255,255,0.45);
            font-size: 15px;
        }
        QPushButton#chatSend:hover, QPushButton#micBtn:hover {
            border-color: rgba(255,255,255,0.5);
            color: white;
            background: rgba(255,255,255,0.12);
        }
    )");
}

} // namespace Gidit
