#ifndef OSCIT_INCLUDE_OSCIT_BASE_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_BASE_COMMAND_H_
#include "oscit/thread.h"
#include "oscit/url.h"
#include "oscit/thash.h"

namespace oscit {

class Root;
class Value;
class Object;
class ZeroConfRegistration;
class RootProxy;

#define REMOTE_OBJECTS_HASH_SIZE 10000
#define ROOT_PROXY_HASH_SIZE     100

/** This class is responsible for listening to any kind of incoming command (implemented by subclasses in do_listen)
 *  and passing these commands to the root object.
 *
 * If you do not want the command to publish a service, set service_type to "" (blank).
 */
class Command : public Thread
{
 public:
  TYPED("Mutex.Thread.Command")

  Command(const char *protocol);

  Command(const char *protocol, const char *service_type, uint16_t port);

  Command(Root *root, const char *protocol);

  Command(Root *root, const char *protocol, const char *service_type, uint16_t port);

  virtual ~Command();

  virtual void kill() {
    this->Thread::kill();
  }

  /** Start the command in a new thread.
   */
  void start_command() {
    if (root_ == NULL) {
      fprintf(stderr, "Impossible to start command (no access to root).");
      return;
    }

    start_thread<Command, &Command::listen>(this);
  }

  const std::string &protocol() { return protocol_; }

  /** Send a notification to all observers of this command. */
  virtual void notify_observers(const char *path, const Value &val) = 0;

  /** Send a message to a specific path on a remote end point.
   */
  virtual void send(const Location &remote_endpoint, const char *path, const Value &val) = 0;

  /** Returns a pointer to an object that can be used to send values to a remote object.
   *  The receiver should create an alias for the remote_object (do not keep the pointer).
   *  @param remote_url should contain the full url with protocol and domain: osc://video.local/vid/contrast. */
  Object *remote_object(const Url &remote_url, Value *error);

  uint16_t port() { return port_; }

  /** Adopt a new RootProxy and start using it to route messages for its
   *  endpoint identifier.
   */
  template<class T>
  T * adopt_proxy(T * proxy) {
    proxy->set_command(this); // This will trigger 'register_proxy'.
    return proxy;
  }
  
  RootProxy *find_proxy(const Location &location);

 protected:
  friend class Root;       // set_root
  friend class RootProxy;  // register_proxy, unregister_proxy

  /** We have just adopted this proxy: start routing 'reply' messages to it.
   */
  void register_proxy(RootProxy *proxy);

  /** Forget about this proxy: stop sending content of 'reply' messages to it.
   */
  void unregister_proxy(RootProxy *proxy);

  /** Should be called by sub-classes when they have finished
   *  initializing and are ready to go public.
   */
  void publish_service();

  /** Should only be used by Root.
   */
  void set_root(Root *root) { root_ = root; }

  /** Build an object to communicate with a remote endpoint.
   *  FIXME: maybe we do not need this since we have RootPoxy...
   */
  virtual Object *build_remote_object(const Url &remote_url, Value *error) = 0;

  /** Listen for commands (in new thread). Should call "thread_ready()" when it
   *  has finished initializing.
   */
  virtual void listen() = 0;

  /** FIXME: maybe we do not need this since we have RootPoxy...
   */
  template<class T>
  T * adopt_remote_object(const std::string &path, T* object) {
    remote_objects_.set(path, object);
    return object;
  }

  /** Contains remote_objects (TODO: purge if no alias and not used...).
   *  FIXME: maybe we do not need this since we have RootPoxy...
   */
  THash<std::string, Object*> remote_objects_;

  Root *root_;

  /** Connected port.
   */
  uint16_t port_;

 private:

  /** Type of protocol this command is responsible for. For example if
   *  a command has 'http' protocol, then all urls starting with 'http://' will
   *  be forwarded to this command.
   */
  const std::string protocol_;

  /** Service type published by this command.
   *  If this value is empty, no service will be published.
   */
  const std::string service_type_;

  /** Zeroconf registration thread started when 'publish_service()' is called.
   */
  ZeroConfRegistration *zeroconf_registration_;

  /** Route for 'reply' messages which are sent to the proxy identified
   *  by the IP endpoint (origin of the message).
   */
  THash<Location, RootProxy*> root_proxies_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_BASE_COMMAND_H_
