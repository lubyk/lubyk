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
#ifndef LUBYK_INCLUDE_MIMAS_APPLICATION_H_
#define LUBYK_INCLUDE_MIMAS_APPLICATION_H_

#include "mimas/mimas.h"
#include "mimas/Callback.h"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include <iostream>

using namespace lubyk;

namespace mimas {

/** Application (starts the GUI and manages the event loop). We do not
 * really need to use DeletableOutOfLua here but we never know.
 *
 * @dub lib_name:'Application_core'
 *      constructor: 'MakeApplication'
 *      destructor: 'dub_destroy'
 */
class Application : public QApplication, public DeletableOutOfLua
{
  Q_OBJECT

  lubyk::Worker *worker_;
public:
  /** Private constructor. Use MakeApplication instead.
   */
  Application(lubyk::Worker *worker);

  /** Custom constructor so that we can set the env table that is
   * needed by mimas.Callback.
   */
  static LuaStackSize MakeApplication(lubyk::Worker *worker, lua_State *L) {
    // avoid qt_menu.nib loading
    QApplication::setAttribute(Qt::AA_MacPluginApplication, true);
    Application *app = new Application(worker);

    lua_pushclass2<Application>(L, app, "mimas.Application");
    // new env table
    lua_newtable(L);
    // ... <app> <env>
    lua_setfenv(L, -2);
    return 1;
  }

  ~Application() {
    MIMAS_DEBUG_GC
  }

  // Destructor method called when the object goes out of Lua scope
  virtual void dub_destroy() {
    dub_cleanup();
    deleteLater();
  }

  /** Start event loop.
   */
  int exec();

  /** Key to retrieve 'this' value from a running thread.
   */
  static pthread_key_t sAppKey;

  /** Thread should stop. */
  static void terminate(int sig);

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

  /** Return the size of the desktop as a pair (width, height).
   */
  LuaStackSize screenSize(lua_State *L) {
    QRect rect = desktop()->geometry();
    lua_pushnumber(L, rect.width());
    lua_pushnumber(L, rect.height());
    return 2;
  }

private:
  class LuaEventsProcessor : public QObject
  {
    lubyk::Worker *worker_;
  public:
    LuaEventsProcessor(lubyk::Worker *worker)
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
#endif // LUBYK_INCLUDE_MIMAS_APPLICATION_H_