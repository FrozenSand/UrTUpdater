#include "urtupdater.h"
#include "ui_urtupdater.h"

UrTUpdater::UrTUpdater(QWidget *parent) : QMainWindow(parent), ui(new Ui::UrTUpdater)
{
    ui->setupUi(this);
    init();
}

UrTUpdater::~UrTUpdater()
{
    delete ui;
}

void UrTUpdater::init(){

    updaterPath = getCurrentPath();

    // Check if this is the first launch of the updater
    if(!QFile::exists(updaterPath + URT_GAME_SUBDIR)){
        QMessageBox msg;
        int result;

        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setIcon(QMessageBox::Information);
        msg.setText("The game " URT_GAME_NAME " will be installed in this path:\n" + updaterPath + "\n"
                    + "To change the installation path, please click on \"Cancel\" and copy this Updater to where you want it to be installed.");
        result = msg.exec();

        // If we want to quit
        if(result == QMessageBox::Cancel){
            quit();
        }

        // Create the game folder
        else {
            if(!QDir().mkdir(updaterPath + URT_GAME_SUBDIR)){
                QMessageBox::critical(this, QString(URT_GAME_SUBDIR) + " folder", "Could not create the game folder (" + updaterPath + URT_GAME_SUBDIR + ").\n"
                                      + "Please move the updater to a folder where it has sufficient permissions.");
                quit();
            }
        }
    }

    getManifest("versionInfo");
}

QString UrTUpdater::getPlatform()
{
    #ifdef Q_OS_MAC
    return "Mac";
    #endif

    #ifdef Q_OS_LINUX
    return "Linux";
    #endif

    #ifdef Q_OS_WIN32
    return "Windows";
    #endif

    return "Linux";
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

void UrTUpdater::getManifest(QString query){
    QUrl APIUrl(URT_API_LINK);
    QUrlQuery url;
    QNetworkRequest apiRequest(APIUrl);
    QNetworkAccessManager *apiManager = new QNetworkAccessManager(this);

    qDebug() << "query: " << query << endl;

    apiRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=utf-8");
    url.addQueryItem("platform", getPlatform());
    url.addQueryItem("query", query);

    apiAnswer = apiManager->post(apiRequest, url.query(QUrl::FullyEncoded).toUtf8());
    connect(apiAnswer, SIGNAL(finished()), this, SLOT(parseAPIAnswer()));
    connect(apiAnswer, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
}

void UrTUpdater::parseAPIAnswer(){
    QByteArray  apiByteAnswer = apiAnswer->readAll();
    QString     apiData = QString(apiByteAnswer);

    qDebug() << "apiData: " << apiData << endl;

    parseDOM(apiData);
}

void UrTUpdater::parseDOM(QString data){
    QDomDocument* dom = new QDomDocument();
    dom->setContent(data);

    QDomNode node = dom->firstChild();

    while(!node.isNull()){
        if(node.toElement().nodeName() == "Updater"){
            QDomNode updater = node.firstChild();

            while(!updater.isNull()){

                if(updater.toElement().nodeName() == "VersionInfo"){
                    QDomNode versionInfo = updater.firstChild();

                    while(!versionInfo.isNull()){
                        if(versionInfo.nodeName() == "VersionNumber"){
                            versionNumber = versionInfo.toElement().text();
                        }
                        if(versionInfo.nodeName() == "ReleaseDate"){
                            releaseDate = versionInfo.toElement().text();
                        }
                        versionInfo = versionInfo.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "ServerList"){
                    QDomNode serverListNode = updater.firstChild();

                    while(!serverListNode.isNull()){
                        if(serverListNode.nodeName() == "Server"){
                            QDomNode serverNode = serverListNode.firstChild();
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
                                serverNode = serverNode.nextSibling();
                            }

                            si.serverName = serverName;
                            si.serverURL = serverURL;
                            si.serverLocation = serverLocation;
                            downloadServers.append(si);
                        }
                        serverListNode = serverListNode.nextSibling();
                    }
                }

                else if(updater.toElement().nodeName() == "Files"){
                    QDomNode files = updater.firstChild();

                    while(!files.isNull()){
                        if(files.nodeName() == "File"){
                            QDomNode fileInfo = files.firstChild();
                            QString fileDir;
                            QString fileName;
                            QString fileMd5;
                            QString fileSize;
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
                                fileInfo = fileInfo.nextSibling();
                            }

                            QString filePath(updaterPath + fileDir + fileName);
                            QFile* f = new QFile(filePath);

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
                                    qDebug() << "md5 file: " << getMd5Sum(f) << ", " << fileMd5 << endl;
                                }
                            }

                            qDebug() << "fileDir:" << fileDir << endl;
                            qDebug() << "fileName: " << fileName << endl;
                            qDebug() << "fileMd5: " << fileMd5 << endl;

                            if(mustDownload){
                                fileInfo_s fi;
                                fi.fileName = fileName;
                                fi.filePath = fileDir;
                                fi.fileMd5 = fileMd5;
                                fi.fileSize = fileSize;
                                filesToDownload.append(fi);
                            }

                            delete f;
                        }
                        files = files.nextSibling();
                    }
                }
                updater = updater.nextSibling();
            }
        }
        node = node.nextSibling();
    }

    serverSelection();

    delete dom;
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
            quit();
        }
        else {
            QMessageBox::information(0, "Download error", error);
        }
    }
}

void UrTUpdater::serverSelection(){
    ServerSelection *serverSel = new ServerSelection(this);

    serverSel->downloadServers = downloadServers;
    serverSel->init();
    serverSel->exec();
}

void UrTUpdater::engineSelection(){
    EngineSelection* engineSel = new EngineSelection(this);
    engineSel->exec();
}

void UrTUpdater::quit(){
    exit(0);
}
