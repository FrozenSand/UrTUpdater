#
# Urban Terror Updater
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this software; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# @version    4.0.1
# @author     Charles 'Barbatos' Duprey
# @email      barbatos@urbanterror.info
# @copyright  2013/2014 Frozen Sand / 0870760 B.C. Ltd
#

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
