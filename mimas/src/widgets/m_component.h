#ifndef MIMAS_SRC_WIDGETS_M_COMPONENT_H_
#define MIMAS_SRC_WIDGETS_M_COMPONENT_H_
#include "m_observable.h"
#include "m_range_widget.h"

class MimasWindowContent;
class MViewProxy;
class MRootProxy;

class MComponent : public Component {
public:
  MComponent(MViewProxy *view_proxy, const std::string &name);

  MComponent(MViewProxy *view_proxy, const Value &def);

  /** =========== MComponent callbacks  ========== */

  // TODO: move some of the MRangeWidget callbacks here (dragging...)

  /** Update component from a hash definition.
   */
  virtual void update(const Value &def);

  /** Set color hue (must be a value from 0 to 360 degrees).
   */
  void set_hue(float hue);

protected:
  MimasWindowContent *mimas_;
  MViewProxy *view_proxy_;
  RootProxy *root_proxy_;

  Colour border_color_;
  Colour fill_color_;

  void error(const char *message, const Value &context) {
    std::cerr << "Error: '" << message << " " << context << ".\n";
  }

};

#endif // MIMAS_SRC_WIDGETS_M_COMPONENT_H_
