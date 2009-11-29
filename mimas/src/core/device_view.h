#ifndef MIMAS_SRC_CORE_DEVICE_VIEW_H_
#define MIMAS_SRC_CORE_DEVICE_VIEW_H_

class DeviceProxy;

class DeviceView : public TreeViewItem, public Observer, public Observable {
public:
  /** Class signature. */
  TYPED("Object.Root.RootProxy.DeviceProxy")

  DeviceView(DeviceProxy *proxy);

  DeviceProxy *device_proxy() {
    return device_proxy_;
  }

  virtual void paintItem(Graphics& g, int width, int height);

  bool mightContainSubItems() {
    return true;
  }
private:
  DeviceProxy *device_proxy_;
};

#endif // MIMAS_SRC_CORE_DEVICE_VIEW_H_