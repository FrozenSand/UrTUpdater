#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent){

}

Settings::~Settings(){

}

void Settings::init(){
    QList<versionInfo_s>::iterator  liVersion;
    QList<serverInfo_s>::iterator   liServer;
    QList<engineInfo_s>::iterator   liEngine;
    QString currentVersionName;
    QString currentServerName;
    QString currentEngineName;

    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->move(377, 260);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Go back");
    cancelButton->move(280, 260);
    cancelButton->show();

    versionLabel = new QLabel(this);
    versionLabel->setText("Select the version that you want to download:");
    versionLabel->move(30, 20);
    versionLabel->show();

    serverLabel = new QLabel(this);
    serverLabel->setText("Select a download mirror depending on your location:");
    serverLabel->move(30, 90);
    serverLabel->show();

    engineLabel = new QLabel(this);
    engineLabel->setText("Select the game engine that you want to use to run the game:");
    engineLabel->move(30, 160);
    engineLabel->show();


    versionList = new QComboBox(this);

    for(liVersion = versionsList.begin(); liVersion != versionsList.end(); ++liVersion){
        versionList->addItem(liVersion->versionName);
    }

    currentVersionName = getVersionNameById(currentVersion);

    if(!currentVersionName.isEmpty()){
        versionList->setCurrentIndex((int)versionList->findText(currentVersionName));
    }

    versionList->move(29, 40);
    versionList->show();

    serverList = new QComboBox(this);

    for(liServer = downloadServers.begin(); liServer != downloadServers.end(); ++liServer){
        if(liServer->serverLocation != ""){
            serverList->addItem(QIcon(QString(":/flags/%1.png").arg(liServer->serverLocation)), liServer->serverName);
        }
        else {
            serverList->addItem(liServer->serverName);
        }
    }

    currentServerName = getServerNameById(currentServer);

    if(!currentServerName.isEmpty()){
        serverList->setCurrentIndex((int)serverList->findText(currentServerName));
    }

    serverList->move(29, 110);
    serverList->show();

    engineList = new QComboBox(this);

    for(liEngine = enginesList.begin(); liEngine != enginesList.end(); ++liEngine){
        engineList->addItem(liEngine->engineName);
    }

    currentEngineName = getEngineNameById(currentEngine);

    if(!currentEngineName.isEmpty()){
        engineList->setCurrentIndex((int)engineList->findText(currentEngineName));
    }

    engineList->move(29, 180);
    engineList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    setWindowTitle("Settings");
    setFixedSize(450, 300);
    setModal(true);
}

void Settings::okButtonClicked(){
    int idVersion   = getVersionIdByName(versionList->itemText(versionList->currentIndex()));
    int idServer    = getServerIdByName(serverList->itemText(serverList->currentIndex()));
    int idEngine    = getEngineIdByName(engineList->itemText(engineList->currentIndex()));

    if(idVersion == -1 || idServer == -1 || idEngine == -1){
        return;
    }

    emit settingsUpdated(idVersion, idEngine, idServer);
    close();
}

int Settings::getVersionIdByName(QString name){
    QList<versionInfo_s>::iterator li;

    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        if(li->versionName == name){
            return li->versionId;
        }
    }

    return -1;
}

QString Settings::getVersionNameById(int id){
    QList<versionInfo_s>::iterator li;

    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        if(li->versionId == id){
            return li->versionName;
        }
    }

    return "";
}

int Settings::getServerIdByName(QString name){
    QList<serverInfo_s>::iterator li;

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverName == name){
            return li->serverId;
        }
    }

    return -1;
}

QString Settings::getServerNameById(int id){
    QList<serverInfo_s>::iterator li;

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverId == id){
            return li->serverName;
        }
    }

    return "";
}

int Settings::getEngineIdByName(QString name){
    QList<engineInfo_s>::iterator li;

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineName == name){
            return li->engineId;
        }
    }

    return -1;
}

QString Settings::getEngineNameById(int id){
    QList<engineInfo_s>::iterator li;

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineId == id){
            return li->engineName;
        }
    }

    return "";
}
