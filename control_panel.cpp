#include "control_panel.h"
#include "ui_control_panel.h"
#include <mainwindow.h>

Control_panel::Control_panel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Control_panel)
{
    ui->setupUi(this);
}

Control_panel::~Control_panel()
{
    delete ui;
}
