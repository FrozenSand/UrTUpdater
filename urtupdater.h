#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"

typedef struct {
    QString fileName;
    QString fileMd5;
    QString filePath;
    QString fileSize;
} fileInfo_s;

namespace Ui {
    class UrTUpdater;
}

class UrTUpdater : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrTUpdater(QWidget *parent = 0);
    ~UrTUpdater();
    void init();
    void quit();
    QString getPlatform();
    QString getCurrentPath();
    void getManifest();
    void parseDOM(QString data);
    QString getMd5Sum(QFile* file);

public slots:
    void parseAPIAnswer();
    void errorAPI(QNetworkReply::NetworkError);

private:
    Ui::UrTUpdater *ui;

    QString updaterPath;
    QString versionNumber;
    QString releaseDate;

    QList<fileInfo_s> filesToDownload;

    QNetworkReply *apiAnswer;
};

#endif // URTUPDATER_H
