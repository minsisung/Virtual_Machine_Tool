#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
#include "urdfparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //QWidget widget;

    readURDF("umc500.urdf");

    w.resize(1024,768);
    w.show();
    return a.exec();
}
