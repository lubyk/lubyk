/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_OBJECT_H_
#define OSCIT_INCLUDE_OSCIT_OBJECT_H_

#include <stdlib.h> // atoi
#include <list>
#include <string>

#include "oscit/typed.h"
#include "oscit/values.h"
#include "oscit/thash.h"
#include "oscit/mutex.h"

namespace oscit {
#define OSC_NEXT_NAME_BUFFER_SIZE 20

#define NoIO(info)  Value(info)
#define NilIO(info)  ListValue("N").push_back(info)
#define RealIO(units, info) Value(0.0).push_back(units).push_back(info)
#define StringIO(units, info) Value("").push_back(units).push_back(info)
#define RangeIO(min, max, units, info) \
  Value(0.0).push_back(min).push_back(max).push_back(units).push_back(info)
#define SelectIO(values, units, info) \
  Value("").push_back(values).push_back(units).push_back(info)
#define HashIO(info) Value('H').push_back(info)
#define MatrixIO(rows, cols, info) MatrixValue(rows, cols).push_back(info)
#define MidiIO(info) MidiValue().push_back(info)
#define AnyIO(info) Value('*').push_back(info)

#define DEFAULT_TYPE NoIO("No information on this node.")

class Root;
class Alias;

class Object : public Typed {
 public:
  /** Class signature. */
  TYPED("Object")

  explicit Object() : root_(NULL), parent_(NULL), children_(20), context_(NULL),
    type_(DEFAULT_TYPE) {
    type_changed();
    name_ = "";
    url_  = name_;
  }

  explicit Object(const char *name) : root_(NULL), parent_(NULL), children_(20),
    name_(name), url_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
  }

  explicit Object(const std::string &name) : root_(NULL), parent_(NULL),
    children_(20), name_(name), url_(name), context_(NULL),
    type_(DEFAULT_TYPE) {
    type_changed();
  }

  explicit Object(const Value &type) : root_(NULL), parent_(NULL),
    children_(20), context_(NULL), type_(type) {
    type_changed();
    name_ = "";
    url_  = name_;
  }

  Object(const char *name, const Value &type) : root_(NULL), parent_(NULL),
    children_(20), name_(name), url_(name), context_(NULL), type_(type) {
    type_changed();
  }

  Object(const std::string &name, const Value &type) : root_(NULL),
    parent_(NULL), children_(20), name_(name), url_(name), context_(NULL),
    type_(type) {
    type_changed();
  }

