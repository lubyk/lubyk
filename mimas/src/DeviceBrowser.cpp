#include "DeviceBrowser.h"


/** This method is called just after a new proxy has been added to the list.
 */
void DeviceBrowser::add_device(const mdns::Location &location) {
  emit(message(QString("Adding %1").arg(location.name())));
}

/** This method is called so that you have an opportunity to delete it cleanly.
 */
void DeviceBrowser::remove_device(const char *name) {
  emit(message(QString("Removing %1").arg(name)));
}