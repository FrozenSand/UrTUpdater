#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "common.h"

class Download : public QObject
{
    Q_OBJECT

public:
    Download(QString, QString);
    ~Download();
    void setDownloadServer(QString);

public slots:
    void init();
    void filePart();
    void downloadError(QNetworkReply::NetworkError);
    void downloadFile(QString, QString);
    void downloadFinished();

signals:
    void dlError(QNetworkReply::NetworkError);
    void folderError(QString);
    void bytesDownloaded(int);
    void fileDownloaded();

private:
    QString updaterPath;
    QString downloadServer;
    QNetworkAccessManager* http;
    QNetworkRequest request;
    QNetworkReply* reply;

    int downloadedBytes;
    bool downloadInProgress;

    QString currentFile;
    QString currentFolder;
    QFile* currentDownload;
};

#endif // DOWNLOAD_H
