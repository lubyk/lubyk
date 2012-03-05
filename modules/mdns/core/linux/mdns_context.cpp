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


/** This class is used to have the AvahiPoll API work with lubyk's
 * Scheduler.
 */
class AvahiWatch : public lk::SelectCallback {
  AvahiWatchEvent last_event_;
  AvahiWatchCallback callback_;
  bool in_callback_;
  mdns::Context *ctx_;
public:
  AvahiWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, Context *ctx)
    : lk::SelectCallback(fd)
    , last_event_(event)
    , callback_(callback)
    , in_callback_(false)
    , ctx_(ctx) {
    // This registers the class into lua.
    ctx_->addSelectCallback(this);
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
      callback_(this, fd_, last_event_, ctx_); // ctx_ = userdata
      in_callback_ = false;
    }
    if (write) {
      last_event_ = AVAHI_WATCH_OUT;
      in_callback_ = true;
      callback_(this, fd_, last_event_, ctx_); // ctx_ = userdata
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
  mdns::Context *ctx_;
public:
  AvahiTimeout(
      const struct timeval *tv,
      AvahiTimeoutCallback callback,
      Context *ctx)
    : lk::SelectCallback(0)
    , callback_(callback)
    , ctx_(ctx) {
    // This registers the class into lua.
    ctx_->addSelectCallback(this);
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
      callback_(this, ctx_); // ctx_ = userdata
    }
  }
};

namespace mdns {

class Context::Implementation {
  AvahiClient *avahi_client_;
  Context *ctx_;
public:
  Implementation(mdns::Context *ctx)
    : avahi_client_(NULL)
    , ctx_(ctx)
  {
    int error;
                              
    // This calls sNew
    client_ = avahi_client_new(
        pollAPI(),                 // Our Lua coroutine fd based API
        0,                         // No flags
        sClientCallback,           // Client callbacks
        ctx_,                      // Context
        &error);                   // error

    if (!client_) {
      throw dub::Exception("Could not create avahi client (%s).", avahi_strerror(error));
    }
  }

  ~Implementation() {
    if (client_) avahi_client_delete(client_);
  }

  /** CLIENT Callback (this is called as part of a Lua Thread resume
   * (through AvahiWatch/lk.SelectCallback).
   */
  static void sClientCallback(AvahiClient *c, AvahiClientState state, void *userdata) {
    // Called whenever the client or server state changes.
    if (state == AVAHI_CLIENT_FAILURE) {
      throw dub::Exception("Server connection failure (%s).", avahi_strerror(avahi_client_errno(c)));
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
    mdns::Context *ctx = (mdns::Context*)userdata;
    return new AvahiWatch(fd, event, callback, ctx);
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
    delete w;
  }

  static AvahiTimeout* sNewTimeout(
      const AvahiPoll *api,
      const struct timeval *tv,
      AvahiTimeoutCallback callback,
      void *userdata) {
    mdns::Context *ctx = (mdns::Context*)userdata);
    return new AvahiTimeout(tv, callback, ctx);
  }

  static void sUpdateTimeout(
      AvahiTimeout *t,
      const struct timeval *tv) {
    t->update(tv);
  }

  static void sFreeTimeout(AvahiTimeout *t) {
    delete t;
  }

  /** This is just a C++ trick to allocate and return static
   * data. The poll_functions data contains all our callbacks.
   */
  const AvahiPoll* pollAPI(void) {
    static const AvahiPoll poll_functions = {
      NULL,
      sNew,
      sUpdate,
      sGetEvents,
      sFree,
      sNewTimeout,
      sUpdateTimeout,
      sFreeTimeout,
    };

    return &poll_functions;
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

} // mdns
