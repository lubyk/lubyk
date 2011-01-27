/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef LUBYK_INCLUDE_LUBYK_GL_WINDOW_H_
#define LUBYK_INCLUDE_LUBYK_GL_WINDOW_H_

#include "lubyk/oscit.h"
#include "lubyk/node.h"
#include "lubyk/opengl.h"

namespace rk {

class Planet;

/** This is a wrapper facade around an OpenGL window.
 */
class GLWindow : public Node {
public:
  GLWindow();

  virtual ~GLWindow() {
    close_window();
  }

  /** Closes the OpenGL window if it's already open.
   */
  void close_window();

  /** Open an OpenGL window. If the window is already open, this
   * method does a resize.
   */
  bool open_window(int x, int y, int width, int height);

  /** Forces a redraw of the scene.
   */
  void redraw();

  /** Set fullscreen on/off.
   * @param on_off turns fullscreen on if the value is '1', off otherwise
   * @return current fullscreen status
   */
  const Value fullscreen(const Value &on_off);

  /** This method should be implemented in sub-classes to do the actual
   * drawing.
   */
  virtual void draw() = 0;

  /** This method is called when the window dimension changes.
   */
  virtual void resized(int width, int height) = 0;

  /** Return true if the window is currently opened.
   */
  bool is_open() {
    return impl_ != NULL;
  }

private:
  bool create_window();

  class Implementation;
  Implementation *impl_;

  bool should_be_fullscreen_;

  /** Initial dimension and position of the window.
   */
  int x_,y_,width_,height_;
};

} // rk

#endif // LUBYK_INCLUDE_LUBYK_GL_WINDOW_H_