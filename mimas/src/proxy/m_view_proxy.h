#ifndef MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#define MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#include "m_object_proxy.h"

class MimasWindowContent;

class MViewProxy : public MObjectProxy {
public:
  MViewProxy(MimasWindowContent *mimas, const std::string &name, const Value &type);

  virtual ~MViewProxy() {
    MessageManagerLock mml;
    if (view_) delete view_;
  }

  /** Set root and sync (do not wait for the browser to be opened).
   */
  virtual void set_root(Root *root) {
    ObjectProxy::set_root(root);
    if (root_) {
      if (value_.is_string()) build_view(value_.str());
    }
  }

  void build_view(const std::string &view_defition);
private:
  void error(const char *message, const Value &context);

  void set_bounds_from_hash(Component *component, const Value &val);

  /** TODO: replace with a generic "MWidget::update_from_hash()"
   * or "MWidget::build_from_hash()"
   */
  void build_slider(const Value &val);

  void build_pad(const Value &val);

  Component *workspace_;
  Component *view_;
};

#endif // MIMAS_SRC_PROXY_M_VIEW_PROXY_H_