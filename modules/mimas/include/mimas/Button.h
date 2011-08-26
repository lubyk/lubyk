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
#ifndef LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_
#define LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_

#include "lubyk.h"
using namespace lubyk;

#include "mimas/mimas.h"
#include "mimas/constants.h"

#include <QtGui/QPushButton>
#include <QtGui/QMouseEvent>

#include <iostream>

namespace mimas {

/** Button widget.
 *
 * @dub lib_name:'Button_core'
 *      destructor: 'dub_destroy'
 */
class Button : public QPushButton, public DeletableOutOfLua, public LuaObject
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

public:
  Button(lubyk::Worker *worker, const char *title = NULL, QWidget *parent = NULL) :
    QPushButton(title, parent) {}

  ~Button() {
    MIMAS_DEBUG_GC
  }

  // ============================ common code to all mimas Widgets

  QString cssClass() const {
    return QString("button");
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

  int x() {
    return QWidget::x();
  }

  int y() {
    return QWidget::y();
  }

  int width() {
    return QWidget::width();
  }

  int height() {
    return QWidget::height();
  }

  void setStyle(const char *text) {
    QWidget::setStyleSheet(QString(".%1 { %2 }").arg(cssClass()).arg(text));
  }

  void setStyleSheet(const char *text) {
    QWidget::setStyleSheet(text);
  }

  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  float hue() {
    return hue_;
  }

protected:

  virtual void mousePressEvent(QMouseEvent *event) {
    click(event, MousePress);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    click(event, DoubleClick);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    click(event, MouseRelease);
  }

private:
  void click(QMouseEvent *event, int type);

  float hue_;
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_
