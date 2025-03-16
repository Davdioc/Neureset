#include "datetimemanager.h"

datetimemanager::datetimemanager(QObject *parent)
    : QObject(parent), currentDateTime(QDateTime::currentDateTime()) {}

void datetimemanager::setDateTimeFromUI(const QDateTime &dateTime) {
    setCurrentDateTime(dateTime);
}

void datetimemanager::updateToCurrentLocalTime() {
    setCurrentDateTime(QDateTime::currentDateTime());
}

void datetimemanager::setCurrentDateTime(const QDateTime &value) {
    if (currentDateTime != value) {
        currentDateTime = value;
        emit dateTimeChanged(currentDateTime);
    }
}
