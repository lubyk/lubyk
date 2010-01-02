#ifndef MIMAS_SRC_WIDGETS_M_PAD_H_
#define MIMAS_SRC_WIDGETS_M_PAD_H_
#include "m_observable.h"
#include "m_component.h"
#include "m_range_widget.h"

class MPad;

class MPadRange : public MRangeWidget {
public:
  MPadRange(MPad *pad)
      : pad_(pad) {}

  virtual ~MPadRange() {}

  virtual void set_enabled(bool enabled);

  virtual bool is_dragged();

  virtual void redraw();

private:
  friend class MPad;
  MPad *pad_;
};

class MViewProxy;

class MPad : public MComponent, public MObservable {
public:
  MPad(MViewProxy *view_proxy, const Value &def)
      : MComponent(view_proxy, def),
        range_x_(this),
        range_y_(this),
        dragged_(false) {}

  virtual bool is_dragged() {
    return dragged_;
  }

  virtual void update(const Value &def);

  /** ======== Component callbacks ======== */

  virtual void mouseDown(const MouseEvent &e);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseUp(const MouseEvent &e);

  virtual void paint(Graphics& g);
private:
  bool dragged_;
  MPadRange range_x_;
  MPadRange range_y_;
};

#endif // MIMAS_SRC_WIDGETS_M_PAD_H_