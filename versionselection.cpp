#include "versionselection.h"

VersionSelection::VersionSelection(QWidget *parent) : QDialog(parent) {

}

void VersionSelection::init(){
    QList<versionInfo_s>::iterator li;
    QString currentVersionName;

    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->move(377, 160);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Go back");
    cancelButton->move(280, 160);
    cancelButton->show();

    selectLabel = new QLabel(this);
    selectLabel->setText("Select the version that you want to download:");
    selectLabel->move(45, 30);
    selectLabel->show();

    versionList = new QComboBox(this);

    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        versionList->addItem(li->versionName);
    }

    currentVersionName = getVersionNameById(currentVersion);

    if(!currentVersionName.isEmpty()){
        versionList->setCurrentIndex((int)versionList->findText(currentVersionName));
    }

    versionList->move(44, 70);
    versionList->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    setWindowTitle("Game version selection");
    setFixedSize(450, 200);
    setModal(true);
}

void VersionSelection::okButtonClicked(){
    int id = getVersionIdByName(versionList->itemText(versionList->currentIndex()));

    if(id == -1){
        return;
    }

    emit versionSelected(id);
    close();
}

int VersionSelection::getVersionIdByName(QString name){
    QList<versionInfo_s>::iterator li;

    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        if(li->versionName == name){
            return li->versionId;
        }
    }

    return -1;
}

QString VersionSelection::getVersionNameById(int id){
    QList<versionInfo_s>::iterator li;

    for(li = versionsList.begin(); li != versionsList.end(); ++li){
        if(li->versionId == id){
            return li->versionName;
        }
    }

    return "";
}

VersionSelection::~VersionSelection(){

}
