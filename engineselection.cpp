#include "engineselection.h"

EngineSelection::EngineSelection(QWidget* parent) : QDialog(parent){
    QPushButton* okButton = new QPushButton(this);
    QPushButton* cancelButton = new QPushButton(this);
    QLabel* selectLabel = new QLabel(this);

    okButton->setText("Ok");
    okButton->move(377, 160);
    okButton->show();

    cancelButton->setText("Go back");
    cancelButton->move(280, 160);
    cancelButton->show();

    selectLabel->setText("Select the game engine that you wish to use to run Urban Terror:");
    selectLabel->move(20, 30);
    selectLabel->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle("Game Engine selection");
    setFixedSize(450, 200);
}

EngineSelection::~EngineSelection(){

}
