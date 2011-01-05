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
#ifndef RUBYK_INCLUDE_MIMAS_SLIDER_H_
#define RUBYK_INCLUDE_MIMAS_SLIDER_H_

#include "mimas/mimas.h"

#include "mimas/RangeWidget.h"

#include <QtGui/QFrame>

#include <iostream>

using namespace rubyk;

namespace mimas {

/** Slider (async slider).
 *
 * @dub lib_name:'Slider_core'
 */
class Slider : public QFrame, public LuaCallback
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)
  Q_PROPERTY(int   border READ borderWidth WRITE setBorderWidth)
public:

  enum Defaults {
    DefaultWidth  = 30,
    DefaultHeight = 150
  };

  enum SliderType {
    HorizontalSliderType = 1,
    VerticalSliderType = 2,
  };


  Slider(rubyk::Worker *worker, int type = (int)VerticalSliderType, QWidget *parent = 0)
   : LuaCallback(worker),
     slider_type_((SliderType)type),
     hue_(-1),
     border_width_(2) {}

  ~Slider() {}

  // ============================ common code to all mimas Widgets
  QString cssClass() const {
    return QString("slider");
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

  // =============================================================

  virtual QSize sizeHint() const {
    if (slider_type_ == VerticalSliderType) {
      return QSize(DefaultWidth, DefaultHeight);
    } else {
      return QSize(DefaultHeight, DefaultWidth);
    }
  }

  void setBorderWidth(int width) {
    border_width_ = width;
  }

  int borderWidth() const {
    return border_width_;
  }

  /** Stack is
   * <self> <func>
   */
  void set_callback(lua_State *L) {
    set_lua_callback(L);

    QObject::connect(
      this,  SIGNAL(valueChanged(double)),
      this,  SLOT(callback(double)));
  }
public slots:
  /** Update slider when remote changes.
   * This is called by zmq when it receives a value change notification.
   */
  void setValue(double remote_value) {
    if (range_.setValue(remote_value)) {
      update();
    }
  }

  /** Lua method to trigger on
   * value changed.
   */
  void callback(double value) {
    if (!callback_set()) {
      printf("Slider callback called without a function set.");
      return;
    }

    rubyk::ScopedLock lock(worker_);

    // push self
    push_lua_callback();

    // 1 number param
    lua_pushnumber(lua_, value);

    // lua_ = LuaCallback's thread state
    int status = lua_pcall(lua_, 2, 0, 0);

    if (status) {
      fprintf(stderr, "Error in receive callback: %s\n", lua_tostring(lua_, -1));
    }
  }

signals:

  /** Emit when the user drags and changes the value.
   */
  void valueChanged(double);

protected:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void paintEvent(QPaintEvent *event);

private:
  SliderType slider_type_;
  float hue_;
  int   border_width_;
  RangeWidget range_;
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_SLIDER_H_