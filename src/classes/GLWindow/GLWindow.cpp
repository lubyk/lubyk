/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#include "rubyk.h"
using namespace rk;

#include "GLWindow/gl_window.h"

class GLWindowNode : public GLWindow {
  Real red_;
public:
  const Value start() {
    return open_close(Value(1.0));
  }

  const Value open_close(const Value &val) {
    if (val.is_real()) {
      if (val.r == 0) {
        close_window();
        send(Value(0).push_back(0));
        return Value(0.0);
      } else {
        open_window(50, 50, 400, 200);
        return Value(1.0);
      }
    }

    return Value((Real)is_open());
  }

  virtual void resized(int width, int height) {
    // Reset current viewport
    glViewport( 0, 0, width, height );
    size_ = Value((Real)width).push_back((Real)height);
  }

  void draw() {
    send(size_);
  }

  const Value redraw(const Value &val) {
    GLWindow::redraw();
    return gNilValue;
  }

private:
  Value size_;
};

extern "C" void init(Planet &planet) {
  CLASS_NAMED(GLWindowNode, "GLWindow", "OpenGL window", "no options yet")
  // [1]
  METHOD(GLWindowNode, redraw, Attribute::bang_io("Redraws on bang."))
  OUTLET(GLWindowNode, draw,   Attribute::io("Sends [width, height] of view to execute OpenGL.", "opengl", "ff"))
  METHOD(GLWindowNode, open_close,  Attribute::range_io("Open/close a window (1 open, 0 close).", 0, 1))
  SUPER_METHOD(GLWindowNode, GLWindow, fullscreen, Attribute::range_io("1 to go fullscreen, 0 for window mode.", 0, 1))
}