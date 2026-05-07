#pragma once
#include <QSettings>
#include <QString>

namespace Gidit {

class Settings {
public:
    static Settings &instance();

    // Window geometry
    QByteArray windowGeometry() const;
    void setWindowGeometry(const QByteArray &geo);

    // Last active mode
    QString lastMode() const;       // "autopilot" | "manual"
    void setLastMode(const QString &mode);

    // Dashboard layout (JSON serialized widget positions)
    QString dashboardLayout() const;
    void setDashboardLayout(const QString &json);

    // Face settings
    bool autoEmotions() const;
    void setAutoEmotions(bool on);
    bool faceTracking() const;
    void setFaceTracking(bool on);

private:
    Settings();
    QSettings m_settings;
};

} // namespace Gidit
