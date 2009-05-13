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

#define REMOTE_OBJECTS_HASH_SIZE 10000

/** This class is responsible for listening to any kind of incoming command (implemented by subclasses in do_listen)
 *  and passing these commands to the root object. */
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

  void listen() {
    if (root_ == NULL) {
      fprintf(stderr, "Impossible to start command (no access to root).");
      return;
    }

    start<Command, &Command::do_listen>(this);
  }

  const std::string &protocol() { return protocol_; }

  /** Send a notification to all observers of this command. */
  virtual void notify_observers(const char *url, const Value &val) {}

  /** Returns a pointer to an object that can be used to send values to a remote object.
   * The receiver should create an alias for the remote_object (do not keep the pointer).
   * @param remote_url should contain the full url with protocol and domain: osc://video.local/vid/contrast. */
  Object *remote_object(const Url &remote_url, Value *error);
  
  uint16_t port() { return port_; }

 protected:
  friend class Root;
  
  /** Should be called by sub-classes when they have finished
   * initializing and are ready to go public.
   */
  void publish_service();
  
  /** Should only be used by Root.
   */
  void set_root(Root *root) { root_ = root; }

  /** Build an object to communicate with a remote endpoint. */
  virtual Object *build_remote_object(const Url &remote_url, Value *error) = 0;

  /** Run in new thread.
   */
  virtual void do_listen() = 0;

  template<class T>
  T * adopt_remote_object(const std::string &url, T* object) {
    remote_objects_.set(url, object);
    return object;
  }

  /**< Contains remote_objects (TODO: purge if no alias and not used...).
   */
  THash<std::string, Object*> remote_objects_;

  Root *root_;

 private:
  
  /** Type of protocol this command is responsible for. For example if
   * a command has 'http' protocol, then all urls starting with 'http://' will
   * be forwarded to this command.
   */
  const std::string protocol_;
  
  /** Service type published by this command.
   * If this value is empty, no service will be published.
   */
  const std::string service_type_;
  
  /** Connected port.
   */
  uint16_t port_;
  
  /** Zeroconf registration thread started when 'publish_service()' is called.
   */
  ZeroConfRegistration *zeroconf_registration_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_BASE_COMMAND_H_
