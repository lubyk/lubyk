#ifndef MIMASWINDOW_H
#define MIMASWINDOW_H

#include <QMainWindow>

class DeviceBrowser;

namespace Ui {
    class MimasWindow;
}

class MimasWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MimasWindow(QWidget *parent = 0);
    ~MimasWindow();

private:
    Ui::MimasWindow *ui;
    DeviceBrowser   *device_browser_;
};

#endif // MIMASWINDOW_H
