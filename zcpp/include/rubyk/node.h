/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_NODE_H_
#define LUBYK_INCLUDE_LUBYK_NODE_H_

#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <list>
//#include <pthread.h>

#include "lubyk/oscit.h"
#include "lubyk/event.h"
#include "lubyk/inlet.h"
#include "lubyk/outlet.h"
#include "lubyk/worker.h"

namespace rk {

class Observer;

/** Base class for all nodes in lubyk. You should *NEVER* create sub-classes of Node with
 * multiple inheritance (cannot cast).
 *
 *  Initialization is done in the following order:
 *  1. create : you should not write constructors.
 *  2. init   : return error messages if this step fails.
 *  2. set    : call methods with parameters, return error messages if this fails.
 *  3. start  : this is called only if init and set did not fail.
 */
class Node : public Object {
 public:
  TYPED("Object.Node")

  Node() : Object("n", Oscit::string_io("Basic Node")), worker_(NULL), looped_(false) {
    // create a key to detect if we are running in an OpenGL thread
    if (!Node::sOpenGLThreadKey) pthread_key_create(&Node::sOpenGLThreadKey, NULL);
  }

  /** Trigger for nodes is just an inspection method.
   */
  virtual const Value trigger(const Value &val) {
    return do_inspect();
  }

  /** Serialize Node into a hash.
   */
  virtual const Value to_hash();

  virtual ~Node();

  const Value do_inspect() const;

  /** This is used as return value / inspect in text command.
   */
  virtual void inspect(Value *hash) const {}

  /** Default method to use on Node creation if the argument is not a hash.
   */
  virtual const char *default_set_key() const {
    return NULL; // no default method
  }

  /** Add an inlet and set the inlet id that is used to sort outlet links.
   */
  void register_inlet(Inlet *inlet) {
    inlet->set_id(inlets_.size()); /* first inlet has id 0 */
    inlets_.push_back(inlet);
  }

  /** Add an outlet. (used by Class during instantiation).
   */
  void register_outlet(Outlet *outlet) {
    outlets_.push_back(outlet);
  }

  /** Remove inlet from inlets list of callbacks.
   */
  void unregister_inlet(Inlet * inlet) {
    std::vector<Inlet*>::iterator it;
    std::vector<Inlet*>::iterator end = inlets_.end();

    for(it = inlets_.begin(); it != end; it++) {
      if (*it == inlet) {
        inlets_.erase(it);
        break;
      }
    }
  }

  /** Remove outlet from outlets list of callbacks.
   */
  void unregister_outlet(Outlet * outlet) {
    std::vector<Outlet*>::iterator it;
    std::vector<Outlet*>::iterator end = outlets_.end();

    for(it = outlets_.begin(); it != end; it++) {
      if (*it == outlet) {
        outlets_.erase(it);
        break;
      }
    }
  }

  /** Sending order from incoming connections should be updated.
   * This is triggered when our position changes.
   */
  void sort_connections();

  /** Set defaults, initialize before parameters are changed by calling
   * methods during runtime.
   * When overwriten by sub-classes, this method must always call the
   * superclass' implementation.
   */
  virtual const Value init() {
    // set Script Widget
    std::cout << url() << "->Node::init()\n";
    attributes_.set(Oscit::VIEW, Oscit::WIDGET, "Node");
    attributes_.set(Oscit::VIEW, Oscit::HUE,    Lubyk::NODE_DEFAULT_HUE);
    attributes_.set(Oscit::VIEW, Oscit::WIDTH,  Lubyk::NODE_DEFAULT_WIDTH);
    attributes_.set(Oscit::VIEW, Oscit::HEIGHT, Lubyk::NODE_DEFAULT_HEIGHT);

    return gNilValue;
  }

  /** When this method is implemented in subclasses, it is used as
   *  the last call in the initialization chain (after parameters are set).
   */
  virtual const Value start() { return gNilValue; }

  /** Set is_ok_ flag. If this flag is not true, the node is considered "broken" and usually does not do any processing. */
  void set_is_ok(bool status) { is_ok_ = status; }

  /** Return true if the node is not broken.
   */
  bool is_ok() const { return is_ok_; }

  /** This method must be implemented in subclasses that want to be looped.
   */
  virtual void bang(const Value &val) {
    fprintf(stderr, "Default Node::bang method called !\n");
  }

