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
    QList<serverInfo_s> downloadServers;

signals:
    void getServerManifest();

};

#endif // SERVERSELECTION_H
