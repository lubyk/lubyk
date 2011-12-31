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
#ifndef LUBYK_INCLUDE_MIMAS_V_BOX_LAYOUT_H_
#define LUBYK_INCLUDE_MIMAS_V_BOX_LAYOUT_H_

#include "mimas/mimas.h"

using namespace lubyk;

#include <QtGui/QVBoxLayout>

#include <iostream>

namespace mimas {

/** VBoxLayout (arrange widgets vertically).
 *
 * @dub lib_name:'VBoxLayout_core'
 */
class VBoxLayout : public QVBoxLayout, public LuaObject
{
  Q_OBJECT

public:
  VBoxLayout(QWidget *parent = NULL)
   : QVBoxLayout(parent) {
    MIMAS_DEBUG_CC
  }

  ~VBoxLayout() {
    MIMAS_DEBUG_GC
  }

  void addWidget(QWidget *widget, int stretch = 0, int alignment = 0) {
    QBoxLayout::addWidget(widget, stretch, (Qt::Alignment)alignment);
  }
  
  void insertWidget(int pos, QWidget *widget, int stretch = 0, int alignment = 0) {
    if (pos < 0) {
      pos = count() + pos;
    }
    QBoxLayout::insertWidget(pos, widget, stretch, (Qt::Alignment)alignment);
  }

  void addLayout(QLayout *layout, int stretch = 0) {
    QBoxLayout::addLayout(layout);
  }

  void insertLayout(int pos, QLayout *layout, int stretch = 0) {
    if (pos < 0) {
      pos = count() + pos;
    }
    QBoxLayout::insertLayout(pos, layout, stretch);
  }

  /** Add a strechable item.
   */
  void addStretch(int stretch = 1) {
    QVBoxLayout::addStretch(stretch);
  }

  /** Add a single fixed space.
   */
  void addSpacing(int size) {
    QVBoxLayout::addSpacing(size);
  }

  /** Fixed spacing between items.
   */
  void setSpacing(int space) {
    QVBoxLayout::setSpacing(space);
  }

  void setContentsMargins(int left, int top, int right, int bottom) {
    QVBoxLayout::setContentsMargins(left, top, right, bottom);
  }

  QLayout *layout() {
    return this;
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_V_BOX_LAYOUT_H_
