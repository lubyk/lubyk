#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#include "oscit/zeroconf.h"


namespace oscit {

#define MAX_NAME_COUNTER_BUFFER_SIZE 10


class ZeroConfRegistration::Implementation {
public:
  Implementation(ZeroConfRegistration *master) : master_(master), counter_(0) {
    do_start();
  }
  
  ~Implementation() {
    quit();
    if (avahi_poll_) avahi_simple_poll_free(avahi_poll_);
    if (avahi_client_) avahi_client_free(avahi_client_);
  }
  
  void quit() {
    avahi_simple_poll_quit(avahi_poll_);
  }
  
  void create_services() {
    int error;
    if (avahi_entry_group_is_empty(group_)) {
      // new group or after reset
      error = avahi_entry_group_add_service(group_,  // group
        AVAHI_IF_UNSPEC,                        // interface
        AVAHI_PROTO_UNSPEC,                     // protocol to announce service with
        (AvahiPublishFlags)0,                   // flags
        name_.c_str(),                          // name
        master_->service_type_.c_str(),         // service type
        NULL,                                   // domain
        NULL,                                   // host
        master_->port_,                         // port
        NULL);                                  // list of txt records

      if (error < 0) {
          // error
        if (error == AVAHI_ERR_COLLISION) {
          next_name();
          create_services();
        } else {
          fprintf(stderr, "Could not add service to avahi group (%s)\n", avahi_strerror(error));
          return;
        }
      }

      // start registering the service
      error = avahi_entry_group_commit(group_);
      if (error < 0) {
        fprintf(stderr, "Could not commit avahi group (%s)\n", avahi_strerror(error));
      }
    }
  }
  
  void do_start() {
    int error;
    // create poll object
    avahi_poll_ = avahi_simple_poll_new();
    if (avahi_poll_ == NULL) {
      fprintf(stderr, "Could not create avahi simple poll object.\n");
      return;
    }

    // create client
    avahi_client_ = avahi_client_new(avahi_simple_poll_get(avahi_poll_),
                              (AvahiClientFlags)0,             // flags
                              Implementation::client_callback, // callback
                              this,                            // context
                              &error);

    if (avahi_client_ == NULL) {
      fprintf(stderr, "Failed to create avahi client (%s).\n", avahi_strerror(error));
      return;
    }
    
    group_ = avahi_entry_group_new(avahi_client_,  // client
                 Implementation::entry_group_callback,   // calback
                 this);                                  // context

    if (group_ == NULL) {
      fprintf(stderr, "Could not create avahi group (%s).\n",
                      avahi_strerror(avahi_client_errno(avahi_client_)));
      return;
    }

    avahi_simple_poll_loop(avahi_poll_);

    avahi_entry_group_reset(group_);
  }
  
  void next_name() {
    char name_buffer[MAX_NAME_COUNTER_BUFFER_SIZE+1];
    snprintf(name_buffer, MAX_NAME_COUNTER_BUFFER_SIZE, " (%i)", ++counter_);
    name_ = std::string(master_->name_).append(name_buffer);
    avahi_entry_group_reset(group_);
  }
  
  
  static void client_callback(AvahiClient *client, AvahiClientState state, void *context) {
    Implementation *impl = (Implementation*)context;
    
    // called on every server state change
    switch (state) {
      case AVAHI_CLIENT_S_RUNNING:
        // server is fine, we can register
        impl->create_services();
        break;
      case AVAHI_CLIENT_S_COLLISION:
      case AVAHI_CLIENT_S_REGISTERING:
        // server not ready (collision, registering)
        // make sure we restart registration
        avahi_entry_group_reset(impl->group_);
        break;
      case AVAHI_CLIENT_CONNECTING:
        // wait
        break;
      case AVAHI_CLIENT_FAILURE:
        fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(client)));
        impl->quit();
    }
  }
  
  static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *context) {
    Implementation *impl = (Implementation*)context;

    // callback called whenever our entry group state changes
    switch (state) {
      case AVAHI_ENTRY_GROUP_ESTABLISHED:
        // done !
        impl->master_->name_ = impl->name_;
        impl->master_->host_ = impl->host_;
        impl->master_->registration_done();
        break;
      case AVAHI_ENTRY_GROUP_COLLISION:
        // build new name
        impl->next_name();
        // retry
        impl->create_services();
        break;
      case AVAHI_ENTRY_GROUP_FAILURE:
        fprintf(stderr, "Registration failure (%s).\n",
                        avahi_strerror(avahi_client_errno(impl->avahi_client_)));
        impl->quit();
        break;
      case AVAHI_ENTRY_GROUP_UNCOMMITED:
      case AVAHI_ENTRY_GROUP_REGISTERING:
        ;
    }
  }

  ZeroConfRegistration *master_;
  AvahiSimplePoll *avahi_poll_;
  AvahiClient     *avahi_client_;
  std::string name_;
  std::string host_;
  AvahiEntryGroup *group_;
  int counter_;
};


ZeroConfRegistration::ZeroConfRegistration(const std::string &name, const std::string &service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {
  impl_ = new ZeroConfRegistration::Implementation(this);
}

ZeroConfRegistration::~ZeroConfRegistration() {
  impl_->quit();
  delete impl_;
}

} // oscit
