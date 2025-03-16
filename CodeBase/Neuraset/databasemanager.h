#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QTime>
#include <QMap>
#include <QVariant>
#include <QSqlError>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    void addSessionData(const QDateTime &dateTime, const QString &sessionInfo);
    QList<QVariantMap> getSessionData();
    QList<QDateTime> getDateTimeValues();


private:
    QSqlDatabase db;
    void initializeDatabase();
};

#endif // DATABASEMANAGER_H
