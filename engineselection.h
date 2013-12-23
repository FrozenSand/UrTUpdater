#ifndef ENGINESELECTION_H
#define ENGINESELECTION_H

#include "common.h"

class EngineSelection : public QDialog
{

public:
    explicit EngineSelection(QWidget *parent = 0);
    ~EngineSelection();
    void init();
    QList<engineInfo_s> enginesList;

private:
    QPushButton* okButton;
    QPushButton* cancelButton;
    QLabel* selectLabel;
    QComboBox* el;

};

#endif // ENGINESELECTION_H
