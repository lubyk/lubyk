#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <list>
#include "values.h"
#include "hash.h"

#define OBJECT_HASH_SIZE 10000
#define NEXT_NAME_BUFFER_SIZE 20

class Object
{
public:
  // FIXME: remove this constructor !
  Object() : mParent(NULL)
  {
    init_object(NULL, std::string(""));
  }
  
  Object(const char * pName) : mParent(NULL)
  {
    init_object(NULL, std::string(pName));
  }
  
  Object(const std::string& pName) : mParent(NULL)
  {
    init_object(NULL, pName);
  }
  
  Object(Object * pParent, const std::string &pName) : mParent(NULL)
  {
    init_object(pParent, pName);
  }
  
  Object(Object& pParent, const std::string &pName) : mParent(NULL)
  {
    init_object(&pParent, pName);
  }
  
  virtual ~Object()
  {
    // notify parent
    if (mParent) mParent->remove_child(this);
    
    // notify global list
    remove_object(this);
    
    // destroy children
    while (!mChildren.empty())
    {
      Object * child = mChildren.front();
      child->mParent = NULL;
      mChildren.pop_front();
    }
  }
  
  /** The operation to be executed on call (method for Method class / object listing for Nodes) */
  virtual const Value trigger (const Value& sig)
  {
    if (mChildrenStrList == "") {
      std::list<Object *>::iterator it  = mChildren.begin();
      std::list<Object *>::iterator end = mChildren.end();
      bool start = true;
    
      while(it != end) {
        Object * obj = *it;
        if (!start) mChildrenStrList.append(",");
        mChildrenStrList.append(obj->mName);
        if (!(obj->mChildren.empty())) mChildrenStrList.append("/");
        start = false;
        it++;
      }
    }
    
    return mChildrenStrList;
  }
  
  /** Execute a method / call the default operation for an object. */
  static Value call (const char* pUrl, const Value& sig)
  {
    return call(std::string(pUrl), sig);
  }
  
  /** Execute a method / call the default operation for an object. */
  static Value call (const std::string& pUrl, const Value& sig)
  {
    Object * target;
    // 1. find object from url
    if (get(&target, pUrl)) {
      // 2. call
      return target->trigger(sig);
    } else {
      return Error("object not found");
    }
  }
  
  static bool get (Object ** pResult, const std::string& pUrl)
  {
    return sObjects.get(pResult, pUrl);
  }
  
  static bool get (Object ** pResult, const char* pUrl)
  {
    return sObjects.get(pResult, std::string(pUrl));
  }
  
  void set_parent(Object& parent)
  { set_parent(&parent); }
  
  /** Define the object's container (parent). */
  void set_parent(Object * parent)
  {
    if (mParent) {
      mParent->remove_child(this);
    }
    
    mParent = parent;
    
    if (mParent) {
      mParent->add_child(this);
    }
    
    moved();
  }
  
  const std::string& url()
  {
    if (mUrl == "") {
      // build fullpath
      if (mParent) {
        mUrl = std::string(mParent->url()).append("/").append(mName);
      } else {
        mUrl = std::string("/").append(mName);
      }
    }
    return mUrl;
  }
  
  void set_name (const char* pName) 
  { set_name(std::string(pName)); }
  
  void set_name (const std::string& pName) 
  { 
    if (pName == "") return;
    mName = pName; // FIXME: gsub(/[^a-zA-Z\-0-9_],"")
    
    // this forces the name to sync in the parent's scope
    if (mParent) set_parent(mParent);
  }
  
  const std::list<Object *> children() const
  {
    return mChildren;
  }

protected:
  static void object_moved(Object * pObj)
  {
    // url changed, make sure dict is in sync
    // 1. remove object
    // 2. add with new key
    add_object(pObj);
  }
  
  static void remove_object(Object * pObj)
  {
    sObjects.remove_element(pObj);
  }
  
  static void add_object(Object * pObj)
  {
    remove_object(pObj);
    sObjects.set(pObj->url(), pObj);
  }
  
  void moved()
  {
    std::list<Object *>::iterator it  = mChildren.begin();
    std::list<Object *>::iterator end = mChildren.end();
    
    mUrl = "";
    
    while(it != end) (*it++)->moved();
    
    object_moved(this);
  }
  
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
  void use_default_name()
  {
    char buf[50];
    sIdCounter++;
    sprintf(buf,"_%i",sIdCounter);
    mName = std::string(buf);  // default variable name is 'id'
  }
  
  void init_object(Object * pParent, const std::string &pName)
  { 
    if (pName == "")
      use_default_name();
    else
      mName = pName;
    
    set_parent(pParent);
  }
  
  /** Add a child to the children list, updating 'child' to make sure the name is unique. Called by child's set_parent method. */
  void add_child(Object * child)
  { 
    std::list<Object *>::iterator it  = mChildren.begin();
    std::list<Object *>::iterator end = mChildren.end();
    
    while (it != end) {
      Object * obj = *it;
      if (obj == child) return;
      if (obj->mName == child->mName) {
        // name conflict. Change child name.
        child->next_name();
        add_child(child);
        return;
      }
      it++;
    }
    mChildren.push_back(child);
    mChildrenStrList = ""; // reset cached list
  }
  
  /** Remove the child from the list of children. Called by child's set_parent method. */
  void remove_child(Object * child)
  {
    std::list<Object *>::iterator it  = mChildren.begin();
    std::list<Object *>::iterator end = mChildren.end();

    while (it != end) {
      if (*it == child) {
        mChildren.erase(it);
        mChildrenStrList = ""; // reset cached list
        return;
      }
      it++;
    }
    // child not found. ignore
  }
  
  static Hash<std::string, Object*> sObjects;  /**< Dictionary to find objects from their url. */
  
  Object * mParent;              /**< Pointer to parent object. */
  std::list<Object *> mChildren; /**< Pointer to sub-objects / methods */
  String       mChildrenStrList;  /**< Comma separated list of children (cached). */
  std::string mUrl;              /**< Absolute path to object (cached). */
protected:  
  std::string mName;                  /**< Unique name in parent's context. */
  static unsigned int sIdCounter;     /**< Use to set a default id and position. */
};

#endif