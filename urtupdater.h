#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>
#include "common.h"

namespace Ui {
    class UrTUpdater;
}

class UrTUpdater : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrTUpdater(QWidget *parent = 0);
    ~UrTUpdater();
    void init();
    void quit();
    QString getPlatform();
    QString getCurrentPath();

private:
    Ui::UrTUpdater *ui;

    QString updaterPath;
};

#endif // URTUPDATER_H
