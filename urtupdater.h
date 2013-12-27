#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"
#include "serverselection.h"
#include "engineselection.h"
#include "versionselection.h"

namespace Ui {
    class UrTUpdater;
}

class UrTUpdater : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrTUpdater(QWidget *parent = 0);
    ~UrTUpdater();
    QNetworkReply *apiAnswer;

public slots:
    void parseAPIAnswer();
    void networkError(QNetworkReply::NetworkError);
    void quit();
    void init();
    QString getPlatform();
    QString getCurrentPath();
    void getManifest(QString query);
    void parseManifest(QString data);
    QString getMd5Sum(QFile* file);
    void serverSelection();
    void engineSelection();
    void versionSelection();
    void parseLocalConfig();
    void saveLocalConfig();
    void checkDownloadServer();
    void checkGameEngine();
    void checkVersion();
    void setDownloadServer(int);
    void setEngine(int);
    void setVersion(int);
    void setPassword(QString);
    void drawNews();

private:
    Ui::UrTUpdater *ui;

    QString updaterPath;
    QString versionNumber;
    QString releaseDate;

    QString updaterVersion;
    QString password;

    int downloadServer;
    int gameEngine;
    int currentVersion;
    bool configFileExists;

    QList<fileInfo_s>       filesToDownload;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;
    QList<versionInfo_s>    versionsList;
    QList<QString>          newsList;
};

#endif // URTUPDATER_H
