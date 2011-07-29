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
#ifndef LUBYK_INCLUDE_MIMAS_LINE_EDIT_H_
#define LUBYK_INCLUDE_MIMAS_LINE_EDIT_H_

#include "mimas/mimas.h"

using namespace lubyk;

#include <QtGui/QLineEdit>

namespace mimas {

/** Label widget.
 * @dub lib_name:'LineEdit_core'
 *      destructor: 'dub_destroy'
 */
class LineEdit : public QLineEdit, public DeletableOutOfLua
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

  Worker *worker_;
  LuaCallback editing_finished_clbk_;
  LuaCallback text_edited_clbk_;
  /** The component's color.
   */
  float hue_;
public:
  LineEdit(lubyk::Worker *worker, const char *content = NULL, QWidget *parent = NULL)
   : QLineEdit(content),
     worker_(worker),
     editing_finished_clbk_(worker),
     text_edited_clbk_(worker),
     hue_(-1) {
    QObject::connect(this, SIGNAL(editingFinished()),
                     this, SLOT(editingFinished())
    );

    QObject::connect(this, SIGNAL(textEdited(QString)),
                     this, SLOT(textEdited(QString))
    );
  }

  // ============================ common code to all mimas Widgets

  QString cssClass() const {
    return QString("line_edit");
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
    QWidget::move(x, y);
  }

  void resize(int w, int h) {
    QWidget::resize(w, h);
  }

  void setStyle(const char *text) {
    setStyleSheet(QString(".%1 { %2 }").arg(cssClass()).arg(text));
  }

  void setHue(float hue) {
    hue_ = hue;
    QWidget::update();
  }

  float hue() {
    return hue_;
  }

  // =============================================================

  void setText(const char *text) {
    QLineEdit::setText(QString(text));
    QWidget::update();
  }

  const char *text() const {
    return QLineEdit::text().toUtf8().data();
  }

  void setSelection(int start, int length) {
    QLineEdit::setSelection(start, length);
  }

  void selectAll() {
    QLineEdit::selectAll();
  }

  void setFocus() { // Qt::FocusReason reason
    QWidget::setFocus(Qt::OtherFocusReason);
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
    if (key == "editingFinished") {
      editing_finished_clbk_.set_lua_callback(L);
    } else if (key == "textEdited") {
      text_edited_clbk_.set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are 'editingFinished', 'textEdited').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }

private:
  virtual void paintEvent(QPaintEvent *event);

private slots:

  // set function to call on text change
  // textChanged(const QString & text)

  // connected to the pathChanged signal
	void editingFinished() {
    // get data from Lua
    lua_State *L = editing_finished_clbk_.lua_;
    if (!L) return;

    ScopedLock lock(worker_);

    editing_finished_clbk_.push_lua_callback(false);

    lua_pushstring(L, text());
    // <func> <text>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      // cannot raise an error here...
      fprintf(stderr, "Error in 'editingFinished' callback: %s\n", lua_tostring(L, -1));
    }
  }

  // connected to the textEdited signal
	void textEdited(const QString &text) {
    // get data from Lua
    lua_State *L = text_edited_clbk_.lua_;
    if (!L) return;

    ScopedLock lock(worker_);

    text_edited_clbk_.push_lua_callback(false);

    lua_pushstring(L, text.toUtf8().data());
    // <func> <text>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      // cannot raise an error here...
      fprintf(stderr, "Error in 'textEdited' callback: %s\n", lua_tostring(L, -1));
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_LINE_EDIT_H_