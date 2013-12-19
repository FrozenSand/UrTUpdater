#-------------------------------------------------
#
# Project created by QtCreator 2013-12-15T00:08:52
#
#-------------------------------------------------

QT       += core gui network widgets xml

CONFIG   += x86

TARGET = UrTUpdater
TEMPLATE = app


SOURCES += main.cpp\
        urtupdater.cpp \
    serverselection.cpp \
    engineselection.cpp

HEADERS  += urtupdater.h \
    common.h \
    serverselection.h \
    engineselection.h

FORMS    += urtupdater.ui

RESOURCES += \
    images.qrc
