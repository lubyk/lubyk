#ifndef MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_
#define MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_

class RootProxy;

class RootProxyListItem : public TreeViewItem, public Observable {
public:

  RootProxyListItem(RootProxy *proxy);

  RootProxy *device_proxy() {
    return device_proxy_;
  }

  virtual void paintItem(Graphics& g, int width, int height);

  bool mightContainSubItems() {
    return true;
  }

  void itemOpennessChanged( bool isNowOpen);

  virtual void observer_lock() {
    mml_ = new MessageManagerLock();
  }

  virtual void observer_unlock() {
    delete mml_;
  }

private:
  MessageManagerLock *mml_;
  RootProxy *device_proxy_;
  bool subnodes_added_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_