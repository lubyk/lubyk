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

#include "mimas/TableView.h"
#include "mimas/Painter.h"

namespace mimas {

void TableView::mouseMoveEvent(QMouseEvent *event) {
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  if (!pushLuaCallback("mouse")) return;
  lua_pushnumber(L, event->x());
  lua_pushnumber(L, event->y());
  // <func> <self> <x> <y>
  int status = lua_pcall(L, 3, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'mouse' callback: %s\n", lua_tostring(L, -1));
  }

  if (lua_isfalse(L, -1)) {
    // Pass to QTableView
    QTableView::mouseMoveEvent(event);
  }
  lua_pop(L, 1);
}

bool TableView::click(QMouseEvent *event, int type) {
  lua_State *L = lua_;
  ScopedLock lock(worker_);

  if (pushLuaCallback("select")) {
    // ... <select> <self>
    return select(event, type);
  } else {
    if (!pushLuaCallback("click")) return false;
    // ... <click> <self>
  }
  lua_pushnumber(L, event->x());
  lua_pushnumber(L, event->y());
  lua_pushnumber(L, type);
  lua_pushnumber(L, event->button());
  lua_pushnumber(L, event->modifiers());
  // ... <func> <self> <x> <y> <type> <btn> <modifiers>
  int status = lua_pcall(L, 6, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'click' callback: %s\n", lua_tostring(L, -1));
  }
  // FIXME: find another way to remove the dotted lines around text after click.
  clearFocus();
  if (lua_isfalse(L, -1)) {
    // Pass to TableView
    lua_pop(L, 1);
    return false;
  }

  lua_pop(L, 1);
  return true;
}

bool TableView::select(QMouseEvent *event, int type) {
  lua_State *L = lua_;
  // in ScopedLock
  // ... <select> <self>
  if (type != MousePress) {
    lua_pop(L, 2);
    return true; // ignore
  }

  QModelIndex idx = QTableView::indexAt(QPoint(event->x(), event->y()));
  if (!idx.isValid()) {
    lua_pop(L, 2);
    return false;
  }

  lua_pushnumber(L, idx.row() + 1);
  lua_pushnumber(L, idx.column() + 1);
  // ... <select> <self> <row> <col>
  int status = lua_pcall(L, 3, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'select' callback: %s\n", lua_tostring(L, -1));
  }

  // FIXME: find another way to remove the dotted lines around text after click.
  clearFocus();
  if (lua_isfalse(L, -1)) {
    // Pass to TableView
    lua_pop(L, 1);
    return false;
  }

  lua_pop(L, 1);
  return true;
}

} // mimas

