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
 * @dub destructor: 'luaDestroy'
 *      ignore: 'index'
 */
class DataSource : public QAbstractItemModel, public LuaObject
{
  Q_OBJECT

public:
  DataSource() {}

  ~DataSource() {
    MIMAS_DEBUG_GC
  }

  void reset() {
    // reload all data
    // ??? Does not work
    QAbstractItemModel::reset();
  }

  void emitLayoutChanged() {
    emit(layoutChanged());
  }

  //  index(), parent(), rowCount(), columnCount(), and data()
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
    // ignore parent (hierarchical views) for now
    return createIndex(row, column);
  }
protected:
  virtual QModelIndex parent(const QModelIndex &child) const {
    return QModelIndex(); // no parent
  }

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("rowCount")) {
      return 0;
    }
    // <func> <self>
    int status = lua_pcall(L, 1, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'rowCount' callback: %s\n", lua_tostring(L, -1));
      return 0;
    }

    int count = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return count < 0 ? 0 : count;
  }

  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("columnCount")) return 0;
    // <func> <self>
    int status = lua_pcall(L, 1, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'columnCount' callback: %s\n", lua_tostring(L, -1));
      return 0;
    }

    int count = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return count < 0 ? 0 : count;
  }

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (role != Qt::DisplayRole) return QVariant();
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("data")) return QVariant();
    lua_pushnumber(L, index.row() + 1);
    lua_pushnumber(L, index.column() + 1);
    // <func> <self> <row> <column>
    int status = lua_pcall(L, 3, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'data' callback: %s\n", lua_tostring(L, -1));
      return QVariant();
    }
    QVariant res = variantFromLua(L, -1);
    lua_pop(L, 1);
    return res;
  }

  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
    if (role != Qt::DisplayRole) return QVariant();
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    if (!pushLuaCallback("header")) return QVariant();
    lua_pushnumber(L, section + 1);
    lua_pushnumber(L, orientation);
    // <func> <self> <section> <orientation>
    int status = lua_pcall(L, 3, 1, 0);

    if (status) {
      fprintf(stderr, "Error in 'header' callback: %s\n", lua_tostring(L, -1));
      return QVariant();
    }
    QVariant res = variantFromLua(L, -1);
    lua_pop(L, 1);
    return res;
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_DATA_SOURCE_H
