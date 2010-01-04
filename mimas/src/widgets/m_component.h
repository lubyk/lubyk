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

  /** =========== Component callbacks   ========== */

  virtual void mouseDown(const MouseEvent &e);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseUp(const MouseEvent &e);

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

  /** Return true if the component should react to mouse event.
   */
  bool should_handle_mouse();

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
