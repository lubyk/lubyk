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

#include "mimas/mimas.h"
using namespace lubyk;

#include <QtGui/QWidget>

#include <iostream>


namespace mimas {

class Painter;

/** The Widget is used to display custom elements or windows.
 *
 * @dub destructor: 'dub_destroy'
 */
class Widget : public QWidget, public DeletableOutOfLua
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

  Worker *worker_;
  LuaCallback paint_clbk_;
  LuaCallback resized_clbk_;
public:
  Widget(lubyk::Worker *worker) :
   worker_(worker),
   paint_clbk_(worker),
   resized_clbk_(worker) {
    setAttribute(Qt::WA_DeleteOnClose);
  }

  ~Widget() {
    MIMAS_DEBUG_GC
  }

  // ============================ common code to all mimas Widgets

  QString cssClass() const {
    return parent() ? QString("window") : QString("widget");
  }

  QWidget *widget() {
    return this;
  }

  void addWidget(QWidget *widget, float x=0, float y=0) {
    widget->setParent(this);
    widget->move(x, y);
    widget->show();
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
    ScopedUnlock unlock(worker_);
    QWidget::resize(w, h);
  }

  void setStyle(const char *text) {
    QWidget::setStyleSheet(QString(".%1 { %2 }").arg(cssClass()).arg(text));
  }

  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  float hue() {
    return hue_;
  }

  void update() {
    ScopedUnlock unlock(worker_);
    QWidget::update();
  }

  /** Get size of text with current widget font.
   */
  LuaStackSize textSize(const char *text, lua_State *L) {
    lua_pushnumber(L, fontMetrics().width(text));
    lua_pushnumber(L, fontMetrics().height());
    return 2;
  }

  void setMinimumSize(float w, float h) {
    ScopedUnlock unlock(worker_);
    QWidget::setMinimumSize(w, h);
  }

  // =============================================================

  /** Close and delete the window.
   */
  bool close() {
    ScopedUnlock unlock(worker_);
    return QWidget::close();
  }

  bool isVisible() const {
    return QWidget::isVisible();
  }

  void show() {
    ScopedUnlock unlock(worker_);
    QWidget::show();
  }

  void activateWindow() {
    QWidget::activateWindow();
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
    if (key == "paint") {
      paint_clbk_.set_lua_callback(L);
    } else if (key == "resized") {
      resized_clbk_.set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are 'paint' and 'resized').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }
protected:
  //virtual void mousePressEvent(QMouseEvent *event);
  //virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void paintEvent(QPaintEvent *event);
  virtual void resizeEvent(QResizeEvent *event);

  /** The component's color.
   */
  float hue_;

private:
  void paint(Painter &p);
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_WIDGET_H_
