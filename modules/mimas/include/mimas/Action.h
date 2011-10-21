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
#ifndef LUBYK_INCLUDE_MIMAS_ACTION_H_
#define LUBYK_INCLUDE_MIMAS_ACTION_H_

#include "mimas/mimas.h"
#include <QtGui/QAction>

#include <iostream>

namespace mimas {

/** An Action can be connected to a menu or and will trigger it's
 * callback on call (or shortcut).
 *
 * @see QAction
 * @dub destructor: 'luaDestroy'
 *      super: 'QAction'
 */
class Action : public QAction, public ThreadedLuaObject {
  Q_OBJECT
public:
  Action(const char *name, QObject *parent = 0)
      : QAction(QString(name), parent) {
    QObject::connect(this, SIGNAL(triggered()),
                     this, SLOT(triggeredSlot()));
    MIMAS_DEBUG_CC
  }

  /** A sequence can contain multiple shortcuts and the format is:
   * "Ctrl+O,Alt+O,Q"
   */
  void setShortcut(const char *sequence) {
    QAction::setShortcut(QKeySequence(QString(sequence)));
  }

  ~Action() {
    MIMAS_DEBUG_GC
  }

private slots:
  void triggeredSlot() {
    lua_State *L = lua_;

    if (!pushLuaCallback("trigger")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      // cannot raise an error here...
      fprintf(stderr, "Error in 'trigger' callback: %s\n", lua_tostring(L, -1));
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_ACTION_H_

