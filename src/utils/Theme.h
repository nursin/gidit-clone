// ══════════════════════════════════════════════════════════════
//  Gidit — Theme System
//  Centralized colors, fonts, spacing, and QSS generation
// ══════════════════════════════════════════════════════════════
#pragma once

#include <QColor>
#include <QFont>
#include <QString>
#include <QPalette>

namespace Gidit {

// ── Brand Colors ──
namespace Colors {
    // Primary
    constexpr auto Primary       = "#3391F3";
    constexpr auto PrimarySoft   = "#3391F315";
    constexpr auto PrimaryMed    = "#3391F330";
    constexpr auto PrimaryDark   = "#1a6fd4";

    // Accent
    constexpr auto Accent        = "#33F3CD";
    constexpr auto AccentSoft    = "#33F3CD18";
    constexpr auto AccentDark    = "#1dd4ac";

    // Surfaces (Light theme)
    constexpr auto Background    = "#F0F5FA";
    constexpr auto Surface       = "#FFFFFF";
    constexpr auto SurfaceSunken = "#E8EFF6";
    constexpr auto SurfaceHover  = "#F5F8FC";

    // Borders
    constexpr auto Border        = "#D8E2ED";
    constexpr auto BorderLight   = "#E8EFF6";

    // Text
    constexpr auto Text          = "#1A2332";
    constexpr auto TextSecondary = "#5A6B80";
    constexpr auto TextTertiary  = "#8B9BB0";
    constexpr auto TextInverse   = "#FFFFFF";

    // Semantic
    constexpr auto Success       = "#22C55E";
    constexpr auto Warning       = "#F59E0B";
    constexpr auto Danger        = "#EF4444";
    constexpr auto Info          = "#3391F3";

    // Face / Dark theme (Autopilot)
    constexpr auto FaceBg        = "#000000";
    constexpr auto FaceWhite     = "#FFFFFF";
    constexpr auto FaceDim       = "#FFFFFF73";
    constexpr auto FaceFaint     = "#FFFFFF1F";
    constexpr auto FaceBorder    = "#FFFFFF2E";
}

// ── Spacing Constants ──
namespace Spacing {
    constexpr int XS     = 4;
    constexpr int SM     = 8;
    constexpr int MD     = 12;
    constexpr int LG     = 16;
    constexpr int XL     = 24;
    constexpr int XXL    = 32;
    constexpr int Radius = 12;
    constexpr int RadiusSm = 8;
    constexpr int RadiusXs = 6;
}

// ── Layout Constants ──
namespace Layout {
    constexpr int HeaderHeight  = 56;
    constexpr int SidebarWidth  = 260;
    constexpr int PanelWidth    = 340;
    constexpr int FaceSidebarW  = 64;
}

class Theme {
public:
    // Get the global application stylesheet (Dashboard / Light mode)
    static QString dashboardStyleSheet();

    // Get the dark stylesheet for Autopilot overlays
    static QString autopilotStyleSheet();

    // Build QPalette for the light theme
    static QPalette lightPalette();

    // Get font presets
    static QFont headingFont(int pointSize = 14);
    static QFont bodyFont(int pointSize = 10);
    static QFont monoFont(int pointSize = 10);
    static QFont labelFont(int pointSize = 9);

private:
    Theme() = default;
};

} // namespace Gidit
