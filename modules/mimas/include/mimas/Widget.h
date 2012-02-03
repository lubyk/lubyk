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
#include "mimas/constants.h"

using namespace lubyk;

#include <QtGui/QWidget>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>

namespace mimas {

class Painter;

/** The Widget is used to display custom elements or windows.
 * The Widget uses the following callbacks:  paint, mouse,
 * click, keyboard, move and resized.
 *
 * @see QWidget
 * @dub destructor: 'luaDestroy'
 *      ignore: 'keyboard,click,paint,resized,moved,closed,paint'
 *      super: 'QWidget'
 */
class Widget : public QWidget, public ThreadedLuaObject {
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

public:
  Widget(int window_flags) :
    QWidget(NULL, (Qt::WindowFlags)window_flags) {
    setAttribute(Qt::WA_DeleteOnClose);
    MIMAS_DEBUG_CC
  }

  Widget(QWidget *parent = 0, int window_flags = 0) :
    QWidget(parent, (Qt::WindowFlags)window_flags) {
    setAttribute(Qt::WA_DeleteOnClose);
    MIMAS_DEBUG_CC
  }

  ~Widget() {
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return parent() ? QString("window") : QString("widget");
  }

  /** Is this used ???
   */
  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  float hue() {
    return hue_;
  }

  QSize size_hint_;
  // ============================================================ Dialog
  LuaStackSize getOpenFileName(const char *caption,
                          const char *base_dir,
                          const char *filter,
                          int options,
                          lua_State *L);

  LuaStackSize getExistingDirectory(const char *caption,
                          const char *base_dir,
                          int options,
                          lua_State *L);

  /** Common keyboard, mouse and click event handling to many widgets.
   */
  static bool keyboard(ThreadedLuaObject *obj, QKeyEvent *event, bool isPressed);
  static bool mouse(ThreadedLuaObject *obj, QMouseEvent *event);
  static bool click(ThreadedLuaObject *obj, QMouseEvent *event, int type);
  static void paint(ThreadedLuaObject *obj, Painter *p, int w, int h);
  static void resized(ThreadedLuaObject *obj, double width, double height);
  static void moved(ThreadedLuaObject *obj, QMoveEvent *event);
  static void closed(ThreadedLuaObject *obj, QCloseEvent *event);
protected:
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

  virtual void paintEvent(QPaintEvent *event);

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

  virtual QSize sizeHint() const {
    return size_hint_;
  }

  /** The component's color.
   */
  float hue_;
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_WIDGET_H_
