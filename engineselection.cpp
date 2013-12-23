#include "engineselection.h"

EngineSelection::EngineSelection(QWidget* parent) : QDialog(parent){

}

void EngineSelection::init(){
    QList<engineInfo_s>::iterator li;

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

    el->move(44, 70);
    el->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle("Engine selection");
    setFixedSize(450, 200);
    setModal(true);
}

EngineSelection::~EngineSelection(){

}
