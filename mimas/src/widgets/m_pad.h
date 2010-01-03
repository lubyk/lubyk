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

class MPad : public MComponent {
public:
  MPad(const std::string &part_id, MViewProxy *view_proxy)
      : MComponent(part_id, view_proxy),
        range_x_(this),
        range_y_(this),
        is_dragged_(false) {}

  virtual bool is_dragged() {
    return is_dragged_;
  }

  virtual void update(const Value &def);

  /** Returns true if the slider is connected to a proxy.
   */
  virtual bool is_connected();

  /** ======== Component callbacks ======== */

  virtual void mouseDown(const MouseEvent &e);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseUp(const MouseEvent &e);

  virtual void paint(Graphics& g);

  /** ======== MPadRange callbacks ======== */

  void set_enabled(bool enabled);
private:
  bool is_dragged_;
  MPadRange range_x_;
  MPadRange range_y_;
};

#endif // MIMAS_SRC_WIDGETS_M_PAD_H_