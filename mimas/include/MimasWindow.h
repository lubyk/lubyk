#ifndef MIMASWINDOW_H
#define MIMASWINDOW_H

#include <QMainWindow>

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
};

#endif // MIMASWINDOW_H
