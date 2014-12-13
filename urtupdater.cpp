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

#include "urtupdater.h"
#include "ui_urtupdater.h"

UrTUpdater::UrTUpdater(QWidget *parent) : QMainWindow(parent), ui(new Ui::UrTUpdater)
{
    ui->setupUi(this);

    updaterVersion = URT_UPDATER_VERSION;
    changelog = CHANGELOG_EMPTY_TEXT;
    password = "";
    downloadServer = -1;
    gameEngine = -1;
    currentVersion = -1;
    askBeforeUpdating = -1;
    nbFilesToDl = 0;
    nbFilesDled = 0;
    readyToProcess = false;
    configFileExists = false;
    threadStarted = false;
    updateInProgress = false;
    firstLaunch = false;

    QStringList arguments = QCoreApplication::arguments();
    if(arguments.count() >= 1) {
        for(int i = 0; i < arguments.count(); i++) {
            if(arguments.at(i) == "--password" && (i + 1 < arguments.count()) ) {
                i++;
                password = arguments.at( i );
            }
        }
    }

    QMenu *menuFile = menuBar()->addMenu("&File");
    QMenu *menuHelp = menuBar()->addMenu("&Help");

    QAction *actionSettings = menuFile->addAction("&Settings");
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));

    QAction *actionChangelog = menuFile->addAction("&Changelog");
    connect(actionChangelog, SIGNAL(triggered()), this, SLOT(openChangelogPage()));

    QAction *actionAbout = menuHelp->addAction("&About");
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(openAboutPage()));

    QAction *actionHelp = menuHelp->addAction("&Get help");
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(openHelpPage()));
    actionHelp->setShortcut(QKeySequence("Ctrl+H"));

    QAction *actionQuitter = menuFile->addAction("&Quit");
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quit()));
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));

    dlText = new QLabel(this);
    dlText->move(150, 222);
    dlText->setStyleSheet("color:white;");
    dlText->setMinimumWidth(450);
    dlText->setText("Getting information from the API...");
    dlText->show();

    currentChecksum = new QLabel(this);
    currentChecksum->move(150, 270);
    currentChecksum->setStyleSheet("color:white;");
    currentChecksum->setMinimumWidth(450);
    currentChecksum->setText("");
    currentChecksum->hide();

    globalDlText = new QLabel(this);
    globalDlText->move(150, 273);
    globalDlText->setStyleSheet("color:white;");
    globalDlText->setMinimumWidth(450);
    globalDlText->setText("Overall Progress:");
    globalDlText->hide();

    dlSpeed = new QLabel(this);
    dlSpeed->move(150, 318);
    dlSpeed->setStyleSheet("color:white;");
    dlSpeed->setMinimumWidth(200);
    dlSpeed->hide();

    dlSize = new QLabel(this);
    dlSize->move(470, 318);
    dlSize->setStyleSheet("color:white;");
    dlSize->setMinimumWidth(150);
    dlSize->hide();

    dlBar = new QProgressBar(this);
    dlBar->move(150, 250);
    dlBar->setFixedWidth(485);
    dlBar->setFixedHeight(20);
    dlBar->setRange(0, 100);
    dlBar->show();

    globalDlBar = new QProgressBar(this);
    globalDlBar->move(150, 301);
    globalDlBar->setFixedHeight(20);
    globalDlBar->hide();

    if(!menuBar()->isNativeMenuBar()){
        dlBar->setFixedWidth(485);
        globalDlBar->setFixedWidth(485);
    }
    else {
        dlBar->setFixedWidth(450);
        globalDlBar->setFixedWidth(450);
    }

    playButton = new QPushButton(this);
    playButton->move(400, 385);
    playButton->setMinimumWidth(200);
    playButton->setMinimumHeight(50);
    playButton->setText("Updating...");
    playButton->setStyleSheet("padding-bottom: 2px; color: white;font-weight: bold; font-size: 120%; text-transform: uppercase;background-color:#727272;height:50px;");
    playButton->setIconSize(QSize(40, 40));
    playButton->show();

    changelogButton = new QPushButton(this);
    changelogButton->move(150, 385);
    changelogButton->setMinimumWidth(200);
    changelogButton->setMinimumHeight(50);
    changelogButton->setStyleSheet("padding-bottom: 2px; color: white;font-weight: bold; font-size: 120%; text-transform: uppercase;background-color:#727272;height:50px;");
    changelogButton->setText("Changelog");
    changelogButton->hide();

    loaderAnim = new QMovie(":/images/urt_updating.gif");
    connect(loaderAnim, SIGNAL(frameChanged(int)), this, SLOT(setLoadingIcon(int)));

    playAnim = new QMovie(":/images/urt_play.gif");
    connect(playAnim, SIGNAL(frameChanged(int)), this, SLOT(setPlayIcon(int)));

    loaderAnim->start();

    connect(playButton, SIGNAL(clicked()), this, SLOT(launchGame()));
    connect(changelogButton, SIGNAL(clicked()), this, SLOT(openChangelogPage()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(quit()));
    connect(this, SIGNAL(checkingChanged(int)), this, SLOT(setDLValue(int)));
    connect(this, SIGNAL(requestNewDlLabel(QString)), this, SLOT(updateDlLabel(QString)));

    init();
}

