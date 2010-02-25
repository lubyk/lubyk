/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef RUBYK_INCLUDE_RUBYK_PLANET_H_
#define RUBYK_INCLUDE_RUBYK_PLANET_H_

#include <queue>

#include "rubyk/oscit.h"
#include "rubyk/group.h"
#include "rubyk/text_command.h"
#include "rubyk/worker.h"

class ClassFinder;

#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"
#define RUBYK_DEFAULT_NAME "rubyk"
#define PLANET_VERSION "0.5.0"
#define PLANET_WELCOME "# Welcome to rubyk !\n# version: 0.5.0\n# \n"

/** A planet is just a root with a worker. */
class Planet : public Root
{
 public:
  TYPED("Object.Root.Planet")

  Planet() : Root(RUBYK_DEFAULT_NAME), worker_(this), classes_(NULL), gui_started_(false) {
    init();
  }

  Planet(uint port) : Root(RUBYK_DEFAULT_NAME), worker_(this), classes_(NULL) {
    init();
    open_port(port);
  }

  Planet(int argc, char * argv[]) : Root(RUBYK_DEFAULT_NAME), worker_(this), classes_(NULL), gui_started_(false) {
    // TODO: get port from command line
    init();

    if (argc > 1) {
      std::string file_name(argv[1]);
      set_name(file_name.substr(0, file_name.rfind(".")));
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

  /** FIXME: remove this method: we should use "adopt_command". */
  void open_port(uint port) {
    std::cout << "Command on port " << port << "\n";
    adopt_command(new OscCommand("oscit", "_oscit._udp", port));
  }

  void start_worker() {
    worker_.start();
  }

  /** Only used with direct loop control. */
  void should_run(bool should_run) {
    worker_.should_run(should_run);
  }

  void join() {
    worker_.join();
  }

  /** Blocking call to use instead of 'join' when we want to reserve the main
   * thread for objects that may need a GUI event loop to handle window
   * events.
   */
  void wait_for_gui();

  /** Used for testing.
   */
  bool loop() {
    return worker_.loop();
  }

  time_t current_time() {
    return worker_.current_time_;
  }

  inline Worker *worker() { return &worker_; }

  /** Used to access '/class' when rko objects are loaded. */
  ClassFinder *classes() { return classes_; }

  /** Create/remove a link between two slots. */
  const Value link(const Value &val);

  /** Tell worker to stop.
   */
  const Value quit(const Value &val);

  /** Calls 'inspect' on a node. */
  const Value inspect(const Value &val);

  /** Should be called by objects before the create windows.
   */
  static bool gui_ready();

  /** @internal.
   * Called by NSApp or other event loop on quit.
   */
  virtual void quit();
 private:
  /** Add a pending link. */
  const Value add_pending_link(const Value &val) {
    Value params;
    params.copy(val);
    params[1].set("?");
    pending_links_.push_back(Call(LINK_URL, params));
    return params;
  }

  /** Remove a link from the pending link list. */
  const Value remove_pending_link(const Value &val);

  /** Create pending links. Return a list of created links [[sffs][sffs]...]. */
  const Value create_pending_links();


  /** Create base objects (public for testing, should not be used). */
  void init();

  /** List of pending connections waiting for variable assignements.
   */
  std::list<Call>  pending_links_;

  Worker worker_;
  ClassFinder *classes_;

  /** Semaphore to lock main thread until a sub-thread 'quits' or needs
   * to initialize GUI event loop.
   */
  static Semaphore s_need_gui_semaphore_;

  /** Semaphore to lock sub-thread until a main thread has started the event loop.
   */
  static Semaphore s_start_gui_semaphore_;

  /** Flag to initialize GUI event loop.
   */
  static bool s_need_gui_;

  /** Flag set to true if this instance was used to start the
   * gui event loop.
   */
  bool gui_started_;
};

#endif // RUBYK_INCLUDE_RUBYK_PLANET_H_