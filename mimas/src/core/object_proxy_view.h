#ifndef MIMAS_SRC_CORE_OBJECT_PROXY_VIEW_H_
#define MIMAS_SRC_CORE_OBJECT_PROXY_VIEW_H_
#include "oscit/object_proxy.h"
#include "observable_in_juce.h"
class SimpleObjectProxy;

class ObjectProxyView : public TreeViewItem, public ObservableInJuce {
public:

  ObjectProxyView(SimpleObjectProxy *proxy);

  SimpleObjectProxy *object_proxy() {
    return object_proxy_;
  }

  virtual void paintItem(Graphics& g, int width, int height);

  virtual Component *createItemComponent();

  virtual bool mightContainSubItems();

  virtual void itemOpennessChanged(bool isNowOpen);

private:
  SimpleObjectProxy *object_proxy_;
  bool subnodes_added_;
};

#endif // MIMAS_SRC_CORE_OBJECT_PROXY_VIEW_H_