#ifndef COMMON_H
#define COMMON_H

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
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

#define URT_GAME_NAME       "Urban Terror"
#define URT_GAME_SUBDIR     "q3ut4"
#define URT_API_LINK        "http://barbatos.fr/updater/api.php"
#define URT_UPDATER_CFG     "updater-cfg.xml"

typedef struct {
    QString fileName;
    QString fileMd5;
    QString filePath;
    QString fileSize;
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
} engineInfo_s;

typedef struct {
    int     versionId;
    QString versionName;
} versionInfo_s;

#endif // COMMON_H
