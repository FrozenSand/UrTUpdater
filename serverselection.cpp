#include "serverselection.h"

ServerSelection::ServerSelection(QWidget *parent) : QDialog(parent) {

}

void ServerSelection::init(){
    QList<serverInfo_s>::iterator li;

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
        serverList->addItem(QIcon(QString(":/flags/%1.png").arg(li->serverLocation)), li->serverName);
    }

    serverList->move(44, 70);
    serverList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle("Mirror server selection");
    setFixedSize(450, 200);
    setModal(true);
}

ServerSelection::~ServerSelection(){

}
