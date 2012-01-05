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
   */
  int insertTab(int pos, QWidget *page, const char *name) {
    if (pos < 0) {
      // -1 = add after last, -2 = add after element before last
      pos = count() + 1 + pos;
    }
    return QTabWidget::insertTab(pos, page, QString(name));
  }

  /** Add a tab to the view.
   */
  int addTab(QWidget *page, const char *name) {
    return QTabWidget::addTab(page, QString(name));
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


