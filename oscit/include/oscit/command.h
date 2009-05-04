#ifndef _OSCIT_BASE_COMMAND_H_
#define _OSCIT_BASE_COMMAND_H_
#include "oscit/thread.h"
#include "oscit/url.h"
#include "oscit/thash.h"

namespace oscit {

class Root;
class Value;
class Object;
#define REMOTE_OBJECTS_HASH_SIZE 10000

/** This class is responsible for listening to any kind of incoming command (implemented by subclasses in do_listen)
 *  and passing these commands to the root object. */
class Command : public Thread
{
 public:
   TYPED("Mutex.Thread.Command")
   
   Command(const char *protocol) : remote_objects_(REMOTE_OBJECTS_HASH_SIZE), root_(NULL), protocol_(protocol) {}

   Command(const std::string &protocol) : remote_objects_(REMOTE_OBJECTS_HASH_SIZE), root_(NULL), protocol_(protocol) {}

   Command(Root *root, const char *protocol) : remote_objects_(REMOTE_OBJECTS_HASH_SIZE), root_(root), protocol_(protocol) {}

   Command(Root *root, const std::string &protocol) : remote_objects_(REMOTE_OBJECTS_HASH_SIZE), root_(root), protocol_(protocol) {}
  
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
   *  The receiver should create an alias for the remote_object (do not keep the pointer).
   *  @param remote_url should contain the full url with protocol and domain: osc://video.local/vid/contrast. */
  Object *remote_object(const Url &remote_url, Value *error);
  
 protected:
  friend class Root;
  /** Should only be used by Root. */
  virtual void set_root(Root *root) { root_ = root; }
   
  /** Build an object to communicate with a remote endpoint. */
  virtual Object *build_remote_object(const Url &remote_url, Value *error) = 0;
  
  /** Run in new thread. */
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
  const std::string  protocol_;
};

} // oscit

#endif // _OSCIT_BASE_COMMAND_H_