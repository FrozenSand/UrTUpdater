#include "engineselection.h"

EngineSelection::EngineSelection(QWidget* parent) : QDialog(parent){

}

void EngineSelection::init(){
    QList<engineInfo_s>::iterator li;
    QString currentEngineName;

    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->move(377, 160);
    okButton->show();

    cancelButton = new QPushButton(this);
    cancelButton->setText("Go back");
    cancelButton->move(280, 160);
    cancelButton->show();

    selectLabel = new QLabel(this);
    selectLabel->setText("Select the game engine that you want to use:");
    selectLabel->move(45, 30);
    selectLabel->show();

    el = new QComboBox(this);

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        el->addItem(li->engineName);
    }

    currentEngineName = getEngineNameById(currentEngine);

    if(!currentEngineName.isEmpty()){
        el->setCurrentIndex((int)el->findText(currentEngineName));
    }

    el->move(44, 70);
    el->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    setWindowTitle("Engine selection");
    setFixedSize(450, 200);
    setModal(true);
}

void EngineSelection::okButtonClicked(){
    int id = getEngineIdByName(el->itemText(el->currentIndex()));

    if(id == -1){
        return;
    }

    emit engineSelected(id);
    close();
}

int EngineSelection::getEngineIdByName(QString name){
    QList<engineInfo_s>::iterator li;

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineName == name){
            return li->engineId;
        }
    }

    return -1;
}

QString EngineSelection::getEngineNameById(int id){
    QList<engineInfo_s>::iterator li;

    for(li = enginesList.begin(); li != enginesList.end(); ++li){
        if(li->engineId == id){
            return li->engineName;
        }
    }

    return "";
}


EngineSelection::~EngineSelection(){

}
