#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "common.h"

class Download : public QObject
{
    Q_OBJECT

public:
    Download(QString, QString, QString);
    ~Download();
    void setDownloadServer(QString);
    bool errorDl;

public slots:
    void init();
    void filePart();
    void downloadError(QNetworkReply::NetworkError);
    void downloadFile(QString, QString, int, QString);
    void downloadFinished();
    void reconnect();

signals:
    void dlError(QNetworkReply::NetworkError);
    void folderError(QString);
    void bytesDownloaded(qint64, QString, int);
    void fileDownloaded();

private:
    QString updaterPath;
    QString downloadServer;
    QString platform;
    QNetworkAccessManager* http;
    QNetworkRequest request;
    QNetworkReply* reply;

    int downloadedBytes;
    int fileSize;
    bool downloadInProgress;

    QString fileUrl;
    QString currentFile;
    QString currentFolder;
    QFile* currentDownload;

    QTime downloadTime;
    QTimer* timeout;
};

#endif // DOWNLOAD_H
