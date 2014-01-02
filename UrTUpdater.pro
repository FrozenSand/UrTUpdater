#-------------------------------------------------
#
# Project created by QtCreator 2013-12-15T00:08:52
#
#-------------------------------------------------

QT       += core gui network widgets xml

CONFIG   += x86 static

TARGET = UrTUpdater
TEMPLATE = app


SOURCES += main.cpp\
        urtupdater.cpp \
    download.cpp \
    settings.cpp

HEADERS  += urtupdater.h \
    common.h \
    download.h \
    settings.h

FORMS    += urtupdater.ui

RESOURCES += \
    images.qrc

static {
    DEFINES += STATIC
}

RC_FILE = urtupdater.rc
ICON = images/urtupdater.icns
