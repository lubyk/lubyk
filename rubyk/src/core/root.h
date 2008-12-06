#ifndef _ROOT_H_
#define _ROOT_H_
#include "object.h"

class ClassListing;

#define OBJECT_HASH_SIZE 10000
#define CLASS_ROOT "/classes"
#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

/** Call types. */
enum call_action_t {
  CallTrigger  = 1,
  CallInfo     = 2,
  CallInspect  = 4,
  CallNotFound = 8,
};

/** Root object. You can only start new trees with root objects. */
class Root : public Object
{
public:
  Root() : Object(""), mObjects(OBJECT_HASH_SIZE)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
  }
  
  Root(size_t pHashSize) : mObjects(pHashSize)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
  }
  
  /** We need to do this before ~Object so that mObjects is still availlable. */
  virtual ~Root()
  {
    clear();
  }
  
  /** Execute the default operation for an object. */
  Value call (const char* pUrl)
  {
    return call(std::string(pUrl), gNilValue);
  }
  
  /** Execute the default operation for an object. */
  Value call (std::string& pUrl)
  {
    return call(pUrl, gNilValue);
  }
  
  /** Execute the default operation for an object. */
  Value call (const char* pUrl, const Value& val)
  {
    return call(std::string(pUrl), val);
  }
  
  /** Execute the default operation for an object. */
  Value call (const std::string& pUrl, const Value& val)
  {
    Object * target = NULL;
    size_t pos;
    std::string url = pUrl;
    call_action_t act;
    Value res;
    
    // FIXME: move the #info and #inspect into the method space ?
    
    if ( (pos = url.rfind("/#info")) != std::string::npos) {
      url = pUrl.substr(0, pos);
      act = CallInfo;
    } else if ( (pos = pUrl.rfind("/#inspect")) != std::string::npos) {
      url = pUrl.substr(0, pos);
      act = CallInspect;
    } else {
      act = CallTrigger;
    }
    
    while (!get(&target, url) && url != "") {
      act = CallNotFound;
      pos = url.rfind("/");
      if (pos == std::string::npos)
        url = "";
      else
        url = url.substr(0, pos);
      
    }
    
    if (!target) return not_found(pUrl, val);
    
    switch (act) {
      case CallTrigger:
        return target->trigger(val);
      case CallInfo:
        return String(target->mInfo);
      case CallInspect:
        return target->inspect(val);
      default:
        return target->not_found(pUrl, val);
    }
  }
  
  /** Notification of name/parent change from an object. */
  void object_moved(Object * pObj)
  {
    // url/tree changed, make sure dict is in sync
    
    // 1. remove object
    if (pObj->mRoot) pObj->mRoot->remove_object(pObj);
    
    // 1.2 FIXME: remove children !
    
    // 2. add with new key
    mObjects.set(pObj->url(), pObj);
    
    // 4. this is the new root
    pObj->mRoot = this;
  }
  
  /** Remove object from tree. */
  void remove_object(Object * pObj)
  {
    mObjects.remove_element(pObj);
  }
  
  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const std::string& pUrl)
  {
    return mObjects.get(pResult, pUrl);
  }
  
  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const char* pUrl)
  {
    return mObjects.get(pResult, std::string(pUrl));
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const std::string& pUrl)
  {
    Object * res = NULL;
    get(&res, pUrl);
    return res;
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const char * pUrl)
  {
    return find(std::string(pUrl));
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const String& pUrl)
  {
    return find(pUrl.string());
  }
  
  /** Return the class listing Object (create one if needed). */
  ClassListing * classes();
  
protected:
  THash<std::string, Object*> mObjects;   /**< Hash to find any object in the tree from its url. */
};

#endif // _ROOT_H_