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

#include "download.h"

Download::Download(QString server, QString _updaterPath, QString _platform) :
    errorDl(false), updaterPath(_updaterPath), downloadServer(server), platform(_platform){
}

Download::~Download(){

}

void Download::init(){
    downloadInProgress = false;
    downloadedBytes = 0;
    fileSize = 0;

    timeout = new QTimer(this);

    http = new QNetworkAccessManager(this);

    connect(timeout, SIGNAL(timeout()), this, SLOT(reconnect()));
}

void Download::reconnect(){
    disconnect(timeout, SIGNAL(timeout()), this, SLOT(reconnect()));
    http->deleteLater();

    http = new QNetworkAccessManager(this);
    connect(timeout, SIGNAL(timeout()), this, SLOT(reconnect()));

    emit downloadFile(currentFolder, currentFile, fileSize, fileUrl);
}

void Download::setDownloadServer(QString server){
    downloadServer = server;
}

void Download::downloadFile(QString folder, QString file, int size, QString url){

    if(errorDl){
        return;
    }

    currentDownload = new QFile(updaterPath + folder + file);
    fileSize = size;
    downloadedBytes = 0;
    currentFile = file;
    currentFolder = folder;
    fileUrl = url;

    request = QNetworkRequest(QString(url));

    // Check if we have to create the folder
    if(!currentFolder.isEmpty() && !QDir().exists(updaterPath + currentFolder)){
        if(!QDir().mkdir(updaterPath + currentFolder)){
            emit folderError(QString(updaterPath + currentFolder));
        }
    }

    // If the file already exists, remove it
    if(currentDownload->exists()){
        currentDownload->remove();
    }

    // Open the file in write mode
    if(!currentDownload->open(QIODevice::ReadWrite)){
        emit folderError(QString(folder + file));
    }

    reply = http->get(request);
    reply->setReadBufferSize(64 * 1024);
    reply->ignoreSslErrors();
    downloadInProgress = true;

    downloadTime.start();

    connect(reply, SIGNAL(readyRead()), this, SLOT(filePart()));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
}

void Download::filePart(){
    QString unit;
    qint64 speed;

    int count = currentDownload->write(reply->readAll());
    downloadedBytes += count;

    if (downloadedBytes == 0){
        emit bytesDownloaded(0, "b/s", 0, 0);
        return;
    }

    speed = downloadedBytes * 1000.0 / downloadTime.elapsed();

    if (speed < 1024) {
        unit = "b/s";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kb/s";
    } else {
        speed /= 1024*1024;
        unit = "Mb/s";
    }

    emit bytesDownloaded(speed, unit, downloadedBytes, count);
}

void Download::downloadFinished(){
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(filePart()));
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    reply->deleteLater();

    if(downloadInProgress){
        downloadInProgress = false;

        currentDownload->close();

        // Apply chmod +x for executable files on linux
        if((currentFile.contains(".i386", Qt::CaseInsensitive) || (currentFile.contains(".x86_64", Qt::CaseInsensitive))) && (platform == "Linux"))
        {
            QString cmd("chmod +x "+currentFile);
            QProcess* process = new QProcess(this);
            process->start(QFile::encodeName(cmd).data());
            process->waitForFinished(3000);
        }

        if((platform == "Linux") || (platform == "Mac"))
        {
            if(currentFile.contains(".zip", Qt::CaseInsensitive))
            {
                QString cmd("unzip -q -o "+updaterPath+currentFile+" -d "+updaterPath);
                QProcess* process = new QProcess(this);
                process->start(QFile::encodeName(cmd).data());
                process->waitForFinished(10000);
            }
        }

        delete currentDownload;

        emit fileDownloaded();
    }
}

void Download::downloadError(QNetworkReply::NetworkError code){
    errorDl = true;
    emit dlError(code);
}
