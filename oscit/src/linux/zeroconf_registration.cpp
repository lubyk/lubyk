
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

struct RegistrationContext {
  RegistrationContext(ZeroConfRegistration *registration, AvahiEntryGroup *group, 
                      const std::string &name, const std::string &host) :
                      registration_(registration), name_(name), host_(host), group_(group), counter_(0) {}
  ZeroConfRegistration *registration_;
  std::string name_;
  std::string host_;
  AvahiEntryGroup *group_;
  int counter_;
}

static void s_create_services(RegistrationContext *context);

static void s_entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *userdata) {
  RegistrationContext *context = (RegistrationContext*)userdata;
  char name_buffer[MAX_NAME_COUNTER_BUFFER_SIZE+1];
  
  // callback called whenever our entry group state changes
  switch (state) {
    case AVAHI_ENTRY_GROUP_ESTABLISHED:
      // done !
      context->registration_->finish_registration(context->name_, context->host_);
      break;
    case AVAHI_ENTRY_GROUP_COLLISION:
      // build new name
      snprintf(name_buffer, MAX_NAME_COUNTER_BUFFER_SIZE, " (%i)", ++context->counter_);
      context->name_ = context->registration_->name().append(name_buffer);
      // retry
      s_create_services(context);
      break;
    case AVAHI_ENTRY_GROUP_FAILURE:
      fprintf(stderr, "Registration failure (%s).\n",
      avahi_strerror(avahi_client_errno(context->registration_->avahi_client())));
      context->registration_->quit();
      break;
    case AVAHI_ENTRY_GROUP_UNCOMMITED:
    case AVAHI_ENTRY_GROUP_REGISTERING:
      ;
  }
}

static void s_create_services(RegistrationContext *context) {
    char *n, r[128];
    int ret;
    
    if (avahi_entry_group_is_empty(context->group_)) {
      // new group or after reset
      ret = avahi_entry_group_add_service(context->group_,  // group
                    AVAHI_IF_UNSPEC,                        // interface
                    AVAHI_PROTO_UNSPEC,                     // protocol to announce service with
                    0,                                      // flags
                    context->name_.c_str(),                 // name
                    context->registration_->service_type(), // service type
                    NULL,                                   // domain
                    NULL,                                   // host
                    context->registration_->port(),         // port
                    NULL);                                  // list of txt records
      
      if (ret < 0) {
        // error
        if (ret == AVAHI_ERR_COLLISION) {
          
        }
              goto collision;

          fprintf(stderr, "Failed to add _ipp._tcp service: %s\n", avahi_strerror(ret));
          goto fail;
      }

        /* Tell the server to register the service */
        if ((ret = avahi_entry_group_commit(group)) < 0) {
            fprintf(stderr, "Failed to commit entry group: %s\n", avahi_strerror(ret));
            goto fail;
        }
    }

    return;

collision:

    /* A service name collision with a local service happened. Let's
     * pick a new name */
    n = avahi_alternative_service_name(name);
    avahi_free(name);
    name = n;

    fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);

    avahi_entry_group_reset(group);

    create_services(c);
    return;

fail:
    avahi_simple_poll_quit(simple_poll);
}


ZeroConfRegistration::ZeroConfRegistration(const std::string &name, const std::string &service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {
  listen_thread_.start<ZeroConfRegistration, &ZeroConfRegistration::do_start>(this, NULL);
}

void ZeroConfRegistration::client_callback(AvahiClientState state) {
  switch (state) {
    case AVAHI_CLIENT_S_RUNNING:
      // server is fine, we can register
      s_create_services(this);
      break;
    case AVAHI_CLIENT_S_COLLISION:
    case AVAHI_CLIENT_S_REGISTERING:
      // server not ready (collision, registering)
      if (avahi_group_) avahi_entry_group_reset(avahi_group_);
      break;
    case AVAHI_CLIENT_CONNECTING: /* continue */
    case AVAHI_CLIENT_FAILURE:
      // never comes here, ZeroConf class handles this event.
      ;
  }
}

void ZeroConfRegistration::do_start(Thread *thread) {
  if (avahi_client_ == NULL) return;
  RegistrationContext *context = new RegistrationContext(this, avahi_group_, name_, host_);
  
  avahi_group_ = avahi_entry_group_new(avahi_client_, // client
                         s_entry_group_callback,      // calback
                         context);                    // context
  if (avahi_group_ == NULL) {
    fprintf(stderr, "Could not create avahi group (%s).\n", avahi_strerror(avahi_client_errno(avahi_client_)));
    delete context;
    return;
  }
  
  listen(thread);
  
  avahi_entry_group_reset(avahi_group_);
  delete context;
}

} // oscit
