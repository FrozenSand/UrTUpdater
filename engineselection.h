#ifndef ENGINESELECTION_H
#define ENGINESELECTION_H

#include "common.h"

class EngineSelection : public QDialog
{
    Q_OBJECT

public:
    explicit EngineSelection(QWidget *parent = 0);
    ~EngineSelection();
    void init();
    int getEngineIdByName(QString);
    QString getEngineNameById(int);

    QList<engineInfo_s> enginesList;
    int currentEngine;

public slots:
    void okButtonClicked();

signals:
    void engineSelected(int);

private:
    QPushButton* okButton;
    QPushButton* cancelButton;
    QLabel* selectLabel;
    QComboBox* el;

};

#endif // ENGINESELECTION_H
