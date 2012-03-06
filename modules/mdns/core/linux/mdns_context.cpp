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
#include "mdns/Context.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/timeval.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>


using namespace mdns;

/** This class is used to have the AvahiPoll API work with lubyk's
 * Scheduler.
 */
class AvahiWatch : public lk::SelectCallback {
  AvahiWatchEvent last_event_;
  AvahiWatchCallback callback_;
  bool in_callback_;
  Context *ctx_;
  /** Obscure field used internally by Avahi.
   * This is *NOT* our own userdata passed in sNew.
   */
  void *userdata_;
public:
  AvahiWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, Context *ctx, void *userdata)
    : lk::SelectCallback(fd)
    , last_event_(event)
    , callback_(callback)
    , in_callback_(false)
    , ctx_(ctx)
    , userdata_(userdata) {
    // This registers the class into lua (no GC in lua).
    ctx_->addSelectCallback(this, false);
    // Insert fd in proper fields.
    update(event);
  }

  virtual ~AvahiWatch() {
  }

  void update(AvahiWatchEvent event) {
    lk::SelectCallback::update(
        event & AVAHI_WATCH_IN,
        event & AVAHI_WATCH_OUT,
        // no timeout
        -1);
  }

  virtual void callback(bool read, bool write, bool timeout) {
    if (read) {
      last_event_ = AVAHI_WATCH_IN;
      in_callback_ = true;
      callback_(this, fd_, last_event_, userdata_);
      in_callback_ = false;
    }
    if (write) {
      last_event_ = AVAHI_WATCH_OUT;
      in_callback_ = true;
      callback_(this, fd_, last_event_, userdata_);
      in_callback_ = false;
    }
  }

  AvahiWatchEvent events() const {
    return in_callback_ ? last_event_ : (AvahiWatchEvent)0;
  }
};

/** This class is used to have the AvahiPoll API work with lubyk's
 * Scheduler. It provides the Timeout callback.
 */
class AvahiTimeout : public lk::SelectCallback {
  AvahiTimeoutCallback callback_;
  Context *ctx_;
  /** Obscure field used internally by Avahi.
   * This is *NOT* our own userdata passed in sNew.
   */
  void *userdata_;
public:
  AvahiTimeout(
      const struct timeval *tv,
      AvahiTimeoutCallback callback,
      Context *ctx,
      void *userdata)
    : lk::SelectCallback(0)
    , callback_(callback)
    , ctx_(ctx)
    , userdata_(userdata) {
    // This registers the class into lua (no gc).
    ctx_->addSelectCallback(this, false);
    // Insert fd in proper fields.
    update(tv);
  }

  virtual ~AvahiTimeout() {
  }

  void update(const struct timeval *tv) {
    double t = 0;
    if (tv) {
      // avahi_age returns age of 'tv' in micro seconds
      // (negative if tv is in the future).
      t = -avahi_age(tv)/1000;
      if (t < 0) {
        t = 0; // fire as soon as possible
      }
    } else {
      t = -1; // disable timeout
    }
    lk::SelectCallback::update(false, false, t);
  }

  virtual void callback(bool read, bool write, bool timeout) {
    if (timeout) {
      callback_(this, userdata_);
    }
  }
};

namespace mdns {

class Context::Implementation {
  AvahiClient *client_;
  Context *ctx_;
  AvahiPoll *poll_api_;
public:
  Implementation(Context *ctx)
    : client_(NULL)
    , ctx_(ctx)
    , poll_api_(NULL)
  {}

  /** This is called once the Lua state is properly initialized because
   * addSelectCallback will get called directly.
   */
  void start() {
    int error;
                              
    // This calls sNew
    poll_api_ = new AvahiPoll;
    setupAPI(poll_api_, ctx_);     // ctx_ ends in poll_api_->userdata
    client_ = avahi_client_new(
        poll_api_,                 // Our Lua coroutine fd based API
        (AvahiClientFlags)0,                         // No flags
        sClientCallback,           // Client callbacks
        ctx_,                      // This ends in client_->userdata
        &error);                   // error

    if (!client_) {
      throw dub::Exception("Could not create avahi client (%s).", avahi_strerror(error));
    }
  }

