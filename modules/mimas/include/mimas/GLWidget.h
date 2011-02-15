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
#ifndef LUBYK_INCLUDE_MIMAS_WIDGET_H_
#define LUBYK_INCLUDE_MIMAS_WIDGET_H_

#include "lubyk.h"
using namespace lubyk;

#include "mimas/mimas.h"
#include <QtGui/QWidget>
#include <QtOpenGL/QtOpenGL>

#include <iostream>

namespace mimas {

/** GLWidget.
 *
 * @dub destructor: 'dub_destroy'
 *      ignore: 'initializeGL,paintGL,resizeGL,keyboard'
 */
class GLWidget : public QGLWidget, public DeletableOutOfLua
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

  LuaCallback initializeGL_;
  LuaCallback resizeGL_;
  LuaCallback paintGL_;
  LuaCallback keyboard_;

  /** Set to true while we are already holding the worker
   * lock (avoids deadlock).
   */
  lubyk::Worker *worker_;
public:
  GLWidget(lubyk::Worker *worker)
   : initializeGL_(worker),
     resizeGL_(worker),
     paintGL_(worker),
     keyboard_(worker),
     worker_(worker) {
    setAttribute(Qt::WA_DeleteOnClose);
    // get focus on tab and click
    setFocusPolicy(Qt::StrongFocus);
  }

  ~GLWidget() {
    MIMAS_DEBUG_GC
  }

  // ============================ common code to all mimas Widgets

  QString cssClass() const {
    return parent() ? QString("glwindow") : QString("glwidget");
  }

  QWidget *widget() {
    return this;
  }

  QObject *object() {
    return this;
  }

  /** Get the widget's name.
   */
  LuaStackSize name(lua_State *L) {
    lua_pushstring(L, QObject::objectName().toUtf8().data());
    return 1;
  }

  /** Set the widget's name.
   */
  void setName(const char *name) {
    QObject::setObjectName(QString(name));
  }

  void move(int x, int y) {
    QWidget::move(x, y);
  }

  // We add lua_State to make sure this is only called by Lua
  void resize(int w, int h, lua_State *L) {
    ScopedUnlock unlock(worker_);
    QWidget::resize(w, h);
  }

  // =============================================================

  /** Close and delete the window.
   */
  bool close() {
    return QWidget::close();
  }

  bool isVisible() const {
    return QWidget::isVisible();
  }

  // We add lua_State to make sure this is only called by Lua
  void show(lua_State *L) {
    ScopedUnlock unlock(worker_);
    QWidget::show();
  }

  void activateWindow(lua_State *L) {
    ScopedUnlock unlock(worker_);
    QWidget::activateWindow();
  }

  // We add lua_State to make sure this is only called by Lua
  void updateGL(lua_State *L) {
    ScopedUnlock unlock(worker_);
    QGLWidget::updateGL();
  }

  /** Set a callback function.
   *
   */
  void __newindex(lua_State *L) {
    // Stack should be ... <self> <key> <value>
    std::string key(luaL_checkstring(L, -2));

    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -3);
    // ... <self> <key> <value> <self>
    lua_pushvalue(L, -2);
    // ... <self> <key> <value> <self> <value>
    if (key == "initializeGL") {
      initializeGL_.set_lua_callback(L);
    } else if (key == "resizeGL") {
      resizeGL_.set_lua_callback(L);
    } else if (key == "paintGL") {
      paintGL_.set_lua_callback(L);
    } else if (key == "keyboard") {
      keyboard_.set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are initializeGL, resizeGL, paintGL, keyboard).", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }


  virtual void initializeGL() {
    lua_State *L = initializeGL_.lua_;
    if (!L) return;
    // cannot lock because this is called by Lua through 'show'...
    ScopedLock lock(worker_);

    initializeGL_.push_lua_callback(false);

    int status = lua_pcall(L, 0, 0, 0);

    if (status) {
      fprintf(stderr, "Error in initializeGL callback: %s\n", lua_tostring(L, -1));
    }
  }

  virtual void resizeGL(int width, int height) {
    lua_State *L = resizeGL_.lua_;
    if (!L) return;
    ScopedLock lock(worker_);

    resizeGL_.push_lua_callback(false);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);

    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      fprintf(stderr, "Error in resizeGL callback: %s\n", lua_tostring(L, -1));
    }
  }

  virtual void paintGL() {
    lua_State *L = paintGL_.lua_;
    if (!L) return;
    ScopedLock lock(worker_);

    paintGL_.push_lua_callback(false);

    int status = lua_pcall(L, 0, 0, 0);

    if (status) {
      fprintf(stderr, "Error in paintGL callback: %s\n", lua_tostring(L, -1));
    }
  }
protected:
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
    lua_State *L = keyboard_.lua_;
    if (!L) return;
    ScopedLock lock(worker_);

    keyboard_.push_lua_callback(false);
    lua_pushnumber(L, event->key());
    lua_pushboolean(L, isPressed);
    lua_pushstring(L, event->text().toUtf8());
    int status = lua_pcall(L, 3, 0, 0);

    if (status) {
      fprintf(stderr, "Error in keyboard callback: %s\n", lua_tostring(L, -1));
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_WIDGET_H_
