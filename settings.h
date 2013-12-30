#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void init();

    int getVersionIdByName(QString);
    int getServerIdByName(QString);
    int getEngineIdByName(QString);

    QString getVersionNameById(int);
    QString getServerNameById(int);
    QString getEngineNameById(int);

    QList<versionInfo_s>    versionsList;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;

    int currentVersion;
    int currentServer;
    int currentEngine;

    QString password;

public slots:
    void okButtonClicked();

signals:
    void settingsUpdated(int, int, int);

private:
    QPushButton*    okButton;
    QPushButton*    cancelButton;
    QLabel*         selectLabel;

    QComboBox*      versionList;
    QComboBox*      serverList;
    QComboBox*      engineList;

    QLabel*         versionLabel;
    QLabel*         serverLabel;
    QLabel*         engineLabel;

};

#endif // SETTINGS_H
