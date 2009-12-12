#ifndef MIMAS_SRC_PROXY_VIEW_PROXY_VIEW_H_
#define MIMAS_SRC_PROXY_VIEW_PROXY_VIEW_H_
#include "m_object_proxy.h"

class ProxyView : public MObjectProxy {
public:
  ProxyView(const std::string &name, const Value &type, Component *main_view) : MObjectProxy(name, type), main_view_(main_view) {}

  /** Set root and sync (do not wait for the browser to be opened).
   */
  virtual void set_root(Root *root) {
    ObjectProxy::set_root(root);
    if (value_.is_string()) build_view(value_.str());
  }

  void build_view(const std::string &view_defition);
private:
  bool check_attribute(XmlElement &definition, const char *type, const tchar *const name);
  void build_slider(XmlElement &def);
  Component *main_view_;
};

#endif // MIMAS_SRC_PROXY_VIEW_PROXY_VIEW_H_