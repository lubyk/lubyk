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

#include "mdns/Registration.h"
#include "mdns/Context.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/strlst.h>

#include <unistd.h> // pipe
#include <fcntl.h>

namespace mdns {

class Registration::Implementation {
  Registration *master_;
  AvahiClient *client_;
  AvahiEntryGroup *avahi_group_;
  /** Our current name (allocated by avahi_strdup)
   */
  char *name_;
  /** A simple pipe to keep same API as macosx.
   */
  int pipe_fd_[2];
public:
  Implementation(Registration *master, Context *ctx)
    : master_(master)
    , client_(NULL)
    , avahi_group_(NULL)
    , name_(NULL)
  {
    client_ = (AvahiClient*)ctx->context();
    // The Registration object will be added to the context from Lua.
    resetName();
    // setup the pipe
    if (pipe(pipe_fd_) == -1) {
      throw dub::Exception("Could not setup pipe (%s).", strerror(errno));
    }
    
    // Set pipe fd as beeing NON-BLOCKING
    fcntl(pipe_fd_[0], F_SETFL, fcntl(pipe_fd_[0], F_GETFL) | O_NONBLOCK);
    fcntl(pipe_fd_[1], F_SETFL, fcntl(pipe_fd_[1], F_GETFL) | O_NONBLOCK);
  }

  ~Implementation() {
    stop();
    avahi_free(name_);
  }

  int fd() {
    return pipe_fd_[0];
  }

  void resetName() {
    if (name_) avahi_free(name_);
    name_ = avahi_strdup(master_->name_.c_str());
  }

  void nextName() {
    char *new_name = avahi_alternative_service_name(name_);
    avahi_free(name_);
    name_ = new_name;
  }

  bool getService() {
    char read_buffer[2];
    // read a single char
    read(pipe_fd_[0], read_buffer, 1);
    return true;
  }


  /** Client ready, register.
   */
  void start() {
    int error;

    if (avahi_group_ == NULL) {
      avahi_group_ = avahi_entry_group_new(client_,   // client
                   sGroupCallback,                    // callback
                   this);                             // context

      if (!avahi_group_) {
        throw dub::Exception("Could not create avahi group (%s).\n",
            avahi_strerror(avahi_client_errno(client_)));
      }
    }

    if (avahi_entry_group_is_empty(avahi_group_)) {
      size_t txt_len = master_->txt_.size();
      const char *txt = master_->txt_.c_str();
      AvahiStringList *list = NULL;
      if (txt_len) {
        for(size_t i = 0; i < txt_len;) {
          size_t len = txt[i];
          const unsigned char *text = (unsigned char*)txt+i+1;
          list = avahi_string_list_add_arbitrary(list, text, len);
          i += len + 1;
        }
      }

      // new group or after reset
      error = avahi_entry_group_add_service_strlst(avahi_group_,  // group
        AVAHI_IF_UNSPEC,                  // interface
        AVAHI_PROTO_UNSPEC,               // protocol to announce service with
        (AvahiPublishFlags)0,             // flags
        name_,                            // name
        master_->service_type_.c_str(),   // service type
        NULL,                             // domain
        NULL,                             // host
        master_->port_,                   // port
        list                              // AvahiStringList containing txt rec.
        );

      if (list) {
        avahi_string_list_free(list);
      }

      if (error < 0) {
        // error
        if (error == AVAHI_ERR_COLLISION) {
          // collision with local service name
          nextName();
          avahi_entry_group_reset(avahi_group_);
          // retry
          start();
        } else {
          // Other error
          throw dub::Exception("Could not add '%s' with type '%s' (%s).",
                                  name_,
                                  master_->service_type_.c_str(),
                                  avahi_strerror(error));
        }
      }

      // start registering the service
      error = avahi_entry_group_commit(avahi_group_);
      if (error < 0) {
        throw dub::Exception("Could not commit avahi group '%s' (%s).", name_, avahi_strerror(error));
      }
    }
  }

  void stop() {
    if (avahi_group_) {
      avahi_entry_group_reset(avahi_group_);
    }
  }

  static void sGroupCallback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *userdata) {
    Implementation *impl = (Implementation*)userdata;
    // callback called whenever our entry group state changes
    switch (state) {
      case AVAHI_ENTRY_GROUP_ESTABLISHED:
        // done !
        impl->master_->name_ = impl->name_;
        impl->master_->host_ = avahi_client_get_host_name(impl->client_);
        // Write a single byte into our pipe so that the
        // scheduler halts by. This is used to keep the same
        // fd based interface as with macosx's version.
        write(impl->pipe_fd_[1], "", 1);
        break;
      case AVAHI_ENTRY_GROUP_COLLISION:
        // build new name
        impl->nextName();
        // TODO: Not sure this is needed (it's not in the example).
        avahi_entry_group_reset(impl->avahi_group_);
        // retry
        impl->start();
        break;
      case AVAHI_ENTRY_GROUP_FAILURE:
        throw dub::Exception("Registration failure (%s).",
                        avahi_strerror(avahi_client_errno(impl->client_)));
        break;
      case AVAHI_ENTRY_GROUP_UNCOMMITED: /* continue */
      case AVAHI_ENTRY_GROUP_REGISTERING:
        // ignore
        ;
    }
  }
};

Registration::Registration(
    Context *ctx,
    const char *service_type,
    const char *name,
    int port,
    const char *txt)
    : name_(name), service_type_(service_type), port_(port), txt_(txt), fd_(0) {
  impl_ = new Registration::Implementation(this, ctx);
  fd_ = impl_->fd();
}

Registration::~Registration() {
  delete impl_;
}

bool Registration::getService() {
  return impl_->getService();
}

void Registration::start() {
  impl_->start();
}

void Registration::stop() {
  impl_->stop();
}

} // mdns

