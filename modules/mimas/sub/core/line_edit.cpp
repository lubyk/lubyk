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

#include <QtGui/QKeyEvent>
#include "mimas/LineEdit.h"

namespace mimas {

bool LineEdit::keyboard(QKeyEvent *event, bool isPressed) {
  lua_State *L = keyboard_clbk_.lua_;
  if (!L) return false;
  ScopedLock lock(worker_);

  keyboard_clbk_.push_lua_callback(false);
  lua_pushnumber(L, event->key());
  lua_pushboolean(L, isPressed);
  lua_pushstring(L, event->text().toUtf8());
  int status = lua_pcall(L, 3, 1, 0);

  if (status) {
    fprintf(stderr, "Error in keyboard callback: %s\n", lua_tostring(L, -1));
  }

  if (!lua_isnil(L, -1)) {
    // Pass to LineEdit
    return false;
  } else {
    return true;
  }
}


void LineEdit::moveEvent(QMoveEvent * event) {
  lua_State *L = moved_clbk_.lua_;
  if (!L) return;

  ScopedLock lock(worker_);

  moved_clbk_.push_lua_callback(false);

  lua_pushnumber(L, event->pos().x());
  lua_pushnumber(L, event->pos().y());

  // <func> <Painter> <width> <height>
  int status = lua_pcall(L, 2, 0, 0);

  if (status) {
    fprintf(stderr, "Error in 'moved' callback: %s\n", lua_tostring(L, -1));
  }
}

void LineEdit::resizeEvent(QResizeEvent *event) {
  lua_State *L = resized_clbk_.lua_;
  if (!L) return;

  ScopedLock lock(worker_);

  resized_clbk_.push_lua_callback(false);

  lua_pushnumber(L, width());
  lua_pushnumber(L, height());

  // <func> <Painter> <width> <height>
  int status = lua_pcall(L, 2, 0, 0);

  if (status) {
    fprintf(stderr, "Error in 'resized' callback: %s\n", lua_tostring(L, -1));
  }
}
} // mimas