#include "MimasWindow.h"
#include "ui_MimasWindow.h"

#include "DeviceBrowser.h"

MimasWindow::MimasWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MimasWindow),
    device_browser_(new DeviceBrowser("_rubyk._tcp"))
{
    ui->setupUi(this);
}

MimasWindow::~MimasWindow()
{
    delete device_browser_;
    delete ui;
}
