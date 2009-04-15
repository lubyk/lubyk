#ifndef _PLANET_H_
#define _PLANET_H_
#include "oscit.h"

#include "group.h"
#include "text_command.h"
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

      TextCommand  * command = new TextCommand(std::cin, std::cout);
      command->set_planet(this);
      command->set_silent();
      oss << "\n";
      command->parse(oss.str());
      delete command;
    }
  }
  
  void open_port(uint port) {
    adopt_command(new OscCommand(port));
  }
  
  void run() {
    worker_.run();
  }
  
  void quit() {
    worker_.kill();
    clear(); // kill commands and destroy objects
  }
  
  inline Worker *worker() { return &worker_; }
  
  /** Return the class finder (create one if needed). */
  ClassFinder * classes();
  
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
  const Value create_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_node);

  /** Remove a link between two slots. */
  const Value remove_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_node);
 
 private:
  void init() {
    // force build of "/class"
    classes();
  }
  
  std::list<oscit::Call>  pending_links_;        /**< List of pending connections waiting for variable assignements. */
  Worker worker_;
};

#endif // _PLANET_H_