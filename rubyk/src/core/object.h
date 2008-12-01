#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <list>
#include "values.h"
#include "thash.h"

#define OBJECT_HASH_SIZE 10000
#define NEXT_NAME_BUFFER_SIZE 20

class Object
{
public:
  // FIXME: remove this constructor !
  Object() : mParent(NULL), mChildren(20)
  {
    init_object(NULL, std::string(""));
  }
  
  Object(const char * pName) : mParent(NULL), mChildren(20)
  {
    init_object(NULL, std::string(pName));
  }
  
  Object(const std::string& pName) : mParent(NULL), mChildren(20)
  {
    init_object(NULL, pName);
  }
  
  Object(Object * pParent, const std::string &pName) : mParent(NULL), mChildren(20)
  {
    init_object(pParent, pName);
  }
  
  Object(Object& pParent, const std::string &pName) : mParent(NULL), mChildren(20)
  {
    init_object(&pParent, pName);
  }
  
  virtual ~Object()
  {
    string_iterator it;
    string_iterator end = mChildren.end();
    // notify parent
    if (mParent) mParent->remove_child(this);
    
    // notify global list
    remove_object(this);
    
    // remove parent reference in all children
    for(it = mChildren.begin(); it != end; it++) {
      Object * child;
      if (mChildren.get(&child, *it)) {
        child->mParent = NULL;
      }
    }
  }
  
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
  
  /** Execute a method / call the default operation for an object. */
  static Value call (const char* pUrl, const Value& val)
  {
    return call(std::string(pUrl), val);
  }
  
  /** Execute a method / call the default operation for an object. */
  static Value call (const std::string& pUrl, const Value& val)
  {
    Object * target;
    size_t info_pos = pUrl.rfind("/#info");
    // 1. does the url end with '/#info' ?
    if (info_pos != std::string::npos) {
      // return info string
      // find object from url
      if (get(&target, pUrl.substr(0, info_pos))) {
        return String(target->mInfo);
      } else {
        return Error(std::string("Object '").append(pUrl.substr(0, info_pos)).append("' not found."));
      }
    } else {
      // call
      // find object from url
      if (get(&target, pUrl)) {
        // call
        return target->trigger(val);
      } else {
        return Error(std::string("Object '").append(pUrl).append("' not found."));
      }
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
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  static Object * find(const std::string& pUrl)
  {
    Object * res = NULL;
    get(&res, pUrl);
    return res;
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  static Object * find(const char * pUrl)
  {
    return find(std::string(pUrl));
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
    string_iterator it;
    string_iterator end = mChildren.end();
    Object * obj;
    
    mUrl = "";
    
    for(it = mChildren.begin(); it != end; it++) {
      if (mChildren.get(&obj, *it)) obj->moved();
    }
    
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
  std::string default_name()
  {
    char buf[50];
    sIdCounter++;
    sprintf(buf,"_%i",sIdCounter);
    return std::string(buf);  // default variable name is 'id'
  }
  
  virtual void init_object(Object * pParent, const std::string &pName)
  { 
    if (pName == "")
      mName = default_name();
    else
      mName = pName;
    
    set_parent(pParent);
  }
  
  /** Add a child to the children list, updating 'child' to make sure the name is unique. Called by child's set_parent method. */
  void add_child(Object * pChild)
  { 
    while (child(pChild->mName))
      pChild->next_name();
    
    mChildren.set(pChild->mName,pChild);
  }
  
  /** Remove the child from the list of children. Called by child's set_parent method. */
  void remove_child(Object * pChild)
  {
    mChildren.remove_element(pChild);
  }
  
  static THash<std::string, Object*> sObjects;   /**< Hash to find objects from their url. */
  
  Object *                    mParent;           /**< Pointer to parent object. */
  THash<std::string,Object *> mChildren;         /**< Hash with pointers to sub-objects / methods */
  std::string                 mUrl;              /**< Absolute path to object (cached). TODO: this cache is not really needed. */
protected:  
  std::string                 mName;             /**< Unique name in parent's context. */
  std::string                 mInfo;             /**< Help/information string. */
  static unsigned int         sIdCounter;        /**< Use to set a default id and position. */

};

#endif