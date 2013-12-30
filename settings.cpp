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
    okButton->move(377, 210);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Go back");
    cancelButton->move(280, 210);
    cancelButton->show();

    /*selectLabel = new QLabel(this);
    selectLabel->setText("Select the version that you want to download:");
    selectLabel->move(45, 30);
    selectLabel->show();

    passwordLabel = new QLabel(this);
    passwordLabel->setText("If you are a developer or tester, enter your password:");
    passwordLabel->move(45, 130);
    passwordLabel->show();

    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->move(45, 155);

    if(!password.isEmpty()){
        passwordField->setText(password);
    }
    passwordField->show();

    passwordOkButton = new QPushButton(this);
    passwordOkButton->setText("Ok");
    passwordOkButton->move(195, 152);
    passwordOkButton->show();
    */

    versionList = new QComboBox(this);

    for(liVersion = versionsList.begin(); liVersion != versionsList.end(); ++liVersion){
        versionList->addItem(liVersion->versionName);
    }

    currentVersionName = getVersionNameById(currentVersion);

    if(!currentVersionName.isEmpty()){
        versionList->setCurrentIndex((int)versionList->findText(currentVersionName));
    }

    versionList->move(44, 70);
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

    serverList->move(44, 70);
    serverList->show();

    engineList = new QComboBox(this);

    for(liEngine = enginesList.begin(); liEngine != enginesList.end(); ++liEngine){
        engineList->addItem(liEngine->engineName);
    }

    currentEngineName = getEngineNameById(currentEngine);

    if(!currentEngineName.isEmpty()){
        engineList->setCurrentIndex((int)engineList->findText(currentEngineName));
    }

    engineList->move(44, 70);
    engineList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    //connect(passwordOkButton, SIGNAL(clicked()), this, SLOT(passwordEntered()));

    setWindowTitle("Settings");
    setFixedSize(450, 250);
    setModal(true);
}

void Settings::okButtonClicked(){
    int idVersion   = getVersionIdByName(versionList->itemText(versionList->currentIndex()));
    int idServer    = getServerIdByName(serverList->itemText(serverList->currentIndex()));
    int idEngine    = getEngineIdByName(engineList->itemText(engineList->currentIndex()));
    QString pw      = passwordField->text();

    if(idVersion == -1 || idServer == -1 || idEngine == -1){
        return;
    }

    emit settingsUpdated(idVersion, idEngine, idServer, pw);
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
