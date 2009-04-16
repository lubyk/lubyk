#ifndef _OSCIT_BASE_OBJECT_H_
#define _OSCIT_BASE_OBJECT_H_
#include "oscit/values.h"
#include "oscit/thash.h"
#include "oscit/mutex.h"

#include <list>

namespace oscit {
#define OSC_NEXT_NAME_BUFFER_SIZE 20
#define DEFAULT_INFO "no information defined for this object"

class Root;
class Alias;

class BaseObject
{
 public:
  BaseObject() : root_(NULL), parent_(NULL), children_(20), context_(NULL), type_tag_id_(NO_TYPE_TAG_ID), info_(DEFAULT_INFO) {
   name_ = "";
   url_  = name_;
  }
  
  BaseObject(const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_tag_id_(NO_TYPE_TAG_ID), info_(DEFAULT_INFO) {}
  
  BaseObject(const std::string &name) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_tag_id_(NO_TYPE_TAG_ID), info_(DEFAULT_INFO) {}
  
  BaseObject(TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {
    name_ = "";
    url_  = name_;
  }

  BaseObject(const char *name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {}

  BaseObject(const std::string &name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), url_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {}

  BaseObject(BaseObject *parent, const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(NO_TYPE_TAG_ID), info_(DEFAULT_INFO) {
    parent->adopt(this);
  }

  BaseObject(BaseObject *parent, const char *name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {
    parent->adopt(this);
  }

  BaseObject(BaseObject *parent, const std::string &name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {
    parent->adopt(this);
  }

  BaseObject(BaseObject &parent, const char *name) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(NO_TYPE_TAG_ID), info_(DEFAULT_INFO) {
    parent.adopt(this);
  }

  BaseObject(BaseObject &parent, const char *name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {
    parent.adopt(this);
  }

  BaseObject(BaseObject &parent, const std::string &name, TypeTagID type_tag_id) : root_(NULL), parent_(NULL), children_(20), name_(name), context_(NULL), type_tag_id_(type_tag_id), info_(DEFAULT_INFO) {
    parent.adopt(this);
  }

  virtual ~BaseObject();

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
    BaseObject * old_parent = object->parent_;

    if (old_parent) old_parent->release(object);

    object->parent_ = this;
    object->set_context(context_);

    object->moved();
    return (T*)object;
  }
  
  /** Class type id. */
  virtual uint class_type() { return H("BaseObject"); }

  template<class T>
  static inline T * type_cast(uint type, BaseObject * obj) {
    return (obj && obj->class_type() == type) ? (T*)obj : NULL;
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
  virtual BaseObject * build_child(const std::string &name, Value *error) {
    return NULL;
  }
  
  /** Define the object's container (parent). */
  void set_parent(BaseObject& parent)
  { set_parent(&parent); }

  /** Define the object's container (parent). */
  void set_parent(BaseObject * parent) {
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
  inline const std::string &url() {
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
  /* inherit from osc::BaseObject just need to overwrite these in order to return more  */
  /* meaningful information / content.                                              */
  
  /** List sub-nodes. */
  const std::string list () const {
    std::string res;
    const_string_iterator it  = children_.begin();
    const_string_iterator end = children_.end();
    bool start = true;
    if (it == end) return NULL;

    while(it != end) {
      BaseObject * obj;
      if (children_.get(*it, &obj)) {
        if (obj->class_type() != H("Alias")) {
            // do not list alias (Alias are used as internal helpers and do not need to be advertised)
          if (!start) res.append(",");
          res.append(obj->name_);
          if (!(obj->children_.empty())) res.append("/");
          start = false;
        }
      }
      it++;
    }
  
    return res;
  }
  
  /** Human readable information method.
   *  Called as a response to "/.info '/this/url'". 
   */
  const std::string &info() const {
    return info_;
  }
  
  /** Set info string. */
  void set_info (const char* pInfo) {
    info_ = pInfo;
  }

  /** Set info string. */
  void set_info (const std::string &pInfo) {
    info_ = pInfo;
  }
  
  /** Type information on node (used to automatically generate the correct control). 
   *  Called during response to "/.type '/this/url'".
   */
  const Value &type() const {
    return type_;
  }
  
  /** Set meta type (signature, range, units). */
  void set_type(const Value &type) {
    type_ = type;
  }
  
  /** Set meta type as range (slider). */
  void set_type(Real current, Real min, Real max, const char *unit) {
    Value type;
    type.push_back(current).push_back(min).push_back(max).push_back(unit);
    set_type(type);
  }
  
  /** Set meta type as list (menu). */
  void set_type(const char *current, const char *values, const char *unit) {
    Value type;
    type.push_back(current).push_back(values).push_back(unit);
    set_type(type);
  }
  
  /** Set meta type as any real (entry field). */
  void set_type(Real current, const char *unit) {
    Value type;
    type.push_back(current).push_back(unit);
    set_type(type);
  }
  
  /** Return the list of children as a hash. */
  const THash<std::string,BaseObject *> children() const {
    return children_;
  }

  /** Return first child. */
  BaseObject *first_child() {
    return children_.size() > 0 ? child(children_.keys().front()) : NULL;
  }

  /** Return the direct child named 'name'. */
  BaseObject *child(const std::string &name) {
    BaseObject * child;
    if (children_.get(name, &child)) {
      return child;
    } else {
      return NULL;
    }
  }
  
  BaseObject *parent() {
    return parent_;
  }
  
  virtual void set_root(Root *root) {
    root_ = root;
  }
  
  /** Parent changed, set new context. */
  virtual void set_context(Mutex *context) { context_ = context; }
  
protected:

  /** Child sends a notification to the parent when it's name changes so that the parent/root keep their url hash in sync. */
  void register_child(BaseObject * pChild);

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
  
  /** Define object type_tag_id. This cannot be changed during runtime.
    * @param typeTag a string with the osc type type_tag_id.
    */
  void set_type_tag_id(TypeTagID type_tag_id) {
    type_tag_id_ = type_tag_id;
  }
  
  /** Return the type tag signature id as an unsigned int. */
  TypeTagID type_tag_id() {
    return type_tag_id_;
  }
  
  bool accept_any_type() {
    return type_tag_id_ == ANY_TYPE_TAG_ID;
  }
  
 private:
  
  /** Free the child from the list of children. */
  void release(BaseObject *pChild);

  std::list<Alias *>              aliases_;   /**< List of aliases to destroy when this node disappears. */
  
protected:
  friend class Root;
  friend class Alias;

  Root                            *root_;     /**< Root object. */
  BaseObject                      *parent_;   /**< Pointer to parent object. */
  THash<std::string,BaseObject *> children_;  /**< Hash with pointers to sub-objects / methods */
  std::string                     name_;      /**< Unique name in parent's context. */
  std::string                     url_;       /**< Absolute path to object (cached). TODO: this cache is not really needed. */
  static size_t                   sIdCounter; /**< Use to set a default id and position. */
  
  Mutex *context_;                            /**< Mutex to make sure only one thread is using a given context at a time. */

private:
  friend class OscCommand;
  TypeTagID                   type_tag_id_;   /**< OSC type tag type_tag_id. */
  std::string                 info_;          /**< Help/information string. */
  Value                       type_;          /**< Type information (type signature, range & units). */
};
} // namespace osc

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) oscit::BaseObject::type_cast<klass>(H(#klass), op);

#endif // _OSCIT_BASE_OBJECT_H_