#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"
#include "serverselection.h"
#include "engineselection.h"
#include "versionselection.h"
#include "download.h"

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
    void folderError(QString);
    void bytesDownloaded(qint64, qint64, QString);
    QString getServerUrlById(int);
    void downloadFiles();
    void fileDownloaded();
    void startDlThread();

signals:
    void dlFile(QString, QString, int);

private:
    Ui::UrTUpdater *ui;

    bool updateInProgress;

    QString updaterPath;
    QString versionNumber;
    QString releaseDate;

    QString updaterVersion;
    QString password;

    QThread* dlThread;
    Download* dl;
    bool threadStarted;

    int downloadServer;
    int gameEngine;
    int currentVersion;
    bool configFileExists;

    int nbFilesToDl;
    int nbFilesDled;
    int downloadedBytes;
    QProgressBar* dlBar;
    QLabel* dlText;
    QLabel* dlSpeed;

    QPushButton* playButton;
    QPushButton* changelogButton;

    QList<fileInfo_s>       filesToDownload;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;
    QList<versionInfo_s>    versionsList;
    QList<QString>          newsList;

    fileInfo_s currentFile;
};

#endif // URTUPDATER_H
