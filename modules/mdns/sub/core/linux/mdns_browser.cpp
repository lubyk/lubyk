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

#include "mdns/AbstractBrowser.h"
#include "mdns/Service.h"
#include "lk/SelectCallback.h"

#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>      // For errno, EINTR
#include <time.h>

extern "C" {
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
}

#include <queue>

using namespace lubyk;

/** This class is used to have the AvahiPoll API work with lubyk's
 * Scheduler.
 */
class AvahiWatch : public lk::SelectCallback {
  AvahiWatchEvent last_event_;
  AvahiWatchCallback callback_;
  bool in_callback_;
  /** This is the userdata passed in avahi_client_new. In our case, this is
   * an instance of AbstractBrowser::Implementation.
   */
  mdns::Browser *browser_;
public:
  AvahiWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void* userdata)
    : lk::SelectCallback(fd, event & AVAHI_WATCH_IN, event & AVAHI_WATCH_OUT, 0)
    , last_event_(event)
    , callback_(callback)
    , browser_((mdns::Browser*)userdata)
  {
    // This registers the class into lua.
    browser_->addSelectCallback(this);
  }

  virtual ~AvahiWatch() {
  }

  void update(bool read, bool write, double timeout) {
    lk::SelectCallback::update(read, write, timeout);
  }

  AvahiWatchEvent events() const {
    return in_callback_ ? last_event_ : (AvahiWatchEvent)0;
  }
};

namespace mdns {

/** Get struct with all the callbacks.
 */
static const AvahiPoll* get_avahi_poll(void);

class AbstractBrowser::Implementation {
  /** Last detected services.
   */
  std::queue<Service*> found_services_;
  bool query_next_;
  AvahiClient *avahi_client_;
public:
  Implementation(AbstractBrowser *master)
    : master_(master)
    , query_next_(true)
    , avahi_client_(NULL)
  {
    // This will call new AvahiWatch
    avahi_client_ = avahi_client_new(pollAPI(), ..., master_, ...);
  }

  ~Implementation() {
    while (!found_services_.empty()) {
      delete found_services_.front();
      found_services_.pop();
    }
  }

  int fd() {
    return fd_;
  }

  // [2] Return found services. If this returns NULL, the calling app must
  // waitRead on fd() before asking again.
  Service *getService() {
    if (query_next_) {
      // calls sBrowseCallback
      DNSServiceErrorType err = DNSServiceProcessResult(service_);
      if (err) {
        // An error occured. Halt.
        fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
        return 0;
      }
      query_next_ = false;
    }

    if (found_services_.empty()) {
      query_next_ = true;
      return NULL;
    }

    Service *service = found_services_.front();
    found_services_.pop();
    return service;
  }

  // CLIENT Callback

  // POLL API Callbacks
  static AvahiWatch* sNew(const AvahiPoll *api, int fd, AvahiWatchEvent event, AvahiWatchCallback callback, 
      void *userdata) {
    Implementation *impl = (Implementation*)userdata;
    Service *service = new Service(fd, event);
    // This will add the service's fd to our select loop.
    impl->found_services_.push(service);
    return service;
  }

  static void sUpdate(AvahiWatch *w, AvahiWatchEvent events) {
    w->event_ = events;
  }

  static AvahiWatchEvent sGetEvents(AvahiWatch *w) {
    return w->event_;
  }

  static void sFree(AvahiWatch *w) {
    delete w;
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
      NULL,
      NULL,
      NULL
    };

    return &poll_functions;
  }

};

AbstractBrowser::AbstractBrowser(const char *service_type)
      : service_type_(service_type) {
  setProtocolFromServiceType();
  impl_ = new AbstractBrowser::Implementation(this);
  fd_ = impl_->fd();
}

AbstractBrowser::~AbstractBrowser() {
  delete impl_;
}

Service *AbstractBrowser::getService() {
  return impl_->getService();
}

} // mdns




