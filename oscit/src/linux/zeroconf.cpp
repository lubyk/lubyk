#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

#include "oscit/zeroconf.h"


namespace oscit {

static void s_client_callback(AvahiClient *client, AvahiClientState state, void *context) {
  ZeroConf *zero_conf = (ZeroConf*)context;

  if (state == AVAHI_CLIENT_FAILURE) {
    fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(client)));
    zero_conf->quit();
  } else {
    zero_conf->client_callback(state);
  }
}

// Note: the select() implementation on Windows (Winsock2)
//       fails with any timeout much larger than this
#define LONG_TIME 100000000

ZeroConf::ZeroConf() : timeout_(LONG_TIME), avahi_poll_(NULL), avahi_client_(NULL) {
  int error;
  // create poll object
  avahi_poll_ = avahi_avahi_poll_new();
  if (avahi_poll_ == NULL) {
    fprintf(stderr, "Could not create avahi simple poll object.\n");
    return;
  }
  
  // create client
  avahi_client_ = avahi_client_new(avahi_avahi_poll_get(avahi_poll_),
                            0,                 // flags
                            s_client_callback, // callback
                            this,              // context
                            &error);

  if (avahi_client_ == NULL) {
    fprintf(stderr, "Failed to create avahi client (%s).\n", avahi_strerror(error));
    return;
  }
}

ZeroConf::quit() {
  avahi_simple_poll_quit(avahi_poll_);
  listen_thread_.kill();
}

ZeroConf::~ZeroConf() {
  if (avahi_poll_) avahi_simple_poll_free(avahi_poll_);
  if (avahi_client_) avahi_client_free(avahi_client_);
}

void ZeroConf::listen(Thread *thread) {
  // Run until poll quit.
  avahi_avahi_poll_loop(avahi_poll_);
}

} // oscit
