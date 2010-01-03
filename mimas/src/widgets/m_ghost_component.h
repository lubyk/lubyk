#ifndef MIMAS_SRC_WIDGETS_M_GHOST_COMPONENT_H_
#define MIMAS_SRC_WIDGETS_M_GHOST_COMPONENT_H_
#include "m_observable.h"
#include "m_range_widget.h"

class MimasWindowContent;
class MComponent;

/** This component appears when an MComponent is being dragged or resized. It just displays
 * a rectangle and sends a view update when drag ends.
 */
class MGhostComponent : public Component, public MObservable {
public:
  MGhostComponent(MComponent *master);

  /** =========== Component callbacks  ========== */

  // virtual void mouseDown(const MouseEvent &e);
  // virtual void mouseDrag(const MouseEvent &e);
  // virtual void mouseUp(const MouseEvent &e);

  virtual void paint(Graphics &g);

protected:
  MimasWindowContent *mimas_;

  /** The component that was dragged and will be notified on drag end.
   */
  MComponent *master_;
};

#endif // MIMAS_SRC_WIDGETS_M_GHOST_COMPONENT_H_
