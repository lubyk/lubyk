/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_MIMAS_WIDGET_H_
#define RUBYK_INCLUDE_MIMAS_WIDGET_H_

#include "rubyk.h"
using namespace rubyk;

#include "mimas/mimas.h"
#include <QtGui/QWidget>
#include <QtOpenGL/QtOpenGL>

#include <iostream>

namespace mimas {

/** GLWindow.
 *
 * @dub destructor: 'dub_destroy'
 *      ignore: 'initializeGL,paintGL,resizeGL'
 */
class GLWindow : public QGLWidget, public DeletableOutOfLua
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

  LuaCallback initializeGL_;
  LuaCallback resizeGL_;
  LuaCallback paintGL_;
  /** Set to true if we are in the 'show' method
   * to avoid locking in initializeGL and resizeGL.
   */
  bool in_show_;
public:
  GLWindow(rubyk::Worker *worker)
   : initializeGL_(worker),
     resizeGL_(worker),
     paintGL_(worker),
     in_show_(false) {
    setAttribute(Qt::WA_DeleteOnClose);
  }

  ~GLWindow() {
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

  void resize(int w, int h) {
    in_show_ = true;
      QWidget::resize(w, h);
    in_show_ = false;
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

  void show() {
    // show directly calls initializeGL
    in_show_ = true;
      QWidget::show();
    in_show_ = false;
  }

  void activateWindow() {
    QWidget::activateWindow();
  }

  void updateGL() {
    in_show_ = true;
      QGLWidget::updateGL();
    in_show_ = false;
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
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are initializeGL, resizeGL and paintGL).", key.c_str());
    }
  }


  virtual void initializeGL() {
    lua_State *L = initializeGL_.lua_;
    if (!L) return;
    // cannot lock because this is called by Lua through 'show'...
    if (!in_show_) initializeGL_.worker_->lock();

    initializeGL_.push_lua_callback(false);

    int status = lua_pcall(L, 0, 0, 0);

    if (status) {
      printf("Error in initializeGL function: %s\n", lua_tostring(L, -1));
    }

    if (!in_show_) initializeGL_.worker_->unlock();
  }

  virtual void resizeGL(int width, int height) {
    lua_State *L = resizeGL_.lua_;
    if (!L) return;
    if (!in_show_) resizeGL_.worker_->lock();

    resizeGL_.push_lua_callback(false);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);

    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      printf("Error in resizeGL function: %s\n", lua_tostring(L, -1));
    }

    if (!in_show_) resizeGL_.worker_->unlock();
  }

  virtual void paintGL() {
    lua_State *L = paintGL_.lua_;
    if (!L) return;
    if (!in_show_) paintGL_.worker_->lock();

    paintGL_.push_lua_callback(false);

    int status = lua_pcall(L, 0, 0, 0);

    if (status) {
      printf("Error in paintGL function: %s\n", lua_tostring(L, -1));
    }
    if (!in_show_) paintGL_.worker_->unlock();
  }

protected:
  //virtual void mousePressEvent(QMouseEvent *event);
  //virtual void mouseMoveEvent(QMouseEvent *event);
  //virtual void mouseDoubleClickEvent(QMouseEvent *event);
  //virtual void paintEvent(QPaintEvent *event);
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_WIDGET_H_
