#ifndef _PLANET_H_
#define _PLANET_H_
#include "oscit.h"

#include "group.h"
#include "command.h"
#include "worker.h"

#include <queue>

class ClassFinder;

#define CLASS_ROOT "/class"
#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

/** A planet is just a root with a worker. */
class Planet : public Root
{
 public:
  
  Planet() : worker_(this) {
    init();
  }
  
  Planet(uint port) : worker_(this) {
    init();
    open_port(port);
  }
  
  Planet(int argc, char * argv[]) : worker_(this) {
    // TODO: get port from command line
    init();
    
    if (argc > 1) {
      std::ifstream in(argv[1], std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
      in.close();

      Command  * command = new Command(std::cin, std::cout);
      command->set_worker(&worker_);
      command->set_silent(true);
      oss << "\n";
      command->parse(oss.str());
      command->close();
      delete command;
    }
  }
  
  virtual ~Planet();
  
  void run() {
    worker_.run();
  }
  
  /** Return the class finder (create one if needed). */
  ClassFinder * classes();
  
  /** Start listening to a command. */
  void listen_to_command (Command &commend);
  
  /** Create pending links (called on new object creation). */
  const Value create_pending_links();
  
  /** ======== Helpers for commands (builds OSC calls). ======== */
  
  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  const Value new_object(const char *url, const char *class_name, const Value &params);

  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  inline const Value new_object(const std::string &url, const std::string &class_name, const Value &params) {
    return new_object(url.c_str(), class_name.c_str(), params);
  }

  /** Create a new link between two slots. */
  const Value create_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_object);

  /** Remove a link between two slots. */
  const Value remove_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_object);
 
 public:
  std::list<oscit::Call>  pending_links_;        /**< List of pending connections waiting for variable assignements. */
  
 private:
  void init() {
    // force build of "/class"
    classes();
  }
  
  std::queue<Command *>   commands_;      /**< Command line / editors. FIXME: it seems these should live in oscit space ... (not sure). */
  Worker worker_;
};

#endif // _PLANET_H_