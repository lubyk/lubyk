// compile with g++ -g -Wall -lavahi-client -o test registration.cpp

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

// for debugging
struct AvahiSimplePoll;
struct AvahiThreadedPoll {
  AvahiSimplePoll *simple_poll;
  pthread_t thread_id;
  pthread_mutex_t mutex;
  int thread_running;
  int retval;
};

class Registration {
public:
  Registration(const char *name, const char *service_type, uint16_t port) : avahi_poll_(NULL), avahi_client_(NULL), avahi_group_(NULL), port_(port), running_(false) {
    name_ = avahi_strdup(name);
    service_type_ = avahi_strdup(service_type);
    start();
  }

  ~Registration() {
    stop();
    if (avahi_client_) avahi_client_free(avahi_client_);
    if (avahi_poll_) avahi_threaded_poll_free(avahi_poll_);
    avahi_free(name_);
    avahi_free(service_type_);
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
                              Registration::client_callback, // callback
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
                   Registration::entry_avahi_group_callback,  // callback
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
        service_type_,                          // service type
        NULL,                                   // domain
        NULL,                                   // host
        port_,                                  // port
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
                                  service_type_,
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
    Registration *impl = (Registration*)context;

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
    Registration *impl = (Registration*)context;
    // callback called whenever our entry group state changes
    switch (state) {
      case AVAHI_ENTRY_GROUP_ESTABLISHED:
        // done !
        printf("Registration ok: %s:%i (%s)\n", impl->name_, impl->port_, impl->service_type_);
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

  AvahiThreadedPoll *avahi_poll_;
  AvahiClient       *avahi_client_;
  AvahiEntryGroup   *avahi_group_;
  char *name_;
  char *service_type_;
  uint16_t port_;
  bool running_;
};


int main(int argc, char** argv)
{
  printf("Started\n");
  Registration *reg;
  for (int i=0; i < 4; ++i) {
    reg = new Registration("foobar", "_printer._tcp", 515);
    sleep(1);
    delete reg;
  }
	return 0;
}
