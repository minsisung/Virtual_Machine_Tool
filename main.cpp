#include "mainwindow.h"
#include <QApplication>
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //QWidget widget;
    w.resize(1024,768);
    w.show();
    return a.exec();
}
