/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/command.h"

#include <string>
#include <iostream>
#include <list>

#include "oscit/root.h"
#include "oscit/object.h"
#include "oscit/zeroconf_registration.h"
#include "oscit/root_proxy.h"

namespace oscit {

#define REMOTE_OBJECTS_HASH_SIZE 10000
#define ROOT_PROXY_HASH_SIZE     100
#define OBSERVERS_HASH_SIZE      100
#define DEFAULT_TTL              240 // 4 minutes

Command::Command(const char *protocol) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                port_(0),
                                protocol_(protocol),
                                service_type_(""),
                                zeroconf_registration_(NULL),
                                root_proxies_(ROOT_PROXY_HASH_SIZE),
                                observers_(OBSERVERS_HASH_SIZE) {}

Command::Command(const char *protocol, const char *service_type, uint16_t port) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                port_(port),
                                protocol_(protocol),
                                service_type_(service_type),
                                zeroconf_registration_(NULL),
                                root_proxies_(ROOT_PROXY_HASH_SIZE),
                                observers_(OBSERVERS_HASH_SIZE) {}

Command::Command(Root *root, const char *protocol) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                port_(0),
                                protocol_(protocol),
                                service_type_(""),
                                zeroconf_registration_(NULL),
                                root_proxies_(ROOT_PROXY_HASH_SIZE),
                                observers_(OBSERVERS_HASH_SIZE) {}

Command::Command(Root *root, const char *protocol, const char *service_type, uint16_t port) :
                                remote_objects_(REMOTE_OBJECTS_HASH_SIZE),
                                root_(NULL),
                                port_(port),
                                protocol_(protocol),
                                service_type_(service_type),
                                zeroconf_registration_(NULL),
                                root_proxies_(ROOT_PROXY_HASH_SIZE),
                                observers_(OBSERVERS_HASH_SIZE) {}

Command::~Command() {
  kill();
  if (zeroconf_registration_ != NULL) delete zeroconf_registration_;

  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
  string_iterator it;
  string_iterator end = remote_objects_.end();

  // destroy all references to remote objects
  for(it = remote_objects_.begin(); it != end; ++it) {
    Object * object;
    if (remote_objects_.get(*it, &object)) {
      delete object;
    }
  }

  // destroy all references to remote objects
  std::list<Location>::const_iterator p_it;
  std::list<Location>::const_iterator p_end = root_proxies_.end();
  for(p_it = root_proxies_.begin(); p_it != p_end; ++p_it) {
    RootProxy *root_proxy;
    if (root_proxies_.get(*p_it, &root_proxy)) {
      root_proxy->unlink_command();
      delete root_proxy;
    }
  }
}

Object *Command::remote_object(const Url &remote_url, Value *error) {
  Object *object;
  if (remote_objects_.get(remote_url.str(), &object)) {
    return object;
  } else {
    return build_remote_object(remote_url, error);
  }
}

void Command::publish_service() {
  if (zeroconf_registration_ == NULL && service_type_ != "") {
    std::string name(root_->name());
    if (name == "") {
      std::cerr << "## Cannot publish service '" << service_type_ << "': root name missing !\n";
      return;
    }
    zeroconf_registration_ = new ZeroConfRegistration(name.c_str(), service_type_.c_str(), port_);
  }
}

void Command::receive(const Url &url, const Value &val) {
  if (!handle_reply_message(url, val) && !handle_register_message(url, val)) {
    Value res = root_->call(url, val, this);

    if (res.is_error()) {
      // only send reply to caller
      send(url.location(), ERROR_PATH, res);
    } else {
      // prepare reply
      Value reply(url.path());
      reply.push_back(res);
      if (url.is_meta()) {
        // only send to caller
        send(url.location(), REPLY_PATH, reply);
      } else {
        // notify all
        root_->notify_observers(REPLY_PATH, reply, this);
      }
    }
  }
}

/** Add a new satellite to the list of observers. This method is whenever we receive messages from
 * a given location.
 */
bool Command::handle_register_message(const Url &url, const Value &val) {
  if (url.path() == REGISTER_PATH) {
    if (val.is_real()) {
      // other port
      Location remote(url.location());
      remote.set_port((uint)val.r);
      observers_.set(remote, DEFAULT_TTL);
    } else {
      observers_.set(url.location(), DEFAULT_TTL);
    }
    return true;
  } else {
    return false;
  }
}

bool Command::handle_reply_message(const Url &url, const Value &val) {
  if (url.path() == REPLY_PATH) {
    RootProxy *proxy = find_proxy(url.location());
    if (proxy) {
      if (val.size() < 2 || !val[0].is_string()) {
        std::cerr << "Bad argument to " << REPLY_PATH << " :" << val << "\n";
        return true;
      }
      proxy->handle_reply(val[0].str(), val[1]);
    } else {
      // std::cerr << "Could not handle reply from '" << url.location() << "' (no proxy for this location).\n";
    }
    return true;
  } else {
    return false;
  }
}

void Command::register_proxy(RootProxy *proxy) {
  // 1. make sure it is not in dictionary
  unregister_proxy(proxy);

  // 2. add to dictionary
  root_proxies_.set(proxy->remote_location(), proxy);
  root_proxies_vector_.push_back(proxy);
}

void Command::unregister_proxy(RootProxy *proxy) {
  root_proxies_.remove_element(proxy);
  std::vector<RootProxy*>::iterator it, end = root_proxies_vector_.end();
  for(it = root_proxies_vector_.begin(); it != end; ++it) {
    if (*it == proxy) {
      it = root_proxies_vector_.erase(it);
    }
  }
}

RootProxy *Command::find_proxy(const Location &location) {
  RootProxy *found = NULL;
  root_proxies_.get(location, &found);
  return found;
}
} // oscit
