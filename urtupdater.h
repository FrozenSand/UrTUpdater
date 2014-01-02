#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"
#include "download.h"
#include "settings.h"

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
    void quit();
    void init();
    void startDlThread();

    QString getPlatform();
    QString getCurrentPath();

    void parseAPIAnswer();
    void getManifest(QString query);
    void parseManifest(QString data);

    QString getServerUrlById(int);
    QString getEngineLaunchStringById(int id);
    QString getMd5Sum(QFile* file);
    QString getSize(int *bytes);

    void parseLocalConfig();
    void saveLocalConfig();

    void checkDownloadServer();
    void checkGameEngine();
    void checkVersion();

    void openSettings();
    void openHelpPage();

    void setSettings(int, int, int, int);
    void drawNews();

    void bytesDownloaded(qint64, QString, int);
    void downloadFiles();
    void fileDownloaded();

    void setLoadingIcon(int);
    void setPlayIcon(int);

    void folderError(QString);
    void networkError(QNetworkReply::NetworkError);
    void apiError();

    void launchGame();

signals:
    void dlFile(QString, QString, int, QString);

private:
    Ui::UrTUpdater *ui;

    bool updateInProgress;
    bool readyToProcess;
    bool firstLaunch;

    QString updaterPath;

    QString updaterVersion;
    QString password;

    QThread* dlThread;
    Download* dl;
    bool threadStarted;

    int downloadServer;
    int gameEngine;
    int currentVersion;
    int askBeforeUpdating;
    bool configFileExists;

    int nbFilesToDl;
    int nbFilesDled;
    int downloadedBytes;
    QProgressBar* dlBar;
    QLabel* dlText;
    QLabel* dlSpeed;
    QLabel* dlSize;

    QPushButton* playButton;
    QPushButton* changelogButton;

    QList<fileInfo_s>       filesToDownload;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;
    QList<versionInfo_s>    versionsList;
    QList<QString>          newsList;

    fileInfo_s currentFile;

    QMovie* loaderAnim;
    QMovie* playAnim;
};

#endif // URTUPDATER_H