  ~Implementation() {
    if (client_) avahi_client_free(client_);
    if (poll_api_) delete poll_api_;
  }

  void *context() {
    return client_;
  }

  /** CLIENT Callback (this is called as part of a Lua Thread resume
   * (through AvahiWatch/lk.SelectCallback).
   */
  static void sClientCallback(AvahiClient *c, AvahiClientState state, void *userdata) {
    // Called whenever the client or server state changes.
    Context *context = (Context*)userdata;
    switch(state) {
      case AVAHI_CLIENT_S_RUNNING:
        /** Ready to process registrations.
         */
        if (!context->dub_pushcallback("start")) {
          throw dub::Exception("Missing 'start' callback for mdns.Context.");
        }
        context->dub_call(1, 0);
        break;
      case AVAHI_CLIENT_FAILURE:
        /** Something went really wrong...
         */
        if (!context->dub_pushcallback("failure")) {
          throw dub::Exception("Missing 'failure' callback for mdns.Context.");
        }
        lua_pushstring(context->dub_L, avahi_strerror(avahi_client_errno(c)));
        // <func> <self> <err>
        context->dub_call(2, 0);
        break;
      case AVAHI_CLIENT_S_COLLISION: /* continue */
        /** Another host with same name is on the network. Remove our
         * registered services until things come back to normal.
         */
      case AVAHI_CLIENT_S_REGISTERING:
        /** The server is registering, remove our services until things
         * come back to normal.
         */
        if (!context->dub_pushcallback("stop")) {
          throw dub::Exception("Missing 'stop' callback for mdns.Context.");
        }
        context->dub_call(1, 0);
        break;
      case AVAHI_CLIENT_CONNECTING:
        // ignore
        ;
    }
  }

  /** POLL API Callbacks. These callbacks enable the avahi 'select' to live
   * within our event loop (zmq or Qt based).
   *
   * Create an new SelectCallback. The select callback registers itself in
   * the lk.Scheduler (Lua) through the mdns::Context.
   */
  static AvahiWatch* sNew(
      const AvahiPoll *api,
      int fd,
      AvahiWatchEvent event,
      AvahiWatchCallback callback,
      void *userdata) {
    Context *ctx = (Context*)api->userdata;
    return new AvahiWatch(fd, event, callback, ctx, userdata);
  }

  /** Read/write events changed. Update the scheduler (calls lk.SelectCallback::update).
   */
  static void sUpdate(AvahiWatch *w, AvahiWatchEvent events) {
    w->update(events);
  }

  static AvahiWatchEvent sGetEvents(AvahiWatch *w) {
    return w->events();
  }

  static void sFree(AvahiWatch *w) {
    // Lua does not delete the pushed lk.SelectCallbacks
    delete w;
  }

  static AvahiTimeout* sNewTimeout(
      const AvahiPoll *api,
      const struct timeval *tv,
      AvahiTimeoutCallback callback,
      void *userdata) {
    Context *ctx = (Context*)api->userdata;
    return new AvahiTimeout(tv, callback, ctx, userdata);
  }

  static void sUpdateTimeout(
      AvahiTimeout *t,
      const struct timeval *tv) {
    t->update(tv);
  }

  static void sFreeTimeout(AvahiTimeout *t) {
    // Not gc by Lua. Do it ourselves.
    delete t;
  }

  /** Create a new pollAPI struct with our Context as userdata
   * and our callbacks.
   */
  void setupAPI(AvahiPoll *poll, void *userdata) {
    poll->userdata = userdata;
    poll->watch_new = sNew;
    poll->watch_update = sUpdate;
    poll->watch_get_events = sGetEvents;
    poll->watch_free = sFree;
    poll->timeout_new = sNewTimeout;
    poll->timeout_update = sUpdateTimeout;
    poll->timeout_free = sFreeTimeout;
  }

};

Context::Context() {
  impl_ = new Context::Implementation(this);
}

Context::~Context() {
  delete impl_;
}

/** Returns a pointer to the avahi_client_new
 */
void *Context::context() {
  return impl_->context();
}

void Context::pushobject(lua_State *L, void *ptr, const char *type_name, bool gc) {
  dub::Thread::pushobject(L, ptr, type_name, gc);
  impl_->start();
}

} // mdns