  /** Set url for class used to create this Node.
   */
  void set_class_url(const Value &class_url) {
    attributes_.set(Lubyk::CLASS, class_url);
  }

  /** Get url of class object used to create this Node.
   */
  const Value class_url() const {
    return attributes_[Lubyk::CLASS];
  }

  /** Set object from a hash representation.
   */
  virtual const Value set(const Value &hash) {
    Value view = hash[Oscit::VIEW];

    // class update ?

    // update trigger_position
    Value pos;
    if (view.get(Oscit::POS_X, &pos) && pos.is_real()) {
      trigger_position_ = pos.r;
      sort_connections();
    }

    // trigger all normal methods
    return this->Object::set(hash);
  }

  /** Used to sort outlet connections. A node with a high trigger position receives the value before
   * another node with a small trigger position, if they are both connected to the same outlet.
   */
  inline Real pos_x() { return trigger_position_; }

  /** Send a Value out of the first outlet. */
  inline void send(const Value &val) { send(1, val); }

  /** Send a Value out of an outlet. */
  inline void send (size_t port, const Value &val) {
    if (port > outlets_.size() || port < 1) return;
    outlets_[port - 1]->send(val);
  }

  /** Return the outlet at the given port if there is one and
   *  it can receive the provided value.
   */
  Outlet *outlet_for_value(size_t port, const Value &val) {
    if (port > outlets_.size() || port < 1) return NULL;
    Outlet *outlet = outlets_[port - 1];
    return outlet->can_receive(val) ? outlet : NULL;
  }

  /** Remove all events concerning this node for the events queue.
   * FIXME: temporary fix by using current_time(). But we should use some logical time.
   */
  inline void remove_my_events() {
    if (worker_) worker_->free_events_for(this);
  }

  /** Ask to receive a bang in the given interval in [ms]. */
  inline void bang_me_in(time_t interval) {
    worker_->register_event(new BangEvent(this, worker_->current_time() + interval));
  }

  /** Ask to receive a bang with the given parameter in the given interval in [ms].
   * FIXME: temporary fix by using current_time(). But we should use some logical time.
   */
  inline void bang_me_in(time_t interval, const Value &parameter, bool forced = false) {
    worker_->register_event(new BangEvent(this, worker_->current_time() + interval, parameter, forced));
  }

  /** Bang me on every loop. */
  inline void loop_me() {
    if (!looped_) {
      worker_->register_looped_node(this);
      looped_ = true;
    }
  }

  /** Stop banging me on every loop. */
  inline void unloop_me() {
    if (looped_) {
      worker_->free_looped_node(this);
      looped_ = false;
    }
  }

  /** Return true if the current thread runs in an OpenGL context.
   */
  inline bool is_opengl_thread() {
    return pthread_getspecific(sOpenGLThreadKey) != NULL;
  }

  /** This method should be called whenever we open an OpenGL context.
   */
  static void set_is_opengl_thread() {
    pthread_setspecific(sOpenGLThreadKey, (void*)true);
  }

  /** Cast general Mutex context to Worker. */
  virtual void set_context(Mutex *context) {
    // FIXME: what do we do if context is NULL ?? (no more parent)
     // TODO: can we use a static_cast here ?
    context_ = context;
    if (context_ != NULL) {
      worker_  = TYPE_CAST(Worker, context_);
      if (context_ == NULL) {
        fprintf(stderr, "Could not cast '%s' to Worker in %s ! Program might crash.\n", context_->class_path(), class_path());
      }
    }
  }

  /** Key to retrieve 'this' value from a running thread.
   */
  static pthread_key_t sOpenGLThreadKey;

 protected:
  Worker *worker_;  /**< Worker that will give life to object. */

  /** When sending signals from a particular slot, a node with a small pos_.x_ (trigger position)
   * will receive the signal after a node that has a greater trigger position.
   */
  float trigger_position_;
 private:

  bool is_ok_;                   /**< If something bad arrived to the node during initialization or edit, the node goes into
                                  *   broken state and is_ok_ becomes false. In 'broken' mode, the node does nothing. */
  bool looped_;                  /**< Set to true if the node is currently called on every worker loop. */

  std::vector<Inlet*>  inlets_;  /**< List of inlets. This is used to sort outlet sending order on position change. */
  std::vector<Outlet*> outlets_; /**< List of outlets. */
};

} // rk

#endif // LUBYK_INCLUDE_LUBYK_NODE_H_