#ifndef MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_
#define MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_
#include "oscit/object_proxy.h"
#include "m_observable.h"

class MimasWindowContent;
class MObjectProxy;

class ObjectProxyTreeItem : public TreeViewItem, public MObservable {
public:

  ObjectProxyTreeItem(MimasWindowContent *mimas, MObjectProxy *proxy);

  MObjectProxy *object_proxy() {
    return object_proxy_;
  }

  virtual void paintItem(Graphics& g, int width, int height);

  virtual Component *createItemComponent();

  virtual bool mightContainSubItems();

  virtual void itemOpennessChanged(bool isNowOpen);

private:
  MimasWindowContent *mimas_;
  MObjectProxy *object_proxy_;
  bool subnodes_added_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_