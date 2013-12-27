#include "download.h"

Download::Download(QString server, QString _updaterPath) : downloadServer(server), updaterPath(_updaterPath){
}

Download::~Download(){

}

void Download::init(){
    downloadInProgress = false;
    downloadedBytes = 0;

    qDebug() << "downloadServer: " << downloadServer;

    http = new QNetworkAccessManager(this);
}

void Download::setDownloadServer(QString server){
    downloadServer = server;
}

void Download::downloadFile(QString folder, QString file){

    currentDownload = new QFile(updaterPath + folder + file);
    downloadedBytes = 0;
    currentFile = file;
    currentFolder = folder;

    request = QNetworkRequest(QString(downloadServer + folder + file));

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

    connect(reply, SIGNAL(readyRead()), this, SLOT(filePart()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void Download::filePart(){
    int count = currentDownload->write(reply->readAll());
    downloadedBytes += count;
    emit bytesDownloaded(count);
}

void Download::downloadFinished(){
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(filePart()));
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    reply->deleteLater();

    qDebug() << "download finished";

    if(downloadInProgress){
        downloadInProgress = false;
        currentDownload->close();

        delete currentDownload;

        emit fileDownloaded();
    }
}

void Download::downloadError(QNetworkReply::NetworkError code){
    emit dlError(code);
}
