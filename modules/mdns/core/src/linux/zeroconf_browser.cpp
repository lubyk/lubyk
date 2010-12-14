/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/zeroconf_browser.h"
#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>

extern "C" {
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/alternative.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>
}


namespace oscit {

class ZeroConfBrowser::Implementation {
public:
  Implementation(ZeroConfBrowser *browser) : browser_(browser), avahi_poll_(NULL), avahi_client_(NULL) {}

  ~Implementation() {
    stop();
    if (avahi_client_) avahi_client_free(avahi_client_);
    if (avahi_poll_) avahi_threaded_poll_free(avahi_poll_);
  }

	/** Called from outside of thread to stop operations.
	 */
	void stop() {
    if (browser_->running_) {
      avahi_threaded_poll_stop(avahi_poll_);
      browser_->running_ = false;
    }
    // join threads here
	}

  void start() {
    if (browser_->running_) return;

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
                              this,              // context
                              &error);

    if (avahi_client_ == NULL) {
      fprintf(stderr, "Failed to create avahi client (%s).\n", avahi_strerror(error));
      return;
    }

    if (avahi_client_ == NULL) return;

    AvahiServiceBrowser *browser = NULL;


    // create browser service
    browser = avahi_service_browser_new(avahi_client_, // client
                              AVAHI_IF_UNSPEC,         // interface
                              AVAHI_PROTO_UNSPEC,      // protocol
                              browser_->service_type_.c_str(), // service type
                              NULL,                    // domain
                              (AvahiLookupFlags)0,     // flags
                              Implementation::browser_callback,      // callback
                              this);                   // context
    if (browser == NULL) {
      fprintf(stderr, "Could not create avahi service browser (%s).\n", avahi_strerror(avahi_client_errno(avahi_client_)));
      return;
    }

    avahi_threaded_poll_start(avahi_poll_);
    browser_->running_ = true;
  }

  static void client_callback(AvahiClient *client, AvahiClientState state, void *context) {
    Implementation *impl = (Implementation*)context;

    if (state == AVAHI_CLIENT_FAILURE) {
      fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(client)));
      impl->quit();
    }
  }


  /** Callback called on device notification.
   */
  static void browser_callback(AvahiServiceBrowser *browser,
                                 AvahiIfIndex interface,
                                 AvahiProtocol protocol,
                                 AvahiBrowserEvent event,
                                 const char *name,
                                 const char *type,
                                 const char *domain,
                                 AvahiLookupResultFlags flags,
                                 void *context) {
    Implementation *impl = (Implementation*)context;
    AvahiServiceResolver *resolver;

    switch (event) {
      case AVAHI_BROWSER_FAILURE:
        fprintf(stderr, "Avahi browser failure (%s).\n",
                        avahi_strerror(avahi_client_errno(impl->avahi_client_)));
        impl->quit();
        break;
      case AVAHI_BROWSER_NEW: /* continue */
        resolver = avahi_service_resolver_new(impl->avahi_client_, // client
                                  interface,           // interface
                                  protocol,            //
                                  name,                //
                                  type,                //
                                  domain,              //
                                  AVAHI_PROTO_UNSPEC,  // address protocol (IPv4, IPv6)
                                  (AvahiLookupFlags)0, // flags
                                  Implementation::resolve_callback,  // callback
                                  impl);               // context
        if (resolver == NULL) {
          fprintf(stderr,"Error while trying to resolve %s @ %s (%s)\n", name, domain, avahi_strerror(avahi_client_errno(impl->avahi_client_)));
        }
        break;
      case AVAHI_BROWSER_REMOVE:
        impl->browser_->lock();
          impl->browser_->remove_device(name);
        impl->browser_->unlock();
        break;
      case AVAHI_BROWSER_ALL_FOR_NOW:
      case AVAHI_BROWSER_CACHE_EXHAUSTED:
        // fprintf(stderr, "(Browser) %s\n", event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW");
        break;
    }

  }


  static void resolve_callback(AvahiServiceResolver *resolver,
                               AVAHI_GCC_UNUSED AvahiIfIndex interface,
                               AVAHI_GCC_UNUSED AvahiProtocol protocol,
                               AvahiResolverEvent event,
                               const char *name,
                               const char *type,
                               const char *domain,
                               const char *host_name,
                               const AvahiAddress *address,
                               uint16_t port,
                               AvahiStringList *txt,
                               AvahiLookupResultFlags flags,
                               void* context) {

    Implementation *impl = (Implementation*)context;

    switch (event) {
     case AVAHI_RESOLVER_FAILURE:
        fprintf(stderr, "Error while trying to resolve %s @ %s (%s)\n", name, domain,
                        avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(resolver))));
        break;
      case AVAHI_RESOLVER_FOUND:
        impl->browser_->lock();
          // domain ?
          // AvahiAddress ?
          impl->browser_->add_device(Location(
                                      impl->browser_->protocol_.c_str(),
                                      name,
                                      host_name,
                                      port
                                      ));
        impl->browser_->unlock();
        break;
    }

    avahi_service_resolver_free(resolver);
  }

 private:
  /** Called from own callbacks (inside thread).
   */
  void quit() {
    avahi_threaded_poll_quit(avahi_poll_);
  }

  ZeroConfBrowser *browser_;
  AvahiThreadedPoll *avahi_poll_;
  AvahiClient     *avahi_client_;
};

ZeroConfBrowser::ZeroConfBrowser(const char *service_type)
    : running_(false),
      service_type_(service_type),
      command_(NULL),
      proxy_factory_(NULL),
      found_devices_(FOUND_DEVICE_HASH_SIZE) {
  get_protocol_from_service_type();
  impl_ = new ZeroConfBrowser::Implementation(this);
}

ZeroConfBrowser::~ZeroConfBrowser() {
  delete impl_;
  if (proxy_factory_) delete proxy_factory_;
}

void ZeroConfBrowser::start() {
  impl_->start();
}

void ZeroConfBrowser::stop() {
  impl_->stop();
}

} // oscit