void UrTUpdater::init(){

    updaterPath = getCurrentPath();

    // Check if this is the first launch of the updater
    if(!QFile::exists(updaterPath + URT_GAME_SUBDIR)){
        QMessageBox msg;
        int result;

        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setIcon(QMessageBox::Information);
        msg.setText("The game " URT_GAME_NAME " will be installed in this path:\n\n" + updaterPath + "\n\n"
                    + "To change the installation path, please click on \"Cancel\" and copy this Updater to where you want it to be installed.");
        result = msg.exec();

        // If we want to quit
        if(result == QMessageBox::Cancel){
            quit();
        }

        firstLaunch = true;
    }

    parseLocalConfig();
    getManifest("versionInfo");
}

QString UrTUpdater::getPlatform()
{
    #ifdef Q_OS_MAC
    return "Mac";
    #endif

    #ifdef Q_OS_LINUX
    if(QSysInfo::WordSize == 64){
        return "Linux64";
    }
    else {
        return "Linux32";
    }
    #endif

    #ifdef Q_OS_WIN32
    return "Windows";
    #endif

    return "Linux32";
}

QString UrTUpdater::getCurrentPath(){
    QDir dir = QDir(QCoreApplication::applicationDirPath());

    // If we're on Mac, 'dir' will contain the path to the executable
    // inside of the Updater's bundle which isn't what we want.
    // We need to cd ../../..
    if(getPlatform() == "Mac"){
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }

    return dir.absolutePath() + "/";
}

void UrTUpdater::parseLocalConfig(){
    QDomDocument *dom = new QDomDocument();
    QFile *f = new QFile(updaterPath + URT_UPDATER_CFG);

    if(!f->open(QFile::ReadOnly)){
        delete f;
        configFileExists = false;
        return;
    }

    dom->setContent(f);

    QDomNode node = dom->firstChild();
    while(!node.isNull()){
        if(node.toElement().nodeName() == "UpdaterConfig"){
            QDomNode conf = node.firstChild();

            while(!conf.isNull()){
                if(conf.toElement().nodeName() == "DownloadServer"){
                    downloadServer = conf.toElement().text().toInt();
                }
                if(conf.toElement().nodeName() == "GameEngine"){
                    gameEngine = conf.toElement().text().toInt();
                }
                if(conf.toElement().nodeName() == "CurrentVersion"){
                    currentVersion = conf.toElement().text().toInt();
                }
                if(conf.toElement().nodeName() == "AskBeforeUpdating"){
                    askBeforeUpdating = conf.toElement().text().toInt();
                }
                conf = conf.nextSibling();
            }
        }
        node = node.nextSibling();
    }

    configFileExists = true;

    f->close();

    delete f;
    delete dom;
}

