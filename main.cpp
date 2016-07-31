#include "mainwindow.h"
#include "gbldata.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    GblData::initVariables();

    w.show();

    return a.exec();
}
