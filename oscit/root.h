#ifndef _ROOT_H_
#define _ROOT_H_
#include "oscit/object.h"
#include "oscit/call.h"
#include "oscit/receive.h"
#include "oscit/send.h"

class Command;
class Event;
class Node;

namespace oscit {
  
/** Size of the object hash table. */
#define OBJECT_HASH_SIZE 10000

/** Root object. You can only start new trees with root objects. */
class Root : public Object
{
public:
  Root() : Object("",""), mGround(NULL), mObjects(OBJECT_HASH_SIZE), mOscIn(NULL)
  {
    mRoot = this;
    register_object(this);
    mObjects.set(std::string(""), this);
  }

  Root(size_t pHashSize) : Object("",""), mGround(NULL), mObjects(pHashSize), mOscIn(NULL)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
  }
  
  Root(void * pGround) : Object("",""), mGround(pGround), mObjects(OBJECT_HASH_SIZE), mOscIn(NULL)
  {
    mRoot = this;
    register_object(this);
    mObjects.set(std::string(""), this);
  }

  Root(void * pGround, size_t pHashSize) : Object("",""), mGround(pGround), mObjects(pHashSize), mOscIn(NULL)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
  }
  
  virtual ~Root();

  virtual void clear()
  {
    this->Object::clear();
  }
  
  void open_port(uint pPort)
  {
    if (mOscIn) delete mOscIn;
    mOscIn = new OscReceive(this, pPort);
  }
  
  /** Execute the default operation for an object. */
  const Value call (const char* pUrl)
  {
    return call(std::string(pUrl), "", NULL);
  }

  /** Execute the default operation for an object. */
  const Value call (std::string& pUrl)
  {
    return call(pUrl, "", NULL);
  }

  /** Execute the default operation for an object. */
  const Value call (const char* pUrl, const char * pTypeTag, const Value val)
  {
    return call(std::string(pUrl), pTypeTag, val);
  }

  /** Execute the default operation for an object. */
  const Value call (const std::string& pUrl, const char * pTypeTag, const Value val)
  {
    Object * target = NULL;
    size_t pos;
    std::string url = pUrl;
    
    bool notFound = false;
    
    while (!get(&target, url) && url != "") {
      notFound = true;
      pos = url.rfind("/");
      if (pos == std::string::npos)
        url = "";
      else
        url = url.substr(0, pos);
    
    }
  
    if (!target) return not_found(pUrl, pTypeTag, val);
    
    if (notFound) return target->not_found(pUrl, pTypeTag, val);
    
    // FIXME: return type... target->mTypeTagString.c_str()
    return target->mTypeTag == hashId(pTypeTag) ? target->trigger(val) : target->trigger(NULL);
  }

  /** Notification of name/parent change from an object. */
  void register_object(Object * pObj)
  {
    // url/tree changed, make sure dict is in sync
    // std::cout << pObj->url() << " registered !\n";
  
    // 1. remove object
    if (pObj->mRoot) pObj->mRoot->unregister_object(pObj);
  
    // 2. add with new key
    mObjects.set(pObj->url(), pObj);
  
    // 4. this is the new root
    pObj->set_root(this);
  }

  /** Unregister an object from tree. */
  void unregister_object(Object * pObj)
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
  
  /* ======================= META METHODS ===================== */
  
  /** This method responds to /.reply_to 
    * TODO: how to get the IP without checking for "/.reply_to" in the 'receive' method ?*/
  void reply_to(OscSend * pSatellite)
  {
    // FIXME: avoid duplicates, implement TTL...
    mControllers.push_back(pSatellite);
  }
  
  void receive(const std::string& pUrl, const char * pTypeTag, const Value pParams)
  {
    //FIX Value res;
    
    //FIX if (pParams.is_error()) {
    //FIX   res = pParams;
    //FIX } else {
    //FIX   res = call(pUrl,pParams);
    //FIX }
    //FIX send_reply(pUrl, res);
  }
  
  inline void send_reply(const std::string& pUrl, const char * pTypeTag, const Value pVal)
  {
    OscSend::send_all(mControllers, pUrl, pTypeTag, pVal);
  }
  
  void * mGround;                   /**< Context pointer for objects in the tree. */
protected:
  THash<std::string, Object*> mObjects;   /**< Hash to find any object in the tree from its url. */

private:
  OscReceive * mOscIn;              /**< Listening socket. */
  std::list<OscSend*> mControllers; /**< List of satellites that have registered to get return values back. */
};
  
} // namespace oscit
#endif // _ROOT_H_