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
    void parseDOM(QString data);
    QString getMd5Sum(QFile* file);
    void serverSelection();
    void engineSelection();

private:
    Ui::UrTUpdater *ui;

    QString updaterPath;
    QString versionNumber;
    QString releaseDate;

    QList<fileInfo_s> filesToDownload;
    QList<serverInfo_s>    downloadServers;
};

#endif // URTUPDATER_H
