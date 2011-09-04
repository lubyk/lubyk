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
#ifndef LUBYK_INCLUDE_MIMAS_MENU_BAR_H_
#define LUBYK_INCLUDE_MIMAS_MENU_BAR_H_

#include "mimas/mimas.h"
#include "mimas/Color.h"
#include "mimas/Menu.h"

#include <QtGui/QMenuBar>

namespace mimas {

/** MenuBar used by Painter.
 *
 * @see Menu
 * @see QMenuBar
 * @dub destructor: 'luaDestroy'
 *      super: 'QMenuBar'
 */
class MenuBar : public QMenuBar, public LuaObject {
  Q_OBJECT
public:
  MenuBar()
      : QMenuBar(0) {}

  ~MenuBar() {
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return QString("menu_bar");
  }

  QSize size_hint_;
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_MENU_BAR_H_
