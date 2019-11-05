#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
#include <control_panel.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //QWidget widget;
    w.resize(1024,768);
    w.show();

//    Control_panel control_panel;
//    //QWidget widget;
//    control_panel.show();

    return a.exec();
}
