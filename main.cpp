#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dbconnect.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("russian", "D:/QtProjects/MFarm/res");
    a.installTranslator(&translator);

    QSettings settings("Bdm Soft", "MFarm");

    settings.beginGroup("MainWindow");
    settings.endGroup();
    settings.beginGroup("Databases");
    //settings.setValue("size", 777);
    //settings.setValue("pos", pos());

    settings.endGroup();


    MainWindow w;
    w.show();

    
    return a.exec();
}
