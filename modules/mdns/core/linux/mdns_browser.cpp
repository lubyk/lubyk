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

namespace mdns {

class Browser::Implementation {
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
        0,
        sBrowseCallback,
        this);

    if (!browser_) {
      throw dub::Exception("Failed to create service browser (%s).\n", avahi_strerror(avahi_client_errno(client)));
    }
  }

  ~Implementation() {
  }

  // We do not use a file descriptor.
  int fd() {
    return 0;
  }

  // We push services directly on in Lua. Not used.
  Service *getService() {
    return NULL;
  }

  void pushService(Service *service) {
    lua_State *L = master_-lua_;
    if (!master_->pushLuaCallback("browseCallback")) {
      // This is really bad...
      throw dub::Exception("'browseCallback' callback not set for mdns.Browser !");
    }
    // <func> <self>
    service->luaInit(L, service, "mdns.Service");
    // <func> <self> <service>
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      throw dub::Exception("Error in 'browseCallback': %s\n", lua_tostring(L, -1));
    }
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

    Implementation impl = (Implementation*)userdata;
    assert(avahi_browser);

    AvahiClient *client = avahi_service_browser_get_client(browser);
    switch (event) {
      case AVAHI_BROWSER_FAILURE:
        throw dub::Exception("mdns.Browser browser failure (%s).\n",
            avahi_strerror(
              avahi_client_errno(client)));
        return;
      case AVAHI_BROWSER_NEW:
        // New service available.

        AvahiServiceResolver *resolver = avahi_service_resolver_new(
            client,
            interface,
            protocol,
            name,
            type,
            domain,
            AVAHI_PROTO_UNSPEC,
            0,
            sResolveCallback,
            impl);
        /** We do not have to save the resolver object: we free it in
         * the callback. If the server quits before the callback is
         * called, the server will free the resolver for us.
         */
        if (!resolver) {
          throw dub::Exception("Failed to resolve service '%s' (%s).\n", name, avahi_strerror(avahi_client_errno(client)));
        }
      case AVAHI_BROWSER_REMOVE:
        impl_->pushService(new Service(
              impl_->master_->service_type_,
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
  static void resolve_callback(
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
              impl_->master_->service_type_,
              name,
              interface,
              type,
              domain,
              true);
        char *t = avahi_string_list_to_string(txt);
        service->set(host_name, port, t);
        avahi_free(t);

        impl_->pushService(service);
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
  fd_ = impl_->fd();
}

Browser::~Browser() {
  delete impl_;
}

Service *Browser::getService() {
  return impl_->getService();
}

} // mdns

















/***
  This file is part of avahi.

  avahi is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  avahi is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
  Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with avahi; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

static AvahiSimplePoll *simple_poll = NULL;

static void resolve_callback(
    AvahiServiceResolver *r,
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
    AVAHI_GCC_UNUSED void* userdata) {

    assert(r);

    /* Called whenever a service has been resolved successfully or timed out */

    switch (event) {
        case AVAHI_RESOLVER_FAILURE:
            fprintf(stderr, "(Resolver) Failed to resolve service '%s' of type '%s' in domain '%s': %s\n", name, type, domain, avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(r))));
            break;

        case AVAHI_RESOLVER_FOUND: {
            char a[AVAHI_ADDRESS_STR_MAX], *t;

            fprintf(stderr, "Service '%s' of type '%s' in domain '%s':\n", name, type, domain);

            avahi_address_snprint(a, sizeof(a), address);
            t = avahi_string_list_to_string(txt);
            fprintf(stderr,
                    "\t%s:%u (%s)\n"
                    "\tTXT=%s\n"
                    "\tcookie is %u\n"
                    "\tis_local: %i\n"
                    "\tour_own: %i\n"
                    "\twide_area: %i\n"
                    "\tmulticast: %i\n"
                    "\tcached: %i\n",
                    host_name, port, a,
                    t,
                    avahi_string_list_get_service_cookie(txt),
                    !!(flags & AVAHI_LOOKUP_RESULT_LOCAL),
                    !!(flags & AVAHI_LOOKUP_RESULT_OUR_OWN),
                    !!(flags & AVAHI_LOOKUP_RESULT_WIDE_AREA),
                    !!(flags & AVAHI_LOOKUP_RESULT_MULTICAST),
                    !!(flags & AVAHI_LOOKUP_RESULT_CACHED));

            avahi_free(t);
        }
    }

    avahi_service_resolver_free(r);
}


static void client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {
    assert(c);

    /* Called whenever the client or server state changes */

    if (state == AVAHI_CLIENT_FAILURE) {
        fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(c)));
        avahi_simple_poll_quit(simple_poll);
    }
}

int main(AVAHI_GCC_UNUSED int argc, AVAHI_GCC_UNUSED char*argv[]) {
    AvahiClient *client = NULL;
    AvahiServiceBrowser *sb = NULL;
    int error;
    int ret = 1;

    /* Allocate main loop object */
    if (!(simple_poll = avahi_simple_poll_new())) {
        fprintf(stderr, "Failed to create simple poll object.\n");
        goto fail;
    }

    /* Allocate a new client */
    client = avahi_client_new(avahi_simple_poll_get(simple_poll), 0, client_callback, NULL, &error);

    /* Check wether creating the client object succeeded */
    if (!client) {
        fprintf(stderr, "Failed to create client: %s\n", avahi_strerror(error));
        goto fail;
    }

    /* Create the service browser */
    if (!(sb = avahi_service_browser_new(client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, "_http._tcp", NULL, 0, browse_callback, client))) {
        fprintf(stderr, "Failed to create service browser: %s\n", avahi_strerror(avahi_client_errno(client)));
        goto fail;
    }

    /* Run the main loop */
    avahi_simple_poll_loop(simple_poll);

    ret = 0;

fail:

    /* Cleanup things */
    if (sb)
        avahi_service_browser_free(sb);

    if (client)
        avahi_client_free(client);

    if (simple_poll)
        avahi_simple_poll_free(simple_poll);

    return ret;
}
