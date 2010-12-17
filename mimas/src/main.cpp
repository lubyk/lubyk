#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include "MimasWindow.h"

#include "DeviceBrowser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;
    window.resize(320, 240);
    QHBoxLayout *layout = new QHBoxLayout(&window);

    QLabel *label = new QLabel();
    layout->addWidget(label);

    DeviceBrowser *browser = new DeviceBrowser(&window, "_rubyk._tcp");

    QObject::connect(
      browser, SIGNAL(message(const QString&)),
      label,     SLOT(setText(const QString&)));

    window.show();
    return a.exec();
}
