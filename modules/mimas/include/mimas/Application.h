/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_MIMAS_APPLICATION_H_
#define RUBYK_INCLUDE_MIMAS_APPLICATION_H_

#include "rubyk.h"
#include "mimas/Callback.h"

#include <QtGui/QApplication>

#include <iostream>

using namespace rubyk;

namespace mimas {

static char arg0[] = "mimas";
static char arg1[] = "-style";
static char arg2[] = "Plastique";
static char *app_argv[] = {&arg0[0], &arg1[0], &arg2[0], NULL};
static int   app_argc   = (int)(sizeof(app_argv) / sizeof(app_argv[0])) - 1;

/** Application (starts the GUI and manages the event loop).
 *
 * @dub lib_name:'Application_core'
 */
class Application : public QApplication
{
  Q_OBJECT

  rubyk::Worker *worker_;
public:
  Application(rubyk::Worker *worker)
   : QApplication(app_argc, app_argv),
     worker_(worker),
     lua_events_processor_(worker) {}

  ~Application() {}

  /** Start event loop.
   */
  int exec() {
    ScopedUnlock unlock(worker_);
    // is this dumb ?
    return QApplication::exec();
  }

  /** Create a callback to execute events in the GUI thread.
   * Arguments should be 1. self, 2. function
   */
  void post(lua_State *L) {
    Callback *clbk = new Callback(worker_);
    clbk->set_callback_from_app(L);
    postEvent(&lua_events_processor_, new Callback::Event(clbk));
  }

  void quit() {
    QApplication::quit();
  }

  void setStyleSheet(const char *text) {
    QApplication::setStyleSheet(QString(text));
  }

private:
  class LuaEventsProcessor : public QObject
  {
    rubyk::Worker *worker_;
  public:
    LuaEventsProcessor(rubyk::Worker *worker)
      : worker_(worker) {}

    virtual bool event(QEvent *e) {
      if (e->type() == Callback::EventType) {
        Callback::Event *ce = static_cast<Callback::Event*>(e);
        ce->callback_->callback();
        return true;
      }
      return QObject::event(e);
    }
  };

  LuaEventsProcessor lua_events_processor_;
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_APPLICATION_H_