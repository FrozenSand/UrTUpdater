#ifndef VERSIONSELECTION_H
#define VERSIONSELECTION_H

#include "common.h"

class VersionSelection : public QDialog
{
    Q_OBJECT

public:
    explicit VersionSelection(QWidget *parent = 0);
    ~VersionSelection();
    void init();
    int getVersionIdByName(QString);
    QString getVersionNameById(int);

    QList<versionInfo_s> versionsList;
    int currentVersion;
    QString password;

public slots:
    void okButtonClicked();
    void passwordEntered();

signals:
    void versionSelected(int);
    void passwordEntered(QString);

private:
    QPushButton*    okButton;
    QPushButton*    cancelButton;
    QLabel*         selectLabel;
    QComboBox*      versionList;
    QLabel*         passwordLabel;
    QLineEdit*      passwordField;
    QPushButton*    passwordOkButton;
};

#endif // VERSIONSELECTION_H
