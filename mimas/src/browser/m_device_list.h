#ifndef MIMAS_SRC_DEVICE_BROWSER_M_DEVICE_LIST_H_
#define MIMAS_SRC_DEVICE_BROWSER_M_DEVICE_LIST_H_
#include "mimas.h"

#include <vector>

class MimasWindowContent;

class MDeviceList : public ListBox, public ListBoxModel {
public:
  MDeviceList(MimasWindowContent *mimas);

  void add_device(RootProxy *device);

  void remove_device(RootProxy *device);

  // ======== ListBoxModel callbacks =========== //

  virtual int getNumRows();

  virtual void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);

  // ======== ListBox callbacks     ============ //
  virtual void paint(Graphics &g);

private:
  MimasWindowContent *mimas_;

  std::vector<RootProxy*> devices_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_M_DEVICE_LIST_H_
