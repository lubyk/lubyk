#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
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

    QPushButton *quit_btn = new QPushButton("Quit");
    QObject::connect(
      quit_btn, SIGNAL(clicked()),
      &a,         SLOT(quit()));

    layout->addWidget(quit_btn);

    DeviceBrowser *browser = new DeviceBrowser(&window, "_rubyk._tcp");

    QObject::connect(
      browser, SIGNAL(message(const QString&)),
      label,     SLOT(setText(const QString&)));

    window.show();
    return a.exec();
}
