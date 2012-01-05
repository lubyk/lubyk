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
#ifndef LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_
#define LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_

#include "lubyk.h"
using namespace lubyk;

#include "mimas/mimas.h"
#include "mimas/constants.h"
#include "mimas/Widget.h"

#include <QtGui/QPushButton>
#include <QtGui/QMouseEvent>

#include <iostream>

namespace mimas {

/** Button widget.
 *
 * @dub lib_name:'Button_core'
 *      destructor: 'luaDestroy'
 *      super: 'QWidget'
 */
class Button : public QPushButton, public ThreadedLuaObject {
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)
  Q_PROPERTY(float hue READ hue WRITE setHue)

public:
  Button(const char *title = NULL, QWidget *parent = NULL)
      : QPushButton(title, parent) {
    QObject::connect(this, SIGNAL(clicked()),
                     this, SLOT(clickedSlot()));
    MIMAS_DEBUG_CC
  }

  ~Button() {
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return QString("button");
  }

  QSize size_hint_;

  void setHue(float hue) {
    hue_ = hue;
    update();
  }

  void setText(const char *txt) {
    QPushButton::setText(QString(txt));
  }

  float hue() {
    return hue_;
  }

private slots:
  void clickedSlot() {
    lua_State *L = lua_;

    if (!pushLuaCallback("click")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      // cannot raise an error here...
      fprintf(stderr, "Error in 'click' callback: %s\n", lua_tostring(L, -1));
    }
  }
private:
  float hue_;
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PUSH_BUTTON_H_
