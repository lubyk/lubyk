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

#include <QtGui/QListView>
#include <QtGui/QHeaderView>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>

#include <iostream>


namespace mimas {


/** The ListView is used to display data as a list.
 *
 * @dub destructor: 'dub_destroy'
 */
class ListView : public QListView, public DeletableOutOfLua
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

  Worker *worker_;
  //LuaCallback paint_clbk_;
  //LuaCallback resized_clbk_;
  LuaCallback mouse_clbk_;
  LuaCallback click_clbk_;
  //LuaCallback keyboard_clbk_;
  QSize size_hint_;

  DataSource  data_source_;
public:
  ListView(lubyk::Worker *worker) :
   worker_(worker),
   // paint_clbk_(worker),
   // resized_clbk_(worker),
   mouse_clbk_(worker),
   click_clbk_(worker),
   // keyboard_clbk_(worker),
   data_source_(worker) {
    setAttribute(Qt::WA_DeleteOnClose);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setModel(&data_source_);
    // Not editable
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // FIXME: find another way to remove the dotted lines around text after click.
    clearFocus();
  }

  ~ListView() {
    MIMAS_DEBUG_GC
  }

  // ============================ [ all Widgets

  QString cssClass() const {
    return QString("list");
  }

  QWidget *widget() {
    return this;
  }

  QObject *object() {
    return this;
  }

  /** Get the widget's name.
   */
  LuaStackSize name(lua_State *L) {
    lua_pushstring(L, QObject::objectName().toUtf8().data());
    return 1;
  }

  /** Set the widget's name.
   */
  void setName(const char *name) {
    QObject::setObjectName(QString(name));
  }

  void move(int x, int y) {
    ScopedUnlock unlock(worker_);
    QWidget::move(x, y);
  }

  void resize(int w, int h) {
    ScopedUnlock unlock(worker_);
    QWidget::resize(w, h);
  }

  int x() {
    return QWidget::x();
  }

  int y() {
    return QWidget::y();
  }

  int width() {
    return QWidget::width();
  }

  int height() {
    return QWidget::height();
  }

  void setStyle(const char *text) {
    QWidget::setStyleSheet(QString(".%1 { %2 }").arg(cssClass()).arg(text));
  }

  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  float hue() {
    return hue_;
  }

  void update() {
    ScopedUnlock unlock(worker_);
    QWidget::update();
  }

  /** Get size of text with current widget font.
   */
  LuaStackSize textSize(const char *text, lua_State *L) {
    lua_pushnumber(L, fontMetrics().width(text));
    lua_pushnumber(L, fontMetrics().height());
    return 2;
  }

  /** Set the prefered size. Use setSizePolicy to define how the
   * widget resizes compared to this value.
   */
  void setSizeHint(float w, float h) {
    size_hint_ = QSize(w, h);
    ScopedUnlock unlock(worker_);
    updateGeometry();
  }

  /** Control how the widget behaves in a layout related to it's sizeHint().
   */
  void setSizePolicy(int horizontal, int vertical) {
    QWidget::setSizePolicy((QSizePolicy::Policy)horizontal, (QSizePolicy::Policy)vertical);
    ScopedUnlock unlock(worker_);
    updateGeometry();
  }

  // FIXME: maybe we can remove this and only use setSizeHint + setSizePolicy...
  void setMinimumSize(float w, float h) {
    ScopedUnlock unlock(worker_);
    QWidget::setMinimumSize(w, h);
  }

  /** Receive mouse move events even if no button is pressed.
   */
  void setMouseTracking(bool enable) {
    QWidget::setMouseTracking(enable);
  }

  /** Close and delete the window.
   */
  bool close() {
    ScopedUnlock unlock(worker_);
    return QWidget::close();
  }

  bool isVisible() const {
    return QWidget::isVisible();
  }

  void show() {
    ScopedUnlock unlock(worker_);
    QWidget::show();
  }

  void hide() {
    ScopedUnlock unlock(worker_);
    QWidget::hide();
  }

  /** Returns (x,y) position of the widget in the global
   * screen coordinates.
   */
  LuaStackSize globalPosition(lua_State *L) {
    QPoint pt = mapToGlobal(QPoint(0, 0));
    lua_pushnumber(L, pt.x());
    lua_pushnumber(L, pt.y());
    return 2;
  }

  /** Move the widget to the given global coordinates.
   */
  void globalMove(float x, float y) {
    ScopedUnlock unlock(worker_);
    QWidget::move(mapToParent(mapFromGlobal(QPoint(x, y))));
  }

  /** Bring to bottom of parent widget.
   */
  void lower() {
    QWidget::lower();
  }

  /** Bring to top of parent widget.
   */
  void raise() {
    QWidget::raise();
  }
  // ============================================================= all Widgets ]

  // ============================================================= ListView

  /** Return the index of the model at the given x,y position.
   * @return x, y
   */
  LuaStackSize indexAt(float x, float y, lua_State *L) {
    ScopedUnlock unlock(worker_);
    // indexAt calls "data" method in data_source_ (don't ask why...)
    QModelIndex idx = QListView::indexAt(QPoint(x, y));
    if (idx.isValid()) {
      lua_pushnumber(L, idx.row() + 1);
      lua_pushnumber(L, idx.column() + 1);
      return 2;
    } else {
      return 0;
    }
  }

  //void dataChanged(int top_row = -1, int top_col = -1, int bottom_row = -1, int bottom_col = -1) {
  //  // -1 == ALL changed
  //  data_source_.dataChanged(top_row, top_col, bottom_row, bottom_col);
  //}
  // void setViewMode()

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
    /*
    if (key == "paint") {
      paint_clbk_.set_lua_callback(L);
    } else if (key == "resized") {
      resized_clbk_.set_lua_callback(L);
    } else if (key == "keyboard") {
      keyboard_clbk_.set_lua_callback(L);
    } else if (key == "data") {
    } else
    */
    if (key == "click") {
      click_clbk_.set_lua_callback(L);
    } else if (key == "mouse") {
      mouse_clbk_.set_lua_callback(L);
    } else if (key == "data" || key == "header" || key == "rowCount" || key == "columnCount" || key == "index") {
      lua_pop(L, 2);
      data_source_.__newindex(L);
      return;
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are 'data', 'header', 'rowCount', 'columnCount' and 'index').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }

protected:
  virtual void mouseMoveEvent(QMouseEvent *event);

  virtual void mousePressEvent(QMouseEvent *event) {
    if (!click(event, MousePress))
      QListView::mousePressEvent(event);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    if (!click(event, DoubleClick))
      QListView::mouseDoubleClickEvent(event);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    if (!click(event, MouseRelease))
      QListView::mouseReleaseEvent(event);
  }

  virtual QSize sizeHint() const {
    return size_hint_;
  }

  /** The component's color.
   */
  float hue_;

private:
  bool click(QMouseEvent *event, int type);
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_TABLE_VIEW_H_
