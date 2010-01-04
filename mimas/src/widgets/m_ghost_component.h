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
