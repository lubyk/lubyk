#include "MimasWindow.h"
#include "ui_MimasWindow.h"

MimasWindow::MimasWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MimasWindow)
{
    ui->setupUi(this);
}

MimasWindow::~MimasWindow()
{
    delete ui;
}
