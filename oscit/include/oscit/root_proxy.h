/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_

#include "oscit/root.h"

namespace oscit {

/** This class helps maintain a 'ghost' tree that mirrors a remote 'real' tree. It is usually used
 * as an element from an interface to interact with some remote device.
 */
class RootProxy : Root {
public:
  /** Create a proxy from a remote end_point. Do not build meta methods.
   */
  RootProxy(IpEndPoint *end_point) : Root(false) : command_(NULL) {}

  virtual ~RootProxy() {
    // unregister from command_
    set_command(NULL);
  }

  /** This method is used to send a message to the remote tree.
   */
  virtual void send_to_remote(const std::string &path, const Value &val) {}

  /** Keep proxy in sync by parsing replies and sending new queries.
   */
  virtual void handle_list_reply(const std::string &path, const Value &val) {
    ObjectProxy *find_or_build_object_proxy_at(path);
  }

  /** Set proxy's new command (communication channel).
   *  this method registers the RootProxy as into the given command.
   */
  void set_command(Command *command);

private:

  /** Get an object proxy at a defined url.
   */
  ObjectProxy *find_or_build_object_proxy_at(const std::string &path) {
    Object * object = object_at(path);

    if (object == NULL) {
      // build object
      ObjectProxy *object ..
      size_t pos = path.rfind("/");
      if (pos != std::string::npos) {
        // "/foo/bar" ==> "/foo/bar", "/foo", ""
        // find or build parent
        Object * parent = find_or_build_object_proxy_at(path.substr(0, pos));
        parent->adopt(build_object_proxy(path.substr(pos)))
      } else {
        // root object "foo"... error ?
      }
    }
    return object;
  }

  /** Reference to the original tree this root proxies. When the RootProxy is adopted
   *  by a command, this is used as key to route 'reply' messages.
   */
  IpEndPoint *end_point_;

  /** This is the link to the original 'remote'. It is used to send queries to the
   *  remote tree.
   */
  Command *command_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_