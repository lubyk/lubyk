#ifndef MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_
#define MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_

class MRootProxy;

class RootProxyTreeItem : public TreeViewItem, public Observable {
public:

  RootProxyTreeItem(MRootProxy *proxy);

  MRootProxy *device_proxy() {
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
  MRootProxy *device_proxy_;
  bool subnodes_added_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_ROOT_PROXY_TREE_ITEM_H_