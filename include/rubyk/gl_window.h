/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#ifndef RUBYK_INCLUDE_RUBYK_GL_WINDOW_H_
#define RUBYK_INCLUDE_RUBYK_GL_WINDOW_H_

#include "rubyk/oscit.h"
#include "rubyk/node.h"
#include "rubyk/opengl.h"

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

  /** This method should be implemented in sub-classes to do the actual
   * drawing.
   */
  virtual void draw() = 0;

  /** This method is called when the window dimension changes.
   */
  virtual void resized(int width, int height) = 0;

private:
  class Implementation;
  Implementation *impl_;
};
#endif // RUBYK_INCLUDE_RUBYK_GL_WINDOW_H_