  Object(Object *parent, const char *name) : root_(NULL), parent_(NULL),
    children_(20), name_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
    parent->adopt(this);
  }

  Object(Object *parent, const char *name, const Value &type) : root_(NULL),
    parent_(NULL), children_(20), name_(name), context_(NULL), type_(type) {
    type_changed();
    parent->adopt(this);
  }

  Object(Object *parent, const std::string &name, const Value &type) :
    root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL),
    type_(type) {
    type_changed();
    parent->adopt(this);
  }

  virtual ~Object();

  /** Shortcut to call multiple methods on an object.
    * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling
    *            "obj.tempo(45)" and "obj.rubato(1.5)".
    * @return    a hash with the result for each call.
    */
  const Value set(const Value &val);

  /** Shortcut to call multiple methods on an object.
    * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling
    *            "obj.tempo(45)" and "obj.rubato(1.5)".
    * @return true on success, false if any call failed.
    */
  bool set_all_ok(const Value &val);

  /** This is the prefered way to insert new objects in the tree since it clearly
    * highlights ownership in the parent.
    * TODO: make sure a parent is not adopted by it's child. */
  template<class T>
  T * adopt(T * object) {
    object->set_parent(this);
    // FIXME: remove 'moved()' when tests pass: already called in 'set_parent'
    object->moved();
    return object;
  }

  /** Clear all children (delete). */
  void clear();

  /** Lock mutex if needed before calling 'trigger'.
   * This is the method that should be used by objects when they are doing a
   * direct call.
   */
  const Value safe_trigger(const Value &val, const Mutex *caller_context) {
    if (context_ && context_ != caller_context) {
      context_->lock();
        Value res = trigger(val);
      context_->unlock();
      return res;
    } else {
      return trigger(val);
    }
  }

  /** This is the operation executed when the object is called.
   *  In order to benefit from return value optimization and avoid too many copy
   *  you have to use Value v = xxx.trigger(val). */
  virtual const Value trigger(const Value &val) {
    return gNilValue;
  }

  /** Dynamically build a child from the given name. This method is called whenever
   *  a sub-node or branch is not found and this is the last found object along
   *  the path.
   */
  virtual Object * build_child(const std::string &name, Value *error) {
    return NULL;
  }

  /** Lock resource (used by commands). */
  inline void lock() {
    if (context_) context_->lock();
  }

  /** Unlock resource (used by commands). */
  inline void unlock() {
    if (context_) context_->unlock();
  }

  /** Inform the object of an alias linked to this object (has to be deleted
   *  on destruction).
   */
  void register_alias(Alias *alias);

  /** Inform the object that an alias no longer exists. */
  void unregister_alias(Alias *alias);

  /** Return the object's unique url. */
  inline const std::string &url() const {
    return url_;
  }

  /** Set name from string. If the name is not unique in the parent's scope,
   *  the name is changed as "name-1", "name-2", etc.
   */
  void set_name(const char* name) { set_name(std::string(name)); }


  /** Set name from string. If the name is not unique in the parent's scope,
   *  the name is changed as "name-1", "name-2", etc.
   */
  void set_name(const std::string &name) {
    if (name == "") return;
    name_ = name;  // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
    set_parent(parent_);  // reset parent so name is verified in parent
    moved();
  }

  /** Return name of object. */
  inline const std::string name() const {
    return name_;
  }

  /** ========================== REPLIES TO META METHODS ======================
   * The replies to meta methods are implemented as virtuals so that objects
   * that inherit from osc::Object just need to overwrite these in order to
   * return more meaningful information / content.
   */

  /** List sub-nodes.
   * This method is used as a reply to the /.list meta method.
   * The format of the reply is a list of names with the type:
   * [name, name, ...].
   */
  const Value list() const;

  /** List sub-nodes with their current value and type.
   * This method is used as a reply to the /.list_with_type meta method.
   * The format of the reply is a list of names with the type:
   * [name, current, unit, ...], [name, current, unit, ...], etc.
   */
  const Value list_with_type() const;

  /** List full tree under this node.
   *  @param base_length is the length of the url for the initial call
   *                     (removed from results).
   *  @param tree returned value.
   */
  void tree(size_t base_length, Value *tree) const;

  /** Human readable information method.
   *  Called as a response to "/.info '/this/url'".
   */
  const Value &info() const {
    return type_.last();
  }

  /** Type information on node.
   */
  const Value &type() const {
    return type_;
  }

  /** Type information with current value (used to automatically generate the correct control).
   * Called during response to "/.type '/this/url'".
   */
  const Value type_with_current_value();

  /** Set meta type (signature, range, units). The type should be immutable.
   *  this method is not a good idea.
   */
  // bool set_type(const Value &type) {
  //   if (type.type_id() != type_.type_id()) {
  //     return false;
  //   } else {
  //     type_ = type;
  //     type_changed();
  //     return true;
  //   }
  // }

  /** Return the list of children as a hash.
   */
  const THash<std::string, Object *> children() const {
    return children_;
  }

  /** Return first child.
   */
  Object *first_child() {
    return children_.size() > 0 ? child(children_.keys().front()) : NULL;
  }

  /** Return the direct child named 'name'.
   */
  Object *child(const std::string &name) {
    Object * child;
    if (children_.get(name, &child)) {
      return child;
    } else {
      return NULL;
    }
  }

  Object *parent() {
    return parent_;
  }

  /** Set object's new root.
   */
  void set_root(Root *root);

  /** Set object's new parent.
   */
  void set_parent(Object *parent);

  /** Parent changed, set new context. */
  virtual void set_context(Mutex *context) { context_ = context; }

  /** Return the type tag signature id (uint) of the trigger method of this
   *  object (what it wants to receive as arguments).
   */
  inline TypeTagID type_id() {
    return type_id_;
  }

  bool accept_any_type() {
    return type_id_ == ANY_TYPE_TAG_ID;
  }

  inline bool can_receive(const Value &val) {
    if (type_id() == NO_TYPE_TAG_ID) return false;
    if (val.type_id() == type_id() || accept_any_type()) {
      return true;
    } else if (val.is_nil()) {
      return true;
    } else if (!val.is_list() || !type_[0].is_list()) {
      return false;
    } else {
      // first elements should match
      const char * c  = type_[0].type_tag();
      const char * cl = val.type_tag();
      while (*c) {
        if (*c != *cl) return false;
        ++c;
        ++cl;
      }
      return true;
    }
  }

 protected:

  /** Child sends a notification to the parent when it's name changes so that
   *  the parent/root keep their url hash in sync.
   */
  void register_child(Object *child);

  /** Free the child from the list of children.
   */
  void unregister_child(Object *child);

  /** Update cached url, notify root_ of the position change.
   */
  void moved();

  /** Add '-1', '-2', ... at the end of the current name. bob --> bob-1
   */
  void find_next_name() {
    size_t pos = name_.find('-');
    char buffer[OSC_NEXT_NAME_BUFFER_SIZE];
    if (pos == std::string::npos) {
      name_.append("-1");
    } else {
      std::string baseName = name_.substr(0, pos - 1);
      int i = atoi(name_.substr(pos + 1, std::string::npos).c_str());
      snprintf(buffer, OSC_NEXT_NAME_BUFFER_SIZE, "%i", i+1);
      name_ = baseName.append(buffer);
    }
  }

 private:
  /** Keep type_id_ in sync with type_.
   */
  void type_changed() {
    type_id_ = type_.size() > 0 ? type_[0].type_id() : NO_TYPE_TAG_ID;
  }

  /** List of aliases to destroy when
   * this node disappears.
   */
  std::list<Alias *>          aliases_;


 protected:

  /** Root object used to access application methods/
   */
  Root *root_;

  /** Pointer to parent object.
   *  Can be NULL if the object does not have a parent.
   */
  Object *parent_;

  /** Hash with pointers to children objects or methods.
   *  The children objects unregister themselves when they die or change
   *  their parent by calling 'unregister_child'.
   */
  THash<std::string, Object *> children_;

  /** Unique name in parent's context.
   */
  std::string name_;

  /** Absolute path to object (cached).
   */
  std::string url_;

  /** Mutex to make sure only one thread is using a given context at a time.
   */
  Mutex *context_;

 private:

  /** Value that holds type information on the 'trigger' method of this
   *  object.
   *  If the type_ is not a list, this means the object is not callable and
   *  the type_ member only holds a string with general information on the
   *  object.
   *  If the type_ is a list, the first element is the actual type of Value
   *  the object can receive in it's 'trigger' method and the other elements
   *  are information on the range of allowed values and such.
   *  The last element of type_ is always an information string.
   */
  Value type_;

  /** Identifier for the type of the values the element can receive.
   *  The value is a hash of the type tag list (multiple osc type tags) such
   *  as "f" (single Real) or "fss" (Real, String, String).
   *  This is cached from the first element in type_.
   */
  TypeTagID type_id_;
};

}  // namespace osc

#endif  // OSCIT_INCLUDE_OSCIT_OBJECT_H_
