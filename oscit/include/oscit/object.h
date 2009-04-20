#ifndef _OSCIT_BASE_OBJECT_H_
#define _OSCIT_BASE_OBJECT_H_
#include "oscit/typed.h"
#include "oscit/values.h"
#include "oscit/thash.h"
#include "oscit/mutex.h"

#include <list>

namespace oscit {
#define OSC_NEXT_NAME_BUFFER_SIZE 20

#define NoIO(info)  Value(info)
#define NilIO(info)  ListValue("N").push_back(info)
#define FieldIO(units,info) Value(0.0).push_back(units).push_back(info)
#define StringIO(units,info) Value("").push_back(units).push_back(info)
#define RangeIO(min,max,units,info) Value(0.0).push_back(min).push_back(max).push_back(units).push_back(info)
#define SelectIO(values,units,info) Value("").push_back(values).push_back(units).push_back(info)
#define HashIO(info) Value('H').push_back(info)
#define MatrixIO(rows,cols,info) MatrixValue(rows,cols).push_back(info)
#define AnyIO(info) Value('*').push_back(info)

#define DEFAULT_TYPE NoIO("No information on this node.")
  
class Root;
class Alias;

class Object : public Typed
{
 public:
  /** Class signature. */
  TYPED("Object")
  
  Object() : root_(NULL), parent_(NULL), children_(20), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
    name_ = "";
    url_  = name_;
  }
  
  Object(const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
  }
  
