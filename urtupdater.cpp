#include "urtupdater.h"
#include "ui_urtupdater.h"

UrTUpdater::UrTUpdater(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UrTUpdater)
{
    ui->setupUi(this);
}

UrTUpdater::~UrTUpdater()
{
    delete ui;
}
