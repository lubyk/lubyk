#ifndef MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#define MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#include "m_object_proxy.h"
#include "m_component.h"

class MimasWindowContent;
class MUpdateViewProxy;
class MDeviceView;

class MViewProxy : public MObjectProxy {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.MObjectProxy.MViewProxy")

  MViewProxy(MimasWindowContent *mimas, const std::string &name, const Value &type);

  virtual ~MViewProxy();

  /** ViewProxy is ready: force sync and add view to workspace.
   */
  virtual void adopted();

  virtual void value_changed() {
    if (!value_.is_hash()) {
      std::cerr << "'" << url() << "' error: value should be a Hash. Found " << value_ << "\n";
      return;
    }

    MessageManagerLock mml;
    update_view(value_);
  }

  /** This method is called by widgets when something changed on our side. If the
   * remote implements the "update" method, the latter is called, otherwise we just
   * send and update with the full view definition.
   */
  void update_remote(const Value& hash);

  virtual Object *build_child(const std::string &name, const Value &type, Value *error);

  MDeviceView *view() {
    if (!view_) {
      update_view(value_);
    }

    // TODO: resize view
    // this should be done in the caller of the view() method.
    //workspace_->addAndMakeVisible(view_);
    return view_;
  }

  /** Return the current window's content manager.
   */
  MimasWindowContent *mimas() {
    return mimas_;
  }

  /** Return the current tree's root.
   */
  RootProxy *root_proxy() {
    return root_proxy_;
  }

  /** Read a (partial) view definition and build or update the
   * view components.
   */
  void update_view(const Value &def);

private:

  MDeviceView *view_;

  /** Simple object to handle messages to the method "this_view/update".
   */
  MUpdateViewProxy *update_view_proxy_;
};

#endif // MIMAS_SRC_PROXY_M_VIEW_PROXY_H_