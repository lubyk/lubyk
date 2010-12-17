#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include "MimasWindow.h"

#include "DeviceBrowser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.show();

    QLabel *label = new QLabel(&window);
    label->move(50, 100);
    label->resize(200, 20);
    label->show();

    DeviceBrowser *browser = new DeviceBrowser(&window, "_rubyk._tcp");

    QObject::connect(
      browser, SIGNAL(message(const QString&)),
      label,     SLOT(setText(const QString&)));

    return a.exec();
}
