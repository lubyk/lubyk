#ifndef MIMAS_SRC_WIDGETS_M_PAD_H_
#define MIMAS_SRC_WIDGETS_M_PAD_H_
#include "m_observable.h"

#include <iostream>

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


class MPad : public Component, public MObservable {
public:
  MPad(const std::string &name)
      : Component(String(name.c_str())),
        range_x_(this),
        range_y_(this),
        dragged_(false) {}

  virtual bool is_dragged() {
    return dragged_;
  }

  /** ======== Component callbacks ======== */

  virtual void mouseDown(const MouseEvent &e);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseUp(const MouseEvent &e);

  /** TODO: Where is this used ? */
  MRangeWidget *range_x() {
    return &range_x_;
  }

  /** TODO: Where is this used ? */
  MRangeWidget *range_y() {
    return &range_y_;
  }

  virtual void paint(Graphics& g);
private:
  bool dragged_;
  MPadRange range_x_;
  MPadRange range_y_;
};

#endif // MIMAS_SRC_WIDGETS_M_PAD_H_