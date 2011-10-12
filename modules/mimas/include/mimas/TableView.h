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
#ifndef LUBYK_INCLUDE_MIMAS_TABLE_VIEW_H_
#define LUBYK_INCLUDE_MIMAS_TABLE_VIEW_H_

#include "mimas/mimas.h"
#include "mimas/constants.h"
#include "mimas/DataSource.h"

using namespace lubyk;

#include <QtGui/QTableView>
#include <QtGui/QHeaderView>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>

#include <iostream>


namespace mimas {

/** The TableView is used to display data in a table.
 *
 * @dub destructor: 'luaDestroy'
 *      ignore: 'luaInit'
 *      super: 'QWidget'
 */
class TableView : public QTableView, public ThreadedLuaObject {
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

public:
  TableView() {
    setAttribute(Qt::WA_DeleteOnClose);
    // Not editable
    setEditTriggers(QAbstractItemView::NoEditTriggers);
  }

  ~TableView() {
    MIMAS_DEBUG_GC
  }

  /** Show or hide horizontal and vertical headers.
   */
  void setVisibleHeaders(int orientation, bool visible) {
    if (orientation & Qt::Horizontal) {
      if (visible)
        horizontalHeader()->show();
      else
        horizontalHeader()->hide();
    }
    if (orientation & Qt::Vertical) {
      if (visible)
        verticalHeader()->hide();
      else
        verticalHeader()->hide();
    }
  }

  /** Show/hide grid (set to NoPen to hide).
   */
  void setGridStyle(int style) {
    QTableView::setGridStyle((Qt::PenStyle)style);
  }

  /** Enable/disable alternating row background.
   */
  void setAlternatingRowColors(bool should_enable) {
    QTableView::setAlternatingRowColors(should_enable);
  }

  virtual void selectRow(int row) {
    QTableView::selectRow(row - 1);
  }

  virtual void selectColumn(int row) {
    QTableView::selectColumn(row - 1);
  }

  // ============================ common code to all mimas Widgets

  QSize size_hint_;

  QString cssClass() const {
    return QString("table");
  }

  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  float hue() {
    return hue_;
  }

  /** Use an external model instead of the default one.
   */
  void setModel(DataSource *model) {
    QTableView::setModel(model);
  }

  int luaInit(lua_State *L, TableView *obj, const char *class_name) {
    ThreadedLuaObject::luaInit(L, obj, class_name);
    // <self>
    lua_pushlstring(L, "data_source", 5);
    // <self> <'data_source'>
    // <self>
    DataSource *data = new DataSource();
    data->luaInit(L, data, "mimas.DataSource");
    // <self> <'data_source'> <data>
    lua_settable(L, -3); // self.data_source = data
    // <self>
    // make DataSource look in <self> for callbacks
    lua_pushvalue(L, -1);
    // <self> <self>
    lua_pop(data->lua_, 1);
    lua_xmove(L, data->lua_, 1);
    // data: <self>
    // <self>
    setModel(data);
    return 1;
  }

protected:
  virtual void mouseMoveEvent(QMouseEvent *event);

  virtual void mousePressEvent(QMouseEvent *event) {
    if (!click(event, MousePress))
      QTableView::mousePressEvent(event);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    if (!click(event, DoubleClick))
      QTableView::mouseDoubleClickEvent(event);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    if (!click(event, MouseRelease))
      QTableView::mouseReleaseEvent(event);
  }

  virtual QSize sizeHint() const {
    return size_hint_;
  }

  /** The component's color.
   */
  float hue_;

private:
  bool click(QMouseEvent *event, int type);
  bool select(QMouseEvent *event, int type);
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_TABLE_VIEW_H_
