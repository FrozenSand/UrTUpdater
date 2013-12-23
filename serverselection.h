#ifndef SERVERSELECTION_H
#define SERVERSELECTION_H

#include "common.h"

class ServerSelection : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSelection(QWidget *parent = 0);
    ~ServerSelection();
    void init();
    int getServerIdByName(QString);
    QString getServerNameById(int);

    QList<serverInfo_s> downloadServers;
    int currentServer;

public slots:
    void okButtonClicked();

signals:
    void serverSelected(int);

private:
    QPushButton* okButton;
    QPushButton* cancelButton;
    QLabel* selectLabel;
    QComboBox* serverList;
};

#endif // SERVERSELECTION_H
