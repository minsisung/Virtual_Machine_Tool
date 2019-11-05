#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QDialog>

namespace Ui {
class Control_panel;
}

class Control_panel : public QDialog
{
    Q_OBJECT

public:
    explicit Control_panel(QWidget *parent = nullptr);
    ~Control_panel();

private:
    Ui::Control_panel *ui;
};

#endif // CONTROL_PANEL_H
