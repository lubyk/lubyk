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
#ifndef LUBYK_INCLUDE_MIMAS_GLWIDGET_H_
#define LUBYK_INCLUDE_MIMAS_GLWIDGET_H_

#include "lubyk.h"
using namespace lubyk;

#include "mimas/mimas.h"
#include <QtGui/QWidget>
#include <QtOpenGL/QtOpenGL>

#include <iostream>

namespace mimas {

/** GLWidget.
 *
 * @dub destructor: 'luaDestroy'
 *      super: 'QWidget'
 */
class GLWidget : public QGLWidget, public ThreadedLuaObject
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

public:
  GLWidget() {
    setAttribute(Qt::WA_DeleteOnClose);
    // get focus on tab and click
    setFocusPolicy(Qt::StrongFocus);
  }

  ~GLWidget() {
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return parent() ? QString("glwindow") : QString("glwidget");
  }

  QSize size_hint_;
  // =============================================================

  void updateGL() {
    QGLWidget::updateGL();
  }

protected:
  virtual void initializeGL() {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("initializeGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'initializeGL' callback: %s\n", lua_tostring(L, -1));
    }
  }

  virtual void resizeGL(int width, int height) {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

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
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("paintGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'paintGL' callback: %s\n", lua_tostring(L, -1));
    }
  }

  //virtual void mousePressEvent(QMouseEvent *event);
  //virtual void mouseMoveEvent(QMouseEvent *event);
  //virtual void mouseDoubleClickEvent(QMouseEvent *event);
  //virtual void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event) {
    keyboard(event, true);
  }

  void keyReleaseEvent(QKeyEvent *event) {
    keyboard(event, false);
  }

private:
  void keyboard(QKeyEvent *event, bool isPressed) {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("keyboard")) return;
    lua_pushnumber(L, event->key());
    lua_pushboolean(L, isPressed);
    lua_pushstring(L, event->text().toUtf8());
    // <func> <self> <key> <on/off> <utf8>
    int status = lua_pcall(L, 4, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'keyboard' callback: %s\n", lua_tostring(L, -1));
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_GLWIDGET_H_
