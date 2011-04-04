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
#ifndef LUBYK_INCLUDE_MIMAS_DATA_SOURCE_H_
#define LUBYK_INCLUDE_MIMAS_DATA_SOURCE_H_

#include "mimas/mimas.h"
#include "mimas/constants.h"

using namespace lubyk;

#include <QtCore/QAbstractItemModel>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>

#include <iostream>


namespace mimas {

/** The DataSource is used to provide data to Views such as TableView, ListView or
 * TreeView.
 *
 * @dub destructor: 'dub_destroy'
 */
class DataSource : public QAbstractItemModel, public DeletableOutOfLua
{
  Q_OBJECT

  Worker *worker_;
  LuaCallback index_clbk_;
  LuaCallback parent_clbk_;
  LuaCallback row_count_clbk_;
  LuaCallback column_count_clbk_;
  LuaCallback data_clbk_;
  LuaCallback header_clbk_;
public:
  DataSource(lubyk::Worker *worker) :
   worker_(worker),
   index_clbk_(worker),
   parent_clbk_(worker),
   row_count_clbk_(worker),
   column_count_clbk_(worker),
   data_clbk_(worker),
   header_clbk_(worker) {
  }

  ~DataSource() {
    MIMAS_DEBUG_GC
  }

  /** Set a callback function.
   *
   */
  void __newindex(lua_State *L) {
    // Stack should be ... <self> <key> <value>
    std::string key(luaL_checkstring(L, -2));

    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -3);
    // ... <self> <key> <value> <self>
    lua_pushvalue(L, -2);
    // ... <self> <key> <value> <self> <value>
    if (key == "index") {
      // not used for the moment
      index_clbk_.set_lua_callback(L);
    } else if (key == "parent") {
      // not used for the moment
      parent_clbk_.set_lua_callback(L);
    } else if (key == "rowCount") {
      row_count_clbk_.set_lua_callback(L);
    } else if (key == "columnCount") {
      column_count_clbk_.set_lua_callback(L);
    } else if (key == "data") {
      data_clbk_.set_lua_callback(L);
    } else if (key == "header") {
      header_clbk_.set_lua_callback(L);
    } else {
      lua_pop(L, 2);
      luaL_error(L, "Invalid function name '%s' (valid names are 'index', 'parent', 'rowCount', 'columnCount', 'data' and 'header').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }
protected:
  //  index(), parent(), rowCount(), columnCount(), and data()
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
    // ignore parent (hierarchical views) for now
    return createIndex(row, column);
  }

  virtual QModelIndex parent (const QModelIndex &child) const {
    return QModelIndex(); // no parent
  }

  virtual int rowCount (const QModelIndex &parent = QModelIndex()) const {
    // get data from Lua
    lua_State *L = row_count_clbk_.lua_;
    if (!L) return 0;

    ScopedLock lock(worker_);

    row_count_clbk_.push_lua_callback(false);

    // <func>
    int status = lua_pcall(L, 0, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'rowCount' callback: %s\n", lua_tostring(L, -1));
      return 0;
    }

    if (!lua_isnumber(L, -1)) {
      // No data
      return 0;
    }
    int count = lua_tonumber(L, -1);
    return count < 0 ? 0 : count;
  }

  virtual int columnCount (const QModelIndex &parent = QModelIndex()) const {
    // get data from Lua
    lua_State *L = column_count_clbk_.lua_;
    if (!L) return 0;

    ScopedLock lock(worker_);

    column_count_clbk_.push_lua_callback(false);

    // <func>
    int status = lua_pcall(L, 0, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'columnCount' callback: %s\n", lua_tostring(L, -1));
      return 0;
    }

    if (!lua_isnumber(L, -1)) {
      // No data
      return 0;
    }

    int count = lua_tonumber(L, -1);
    return count < 0 ? 0 : count;
  }

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (role != Qt::DisplayRole) return QVariant();
    // get data from Lua
    lua_State *L = data_clbk_.lua_;
    if (!L) return QVariant();

    ScopedLock lock(worker_);

    data_clbk_.push_lua_callback(false);

    lua_pushnumber(L, index.row() + 1);
    lua_pushnumber(L, index.column() + 1);

    // <func> <row> <column>
    int status = lua_pcall(L, 2, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'data' callback: %s\n", lua_tostring(L, -1));
      return QVariant();
    }

    return variantFromLua(L, -1);
  }

  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
    if (role != Qt::DisplayRole) return QVariant();
    // get data from Lua
    lua_State *L = header_clbk_.lua_;
    if (!L) return QVariant();

    ScopedLock lock(worker_);

    // FIXME: all callbacks should push 'self' so that we can define generic callbacks
    // in a sub-class without redefinition in each instance with a closure on self ?
    // function self.super.mouse(x, y)
    //   self:mouse(x, y)
    // end
    header_clbk_.push_lua_callback(false);

    lua_pushnumber(L, section + 1);
    lua_pushnumber(L, orientation);

    // <func> <section> <orientation>
    int status = lua_pcall(L, 2, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'data' callback: %s\n", lua_tostring(L, -1));
      return QVariant();
    }

    return variantFromLua(L, -1);
  }

private:
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_DATA_SOURCE_H
