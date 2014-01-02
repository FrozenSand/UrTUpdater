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
    void openAboutPage();
    void openChangelogPage();

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
    QString changelog;
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
