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

signals:
    void dlError(QNetworkReply::NetworkError);
    void folderError(QString);
    void bytesDownloaded(int);

private:
    QString updaterPath;
    QUrl* url;
    QString downloadServer;
    QNetworkAccessManager* http;
    QNetworkReply* reply;

    int downloadedBytes;
    bool downloadInProgress;

    QString currentFile;
    QString currentFolder;
    QFile* currentDownload;
};

#endif // DOWNLOAD_H
