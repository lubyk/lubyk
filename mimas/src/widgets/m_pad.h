/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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