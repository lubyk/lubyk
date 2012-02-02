/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#ifndef LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_
#define LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_

#include "lubyk.h"
using namespace lubyk;

#include "mimas/mimas.h"
#include "mimas/Widget.h"

#include <QGLWidget>
#include <QGLContext>

#ifdef __macosx__
void* mimasSelectModernOpenGLMac(GDHandle handle);
#endif

namespace mimas {

  
/** GLSLWidget
 * This class lets you draw OpenGL elements with Widgets on top.
 * http://doc.trolltech.com/qq/qq26-openglcanvas.html
 *
 * @dub destructor: 'luaDestroy'
 *      super: 'QWidget'
 */
class GLSLWidget : public QGLWidget, public ThreadedLuaObject
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

#ifdef __macosx__
  struct GLSLContext : public QGLContext {
    GLSLContext(const QGLFormat& format, QPaintDevice* device) : QGLContext(format,device) {}
    GLSLContext(const QGLFormat& format) : QGLContext(format) {}

    virtual void* chooseMacVisual(GDHandle handle) {
      return mimasSelectModernOpenGLMac(handle);
    }
  };
#else
  struct GLSLFormoat {
    GLSLFormoat() {
      setVersion(3, 2);
      setProfile(QGLFormat::CoreProfile);
      setSampleBuffers(true);
    }
  };
#endif // __macosx__

public:
 
  GLSLWidget()
#ifdef __macosx__
    : QGLWidget(new GLSLContext(QGLFormat::defaultFormat()))
#else
    : QGLWidget(GLSLFormoat())
#endif
  {
    setAttribute(Qt::WA_DeleteOnClose);
    // get focus on tab and click
    setFocusPolicy(Qt::StrongFocus);
    MIMAS_DEBUG_CC
  }   

  ~GLSLWidget() {
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return parent() ? QString("glwindow") : QString("glwidget");
  }

  QSize size_hint_;
  // =============================================================

  void updateGL() {
    update();
  }
  
  LuaStackSize openGLVersion(lua_State *L) {
    lua_pushstring(L, (char*)glGetString(GL_VERSION));
    lua_pushstring(L, (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    return 2;
  }

  //bool prepareShaderProgram(const char *xxx, const char *yyy);

protected:
  virtual void initializeGL() {
    lua_State *L = lua_;

    if (!pushLuaCallback("initializeGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'initializeGL' callback: %s\n", lua_tostring(L, -1));
    }
  }

  virtual void resizeGL(int width, int height) {
    lua_State *L = lua_;

    if (!pushLuaCallback("resizeGL")) return;
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    // <func> <self> <width> <height>
    int status = lua_pcall(L, 3, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'resizeGL' callback: %s\n", lua_tostring(L, -1));
    }
  }

  virtual void paintGL() {
    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  /*
    lua_State *L = lua_;

    if (!pushLuaCallback("paintGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'paintGL' callback: %s\n", lua_tostring(L, -1));
    }
  }
  */

  // --=============================================== Widget callbacks
  virtual void closeEvent(QCloseEvent *event) {
    Widget::closed(this, event);
  }

  virtual void mouseMoveEvent(QMouseEvent *event) {
    Widget::mouse(this, event);
  }

  virtual void mousePressEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MousePress))
      QWidget::mousePressEvent(event);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, DoubleClick))
      QWidget::mouseDoubleClickEvent(event);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MouseRelease))
      QWidget::mouseReleaseEvent(event);
  }

  virtual void resizeEvent(QResizeEvent *event) {
    Widget::resized(this, width(), height());
  }

  virtual void moveEvent(QMoveEvent * event) {
    Widget::moved(this, event);
  }

  virtual void keyPressEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, true))
      QWidget::keyPressEvent(event);
  }

  virtual void keyReleaseEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, false))
      QWidget::keyReleaseEvent(event);
  }

  //virtual void paintEvent(QPaintEvent *event);
private:
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_

