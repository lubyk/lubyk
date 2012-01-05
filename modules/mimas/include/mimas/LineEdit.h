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
#include "mimas/constants.h"
#include "mimas/Widget.h"

using namespace lubyk;

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QMouseEvent>

namespace mimas {

/** Label widget.
 * @dub lib_name:'LineEdit_core'
 *      destructor: 'luaDestroy'
 */
class LineEdit : public QLineEdit, public ThreadedLuaObject
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

  /** The component's color.
   */
  float hue_;
public:
  LineEdit(const char *content = NULL, QWidget *parent = NULL)
     : QLineEdit(content, parent), hue_(-1) {
    QObject::connect(this, SIGNAL(editingFinished()),
                     this, SLOT(editingFinishedSlot()));

    QObject::connect(this, SIGNAL(textEdited(QString)),
                     this, SLOT(textEditedSlot(QString)));
    MIMAS_DEBUG_CC
  }

  ~LineEdit() {
    MIMAS_DEBUG_GC
  }

  // ============================ common code to all mimas Widgets

  QString cssClass() const {
    return QString("line_edit");
  }

  QWidget *widget() {
    return this;
  }

  void setParent(QWidget *parent) {
    QWidget::setParent(parent);
  }

  QObject *object() {
    return this;
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
    QWidget::move(mapToParent(mapFromGlobal(QPoint(x, y))));
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
    setStyleSheet(QString(".%1 { %2 }").arg(cssClass()).arg(text));
  }

  void setHue(float hue) {
    hue_ = hue;
    QWidget::update();
  }

  float hue() {
    return hue_;
  }

  void hide() {
    QWidget::hide();
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

protected:

  virtual void keyPressEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, true))
      QLineEdit::keyPressEvent(event);
  }

  virtual void keyReleaseEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, false))
      QLineEdit::keyReleaseEvent(event);
  }

  virtual void mousePressEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MousePress))
      QLineEdit::mousePressEvent(event);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, DoubleClick))
      QLineEdit::mouseDoubleClickEvent(event);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MouseRelease))
      QLineEdit::mouseReleaseEvent(event);
  }

  virtual void moveEvent(QMoveEvent * event);
  virtual void resizeEvent(QResizeEvent *event);

private:
  virtual void paintEvent(QPaintEvent *event);

private slots:

  // set function to call on text change
  // textChanged(const QString & text)

  // connected to the editingFinished signal
	void editingFinishedSlot();

  // connected to the textEdited signal
	void textEditedSlot(const QString &text);
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_LINE_EDIT_H_
