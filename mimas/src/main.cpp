#include <QtGui/QApplication>
#include "MimasWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MimasWindow w;
    w.show();

    return a.exec();
}
