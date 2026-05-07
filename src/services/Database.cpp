#include "services/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QDebug>

namespace Gidit {

Database &Database::instance() { static Database db; return db; }

bool Database::open(const QString &path) {
    QString dbPath = path;
    if (dbPath.isEmpty()) {
        QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dataDir);
        dbPath = dataDir + "/gidit.db";
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qWarning() << "Database open failed:" << m_db.lastError().text();
        return false;
    }
    createTables();
    return true;
}

void Database::close() { m_db.close(); }

void Database::createTables() {
    QSqlQuery q(m_db);
    q.exec("CREATE TABLE IF NOT EXISTS tasks ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "text TEXT NOT NULL,"
           "priority TEXT DEFAULT 'med',"
           "quadrant TEXT DEFAULT '',"
           "completed INTEGER DEFAULT 0,"
           "created_at TEXT DEFAULT (datetime('now'))"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS health_metrics ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "type TEXT NOT NULL,"
           "value REAL NOT NULL,"
           "recorded_at TEXT DEFAULT (datetime('now'))"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS streaks ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT NOT NULL,"
           "completed INTEGER DEFAULT 0,"
           "date TEXT DEFAULT (date('now'))"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS notes ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "content TEXT NOT NULL,"
           "color TEXT DEFAULT 'yellow',"
           "created_at TEXT DEFAULT (datetime('now'))"
           ")");
}

int Database::addTask(const QString &text, const QString &priority, const QString &quadrant) {
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO tasks (text, priority, quadrant) VALUES (?, ?, ?)");
    q.addBindValue(text); q.addBindValue(priority); q.addBindValue(quadrant);
    q.exec();
    return q.lastInsertId().toInt();
}

bool Database::completeTask(int id) {
    QSqlQuery q(m_db);
    q.prepare("UPDATE tasks SET completed = 1 WHERE id = ?");
    q.addBindValue(id);
    return q.exec();
}

QVector<QVariantMap> Database::tasks(const QString &quadrant) {
    QVector<QVariantMap> result;
    QSqlQuery q(m_db);
    if (quadrant.isEmpty()) q.exec("SELECT * FROM tasks ORDER BY created_at DESC");
    else { q.prepare("SELECT * FROM tasks WHERE quadrant = ? ORDER BY created_at DESC"); q.addBindValue(quadrant); q.exec(); }
    while (q.next()) {
        QVariantMap row;
        row["id"] = q.value("id"); row["text"] = q.value("text");
        row["priority"] = q.value("priority"); row["quadrant"] = q.value("quadrant");
        row["completed"] = q.value("completed");
        result.append(row);
    }
    return result;
}

void Database::logHealthMetric(const QString &type, double value) {
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO health_metrics (type, value) VALUES (?, ?)");
    q.addBindValue(type); q.addBindValue(value); q.exec();
}

QVector<QVariantMap> Database::healthMetrics(const QString &type, int days) {
    QVector<QVariantMap> result;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM health_metrics WHERE type = ? AND recorded_at >= datetime('now', ?) ORDER BY recorded_at DESC");
    q.addBindValue(type); q.addBindValue(QString("-%1 days").arg(days)); q.exec();
    while (q.next()) { QVariantMap r; r["value"]=q.value("value"); r["recorded_at"]=q.value("recorded_at"); result.append(r); }
    return result;
}

void Database::logStreak(const QString &name, bool completed) {
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO streaks (name, completed) VALUES (?, ?)");
    q.addBindValue(name); q.addBindValue(completed ? 1 : 0); q.exec();
}

QVector<QVariantMap> Database::streaks() {
    QVector<QVariantMap> result;
    QSqlQuery q(m_db);
    q.exec("SELECT name, SUM(completed) as streak, MAX(date) as last_date FROM streaks GROUP BY name");
    while (q.next()) { QVariantMap r; r["name"]=q.value("name"); r["streak"]=q.value("streak"); r["last_date"]=q.value("last_date"); result.append(r); }
    return result;
}

int Database::addNote(const QString &content, const QString &color) {
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO notes (content, color) VALUES (?, ?)");
    q.addBindValue(content); q.addBindValue(color); q.exec();
    return q.lastInsertId().toInt();
}

QVector<QVariantMap> Database::notes() {
    QVector<QVariantMap> result;
    QSqlQuery q(m_db);
    q.exec("SELECT * FROM notes ORDER BY created_at DESC");
    while (q.next()) { QVariantMap r; r["id"]=q.value("id"); r["content"]=q.value("content"); r["color"]=q.value("color"); result.append(r); }
    return result;
}

} // namespace Gidit
