#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("OpenRacetiming");
    QCoreApplication::setOrganizationDomain("openracetiming.net");
    QCoreApplication::setApplicationName("OpenRacetiming UI");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
