#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QVariantMap>
#include <QVector>

namespace Gidit {

class Database : public QObject {
    Q_OBJECT
public:
    static Database &instance();
    bool open(const QString &path = "");
    void close();

    // Tasks
    int addTask(const QString &text, const QString &priority, const QString &quadrant);
    bool completeTask(int id);
    QVector<QVariantMap> tasks(const QString &quadrant = "");

    // Health metrics
    void logHealthMetric(const QString &type, double value);
    QVector<QVariantMap> healthMetrics(const QString &type, int days = 7);

    // Streaks
    void logStreak(const QString &name, bool completed);
    QVector<QVariantMap> streaks();

    // Notes
    int addNote(const QString &content, const QString &color);
    QVector<QVariantMap> notes();

private:
    Database() = default;
    void createTables();
    QSqlDatabase m_db;
};

} // namespace Gidit
