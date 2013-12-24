#include "serverselection.h"

ServerSelection::ServerSelection(QWidget *parent) : QDialog(parent) {

}

void ServerSelection::init(){
    QList<serverInfo_s>::iterator li;
    QString currentServerName;

    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->move(377, 160);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Go back");
    cancelButton->move(280, 160);
    cancelButton->show();

    selectLabel = new QLabel(this);
    selectLabel->setText("Select a download mirror depending on your location:");
    selectLabel->move(45, 30);
    selectLabel->show();

    serverList = new QComboBox(this);

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverLocation != ""){
            serverList->addItem(QIcon(QString(":/flags/%1.png").arg(li->serverLocation)), li->serverName);
        }
        else {
            serverList->addItem(li->serverName);
        }
    }

    currentServerName = getServerNameById(currentServer);

    if(!currentServerName.isEmpty()){
        serverList->setCurrentIndex((int)serverList->findText(currentServerName));
    }

    serverList->move(44, 70);
    serverList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    setWindowTitle("Mirror server selection");
    setFixedSize(450, 200);
    setModal(true);
}

void ServerSelection::okButtonClicked(){
    int id = getServerIdByName(serverList->itemText(serverList->currentIndex()));

    if(id == -1){
        return;
    }

    emit serverSelected(id);
    close();
}

int ServerSelection::getServerIdByName(QString name){
    QList<serverInfo_s>::iterator li;

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverName == name){
            return li->serverId;
        }
    }

    return -1;
}

QString ServerSelection::getServerNameById(int id){
    QList<serverInfo_s>::iterator li;

    for(li = downloadServers.begin(); li != downloadServers.end(); ++li){
        if(li->serverId == id){
            return li->serverName;
        }
    }

    return "";
}

ServerSelection::~ServerSelection(){

}
