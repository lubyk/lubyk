#ifndef _PLANET_H_
#define _PLANET_H_
#include "oscit.h"

#include "group.h"
#include "text_command.h"
#include "worker.h"

#include <queue>

class ClassFinder;

#define CLASS_URL  "/class"
#define RUBYK_URL  "/rubyk"
#define LINK_URL   "/rubyk/link"
#define UNLINK_URL "/rubyk/unlink"
#define QUIT_URL   "/rubyk/quit"

#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

/** A planet is just a root with a worker. */
class Planet : public Root
{
 public:
  TYPED("Object.Root.Planet")
  
  Planet() : worker_(this), classes_(NULL) {
    init();
  }
  
  Planet(uint port) : worker_(this), classes_(NULL) {
    init();
    open_port(port);
  }
  
  Planet(int argc, char * argv[]) : worker_(this), classes_(NULL) {
    // TODO: get port from command line
    init();
    
    if (argc > 1) {
      std::ifstream in(argv[1], std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
      in.close();

      TextCommand * command = adopt_command(new TextCommand(std::cin, std::cout), false);
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
    // wait for worker to finish
    worker_.join();
  }
  
  void quit() {
    worker_.kill();
    clear(); // kill commands and destroy objects
  }
  
  inline Worker *worker() { return &worker_; }
  
  /** Used to access '/class' when rko objects are loaded. */  
  ClassFinder *classes() { return classes_; }
  
  /** Create a new link between two slots. */
  const Value link(const Value &val);

  /** Remove a link between two slots. */
  const Value unlink(const Value &val);
  
  const Value quit(const Value &val) {
    lock();
      worker_.register_event<Planet, &Planet::do_quit>(200, this, gNilValue);
    unlock();
    return gNilValue;
  }
  
  /** Triggered by quit event. */
  void do_quit(const Value &val) {
    should_run_
  }
  
  /** Create pending links. Return a list of created links [[sffs][sffs]...]. */
  const Value create_pending_links();
  
 private:
  /** Create base objects (public for testing, should not be used). */
  void init();
  
  std::list<oscit::Call>  pending_links_;        /**< List of pending connections waiting for variable assignements. */
  Worker worker_;
  ClassFinder *classes_;
};

#endif // _PLANET_H_