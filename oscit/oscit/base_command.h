#ifndef _OSCIT_BASE_COMMAND_H_
#define _OSCIT_BASE_COMMAND_H_
#include "oscit/thread.h"
#include "oscit/url.h"

namespace oscit {

class Root;
class Value;
class BaseObject;

/** This class is responsible for listening to any kind of incoming command (implemented by subclasses in do_listen)
 *  and passing these commands to the root object. */
class BaseCommand : public Thread
{
 public:
  BaseCommand(const char *protocol);
  BaseCommand(const std::string &protocol);
  BaseCommand(Root *root, const char *protocol);
  BaseCommand(Root *root, const std::string &protocol);
  
  virtual ~BaseCommand();
  
  virtual void kill() {
    this->Thread::kill();
  }
  
  void listen() {
    if (root_ == NULL) {
      fprintf(stderr, "Impossible to start command (no access to root).");
      return;
    }
    start<BaseCommand, &BaseCommand::do_listen>(this);
  }
  
  const std::string &protocol() { return protocol_; }
  
  /** Send a notification to all observers of this command. */
  virtual void notify_observers(const char *url, const Value &val) {}
  
  /** Returns a pointer to an object that can be used to send values to a remote object.
   *  The receiver should create an alias for the remote_object (do not keep the pointer).
   *  @param remote_url should contain the full url with protocol and domain: osc://video.local/vid/contrast. */
  BaseObject *remote_object(const Url &remote_url);
  
 protected:
  /** Build an object to communicate with a remote endpoint. */
  virtual BaseObject *build_remote_object(const Url &remote_url) = 0;

  friend class Root;
  
  /** Should only be used by Root. */
  void set_root(Root *root) { root_ = root; }
  
  /** Run in new thread. */
  virtual void do_listen() = 0;
  
  Root              *root_;
  const std::string  protocol_;
  Root              *remote_objects_; /**< Contains remote_objects (TODO: purge if no alias and not used...). */
};

} // oscit

#endif // _OSCIT_BASE_COMMAND_H_