QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    countdowntimer.cpp \
    databasemanager.cpp \
    devpanel.cpp \
    eegwaveformgenerator.cpp \
    electrode.cpp \
    lensTest.cpp \
    main.cpp \
    mainUI.cpp \
    maincontrol.cpp \
    pcui.cpp \
    qcustomplot.cpp \
    timer.cpp

HEADERS += \
    countdowntimer.h \
    databasemanager.h \
    defs.h \
    devpanel.h \
    eegwaveformgenerator.h \
    electrode.h \
    lensTest.h \
    mainUI.h \
    maincontrol.h \
    pcui.h \
    qcustomplot.h \
    timer.h

FORMS += \
    mainUI.ui

TRANSLATIONS += \

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
