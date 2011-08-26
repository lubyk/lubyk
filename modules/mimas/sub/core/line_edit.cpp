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
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("keyboard");
  lua_pushnumber(L, event->key());
  lua_pushboolean(L, isPressed);
  lua_pushstring(L, event->text().toUtf8());
  // <func> <self> <key> <on/off> <utf8>
  int status = lua_pcall(L, 4, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'keyboard' callback: %s\n", lua_tostring(L, -1));
  }

  if (!lua_isnil(L, -1)) {
    // Pass to LineEdit
    return false;
  } else {
    return true;
  }
}


void LineEdit::moveEvent(QMoveEvent * event) {
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("moved");
  lua_pushnumber(L, event->pos().x());
  lua_pushnumber(L, event->pos().y());
  // <func> <self> <x> <y>
  int status = lua_pcall(L, 3, 0, 0);

  if (status) {
    fprintf(stderr, "Error in 'moved' callback: %s\n", lua_tostring(L, -1));
  }
}

void LineEdit::resizeEvent(QResizeEvent *event) {
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("resized");
  lua_pushnumber(L, width());
  lua_pushnumber(L, height());
  // <func> <self> <width> <height>
  int status = lua_pcall(L, 2, 0, 0);

  if (status) {
    fprintf(stderr, "Error in 'resized' callback: %s\n", lua_tostring(L, -1));
  }
}

bool LineEdit::click(QMouseEvent *event, int type) {
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("initializeGL");
  lua_pushnumber(L, event->x());
  lua_pushnumber(L, event->y());
  lua_pushnumber(L, type);
  lua_pushnumber(L, event->button());
  lua_pushnumber(L, event->modifiers());
  // <func> <self> <x> <y> <type> <btn> <modifiers>
  int status = lua_pcall(L, 6, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'click' callback: %s\n", lua_tostring(L, -1));
  }
  // FIXME: find another way to remove the dotted lines around text after click.
  clearFocus();
  if (!lua_isnil(L, -1)) {
    // Pass to ListView
    return false;
  }

  return true;
}

void LineEdit::editingFinished() {
  // get data from Lua
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("initializeGL");
  lua_pushstring(L, text());
  // <func> <self> <text>
  int status = lua_pcall(L, 2, 0, 0);

  if (status) {
    // cannot raise an error here...
    fprintf(stderr, "Error in 'editingFinished' callback: %s\n", lua_tostring(L, -1));
  }
}

// connected to the textEdited signal
void LineEdit::textEdited(const QString &text) {
  // get data from Lua
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  pushLuaCallback("initializeGL");
  lua_pushstring(L, text.toUtf8().data());
  // <func> <self> <text>
  int status = lua_pcall(L, 2, 0, 0);

  if (status) {
    // cannot raise an error here...
    fprintf(stderr, "Error in 'textEdited' callback: %s\n", lua_tostring(L, -1));
  }
}
} // mimas
