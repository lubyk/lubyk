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
#ifndef LUBYK_INCLUDE_MIMAS_TAB_WIDGET_H_
#define LUBYK_INCLUDE_MIMAS_TAB_WIDGET_H_

#include "mimas/mimas.h"
#include "mimas/Widget.h"
#include <QtGui/QTabWidget>

#include <iostream>

namespace mimas {

/** A TabWidget displays widgets with tabs.
 *
 * @see QWidget
 * @dub destructor: 'luaDestroy'
 *      super: 'QTabWidget'
 */
class TabWidget : public QTabWidget, public ThreadedLuaObject {
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
public:
  TabWidget(QWidget *parent = NULL)
      : QTabWidget(parent) {
    MIMAS_DEBUG_CC
  }

  ~TabWidget() {
    MIMAS_DEBUG_GC
  }

  /** Add a tab to the view at the given position.
   * We can use negative indices here:
   *  0 = add after last
   * -1 = add before last
   * -2 = add before element before last
   */
  int insertTab(int pos, QWidget *page, const char *name) {
    pos = pos - 1;
    if (pos < 0) {
      pos = count() + 1 + pos;
    }
    return QTabWidget::insertTab(pos, page, QString(name)) + 1;
  }

  /** Add a tab to the view.
   */
  int addTab(QWidget *page, const char *name) {
    return QTabWidget::addTab(page, QString(name)) + 1;
  }

  /** Returns the current widget index.
   * @return nil if there is no widget.
   */
  LuaStackSize currentIndex(lua_State *L) {
    int idx = QTabWidget::currentIndex();
    if (idx == -1) {
      return 0;
    } else {
      lua_pushnumber(L, idx + 1);
      return 1;
    }
  }

  /** Select a tab by position.
   */
  void setCurrentIndex(int idx) {
    int sz = count();
    if (!sz) return;
    if (idx <= 0) {
      idx = sz + idx;
    } else {
      idx = idx - 1;
    }
    QTabWidget::setCurrentIndex(idx % sz);
  }

  QString cssClass() const {
    return QString("tab_widget");
  }

  QSize size_hint_;

protected:
  virtual void keyPressEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, true))
      QWidget::keyPressEvent(event);
  }

  virtual void keyReleaseEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, false))
      QWidget::keyReleaseEvent(event);
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_TAB_WIDGET_H_


