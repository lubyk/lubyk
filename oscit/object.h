#ifndef _OSC_OBJECT_H_
#define _OSC_OBJECT_H_
#include <list>
#include "values.h"
#include "thash.h"


namespace oscit {
#define OSC_NEXT_NAME_BUFFER_SIZE 20
#define NO_TYPE_TAG H("")

#define REAL_TYPE 'd'
#define REAL_SIGNATURE H("d")
#define RealValue "d"

#define STRING_TYPE 's'
#define STRING_SIGNATURE H("s")
#define StringValue "s"

#define MULTI_SIGNATURE H("*")
class Root;
class Alias;

class Object
{
public:
  Object(const char * typeTag) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = getDefaultName();
    url_  = name_;
  }

  Object(const char * typeTag, const char * name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    url_  = name_;
  }

  Object(const char * typeTag, const std::string& name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    url_  = name_;
  }

  Object(const char * typeTag, Object * parent) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = getDefaultName();
    parent->adopt(this);
  }

  Object(const char * typeTag, Object * parent, const char * name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    parent->adopt(this);
  }

  Object(const char * typeTag, Object * parent, const std::string& name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    parent->adopt(this);
  }

  Object(const char * typeTag, Object& parent) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = getDefaultName();
    parent.adopt(this);
  }

  Object(const char * typeTag, Object& parent, const char * name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    parent.adopt(this);
  }

  Object(const char * typeTag, Object& parent, const std::string& name) : root_(NULL), parent_(NULL), children_(20)
  {
    setTypeTag(typeTag);
    name_ = name;
    parent.adopt(this);
  }

  virtual ~Object();

  /** Shortcut to call multiple methods on an object.
    * Using "obj.set(foo:4 bar:5)" is equivalent to calling "obj.foo(4)" and "obj.bar(5)". */
  bool set(const Hash& pParams)
  {
    Object * obj;
    
    Hash_iterator it;
    Hash_iterator end = pParams.end();

    for(it = pParams.begin(); it != end; it++) {
      TypedValue tval;
      if ( (obj = child(*it)) && pParams.get(&tval, *it) ) {
        if (obj->mTypeTag == hashId(tval.type)) obj->trigger(&tval.value);
        // TODO: else ?
      }
    }
    return true;
  }
  
  /** Shortcut to call first method on an object.
    * Using "obj.set('waga',3.4,1)" is equivalent to calling "obj.first_method(['waga',3.4,1])". */
  bool set(const char * typeTag, const Value val)
  {
    Object * obj;
    // use first method as default
    string_iterator it  = children_.begin();
    string_iterator end = children_.end();
    if (it != end && (obj = child(*it)) && obj->mTypeTag == hashId(typeTag)) {
      // TODO: if the object type is "M" (matrix) ===> build a matrix from first "ffff" into a values.
      obj->trigger(val);
    }
    return true;
  }

  /** This is the prefered way to insert new objects in the tree since it clearly highlights ownership in the parent. 
    * TODO: make sure a parent is not adopted by it's child. */
  template<class T>
  T * adopt(T * object)
  {
    doAdopt(object);
    return object;
  }
  
  /** Class signature. */
  virtual uint type()
  {
    return H("oscit::Object");
  }

  template<class T>
  static inline T * type_cast(uint pType, Object * obj)
  {
    return (obj && obj->type() == pType) ? (T*)obj : NULL;
  }

  /** Clear all children (delete). */
  virtual void clear();

  /** This is the operation executed when the object is called.
    * @param val The parameters either respect the object's or the list of values is set to NULL.
    * @return This method *MUST* return the exact value types that it has advertised. */
  virtual const Value trigger (const Value val)
  {
    return NULL;
  }

  /** This method is called whenever a sub-node or branch is not found and this is the last found object along the path. */
  virtual const Value not_found (const std::string& pUrl, const char * typeTag, const Value val)
  {
    return Error(std::string("Object '").append(pUrl).append("' not found."));
  }
  
  /** Define the object's container (parent). */
  void setParent(Object& parent)
  { setParent(&parent); }

  /** Define the object's container (parent). */
  void setParent(Object * parent)
  {
    if (!parent) {
      if (parent_) parent_->release(this);
      parent_ = NULL;
      moved();
    } else {
      parent->adopt(this);
    }
  }

  /** Inform the object of an alias linked to this object (has to be deleted on destruction). */
  void registerAlias (Alias * pAlias);

  /** Inform the object that an alias no longer exists. */
  void unregisterAlias (Alias * pAlias);

  /** Return the object's unique url. */
  inline const std::string& url ()
  {
    return url_;
  }
  
  /** Register unique url in the Root object so that methods addressed to this url actually end in this object. */
  void registerUrl ();
  
  /** Set name from string. If the name is not unique in the parent's scope, the name is changed as "name-1", "name-2", etc. */
  void setName (const char* name) 
  { setName(std::string(name)); }


  /** Set name from string. If the name is not unique in the parent's scope, the name is changed as "name-1", "name-2", etc. */
  void setName (const std::string& name) 
  { 
    if (name == "") return;
    name_ = name; // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
  
    moved();
  }
  
  
  /* ========================== REPLIES TO META METHODS =========================== */
  /* The replies to meta methods are implemented as virtuals so that objects that   */
  /* inherit from osc::Object just need to overwrite these in order to return more  */
  /* meaningful information / content.                                              */
  
  /** Inspection method. Called as a response to "/.inspect '/this/url'". */
  virtual const std::string inspect() const
  {
    return mInfo;
  }
  
  /** Human readable information method. Called as a response to "/.info '/this/url'". */
  const std::string& info() const
  {
    return mInfo;
  }

  
  /** List sub-nodes. */
  const std::string list () const
  {
    std::string res;
    const_string_iterator it  = children_.begin();
    const_string_iterator end = children_.end();
    bool start = true;
    if (it == end) return NULL;

    while(it != end) {
      Object * obj;
      if (children_.get(&obj, *it)) {
        if (obj->type() != H("Alias")) {
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
  
  /** Set type tag. */
  
  /** Set info string. */
  void setInfo (const char* pInfo)
  {
    mInfo = pInfo;
  }

  /** Set info string. */
  void setInfo (const std::string& pInfo)
  {
    mInfo = pInfo;
  }
  
  /** Return the list of children as a hash. */
  const THash<std::string,Object *> children() const
  {
    return children_;
  }

  /** Return first child. */
  Object * getFirstChild()
  {
    return children_.size() > 0 ? child(children_.keys().front()) : NULL;
  }

  /** Return the direct child named 'name'. */
  Object * child(const std::string& name)
  {
    Object * child;
    if (children_.get(&child, name)) {
      return child;
    } else {
      return NULL;
    }
  }

  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void doAdopt(Object * object)
  {
    Object * oldParent = object->parent_;

    if (oldParent) oldParent->release(object);

    object->parent_ = this;

    object->moved();
  }
  
  Object * parent()
  {
    return parent_;
  }
  
  virtual void setRoot(Root * root)
  {
    root_ = root;
  }
  
protected:

  /** Child sends a notification to the parent when it's name changes so that the parent/root keep their url hash in sync. */
  void registerChild(Object * pChild);

  /** Update cached url, notify root_ of the position change. */
  void moved();

  /** Add '-1', '-2', ... at the end of the current name. bob --> bob-1 */
  void findNextName()
  {
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
  
  /** Define object signature. This not be changed during runtime.
    * @param typeTag a string with the type signature (should be static since it is not copied).
    */
  void setTypeTag(const char * typeTag)
  {
    if (mTypeTag == MULTI_SIGNATURE) {
      free(mParam.values);
    }
    mTypeTag = hashId(typeTag);
    mTypeTagStr = typeTag; // could be optimized to const char * storage.
    
    if (mTypeTag == MULTI_SIGNATURE) {
      mParam.values = (Value*)malloc(sizeof(Value) * strlen(typeTag));
    }
  }
  
private:
  
  // FIXME: this is a temporary fix for error handling. DO NOT LEAVE THIS AS IS !
  const Value Error(const std::string& pMessage)
  {
    fprintf(stderr, "Error in '%s': %s\n", url().c_str(), pMessage.c_str());
    return NULL;
  }
  
  // TODO: this could be replaced by "o" ("o-1", "o-2", "o-3") ? 
  std::string getDefaultName()
  {
    char buf[50];
    sIdCounter++;
    sprintf(buf,"_%i",sIdCounter);
    return std::string(buf);  // default variable name is 'id'
  }
  
  /** Free the child from the list of children. */
  void release(Object * pChild);

  std::list<Alias *>          mAliases;          /**< List of aliases to destroy when this node disappears. */

protected:
  friend class Root;
  friend class Alias;

  Root   *                    root_;             /**< Root object. */
  Object *                    parent_;           /**< Pointer to parent object. */
  THash<std::string,Object *> children_;         /**< Hash with pointers to sub-objects / methods */
  std::string                 url_;              /**< Absolute path to object (cached). TODO: this cache is not really needed. */
  std::string                 name_;             /**< Unique name in parent's context. */
  std::string                 mInfo;             /**< Help/information string. */
  uint                        mTypeTag;          /**< OSC type tag hash. */
  const char *                mTypeTagStr;       /**< C String representation of the type tag in the form of "ffi" for "array of two floats and an integer".. This should only be set to point to a static char *. */
  static unsigned int         sIdCounter;        /**< Use to set a default id and position. */

private:
  friend class OscReceive;
  Value                       mParam;           /**< Allocated memory to build the parameters from osc message. */
};
} // namespace osc

#define TYPE_CAST(klass, op) oscit::Object::type_cast<klass>(H(#klass), op);

#endif // _OSC_OBJECT_H_