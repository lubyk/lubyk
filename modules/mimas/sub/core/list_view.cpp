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

#include "mimas/ListView.h"
#include "mimas/Painter.h"

namespace mimas {

void ListView::mouseMoveEvent(QMouseEvent *event) {
  lua_State *L = mouse_clbk_.lua_;
  if (!L) return;

  ScopedLock lock(worker_);

  mouse_clbk_.push_lua_callback(false);

  lua_pushnumber(L, event->x());
  lua_pushnumber(L, event->y());

  // <func> <x> <y>
  int status = lua_pcall(L, 2, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'mouse' callback: %s\n", lua_tostring(L, -1));
  }

  if (!lua_isnil(L, -1)) {
    // Pass to QListView
    QListView::mouseMoveEvent(event);
  }
}

bool ListView::click(QMouseEvent *event, int type) {
  if (select_clbk_.lua_) {
    return select(event, type);
  }
  lua_State *L = click_clbk_.lua_;

  if (!L) return false;

  ScopedLock lock(worker_);

  click_clbk_.push_lua_callback(false);

  lua_pushnumber(L, event->x());
  lua_pushnumber(L, event->y());
  lua_pushnumber(L, type);
  lua_pushnumber(L, event->button());
  lua_pushnumber(L, event->modifiers());

  // <func> <x> <y> <type> <btn> <modifiers>
  int status = lua_pcall(L, 5, 1, 0);

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

bool ListView::select(QMouseEvent *event, int type) {
  if (type != MousePress) {
    return true; // ignore
  }
  lua_State *L = select_clbk_.lua_;

  if (!L) return false;

  ScopedLock lock(worker_);

  QModelIndex idx = QListView::indexAt(QPoint(event->x(), event->y()));
  if (!idx.isValid()) {
    return false;
  }

  select_clbk_.push_lua_callback(false);
  // <fun>
  lua_pushnumber(L, idx.row() + 1);
  lua_pushnumber(L, idx.column() + 1);
  // <fun> <row> <col>
  int status = lua_pcall(L, 2, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'select' callback: %s\n", lua_tostring(L, -1));
  }

  // FIXME: find another way to remove the dotted lines around text after click.
  clearFocus();
  if (!lua_isnil(L, -1)) {
    // Pass to ListView
    return false;
  }

  return true;
}

} // mimas
