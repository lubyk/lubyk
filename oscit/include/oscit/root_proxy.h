/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_

#include "oscit/root.h"
#include "oscit/location.h"

namespace oscit {

class ObjectProxy;
class ProxyFactory;

/** This class helps maintain a 'ghost' tree that mirrors a remote 'real' tree. It is usually used
 * as an element from an interface to interact with some remote device.
 */
class RootProxy : public Root {
public:

  /** Class signature. */
  TYPED("Object.Root.RootProxy")

  /** Create a proxy from a remote end_point. Do not build meta methods.
   */
  RootProxy(const Location &remote_location) :
            Root(false), remote_location_(remote_location),
            command_(NULL), proxy_factory_(NULL) {}

  RootProxy(const Location &remote_location, ProxyFactory *proxy_factory) :
            Root(false), remote_location_(remote_location),
            command_(NULL), proxy_factory_(NULL) {
    set_proxy_factory(proxy_factory);
  }

  virtual ~RootProxy() {
    // unregister from command_
    set_command(NULL);
  }

  /** This method is used to send a message to the remote tree.
   */
  void send_to_remote(const char *path, const Value &val) {
    if (command_) {
      command_->send(remote_location_, path, val);
    }
  }

  /** Keep proxy in sync by parsing replies and sending new queries.
   */
  void handle_reply(const std::string &path, const Value &val);

  /** Set proxy's command (communication channel).
   *  this method registers the RootProxy into the given command.
   */
  void set_command(Command *command);

  /** Called by ~Command to avoid further 'unregister' calls.
   */
  void unlink_command() {
    command_ = NULL;
  }

  /** Set proxy's factory (used to create new object proxies).
   *  this method registers the RootProxy into the given factory.
   */
  void set_proxy_factory(ProxyFactory *factory);

  /** Returns the location of the original tree this proxy mirrors.
   */
  const Location &remote_location() {
    return remote_location_;
  }

private:

  void handle_list_with_type_reply(ObjectProxy *target, const Value &children);

  /** Get information on the direct children of the remote root.
   */
  void sync() {
    send_to_remote(LIST_WITH_TYPE_PATH, Value(url()));
  }

  /** Get an object proxy at a defined url.
   */
  // ObjectProxy *find_or_build_object_proxy_at(const std::string &path) {
  //   Object * object = object_at(path);
  //
  //   if (object == NULL) {
  //     // build object
  //     ObjectProxy *object ..
  //     size_t pos = path.rfind("/");
  //     if (pos != std::string::npos) {
  //       // "/foo/bar" ==> "/foo/bar", "/foo", ""
  //       // find or build parent
  //       Object * parent = find_or_build_object_proxy_at(path.substr(0, pos));
  //       parent->adopt(build_object_proxy(path.substr(pos)))
  //     } else {
  //       // root object "foo"... error ?
  //     }
  //   }
  //   return object;
  // }

  /** Reference to the original tree this root proxies. When the RootProxy is adopted
   *  by a command, this is used as key to route 'reply' messages.
   */
  Location remote_location_;

  /** This is the link to the original 'remote'. It is used to send queries to the
   *  remote tree.
   */
  Command *command_;

  /** The link to the proxy factory is used by the RootProxy when it needs to create
   *  new ObjectProxies. */
  ProxyFactory *proxy_factory_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_