#include "app/Settings.h"

namespace Gidit {

Settings::Settings() : m_settings("Gidit", "Gidit") {}

Settings &Settings::instance() { static Settings s; return s; }

QByteArray Settings::windowGeometry() const { return m_settings.value("window/geometry").toByteArray(); }
void Settings::setWindowGeometry(const QByteArray &g) { m_settings.setValue("window/geometry", g); }

QString Settings::lastMode() const { return m_settings.value("mode/last", "autopilot").toString(); }
void Settings::setLastMode(const QString &m) { m_settings.setValue("mode/last", m); }

QString Settings::dashboardLayout() const { return m_settings.value("dashboard/layout").toString(); }
void Settings::setDashboardLayout(const QString &j) { m_settings.setValue("dashboard/layout", j); }

bool Settings::autoEmotions() const { return m_settings.value("face/autoEmotions", true).toBool(); }
void Settings::setAutoEmotions(bool on) { m_settings.setValue("face/autoEmotions", on); }

bool Settings::faceTracking() const { return m_settings.value("face/tracking", true).toBool(); }
void Settings::setFaceTracking(bool on) { m_settings.setValue("face/tracking", on); }

} // namespace Gidit
