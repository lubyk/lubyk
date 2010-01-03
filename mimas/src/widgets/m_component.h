#ifndef MIMAS_SRC_WIDGETS_M_COMPONENT_H_
#define MIMAS_SRC_WIDGETS_M_COMPONENT_H_
#include "m_observable.h"
#include "m_range_widget.h"
#include "m_ghost_component.h"

class MimasWindowContent;
class MViewProxy;
class MRootProxy;

class MComponent : public Component, public MObservable {
public:
  MComponent(const std::string &part_id, MViewProxy *view_proxy);

  /** =========== MComponent callbacks  ========== */

  // TODO: move some of the MRangeWidget callbacks here (dragging...)

  /** Update component from a hash definition.
   */
  virtual void update(const Value &def);

  /** Returns true if the component is connected to a proxy.
   */
  virtual bool is_connected() {
    return false;
  }

  /** Set color hue (must be a value from 0 to 360 degrees).
   */
  void set_hue(float hue);

  MimasWindowContent *mimas() {
    return mimas_;
  }

protected:
  /** The name of this part in the view definition.
   */
  std::string part_id_;

  /** Cached access to the main window content.
   */
  MimasWindowContent *mimas_;

  /** Cached access to the view proxy this component's definition
   * originated from.
   */
  MViewProxy *view_proxy_;

  /** Cached access to the remote end.
   */
  RootProxy *root_proxy_;

  /** A ghost used to asynchronously drag the component.
   */
  MGhostComponent ghost_component_;

  /** Component dragger used to move the ghost
   * component.
   */
  ComponentDragger dragger_;

  /** The component's color.
   */
  float  hue_;

  /** Cached border color.
   */
  Colour border_color_;

  /** Cached fill color.
   */
  Colour fill_color_;

  void error(const char *message, const Value &context) {
    std::cerr << "Error: '" << message << " " << context << ".\n";
  }

  /** Add a callback to process some pending definitions when an url
   * becomes live.
   */
  void add_callback(const std::string &path, const Value &def);

private:
  class OnRegistrationCallback;

  void on_registration_callback(void *data);
};

#endif // MIMAS_SRC_WIDGETS_M_COMPONENT_H_
