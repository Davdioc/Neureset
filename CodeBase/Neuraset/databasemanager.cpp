#include "databasemanager.h"
#include <QDebug>
#include <QVariant>

DatabaseManager::DatabaseManager() {
    initializeDatabase();
}

DatabaseManager::~DatabaseManager() {
    db.close();
}

void DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sessionInfo.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS sessions ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "datetime DATETIME, "
                    "session_info TEXT)")) {
        qDebug() << "Failed to create table:" << query.lastError();
    }
}

void DatabaseManager::addSessionData(const QDateTime &dateTime, const QString &sessionInfo) {
    QSqlQuery query;
    query.prepare("INSERT INTO sessions (datetime, session_info) VALUES (?, ?)");
    query.addBindValue(dateTime);
    query.addBindValue(sessionInfo);

    if (!query.exec()) {
        qDebug() << "addSessionData error:" << query.lastError();
    }
}

QList<QVariantMap> DatabaseManager::getSessionData() {
    QList<QVariantMap> results;
    QSqlQuery query("SELECT datetime, session_info FROM sessions");
    while (query.next()) {
        QVariantMap row;
        row["datetime"] = query.value(0).toDate();
        row["session_info"] = query.value(1).toString();
        results.append(row);
    }
    return results;
}

QList<QDateTime> DatabaseManager::getDateTimeValues(){
    QList<QDateTime> results;
    QSqlQuery query("SELECT datetime FROM sessions");

    while(query.next()){
        QDateTime dateTime = query.value(0).toDateTime();
        results.append(dateTime);
    }

    return results;
}

