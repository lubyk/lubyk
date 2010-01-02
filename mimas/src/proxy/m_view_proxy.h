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
    if (!value_.is_string()) {
      std::cout << "'" << url() << "' error: value should be a string. Found " << value_ << "\n";
      return;
    }

    Value def; // TODO: why doesn't "Value def(Json(view_def));" work ?
    def.build_from_json(value_.c_str());
    if (!def.is_hash()) {
      std::cerr << "Json document is not a hash '" << url() << "': " << def << "\n";
      return;
    }

    MessageManagerLock mml;
    update_view(def);
  }

  /** This method is called by widgets when something changed on our side. If the
   * remote implements the "update" method, the latter is called, otherwise we just
   * send and update with the full view definition.
   */
  void update_remote(const Value& hash);

  virtual Object *build_child(const std::string &name, const Value &type, Value *error);

  MDeviceView *view() {
    if (!view_) {
      if (view_definition_.is_empty() && value_.is_string()) {
        view_definition_.build_from_json(value_.c_str());
      }
      update_view(view_definition_);
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
  Value view_definition_;

  /** Simple object to handle messages to the method "this_view/update".
   */
  MUpdateViewProxy *update_view_proxy_;
};

#endif // MIMAS_SRC_PROXY_M_VIEW_PROXY_H_