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
    detach();
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

  /** Set proxy's factory (used to create new object proxies).
   *  this method registers the RootProxy into the given factory.
   */
  void set_proxy_factory(ProxyFactory *factory);

  /** Returns the location of the original tree this proxy mirrors.
   */
  const Location &remote_location() const {
    return remote_location_;
  }

  /** Dynamically build a child from the given name. We build dummy object proxies
   * that will try to get a "type" from the remote end.
   */
  virtual Object *build_child(const std::string &name, Value *error);

  ProxyFactory *proxy_factory() {
    return proxy_factory_;
  }

  /** IF the RootProxy was adopted by a Command, this removes the link
   * making it an orphan.
   *
   * Used by ZeroConfBrowser before deletion.
   */
  void detach() {
    set_command(NULL);
  }

private:
  friend class Command; // set_command

  /** Set proxy's command (communication channel).
   *  this method registers the RootProxy into the given command.
   */
  void set_command(Command *command);

  /** Called by ~Command to avoid further 'unregister' calls.
   */
  void unlink_command() {
    command_ = NULL;
  }

  void build_children_from_types(Object *base, const Value &types);

  /** Get information on the direct children of the remote root.
   */
  void sync_children() {
    send_to_remote(LIST_WITH_TYPE_PATH, Value(url()));
  }

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