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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#include "oscit/zeroconf_registration.h"

extern "C" {
struct AvahiSimplePoll;

struct AvahiThreadedPoll {
  AvahiSimplePoll *simple_poll;
  pthread_t thread_id;
  pthread_mutex_t mutex;
  int thread_running;
  int retval;
};
}

namespace oscit {


class ZeroConfRegistration::Implementation {
public:
  Implementation(ZeroConfRegistration *master) : registration_(master), avahi_poll_(NULL), avahi_client_(NULL), avahi_group_(NULL), host_(registration_->host_),  counter_(0), running_(false) {
    name_ = avahi_strdup(registration_->name_.c_str());
    start();
  }

  ~Implementation() {
    stop();
    if (avahi_client_) avahi_client_free(avahi_client_);
    if (avahi_poll_) avahi_threaded_poll_free(avahi_poll_);
    avahi_free(name_);
  }

	/** Called from outside of thread to stop operations.
	 */
	void stop() {
    if (running_) {
      int error = avahi_threaded_poll_stop(avahi_poll_);
      if (error < 0) {
        printf("Error stopping avahi threaded poll (%s).\n", avahi_strerror(error));
      }
      running_ = false;
    }
	}

  void start() {
    int error;
    // create poll object
    avahi_poll_ = avahi_threaded_poll_new();
    if (avahi_poll_ == NULL) {
      fprintf(stderr, "Could not create avahi threaded poll object.\n");
      return;
    }

    // create client
    avahi_client_ = avahi_client_new(avahi_threaded_poll_get(avahi_poll_),
                              (AvahiClientFlags)0,             // flags
                              Implementation::client_callback, // callback
                              this,                            // context
                              &error);

    if (avahi_client_ == NULL) {
      fprintf(stderr, "Failed to create avahi client (%s).\n", avahi_strerror(error));
      return;
    }

    error = avahi_threaded_poll_start(avahi_poll_);
    if (error < 0) {
      printf("Error starting avahi threaded poll (%s).\n", avahi_strerror(error));
    } else {
      running_ = true;
    }
  }

  void next_name() {
    char *new_name = avahi_alternative_service_name(name_);
    avahi_free(name_);
    name_ = new_name;
  }

  void create_services(AvahiClient *client) {
    int error;

    if (avahi_group_ == NULL) {
      avahi_group_ = avahi_entry_group_new(client,              // client
                   Implementation::entry_avahi_group_callback,  // callback
                   this);                                       // context

      if (avahi_group_ == NULL) {
        fprintf(stderr, "Could not create avahi group (%s).\n",
                        avahi_strerror(avahi_client_errno(client)));
        return;
      }
    }

    if (avahi_entry_group_is_empty(avahi_group_)) {
      // new group or after reset
      error = avahi_entry_group_add_service(avahi_group_,  // group
        AVAHI_IF_UNSPEC,                        // interface
        AVAHI_PROTO_UNSPEC,                     // protocol to announce service with
        (AvahiPublishFlags)0,                   // flags
        name_,                                  // name
        registration_->service_type_.c_str(),   // service type
        NULL,                                   // domain
        NULL,                                   // host
        registration_->port_,                   // port
        NULL);                                  // list of txt records

      if (error < 0) {
          // error
        if (error == AVAHI_ERR_COLLISION) {
          // collision with local service name
          next_name();
          avahi_entry_group_reset(avahi_group_);
          // retry
          create_services(client);
        } else {
          fprintf(stderr, "Could not add service '%s' (%s) to avahi group (%s)\n",
                                  name_,
                                  registration_->service_type_.c_str(),
                                  avahi_strerror(error));
          return;
        }
      }
      // start registering the service
      error = avahi_entry_group_commit(avahi_group_);
      if (error < 0) {
        fprintf(stderr, "Could not commit avahi group '%s' (%s)\n", name_, avahi_strerror(error));
      }
    }
  }

  static void client_callback(AvahiClient *client, AvahiClientState state, void *context) {
    Implementation *impl = (Implementation*)context;

    // called on every server state change
    switch (state) {
      case AVAHI_CLIENT_S_RUNNING:
        // server is fine, we can register
        impl->create_services(client);
        break;
      case AVAHI_CLIENT_S_COLLISION:
      case AVAHI_CLIENT_S_REGISTERING:
        // server not ready (collision, registering)
        // make sure we restart registration
        avahi_entry_group_reset(impl->avahi_group_);
        break;
      case AVAHI_CLIENT_CONNECTING:
        // wait
        break;
      case AVAHI_CLIENT_FAILURE:
        fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(client)));
        impl->quit();
    }
  }

  static void entry_avahi_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *context) {
    Implementation *impl = (Implementation*)context;
    // callback called whenever our entry group state changes
    switch (state) {
      case AVAHI_ENTRY_GROUP_ESTABLISHED:
        // done !
        impl->registration_->lock();
          impl->registration_->name_ = impl->name_;
          impl->registration_->host_ = avahi_client_get_host_name(avahi_entry_group_get_client(g));
          impl->registration_->registration_done();
        impl->registration_->unlock();
        break;
      case AVAHI_ENTRY_GROUP_COLLISION:
        // build new name
        impl->next_name();
        // retry
        impl->create_services(avahi_entry_group_get_client(g));
        break;
      case AVAHI_ENTRY_GROUP_FAILURE:
        fprintf(stderr, "Registration failure (%s).\n",
                        avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));
        impl->quit();
        break;
      case AVAHI_ENTRY_GROUP_UNCOMMITED:
      case AVAHI_ENTRY_GROUP_REGISTERING:
        ;
    }
  }
private:

  /** Called from inside our own thread (callbacks).
	 */
  void quit() {
    avahi_threaded_poll_quit(avahi_poll_);
    running_ = false;
  }

  ZeroConfRegistration *registration_;
  AvahiThreadedPoll *avahi_poll_;
  AvahiClient     *avahi_client_;
  AvahiEntryGroup *avahi_group_;
  char *name_;
  std::string host_;
  int counter_;
  bool running_;
};


ZeroConfRegistration::ZeroConfRegistration(const std::string &name, const char *service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {
  impl_ = new ZeroConfRegistration::Implementation(this);
}

ZeroConfRegistration::~ZeroConfRegistration() {
  delete impl_;
}

void ZeroConfRegistration::stop() {
  impl_->stop();
}

} // oscit
