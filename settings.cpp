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
  * @version    4.0.2
  * @author     Charles 'Barbatos' Duprey
  * @email      barbatos@urbanterror.info
  * @copyright  2013-2015 Frozen Sand / 0870760 B.C. Ltd
  */

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
    okButton->move(260, 170);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Back");
    cancelButton->move(174, 170);
    cancelButton->show();

    versionLabel = new QLabel(this);
    versionLabel->setText("Version:");
    versionLabel->move(30, 20);
    versionLabel->show();

    engineLabel = new QLabel(this);
    engineLabel->setText("Game engine:");
    engineLabel->move(30, 90);
    engineLabel->show();

    serverLabel = new QLabel(this);
    serverLabel->setText("Download mirror:");
    serverLabel->move(285, 20);
    serverLabel->show();

    updateLabel = new QLabel(this);
    updateLabel->setText("Update policy:");
    updateLabel->move(285, 90);
    updateLabel->show();

    versionList = new QComboBox(this);

    for(liVersion = versionsList.begin(); liVersion != versionsList.end(); ++liVersion){
        versionList->addItem(liVersion->versionName);
    }

    currentVersionName = getVersionNameById(currentVersion);

    if(!currentVersionName.isEmpty()){
        versionList->setCurrentIndex((int)versionList->findText(currentVersionName));
    }

    versionList->move(29, 40);
    versionList->setMinimumWidth(190);
    versionList->show();

    engineList = new QComboBox(this);

    for(liEngine = enginesList.begin(); liEngine != enginesList.end(); ++liEngine){
        engineList->addItem(liEngine->engineName);
    }

    currentEngineName = getEngineNameById(currentEngine);

    if(!currentEngineName.isEmpty()){
        engineList->setCurrentIndex((int)engineList->findText(currentEngineName));
    }

    engineList->move(29, 110);
    engineList->setMinimumWidth(190);
    engineList->show();

    serverList = new QComboBox(this);

    for(liServer = downloadServers.begin(); liServer != downloadServers.end(); ++liServer){
        if(liServer->serverLocation != ""){
            serverList->addItem(QIcon(QString(":/images/flags/%1.png").arg(liServer->serverLocation)), liServer->serverName);
        }
        else {
            serverList->addItem(liServer->serverName);
        }
    }

    currentServerName = getServerNameById(currentServer);

    if(!currentServerName.isEmpty()){
        serverList->setCurrentIndex((int)serverList->findText(currentServerName));
    }

    serverList->move(284, 40);
    serverList->setMinimumWidth(190);
    serverList->show();

    updateBehaviorList = new QComboBox(this);
    updateBehaviorList->addItem(QString("Automatic (default)"));
    updateBehaviorList->addItem(QString("Ask me"));

    if(currentUpdateBehavior != -1){
        updateBehaviorList->setCurrentIndex(currentUpdateBehavior);
    }
    else {
        updateBehaviorList->setCurrentIndex(0);
    }

    updateBehaviorList->move(284, 110);
    updateBehaviorList->setMinimumWidth(190);
    updateBehaviorList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    setWindowTitle("Settings");
    setFixedSize(515, 220);
    setModal(true);
}

void Settings::okButtonClicked(){
    int idVersion   = getVersionIdByName(versionList->itemText(versionList->currentIndex()));
    int idServer    = getServerIdByName(serverList->itemText(serverList->currentIndex()));
    int idEngine    = getEngineIdByName(engineList->itemText(engineList->currentIndex()));
    int idUpdate    = updateBehaviorList->currentIndex();

    if(idVersion == -1 || idServer == -1 || idEngine == -1 || idUpdate == -1){
        return;
    }

    emit settingsUpdated(idVersion, idEngine, idServer, idUpdate);
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