  Object(const std::string &name) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
  }
  
  Object(const Value &type) : root_(NULL), parent_(NULL), children_(20), context_(NULL), type_(type) {
    type_changed();
    name_ = "";
    url_  = name_;
  }

  Object(const char *name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_(type) {
    type_changed();
  }
  
  Object(const std::string &name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_(type) {
    type_changed();
  }
  
  Object(Object *parent, const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
    parent->adopt(this);
  }

  Object(Object *parent, const char *name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(type) {
    type_changed();
    parent->adopt(this);
  }

  Object(Object *parent, const std::string &name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(type) {
    type_changed();
    parent->adopt(this);
  }

  Object(Object &parent, const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(DEFAULT_TYPE) {
    type_changed();
    parent.adopt(this);
  }

  Object(Object &parent, const char *name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(type) {
    type_changed();
    parent.adopt(this);
  }

  Object(Object &parent, const std::string &name, const Value &type) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_(type) {
    type_changed();
    parent.adopt(this);
  }

  virtual ~Object();

  /** Shortcut to call multiple methods on an object.
    * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling "obj.tempo(45)" and "obj.rubato(1.5)".
    * @return a hash with the result for each call.
    */
  const Value set(const Value &val);
  
  /** Shortcut to call multiple methods on an object.
    * @param val Using "obj.set(tempo:45 rubato:1.5)" is equivalent to calling "obj.tempo(45)" and "obj.rubato(1.5)".
    * @return true on success, false if any call failed.
    */
  bool set_all_ok(const Value &val);

  /** This is the prefered way to insert new objects in the tree since it clearly highlights ownership in the parent. 
    * TODO: make sure a parent is not adopted by it's child. */
  template<class T>
  T * adopt(T * object) {
    Object * old_parent = object->parent_;

    if (old_parent) old_parent->release(object);

    object->parent_ = this;
    object->set_context(context_);

    object->moved();
    return (T*)object;
  }

  /** Clear all children (delete). */
  void clear();
  
  /** Lock mutex if needed before calling 'trigger'. */
  const Value safe_trigger(const Value &val) {
    if (context_) {
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
   *  a sub-node or branch is not found and this is the last found object along the path. */
  virtual Object * build_child(const std::string &name, Value *error) {
    return NULL;
  }
  
  /** Define the object's container (parent). */
  void set_parent(Object& parent)
  { set_parent(&parent); }

  /** Define the object's container (parent). */
  void set_parent(Object * parent) {
    if (!parent) {
      if (parent_) parent_->release(this);
      parent_ = NULL;
      set_context(NULL);
      moved();
    } else {
      parent->adopt(this);
    }
  }
  
  /** Lock resource (used by commands). */
  inline void lock() {
    if (context_) context_->lock();
  }
  
  /** Unlock resource (used by commands). */
  inline void unlock() {
    if (context_) context_->unlock();
  }

  /** Inform the object of an alias linked to this object (has to be deleted on destruction). */
  void register_alias(Alias *alias);

  /** Inform the object that an alias no longer exists. */
  void unregister_alias(Alias *alias);

  /** Return the object's unique url. */
  inline const std::string &url() const {
    return url_;
  }
  
  /** Register unique url in the Root object so that methods addressed to this url actually end in this object. */
  void register_url ();
  
  /** Set name from string. If the name is not unique in the parent's scope, the name is changed as "name-1", "name-2", etc. */
  void set_name (const char* name) { set_name(std::string(name)); }


  /** Set name from string. If the name is not unique in the parent's scope, the name is changed as "name-1", "name-2", etc. */
  void set_name (const std::string &name) { 
    if (name == "") return;
    name_ = name; // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
  
    moved();
  }
  
  /** Return name of object. */
  const std::string name() const {
    return name_;
  }
  
  /* ========================== REPLIES TO META METHODS =========================== */
  /* The replies to meta methods are implemented as virtuals so that objects that   */
  /* inherit from osc::Object just need to overwrite these in order to return more  */
  /* meaningful information / content.                                              */
  
  /** List sub-nodes. */
  const Value list() const {
    Value res;
    const_string_iterator it, end = children_.end();
    for (it = children_.begin(); it != end; ++it) {
      Object * obj;
      if (children_.get(*it, &obj)) {
        if (!obj->is_a("Object.Alias")) {
          // do not list alias (Alias are used as internal helpers and do not need to be advertised) ?
          if (obj->children_.empty()) {
            res.push_back(obj->name_);
          } else {
            res.push_back(std::string(obj->name_).append("/"));
          }
        }
      }
    }
    return res;
  }
  
  /** List full tree under this node.
   *  @param base_length is the length of the url for the initial call (removed from results).
   *  @param tree returned value.
   */
  void tree(size_t base_length, Value *tree) const {
    const_string_iterator it, end = children_.end();
    for (it = children_.begin(); it != end; ++it) {
      Object * obj;
      if (children_.get(*it, &obj)) {
        if (!obj->is_a("Object.Alias")) {
          // do not list alias (Alias are used as internal helpers and do not need to be advertised) ?
          tree->push_back(obj->url().substr(base_length));
          obj->tree(base_length, tree);
        }
      }
    }
  }
  
  /** Human readable information method.
   *  Called as a response to "/.info '/this/url'". 
   */
  const Value &info() const {
    return type_.last();
  }
  
  /** Type information on node (used to automatically generate the correct control). 
   *  Called during response to "/.type '/this/url'".
   */
  const Value &type() const {
    return type_;
  }
  
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
  
  /** Return the list of children as a hash. */
  const THash<std::string,Object *> children() const {
    return children_;
  }

  /** Return first child. */
  Object *first_child() {
    return children_.size() > 0 ? child(children_.keys().front()) : NULL;
  }

  /** Return the direct child named 'name'. */
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
  
  virtual void set_root(Root *root) {
    root_ = root;
  }
  
  /** Parent changed, set new context. */
  virtual void set_context(Mutex *context) { context_ = context; }
  
  /** Return the type tag signature id (uint) of the trigger method of this object (what it wants to receive as arguments). */
  inline TypeTagID type_id() {
    return type_id_;
  }
  
  bool accept_any_type() {
    return type_id_ == ANY_TYPE_TAG_ID;
  }
  
  inline bool can_receive(const Value &val) {
    return val.type_id() == type_id() || accept_any_type();
  }
  
protected:

  /** Child sends a notification to the parent when it's name changes so that the parent/root keep their url hash in sync. */
  void register_child(Object * pChild);

  /** Update cached url, notify root_ of the position change. */
  void moved();

  /** Add '-1', '-2', ... at the end of the current name. bob --> bob-1 */
  void find_next_name() {
    size_t pos = name_.find('-');
    char buffer[OSC_NEXT_NAME_BUFFER_SIZE];
    if (pos == std::string::npos) {
      name_.append("-1");
    } else {
      std::string baseName = name_.substr(0,pos - 1);
      int i = atoi(name_.substr(pos + 1, std::string::npos).c_str());
      snprintf(buffer, OSC_NEXT_NAME_BUFFER_SIZE, "%i", i+1);
      name_ = baseName.append(buffer);
    }
  }
  
 private:
  /** Keep type_id_ in sync with type_. */
  void type_changed() {
    type_id_ = type_.size() > 0 ? type_[0].type_id() : type_.type_id();
  }
  
  /** Free the child from the list of children. */
  void release(Object *pChild);

  std::list<Alias *>          aliases_;   /**< List of aliases to destroy when this node disappears. */
  
protected:
  friend class Root;
  friend class Alias;

  Root                        *root_;     /**< Root object. */
  Object                      *parent_;   /**< Pointer to parent object. */
  THash<std::string,Object *> children_;  /**< Hash with pointers to sub-objects / methods */
  std::string                 name_;      /**< Unique name in parent's context. */
  std::string                 url_;       /**< Absolute path to object (cached). TODO: this cache is not really needed. */
  static size_t               sIdCounter; /**< Use to set a default id and position. */
  
  Mutex *context_;                        /**< Mutex to make sure only one thread is using a given context at a time. */

private:
  friend class OscCommand;
  Value                       type_;      /**< Type information (type signature, range & units). */
  TypeTagID                   type_id_;   /**< OSC type tag id (cached from type_). */
  std::string                 info_;      /**< Help/information string (cached from type_). */
};
} // namespace osc

#endif // _OSCIT_BASE_OBJECT_H_