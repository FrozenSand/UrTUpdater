#ifndef URTUPDATER_H
#define URTUPDATER_H

#include <QMainWindow>

namespace Ui {
class UrTUpdater;
}

class UrTUpdater : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrTUpdater(QWidget *parent = 0);
    ~UrTUpdater();

private:
    Ui::UrTUpdater *ui;
};

#endif // URTUPDATER_H
