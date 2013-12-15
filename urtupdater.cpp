#include "urtupdater.h"
#include "ui_urtupdater.h"

UrTUpdater::UrTUpdater(QWidget *parent) : QMainWindow(parent), ui(new Ui::UrTUpdater)
{
    ui->setupUi(this);
    init();
}

UrTUpdater::~UrTUpdater()
{
    delete ui;
}

void UrTUpdater::init(){

    updaterPath = getCurrentPath();

    // Check if this is the first launch of the updater
    if(!QFile::exists(updaterPath + URT_GAME_SUBDIR)){
        QMessageBox msg;
        int result;

        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setIcon(QMessageBox::Information);
        msg.setText("The game " URT_GAME_NAME " will be installed in this path:\n" + updaterPath + "\n"
                    + "To change the installation path, please click on \"Cancel\" and copy this Updater to where you want it to be installed.");
        result = msg.exec();

        // If we want to quit
        if(result == QMessageBox::Cancel){
            quit();
        }

        // Create the game folder
        else {
            if(!QDir().mkdir(updaterPath + URT_GAME_SUBDIR)){
                QMessageBox::critical(this, QString(URT_GAME_SUBDIR) + " folder", "Could not create the game folder (" + updaterPath + URT_GAME_SUBDIR + ").\n"
                                      + "Please move the updater to a folder where it has sufficient permissions.");
                quit();
            }
        }
    }
}

QString UrTUpdater::getPlatform()
{
    #ifdef Q_OS_MAC
    return "Mac";
    #endif

    #ifdef Q_OS_LINUX
    return "Linux";
    #endif

    #ifdef Q_OS_WIN32
    return "Windows";
    #endif

    return "Linux";
}

QString UrTUpdater::getCurrentPath(){
    QDir dir = QDir(QCoreApplication::applicationDirPath());

    // If we're on Mac, 'dir' will contain the path to the executable
    // inside of the Updater's bundle which isn't what we want.
    // We need to cd ../../..
    if(getPlatform() == "Mac"){
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }

    return dir.absolutePath() + "/";
}

void UrTUpdater::quit(){
    exit(0);
}
