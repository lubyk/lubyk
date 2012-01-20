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
#ifndef LUBYK_INCLUDE_MIMAS_H_BOX_LAYOUT_H_
#define LUBYK_INCLUDE_MIMAS_H_BOX_LAYOUT_H_

#include "mimas/mimas.h"

using namespace lubyk;

#include <QtGui/QHBoxLayout>

#include <iostream>

namespace mimas {

/** HBoxLayout (arrange widgets horizontally).
 *
 * @dub lib_name:'HBoxLayout_core'
 *      destructor: 'luaDestroy'
 */
class HBoxLayout : public QHBoxLayout, public DeletableOutOfLua
{
  Q_OBJECT

public:
  HBoxLayout(QWidget *parent = NULL)
      : QHBoxLayout(parent) {
    MIMAS_DEBUG_CC
  }

  ~HBoxLayout() {
    MIMAS_DEBUG_GC
  }

  void addWidget(QWidget *widget, int stretch = 0, int alignment = 0) {
    QHBoxLayout::addWidget(widget, stretch, (Qt::Alignment)alignment);
  }

  void insertWidget(int pos, QWidget *widget, int stretch = 0, int alignment = 0) {
    if (pos < 0) {
      // -1 = add after last, -2 = add after element before last
      pos = count() + 1 + pos;
    }
    QBoxLayout::insertWidget(pos, widget, stretch, (Qt::Alignment)alignment);
  }

  void addLayout(QLayout *layout, int stretch = 0) {
    QBoxLayout::addLayout(layout);
  }

  void insertLayout(int pos, QLayout *layout, int stretch = 0) {
    if (pos < 0) {
      // -1 = add after last, -2 = add after element before last
      pos = count() + 1 + pos;
    }
    QBoxLayout::insertLayout(pos, layout, stretch);
  }

  /** Set the alignment of the given widget.
   * Returns true if the widget was found in this layout (not including
   * children).
   */
  bool setAlignment(QWidget *w, int alignment) {
    return QLayout::setAlignment(w, (Qt::Alignment)alignment);
  }

  /** Add a strechable item.
   */
  void addStretch(int stretch = 1) {
    QHBoxLayout::addStretch(stretch);
  }

  /** Add a single fixed space.
   */
  void addSpacing(int size) {
    QHBoxLayout::addSpacing(size);
  }

  /** Fixed spacing between items.
   */
  void setSpacing(int space) {
    QHBoxLayout::setSpacing(space);
  }

  void setContentsMargins(int left, int top, int right, int bottom) {
    QHBoxLayout::setContentsMargins(left, top, right, bottom);
  }

  LuaStackSize minimumSize(lua_State *L) const {
    QSize s(QHBoxLayout::minimumSize());
    lua_pushnumber(L, s.width());
    lua_pushnumber(L, s.height());
    return 2;
  }

  QLayout *layout() {
    return this;
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_H_BOX_LAYOUT_H_
