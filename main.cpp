#include "urtupdater.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UrTUpdater w;
    w.show();

    return a.exec();
}