void UrTUpdater::saveLocalConfig(){
    QFile *f = new QFile(updaterPath + URT_UPDATER_CFG);
    QXmlStreamWriter *xml = new QXmlStreamWriter();

    if(!f->open(QFile::WriteOnly)){
        QMessageBox::critical(0, "Could not write the config file", "Could not write the Updater config file inside of the game folder. Your updater preferences won't be saved.");
        delete f;
        return;
    }

    xml->setDevice(f);
    xml->writeStartDocument();
    xml->writeStartElement("UpdaterConfig");

    xml->writeStartElement("DownloadServer");
    xml->writeCharacters(QString::number(downloadServer));
    xml->writeEndElement();

    xml->writeStartElement("GameEngine");
    xml->writeCharacters(QString::number(gameEngine));
    xml->writeEndElement();

    xml->writeStartElement("CurrentVersion");
    xml->writeCharacters(QString::number(currentVersion));
    xml->writeEndElement();

    xml->writeStartElement("AskBeforeUpdating");
    xml->writeCharacters(QString::number(askBeforeUpdating));
    xml->writeEndElement();

    xml->writeEndElement();
    xml->writeEndDocument();

    f->close();

    // qDebug() << "Local config saved." << endl;

    delete f;
    delete xml;
}

void UrTUpdater::getManifest(QString query){
    QUrl APIUrl(URT_API_LINK);
    QUrlQuery url;
    QNetworkRequest apiRequest(APIUrl);
    QNetworkAccessManager *apiManager = new QNetworkAccessManager(this);

    apiRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=utf-8");
    url.addQueryItem("platform", getPlatform());
    url.addQueryItem("query", query);
    url.addQueryItem("password", password);
    url.addQueryItem("version", QString::number(currentVersion));
    url.addQueryItem("engine", QString::number(gameEngine));
    url.addQueryItem("server", QString::number(downloadServer));
    url.addQueryItem("updaterVersion", updaterVersion);

    apiAnswer = apiManager->post(apiRequest, url.query(QUrl::FullyEncoded).toUtf8());
    connect(apiAnswer, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(setDLValueP(qint64, qint64)));
    connect(apiAnswer, SIGNAL(finished()), this, SLOT(parseAPIAnswer()));
    connect(apiAnswer, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
}

void UrTUpdater::parseAPIAnswer(){
    QByteArray  apiByteAnswer = apiAnswer->readAll();
    QString     apiData = QString(apiByteAnswer);

    currentChecksum->show();

    QFutureWatcher<void>* watcher = new QFutureWatcher<void>();
    connect(watcher, SIGNAL(finished()), this, SLOT(work()));
    QFuture<void> parser = QtConcurrent::run(this, &UrTUpdater::parseManifest, apiData);
    watcher->setFuture(parser);
}

void UrTUpdater::updateDlLabel(QString label){
    dlText->setText(label);
}

void UrTUpdater::parseManifest(QString data){
    QDomDocument* dom = new QDomDocument();
    dom->setContent(data);

    packsList.clear();
    filesToDownload.clear();
    downloadServers.clear();
    enginesList.clear();
    versionsList.clear();
    newsList.clear();

    QDomNode node = dom->firstChild();

    emit requestNewDlLabel("Parsing the answer of the API...");

    while(!node.isNull()){
        if(node.toElement().nodeName() == "Updater"){
            QDomNode updater = node.firstChild();

            while(!updater.isNull()){

                if(updater.toElement().nodeName() == "APIVersion"){
                    apiVersion = updater.toElement().text();
                }

                if(updater.toElement().nodeName() == "Changelog"){
                    changelog = updater.toElement().text();
                }

                if(updater.toElement().nodeName() == "Licence"){
                    licenceText = updater.toElement().text();
                }

                if(updater.toElement().nodeName() == "NewsList"){
                    QDomNode newsListNode = updater.firstChild();

                    while(!newsListNode.isNull()){
                        if(newsListNode.nodeName() == "NewsText"){
                            newsList.append(newsListNode.toElement().text());
                        }
                        newsListNode = newsListNode.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "ServerList"){
                    QDomNode serverListNode = updater.firstChild();

                    while(!serverListNode.isNull()){
                        if(serverListNode.nodeName() == "Server"){
                            QDomNode serverNode = serverListNode.firstChild();
                            int     serverId;
                            QString serverURL;
                            QString serverName;
                            QString serverLocation;
                            serverInfo_s si;

                            while(!serverNode.isNull()){
                                if(serverNode.nodeName() == "ServerName"){
                                    serverName = serverNode.toElement().text();
                                }
                                if(serverNode.nodeName() == "ServerURL"){
                                    serverURL = serverNode.toElement().text();
                                }
                                if(serverNode.nodeName() == "ServerLocation"){
                                    serverLocation = serverNode.toElement().text();
                                }
                                if(serverNode.nodeName() == "ServerId"){
                                    serverId = serverNode.toElement().text().toInt();
                                }
                                serverNode = serverNode.nextSibling();
                            }

                            si.serverId = serverId;
                            si.serverName = serverName;
                            si.serverURL = serverURL;
                            si.serverLocation = serverLocation;
                            downloadServers.append(si);
                        }
                        serverListNode = serverListNode.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "EngineList"){
                    QDomNode engineListNode = updater.firstChild();

                    while(!engineListNode.isNull()){
                        if(engineListNode.nodeName() == "Engine"){
                            QDomNode engineNode = engineListNode.firstChild();
                            int     engineId;
                            QString engineDir;
                            QString engineName;
                            QString engineLaunchString;
                            engineInfo_s ei;

                            while(!engineNode.isNull()){
                                if(engineNode.nodeName() == "EngineName"){
                                    engineName = engineNode.toElement().text();
                                }
                                if(engineNode.nodeName() == "EngineDir"){
                                    engineDir = engineNode.toElement().text();
                                }
                                if(engineNode.nodeName() == "EngineId"){
                                    engineId = engineNode.toElement().text().toInt();
                                }
                                if(engineNode.nodeName() == "EngineLaunchString"){
                                    engineLaunchString = engineNode.toElement().text();
                                }
                                engineNode = engineNode.nextSibling();
                            }

                            ei.engineId = engineId;
                            ei.engineName = engineName;
                            ei.engineDir = engineDir;
                            ei.engineLaunchString = engineLaunchString;
                            enginesList.append(ei);
                        }
                        engineListNode = engineListNode.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "VersionList"){
                    QDomNode versionListNode = updater.firstChild();

                    while(!versionListNode.isNull()){
                        if(versionListNode.nodeName() == "Version"){
                            QDomNode versionNode = versionListNode.firstChild();
                            int     versionId;
                            QString versionName;
                            versionInfo_s vi;

                            while(!versionNode.isNull()){
                                if(versionNode.nodeName() == "VersionName"){
                                    versionName = versionNode.toElement().text();
                                }
                                if(versionNode.nodeName() == "VersionNumber"){
                                    versionId = versionNode.toElement().text().toInt();
                                }
                                versionNode = versionNode.nextSibling();
                            }

                            vi.versionId = versionId;
                            vi.versionName = versionName;
                            versionsList.append(vi);
                        }
                        versionListNode = versionListNode.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "Files"){
                    emit checkingChanged(0);
                    QDomNode files = updater.firstChild();

                    emit requestNewDlLabel("Checking the game files checksums. It may take a few minutes...");

                    int i = 0, l;

                    l = updater.childNodes().length();

                    while(!files.isNull()){
                        if(files.nodeName() == "File"){
                            QDomNode fileInfo = files.firstChild();
                            QString fileDir;
                            QString fileName;
                            QString fileMd5;
                            QString fileSize;
                            QString fileUrl;
                            bool mustDownload = false;

                            while(!fileInfo.isNull()){
                                if(fileInfo.nodeName() == "FileDir"){
                                    fileDir = fileInfo.toElement().text();
                                }
                                if(fileInfo.nodeName() == "FileName"){
                                    fileName = fileInfo.toElement().text();
                                }
                                if(fileInfo.nodeName() == "FileMD5"){
                                    fileMd5 = fileInfo.toElement().text();
                                }
                                if(fileInfo.nodeName() == "FileSize"){
                                    fileSize = fileInfo.toElement().text();
                                }
                                if(fileInfo.nodeName() == "FileUrl"){
                                    fileUrl = fileInfo.toElement().text();
                                }
                                fileInfo = fileInfo.nextSibling();
                            }

                            QString filePath(updaterPath + fileDir + fileName);
                            QFile* f = new QFile(filePath);

                            i++;

                            currentChecksum->setText(QString("Checking %1 (%2 of %3)").arg(fileName).arg(i).arg(l));
                            emit checkingChanged(i * 100.0 / l);

                            // If the file does not exist, it must be downloaded.
                            if(!f->exists()){
                                mustDownload = true;
                            }

                            // If the md5 string is empty, it means that the API wants
                            // us to delete this file if needed
                            else if(!fileName.isEmpty() && fileMd5.isEmpty()){
                                QFile::remove(filePath);
                            }

                            // Check the file's md5sum to see if it needs to be updated.
                            else if(!fileName.isEmpty() && !fileMd5.isEmpty()){
                                if(getMd5Sum(f) != fileMd5){
                                    mustDownload = true;
                                }
                            }
                            if (!fileMd5.isEmpty() && !fileName.isEmpty()){
                                packsList.append(fileName);
                            }

                            if(mustDownload){
                                fileInfo_s fi;
                                fi.fileName = fileName;
                                fi.filePath = fileDir;
                                fi.fileMd5 = fileMd5;
                                fi.fileSize = fileSize;
                                fi.fileUrl = fileUrl;
                                filesToDownload.append(fi);
                            }

                            delete f;
                        }
                        files = files.nextSibling();
                    }

                    currentChecksum->setText("");
                }
                updater = updater.nextSibling();
            }
        }
        node = node.nextSibling();
    }

    delete dom;
}

void UrTUpdater::work(){
    currentChecksum->hide();

    // Workaround - you can't call ->show() from parseManifest()
    // because this function is running on its own thread.
    if (changelog != CHANGELOG_EMPTY_TEXT){
        changelogButton->show();
    }

    checkAPIVersion();
    checkDownloadServer();
    checkGameEngine();
    checkVersion();
    drawNews();

    if(!threadStarted){
        startDlThread();
    }

    if(readyToProcess){
        checkFiles();
        downloadFiles();
    }
    else {
        if(firstLaunch){
            openLicencePage();

            // Create the game folder
            if(!QDir().mkdir(updaterPath + URT_GAME_SUBDIR)){
                folderError(QString(updaterPath + URT_GAME_SUBDIR));
            }

            firstLaunch = false;

            openSettings();
            return;
        }
        readyToProcess = true;
        getManifest("versionFiles");
    }
}

void UrTUpdater::setDLValue(int v){
    dlBar->setValue(v);
}

void UrTUpdater::setDLValueP(qint64 r, qint64 t){
    dlBar->setValue(r * 100.0 / t);
}

void UrTUpdater::startDlThread(){
    if(threadStarted){
        return;
    }

    dlThread = new QThread();
    dl = new Download(getServerUrlById(downloadServer), updaterPath, getPlatform());
    dl->moveToThread(dlThread);

    connect(dlThread, SIGNAL(started()), dl, SLOT(init()));
    connect(dlThread, SIGNAL(finished()), dlThread, SLOT(deleteLater()));

    connect(dl, SIGNAL(dlError(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
    connect(dl, SIGNAL(folderError(QString)), this, SLOT(folderError(QString)));
    connect(dl, SIGNAL(fileDownloaded()), this, SLOT(fileDownloaded()));
    connect(dl, SIGNAL(bytesDownloaded(qint64, QString, int, int)), this, SLOT(bytesDownloaded(qint64, QString, int, int)));

    connect(this, SIGNAL(dlFile(QString,QString, int, QString)), dl, SLOT(downloadFile(QString, QString, int, QString)));

    threadStarted = true;
    dlThread->start();
}

void UrTUpdater::downloadFiles(){

    if(filesToDownload.size() <= 0){
        dlBar->setRange(0, 100);
        dlBar->setValue(100);
        globalDlBar->hide();
        globalDlText->hide();
        dlSpeed->hide();
        dlSize->hide();
        updateInProgress = false;
        loaderAnim->stop();
        playAnim->start();
        playButton->setText("Play!");

        dlText->setText("Your game is up to date!");
        return;
    }

    else {
        if(askBeforeUpdating == 1){
            QMessageBox msg;
            int result;

            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setIcon(QMessageBox::Information);
            msg.setText("A new update is available. Would you like to download it now?");
            result = msg.exec();

            if(result == QMessageBox::Cancel){
                dlText->setText("Your game is outdated!");
                loaderAnim->stop();
                playAnim->start();
                playButton->setText("Play!");
                return;
            }
        }

        totalSizeToDl = getTotalSizeToDl();
        updateInProgress = true;
        nbFilesToDl = filesToDownload.size();
        nbFilesDled = 0;
        downloadedBytes = 0;
        currentFile = filesToDownload.takeFirst();
        dlBar->setValue(0);
        dlBar->setRange(0, currentFile.fileSize.toInt());
        globalDlBar->setValue(0);
        globalDlBar->setRange(0, totalSizeToDl);
        globalDlBar->show();
        globalDlText->show();
        dlSpeed->show();
        dlSize->show();

        emit dlFile(currentFile.filePath, currentFile.fileName, currentFile.fileSize.toInt(), currentFile.fileUrl);
    }
}

void UrTUpdater::bytesDownloaded(qint64 speed, QString unit, int nbBytes, int dled){
    QString nb;
    QString nb2;
    int totalSize = totalSizeToDl;

    downloadedBytes += dled;

    globalDlBar->setValue(downloadedBytes);
    dlBar->setValue(nbBytes);

    dlText->setText("Current file: " + currentFile.filePath + currentFile.fileName + " (" + (QString::number(nbFilesDled+1)) + "/" + QString::number(nbFilesToDl) + ")");
    dlSpeed->setText("Speed:  " + QString::number(speed, 'f', 2) + " " + QString(unit));

    int bytes = downloadedBytes;

    nb = getSize(&bytes);
    nb2 = getSize(&totalSize);

    dlSize->setText(QString::number(bytes) + nb + "/ " + QString::number(totalSize) + nb2); // / " + QString::number(currentFile.fileSize)/1000 + " kB"
}

void UrTUpdater::fileDownloaded(){
    if(filesToDownload.size() > 0){
        nbFilesDled++;
        currentFile = filesToDownload.takeFirst();
        dlBar->setRange(0, currentFile.fileSize.toInt());
        dlBar->setValue(0);

        emit dlFile(currentFile.filePath, currentFile.fileName, currentFile.fileSize.toInt(), currentFile.fileUrl);
    }

    else {
        dlBar->setRange(0, 100);
        dlBar->setValue(100);
        globalDlBar->hide();
        globalDlText->hide();
        dlSize->hide();
        dlSpeed->hide();

        filesToDownload.clear();
        nbFilesToDl = 0;
        nbFilesDled = 0;
        downloadedBytes = 0;

        getManifest("versionFiles");
    }
}

void UrTUpdater::checkFiles(){
    QStringList nameFilter("zUrT*.pk3");
    QDir filesPath(updaterPath + URT_GAME_SUBDIR);
    QStringList filesList = filesPath.entryList(nameFilter);

    if (packsList.size() <= 0){
        return;
    }

    foreach (QString file, filesList){
        if (!packsList.contains(file)){
            QFile* fileToRm = new QFile(updaterPath + URT_GAME_SUBDIR + "/" + file);
            fileToRm->remove();
            delete fileToRm;
        }
    }
}

void UrTUpdater::checkAPIVersion(){
    if(apiVersion != updaterVersion){
        QMessageBox::critical(0, "Updater outdated", "This version ("+updaterVersion+") of the Updater is outdated. Please download the new Updater here: http://get.urbanterror.info");
        quit();
    }
}

void UrTUpdater::checkDownloadServer(){
    QList<serverInfo_s>::iterator li;
    bool found = false;

    if(downloadServers.size() < 1){
        apiError();
    }

    // Check if the download server that is stored in the config file still exists
    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverId == downloadServer){
            found = true;
        }
    }

    // If the engine isn't available anymore, pick the first one in the list
    if(!found){
        downloadServer = downloadServers.at(0).serverId;
    }
}

void UrTUpdater::checkGameEngine(){
    QList<engineInfo_s>::iterator li;
    bool found = false;

    if(enginesList.size() < 1){
        apiError();
    }

    // Check if the engine that is stored in the config file still exists
    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineId == gameEngine){
            found = true;
        }
    }

    // If the server isn't a mirror anymore, pick the first one in the list
    if(!found){
        gameEngine = enginesList.at(0).engineId;
    }
}

void UrTUpdater::checkVersion(){
    QList<versionInfo_s>::iterator li;
    bool found = false;

    if(versionsList.size() < 1){
        apiError();
    }

    // Check if the version that is stored in the config file still exists
    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        if(li->versionId == currentVersion){
            found = true;
        }
    }

    // If the version isn't available anymore, pick the first one in the list
    if(!found){
        currentVersion = versionsList.at(0).versionId;
    }
}

void UrTUpdater::drawNews(){
    QList<QString>::iterator li;
    int i = 0;

    if(newsList.size() < 1){
        apiError();
    }

    for(li = newsList.begin(); li != newsList.end(); ++li, i++){
        QLabel* news = new QLabel(this);

        if(!menuBar()->isNativeMenuBar()){
            news->move(150, 85 + (i*26));
        }
        else {
            news->move(150, 65 + (i*26));
        }
        news->setMinimumWidth(450);
        news->setText(*li);
        news->setOpenExternalLinks(true);
        news->setVisible(true);
    }
}

void UrTUpdater::networkError(QNetworkReply::NetworkError code){
    QString error = "";
    bool critical = false;

    switch(code){
        case QNetworkReply::ConnectionRefusedError:
            error = "Error: the remote server refused the connection. Please try again later.";
            critical = true;
            break;

        case QNetworkReply::RemoteHostClosedError:
            error = "Error: the remote server closed the connection prematurely. Please try again later.";
            break;

        case QNetworkReply::HostNotFoundError:
            error = "Error: the remote server could not be found. Please check your internet connection!";
            critical = true;
            break;

        case QNetworkReply::TimeoutError:
            error = "Error: the connection to the remote server timed out. Please try again.";
            break;

        case QNetworkReply::TemporaryNetworkFailureError:
            error = "Error: the connection to the remote server was broken due to disconnection from the network. Please try again.";
            break;

        case QNetworkReply::ContentNotFoundError:
            error = "Error: the remote content could not be found. Please report this issue on our website: http://www.urbanterror.info";
            critical = true;
            break;

        case QNetworkReply::UnknownNetworkError:
            error = "Error: an unknown network-related error was encountered. Please try again";
            break;

        case QNetworkReply::UnknownContentError:
            error = "Error: an unknown content-related error was encountered. Please try again";
            break;

        default:
        case QNetworkReply::NoError:
            break;
    }

    if(!error.isEmpty()){
        if(critical == true){
            QMessageBox::critical(0, "Download error", error);
            dlThread->terminate();
            quit();
        }
        else {
            QMessageBox::information(0, "Download error", error);
            if(threadStarted){
                if(dl->errorDl){
                    dl->errorDl = false;
                }
            }
        }
    }
}

void UrTUpdater::folderError(QString folder){
    QMessageBox::critical(this, folder + " file/folder", "Could not create the file/folder " + folder + ".\n"
                          + "Please move the updater to a folder where it has sufficient permissions.");
    quit();
}

void UrTUpdater::apiError(){
    QMessageBox::critical(this, "API error", "The information from the API are missing or wrong. Please report it on www.urbanterror.info and try again later.");
    quit();
}

void UrTUpdater::openHelpPage(){
    QMessageBox::information(this, "Get help", "If you're experiencing issues with this updater, please contact us through:\n" \
                             "- the support forums: http://www.urbanterror.info/forums/\n" \
                             "- irc: #urbanterror @ irc.quakenet.org\n" \
                             "- email: contact@urbanterror.info");
}

void UrTUpdater::openAboutPage(){
    QMessageBox::information(this, "About UrTUpdater", "Urban Terror Updater\n" \
                             "Version " + updaterVersion + "\n" \
                             "Author: Charles 'Barbatos' Duprey\n\n" \
                             "Copyright: FrozenSand / 0870760 B.C. Ltd. All rights reserved.\n" \
                             "This Updater is under the LGPL v2.1 licence.\n\n" \
                             "Source code: https://github.com/Barbatos/UrTUpdater/");
}

void UrTUpdater::openLicencePage(){
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Do you accept the terms of this licence?");
    dialog->setMinimumWidth(600);
    dialog->setMinimumHeight(500);

    QTextEdit* txt = new QTextEdit(this);
    txt->setText(licenceText);
    txt->setMinimumWidth(600);
    txt->setMinimumHeight(450);
    txt->setParent(dialog);
    txt->setReadOnly(true);
    txt->show();

    QPushButton *acceptButton = new QPushButton(dialog);
    acceptButton->move(285, 470);
    acceptButton->setText("Accept");
    acceptButton->show();

    QPushButton* rejectButton = new QPushButton(dialog);
    rejectButton->move(200, 470);
    rejectButton->setText("Reject");
    rejectButton->show();

    connect(acceptButton, SIGNAL(clicked()), dialog, SLOT(close()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(quit()));
    dialog->exec();
}

void UrTUpdater::openChangelogPage(){
    if (changelog == CHANGELOG_EMPTY_TEXT) {
        QMessageBox::information(this, "Sorry", "No changelog available :(");
        return;
    }
    
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Urban Terror Changelog");
    dialog->setFixedWidth(600);
    dialog->setFixedHeight(500);
    dialog->show();

    QTextEdit* txt = new QTextEdit(this);
    txt->setText(changelog);
    txt->setFixedWidth(600);
    txt->setFixedHeight(500);
    txt->setParent(dialog);
    txt->setReadOnly(true);
    txt->show();
}

void UrTUpdater::openSettings(){
    if(updateInProgress){
        QMessageBox::information(this, "Update in progress", "An update is in progress. Please wait until the update is finished.");
        return;
    }

    Settings *settings = new Settings(this);

    connect(settings, SIGNAL(settingsUpdated(int,int,int,int)), this, SLOT(setSettings(int,int,int,int)));

    settings->currentServer = downloadServer;
    settings->currentVersion = currentVersion;
    settings->currentEngine = gameEngine;
    settings->currentUpdateBehavior = askBeforeUpdating;

    settings->downloadServers = downloadServers;
    settings->enginesList = enginesList;
    settings->versionsList = versionsList;

    settings->init();
    settings->exec();
}

void UrTUpdater::setSettings(int version, int engine, int server, int updateType){
    gameEngine = engine;
    currentVersion = version;
    downloadServer = server;
    askBeforeUpdating = updateType;
    saveLocalConfig();
    dlText->setText("Checking the game files checksums. It may take a few minutes...");
    getManifest("versionFiles");
}

void UrTUpdater::launchGame(){
    QProcess process;
    QString launchString = getEngineLaunchStringById(gameEngine);
    QString platform = getPlatform();
    QString s;

    if(updateInProgress){
        return;
    }

    if(platform == "Mac"){
        s = "open \"" + updaterPath + launchString + "\"";
    }
    else {
        s = updaterPath + launchString;
    }

    process.startDetached( s );
    process.waitForStarted();
}

void UrTUpdater::setLoadingIcon(int){
    playButton->setIcon(QIcon(loaderAnim->currentPixmap()));
}

void UrTUpdater::setPlayIcon(int){
    playButton->setIcon(QIcon(playAnim->currentPixmap()));
}

QString UrTUpdater::getMd5Sum(QFile* file)
{
    if (file->exists() && file->open(QIODevice::ReadOnly))
    {
        QByteArray content = file->readAll();
        QByteArray hashed = QCryptographicHash::hash(content, QCryptographicHash::Md5);
        file->close();

        return hashed.toHex().data();
    }

    return "";
}

QString UrTUpdater::getServerUrlById(int id){
    QList<serverInfo_s>::iterator li;

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverId == id){
            return li->serverURL;
        }
    }

    return "";
}

QString UrTUpdater::getEngineLaunchStringById(int id){
    QList<engineInfo_s>::iterator li;

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineId == id){
            return li->engineLaunchString;
        }
    }

    return "";
}

QString UrTUpdater::getSize(int *bytes){
    QString nb;

    if (*bytes < 1024) {
        nb = " B ";
    } else if (*bytes < 1024*1024) {
        *bytes /= 1024;
        nb = " KB ";
    } else {
        *bytes /= 1024*1024;
        nb = " MB ";
    }

    return nb;
}

int UrTUpdater::getTotalSizeToDl(){
    QList<fileInfo_s>::iterator li;
    int size = 0;

    if(filesToDownload.size() <= 0){
        return 0;
    }

    for(li = filesToDownload.begin(); li != filesToDownload.end(); ++li){
        size += li->fileSize.toInt();
    }
    return size;
}

void UrTUpdater::quit(){
    if(!firstLaunch){
        saveLocalConfig();
    }

    exit(0);
}

UrTUpdater::~UrTUpdater(){
    delete ui;
}
