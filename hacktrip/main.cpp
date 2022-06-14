#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
