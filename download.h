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
