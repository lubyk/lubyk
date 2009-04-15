#ifndef _COMMAND_H_
#define _COMMAND_H_
#include "oscit/thread.h"
#include "oscit/values.h"

namespace oscit {

class Root;
class Value;

/** This class is responsible for listening to any kind of incoming command (implemented by subclasses in do_listen)
 *  and passing these commands to the root object. */
class Command : public Thread
{
 public:
  Command() : root_(NULL) {}
  
  Command(Root *root) : root_(root) {}
  
  virtual ~Command();
  
  void listen() {
    if (root_ == NULL) {
      fprintf(stderr, "Impossible to start command (no access to root).");
      return;
    }
    start_using_signals<Command, &Command::do_listen>();
  }
  
  /** Send a notification to all observers of this command. */
  virtual void notify_observers(const char *url, const Value &val) {}
  
 protected:
  friend class Root;
  
  /** Should only be used by Root. */
  void set_root(Root *root) { root_ = root; }
  
  /** Run in new thread. */
  virtual void do_listen() = 0;
  
  Root *root_;
};

} // oscit

#endif // _COMMAND_H_