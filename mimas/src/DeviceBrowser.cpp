#include "DeviceBrowser.h"


/** This method is called just after a new proxy has been added to the list.
 */
void DeviceBrowser::add_device(const mdns::Location &location) {
  std::cout << "ADD: " << location << "\n";
}

/** This method is called so that you have an opportunity to delete it cleanly.
 */
void DeviceBrowser::remove_device(const char *name) {
  std::cout << "DEL: " << name << "\n";
}