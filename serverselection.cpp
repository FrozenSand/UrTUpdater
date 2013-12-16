#include "serverselection.h"

ServerSelection::ServerSelection(QWidget *parent) : QDialog(parent) {
    QPushButton* okButton = new QPushButton(this);
    QPushButton* cancelButton = new QPushButton(this);
    QLabel* selectLabel = new QLabel(this);

    okButton->setText("Ok");
    okButton->move(377, 160);
    okButton->show();

    cancelButton->setText("Go back");
    cancelButton->move(280, 160);
    cancelButton->show();

    selectLabel->setText("Select a download mirror depending on your location:");
    selectLabel->move(45, 30);
    selectLabel->show();

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle("Mirror server selection");
    setFixedSize(450, 200);
}

ServerSelection::~ServerSelection(){

}
