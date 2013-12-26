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

public slots:
    void okButtonClicked();

signals:
    void versionSelected(int);

private:
    QPushButton*    okButton;
    QPushButton*    cancelButton;
    QLabel*         selectLabel;
    QComboBox*      versionList;
};

#endif // VERSIONSELECTION_H
