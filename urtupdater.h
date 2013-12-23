#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"
#include "serverselection.h"
#include "engineselection.h"

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
    void parseLocalConfig();
    void saveLocalConfig();
    void checkDownloadServer();
    void checkGameEngine();
    void setDownloadServer(int);
    void setEngine(int);

private:
    Ui::UrTUpdater *ui;

    QString updaterPath;
    QString versionNumber;
    QString releaseDate;

    QString updaterVersion;

    int downloadServer;
    int gameEngine;
    bool configFileExists;

    QList<fileInfo_s>       filesToDownload;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;
};

#endif // URTUPDATER_H
