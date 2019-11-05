#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <myopenglwidget.h>
#include <joint.h>
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->doubleSpinBox->setValue(0.5);
//    connect(ui->horizontalSlider_1,SIGNAL(valueChanged(double)),ui->doubleSpinBox,SLOT(setValue(double)));

//    qDebug()<<"mainwindow mt: "<<ui->widget->MT.JointVector.begin()<<endl;
//    QVector<Joint>::iterator loop = ui->widget->MT.JointVector.begin();
//    Joint joint1 = ui->widget->MT.JointVector.at(0);
//    ui->NAME_1->setText(QString::fromStdString(joint1.getName()));
//    ui->NAME_2->setText(QString::fromStdString((loop+1)->getName()));
//    ui->NAME_3->setText(QString::fromStdString((loop+2)->getName()));
//    ui->NAME_4->setText(QString::fromStdString((loop+3)->getName()));
//    ui->NAME_5->setText(QString::fromStdString((loop+4)->getName()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->startMoving();
}
