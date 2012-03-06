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

#include "mdns/Browser.h"
#include "mdns/Context.h"
#include "mdns/Service.h"
#include "lk/SelectCallback.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/timeval.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

namespace mdns {

class Browser::Implementation {
  Browser *master_;
  AvahiServiceBrowser *browser_;
public:
  Implementation(Browser *master, Context *ctx)
    : master_(master)
    , browser_(NULL)
  {
    AvahiClient *client = (AvahiClient*)ctx->context();

    // Create the service browser.
    browser_ = avahi_service_browser_new(
        client,                    // 
        AVAHI_IF_UNSPEC,           // any interface
        AVAHI_PROTO_UNSPEC,        // any protocol
        master_->serviceType(),    // service type   ("_http._tcp")
        NULL,                      // default domain (".local")
        (AvahiLookupFlags)0,
        sBrowseCallback,
        this);

    if (!browser_) {
      throw dub::Exception("Failed to create service browser (%s).\n", avahi_strerror(avahi_client_errno(client)));
    }
  }

  ~Implementation() {
    if (browser_) {
      avahi_service_browser_free(browser_);
    }
  }

  void pushService(Service *service) {
    lua_State *L = master_->dub_L;
    if (!master_->dub_pushcallback("browseCallback")) {
      // This is really bad...
      throw dub::Exception("'browseCallback' callback not set for mdns.Browser !");
    }
    // <func> <self>
    service->pushobject(L, service, "mdns.Service", true);
    // <func> <self> <service>
    master_->dub_call(2, 0);
    // The service will be garbage collected by Lua.
  }

  /** Called when a new service appears or is removed from the
   * netwhor.
   */
  static void sBrowseCallback(
    AvahiServiceBrowser *browser,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *name,
    const char *type,
    const char *domain,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void* userdata) {

    Implementation *impl = (Implementation*)userdata;

    AvahiClient *client = avahi_service_browser_get_client(browser);
    switch (event) {
      case AVAHI_BROWSER_FAILURE:
        throw dub::Exception("mdns.Browser browser failure (%s).\n",
            avahi_strerror(
              avahi_client_errno(client)));
        return;
      case AVAHI_BROWSER_NEW: {
          // New service available.

          AvahiServiceResolver *resolver = avahi_service_resolver_new(
              client,
              interface,
              protocol,
              name,
              type,
              domain,
              AVAHI_PROTO_UNSPEC,
              (AvahiLookupFlags)0,
              sResolveCallback,
              impl);
          /** We do not have to save the resolver object: we free it in
           * the callback. If the server quits before the callback is
           * called, the server will free the resolver for us.
           */
          if (!resolver) {
            throw dub::Exception("Failed to resolve service '%s' (%s).\n", name, avahi_strerror(avahi_client_errno(client)));
          }
        }
        break;
      case AVAHI_BROWSER_REMOVE:
        impl->pushService(new Service(
              impl->master_->service_type_,
              name,
              interface,
              type,
              domain,
              false));
        break;
      case AVAHI_BROWSER_ALL_FOR_NOW:
      case AVAHI_BROWSER_CACHE_EXHAUSTED:
        // Done: nothing more.
        break;
    }
  }


  /** Service resolution callback.
   */
  static void sResolveCallback(
      AvahiServiceResolver *r,
      AvahiIfIndex interface,
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
      void *userdata) {
    Implementation *impl = (Implementation*)userdata;
    AvahiClient *client = avahi_service_resolver_get_client(r);

    // Service either resolved or timed out.
    switch (event) {
      case AVAHI_RESOLVER_FAILURE:
        throw dub::Exception("Could not resolve service '%s' (%s).\n",
            name,
            avahi_strerror(avahi_client_errno(client)));
      case AVAHI_RESOLVER_FOUND: {
        Service *service = new Service(
              impl->master_->service_type_,
              name,
              interface,
              type,
              domain,
              true);
        std::string t;
        AvahiStringList *l = txt;
        // This is idiotic. Why can't avahi just give us the
        // raw txt record, eventually with a ctor for AvahiStringList.
        // (The same goes for registration).
        while (l) {
          t.push_back(l->size);
          t.append((const char *)l->text, l->size);
          l = l->next;
        }
        service->set(host_name, port, t);

        impl->pushService(service);
      }
    }

    // Free resolver allocated in sBrowseCallback.
    avahi_service_resolver_free(r);
  }
};

Browser::Browser(Context *ctx, const char *service_type)
      : service_type_(service_type) {
  setProtocolFromServiceType();
  impl_ = new Browser::Implementation(this, ctx);
  // All select is managed in mdns.Context).
  // We do not use a file descriptor.
  fd_ = 0;
}

Browser::~Browser() {
  delete impl_;
}

LuaStackSize Browser::getService(lua_State *L) {
  // We push services directly on in Lua. Not used.
  return 0;
}

} // mdns

