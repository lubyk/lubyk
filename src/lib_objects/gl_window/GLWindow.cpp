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
#include "rubyk.h"
#include "gl_window.h"

class GLWindowNode : public GLWindow {
  Real red_;
public:
  const Value start() {
    return open(gNilValue);
  }

  const Value close(const Value &val) {
    close_window();
    return gNilValue;
  }

  const Value open(const Value &val) {
    open_window(50, 50, 400, 200);
    return gNilValue;
  }

  const Value red(const Value &val) {
    if (val.is_real()) {
      red_ = val.r;
      redraw();
    }
    return Value(red_);
  }

  virtual void resized(int width, int height) {
    // Reset current viewport
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );   // Select the projection matrix
    glLoadIdentity();                // and reset it
    // Calculate the aspect ratio of the view
    gluPerspective( 45.0f, (float)width / height,
      0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix
    glLoadIdentity();                // and reset it
  }

  virtual void draw() {
    // Clear the screen and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();   // Reset the current modelview matrix

    glTranslatef( -1.5f, 0.0f, -6.0f );   // Left 1.5 units, into screen 6.0

    glColor3f(1.0f,1.0f,1.0f);
    glBegin( GL_TRIANGLES );             // Draw a triangle
      glVertex3f(  0.0f,  1.0f, 0.0f );    // Top
      glVertex3f( -1.0f, -1.0f, 0.0f );    // Bottom left
      glColor3f(0.5f,0.2f,0.0f);
      glVertex3f(  1.0f, -1.0f, 0.0f );    // Bottom right
    glEnd();                             // Done with the triangle

    glTranslatef( 3.0f, 0.0f, 0.0f );    // Move right 3 units

    glColor3f(red_,0.5f,0.0f);
    glBegin( GL_QUADS );                // Draw a quad
      glVertex3f( -1.0f,  1.0f, 0.0f );   // Top left
      glVertex3f(  1.0f,  1.0f, 0.0f );   // Top right
      glVertex3f(  1.0f, -1.0f, 0.0f );   // Bottom right
      glVertex3f( -1.0f, -1.0f, 0.0f );   // Bottom left
    glEnd();                            // Quad is complete
  }
};

extern "C" void init(Planet &planet) {
  CLASS_NAMED(GLWindowNode, "GLWindow", "OpenGL window", "no options yet")
  METHOD(GLWindowNode, open,  BangIO("Open a window."))
  METHOD(GLWindowNode, close, BangIO("Close opened window."))
  METHOD(GLWindowNode, red, RangeIO(0,1,"red", "Change triangle color."))
}