/**
  * Urban Terror Updater
  *
  * This software is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This software is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this software; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * @version    4.0.1
  * @author     Charles 'Barbatos' Duprey
  * @email      barbatos@urbanterror.info
  * @copyright  2013/2014 Frozen Sand / 0870760 B.C. Ltd
  */

#ifndef COMMON_H
#define COMMON_H

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrlQuery>
#include <QDomDocument>
#include <QCryptographicHash>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QInputDialog>
#include <QMenuBar>
#include <QXmlStreamWriter>
#include <QLineEdit>
#include <QThread>
#include <QProgressBar>
#include <QTimer>
#include <QTime>
#include <QProcess>
#include <QMovie>
#include <QTextEdit>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QSysInfo>
#include <QNetworkProxyFactory>

#define URT_GAME_NAME           "Urban Terror"
#define URT_GAME_SUBDIR         "q3ut4"
#define URT_API_LINK            "http://www.urbanterror.info/api/updaterv4/"
#define URT_UPDATER_VERSION     "4.0.1"
#define URT_UPDATER_CFG         "updater-cfg.xml"
#define CHANGELOG_EMPTY_TEXT    "Empty."

typedef struct {
    QString fileName;
    QString fileMd5;
    QString filePath;
    QString fileSize;
    QString fileUrl;
} fileInfo_s;

typedef struct {
    int     serverId;
    QString serverName;
    QString serverURL;
    QString serverLocation;
} serverInfo_s;

typedef struct {
    int     engineId;
    QString engineName;
    QString engineDir;
    QString engineLaunchString;
} engineInfo_s;

typedef struct {
    int     versionId;
    QString versionName;
} versionInfo_s;

#endif // COMMON_H
