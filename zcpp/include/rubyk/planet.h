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

namespace rk {

class ClassFinder;

#define DEFAULT_OBJECTS_LIB_PATH "lib:~/rubyk/lib:/usr/local/lib/rubyk"
#define RUBYK_DEFAULT_NAME "rubyk"
#define PLANET_VERSION "0.5.0"
#define PLANET_WELCOME "# Welcome to rubyk !\n# version: 0.5.0\n# \n"

/** A planet is just a root with a worker and CRUD operations to update
 * a patch from Hash based changes. The Create, Update and Delete
 * operations are all implemented through the 'update' method. Read is
 * implemented with 'patch' method. The CRUD operations are usually triggered
 * from the alias '/views/patch' and '/views/patch/update'.
 */
class Planet : public Root {
 public:
  TYPED("Object.Root.Planet")

  Planet() : Root(RUBYK_DEFAULT_NAME), worker_(this), classes_(NULL), gui_started_(false) {
    init();
  }

  Planet(uint port) : Root(RUBYK_DEFAULT_NAME), worker_(this), classes_(NULL), gui_started_(false) {
    init();
    open_port(port);
  }

  static const float NEW_NODE_POS_X_DELTA = 30.0;
  static const float NEW_NODE_POS_Y_DELTA = 30.0;

  /** FIXME: remove this method: we should use "adopt_command". */
  void open_port(uint port) {
    std::cout << "Command on port " << port << "\n";
    adopt_command(new OscCommand("oscit", OSCIT_SRV_TYPE, port));
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
  void ready();

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

  /** Overwride expose_views to insert our 'patch' pseudo-view.
   */
  virtual bool expose_views(const std::string &path, Value *error);

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

  /** Update the patch from a hash representation (by doing a deep merge). If the value is nil,
   * return an empty hash.
   */
  virtual const Value set(const Value &hash);

  /** Build a hash representation of the whole system (patch for serialization).
   * Only include attributes and Nodes.
   */
  virtual const Value to_hash();

  /** Set/get the full patch from a hash representation. If the value is nil,
   * return the full view.
   */
  const Value patch(const Value &hash);

 private:
  /** Create a new node from a Hash definition.
   */
  void create_node(const std::string &name, const Value &class_url, const Value &params, Value *result);

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

} // rk

#endif // RUBYK_INCLUDE_RUBYK_PLANET_H_