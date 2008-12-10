#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <list>
#include "values.h"
#include "thash.h"

#define NEXT_NAME_BUFFER_SIZE 20

class Root;
class Alias;

class Object
{
public:
  Object() : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = default_name();
    rebuild_url();
  }
  
  Object(const char * pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    rebuild_url();
  }
  
  Object(const std::string& pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    rebuild_url();
  }
  
  Object(Object * pParent) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = default_name();
    pParent->adopt(this);
  }
  
  Object(Object * pParent, const char * pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    pParent->adopt(this);
  }
  
  Object(Object * pParent, const std::string& pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    pParent->adopt(this);
  }
  
  Object(Object& pParent) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = default_name();
    pParent.adopt(this);
  }
  
  Object(Object& pParent, const char * pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    pParent.adopt(this);
  }
  
  Object(Object& pParent, const std::string& pName) : mRoot(NULL), mParent(NULL), mChildren(20)
  {
    mName = pName;
    pParent.adopt(this);
  }
  
  /** This is the prefered way to insert new objects in the tree since it clearly highlights ownership in the parent. 
    * TODO: make sure a parent is not adopted by it's child. */
  template<class T>
  T * adopt(T * pObj)
  {
    do_adopt((Object*) pObj);
    return pObj;
  }
  
  virtual ~Object();
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Object");
  }
  
  template<class T>
  inline T * type_cast(uint pType, Object * obj)
  {
    return obj && obj->type() == pType ? (T*)obj : NULL;
  }
  
  /** Clear all children (delete). */
  void clear();
  
  /** The operation to be executed on call (method for Method class / object listing for Nodes) */
  virtual const Value trigger (const Value& val)
  {
    std::string res;
    string_iterator it  = mChildren.begin();
    string_iterator end = mChildren.end();
    bool start = true;
    if (it == end) return gNilValue;
  
    while(it != end) {
      Object * obj;
      if (mChildren.get(&obj, *it)) {
        if (!start) res.append(",");
        res.append(obj->mName);
        if (!(obj->mChildren.empty())) res.append("/");
        start = false;
      }
      it++;
    }
    
    return String(res);
  }
  
  /** This method is called whenever a sub-node or branch is not found and this is the last found object along the path. */
  virtual const Value not_found (const std::string& pUrl, const Value& val)
  {
    return Error(std::string("Object '").append(pUrl).append("' not found."));
  }
  
  /** Inspection method. Called as a response to #inspect.*/
  virtual const Value inspect(const Value& val)
  {
    return String(mInfo);
  }
  
  void set_parent(Object& parent)
  { set_parent(&parent); }
  
  /** Define the object's container (parent). */
  void set_parent(Object * parent)
  {
    if (!parent) {
      if (mParent) mParent->release(this);
      mParent = NULL;
      moved();
    } else {
      parent->adopt(this);
    }
  }
  
  /** Inform the object of an alias to be destroyed on destruction. */
  void register_alias(Alias * pAlias);
  
  /** Inform the object that an alias no longer exists. */
  void unregister_alias(Alias * pAlias);
  
  const std::string& url()
  {
    return mUrl;
  }
  
  void rebuild_url()
  {
    if (mParent) {
      // build fullpath
      mUrl = std::string(mParent->url()).append("/").append(mName);
    } else {
      // no parent
      mUrl = mName;
    }
  }
  
  void set_name (const char* pName) 
  { set_name(std::string(pName)); }
  
  void set_name (const std::string& pName) 
  { 
    if (pName == "") return;
    mName = pName; // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
    
    moved();
  }
  
  /** Get information/help string. */
  const std::string& info() const
  {
    return mInfo;
  }
  
  /** Define information/help string. */
  void set_info (const char* pInfo)
  {
    mInfo = pInfo;
  }
  
  /** Define information/help string. */
  void set_info (const std::string& pInfo)
  {
    mInfo = pInfo;
  }
  
  const THash<std::string,Object *> children() const
  {
    return mChildren;
  }
  
  /** Return the direct child named 'pName'. */
  Object * child(const std::string& pName)
  {
    Object * child;
    if (mChildren.get(&child, pName)) {
      return child;
    } else {
      return NULL;
    }
  }
  
protected:
  /** Actual adoption. Overwritten by Group to manage tree separation. */
  virtual void do_adopt(Object * pObject)
  {
    Object * oldParent = pObject->mParent;

    if (oldParent) oldParent->release(pObject);

    pObject->mParent = this;
    pObject->mRoot   = mRoot;

    pObject->moved();
  }
  
  /** Child sends a notification to the parent when it's name changes so that the parent/root keep their url hash in sync. */
  void child_moved(Object * pChild);
  
  /** Update cached url, notify mRoot of the position change. */
  void moved();
  
  /** Add '-1', '-2', ... at the end of the current name. bob --> bob-1 */
  void next_name()
  {
    size_t pos = mName.find('-');
    char buffer[NEXT_NAME_BUFFER_SIZE];
    if (pos == std::string::npos) {
      mName.append("-1");
    } else {
      std::string baseName = mName.substr(0,pos - 1);
      int i = atoi(mName.substr(pos + 1, std::string::npos).c_str());
      snprintf(buffer, NEXT_NAME_BUFFER_SIZE, "%i", i+1);
      mName = baseName.append(buffer);
    }
  }
  
private:
  std::string default_name()
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
  friend class Group;
  
  Root   *                    mRoot;             /**< Root object. */
  Object *                    mParent;           /**< Pointer to parent object. */
  THash<std::string,Object *> mChildren;         /**< Hash with pointers to sub-objects / methods */
  std::string                 mUrl;              /**< Absolute path to object (cached). TODO: this cache is not really needed. */
  std::string                 mName;             /**< Unique name in parent's context. */
  std::string                 mInfo;             /**< Help/information string. */
  static unsigned int         sIdCounter;        /**< Use to set a default id and position. */

};

#define TYPE_CAST(klass, op) type_cast<klass>(H(#klass), op);
#endif