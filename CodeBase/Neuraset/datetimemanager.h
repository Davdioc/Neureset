#ifndef DATETIMEMANAGER_H
#define DATETIMEMANAGER_H

#include <QObject>
#include <QDateTime>

class datetimemanager : public QObject {
    Q_OBJECT

public:
    explicit datetimemanager(QObject *parent = nullptr);
    QDateTime getCurrentDateTime() const;
    void setCurrentDateTime(const QDateTime &value);

public slots:
    void updateToCurrentLocalTime();
    void setDateTimeFromUI(const QDateTime &dateTime);

signals:
    void dateTimeChanged(QDateTime newDateTime);

private:
    QDateTime currentDateTime;
};

#endif // DATETIMEMANAGER_H
