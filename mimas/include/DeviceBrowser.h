#ifndef MIMAS_INCLUDE_DEVICE_BROWSER_H_
#define MIMAS_INCLUDE_DEVICE_BROWSER_H_

#include "mdns/abstract_browser.h"

#include <qobject.h>
#include <iostream>

class DeviceBrowser : public QObject, public mdns::AbstractBrowser
{
  Q_OBJECT
public:
  explicit DeviceBrowser(QObject *parent, const char *service_type) :
    QObject(parent),
    mdns::AbstractBrowser(service_type) {
    start();
  }

  ~DeviceBrowser() {
    stop();
  }

  /** This method is called just after a new proxy has been added to the list.
  */
  virtual void add_device(const mdns::Location &location);

  /** This method is called so that you have an opportunity to delete it cleanly.
  */
  virtual void remove_device(const char *name);

signals:
  void message(const QString&);
};

#endif // MIMAS_INCLUDE_DEVICE_BROWSER_H_
