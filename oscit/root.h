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
  Root() : Object("",""), ground_(NULL), objectsHash_(OBJECT_HASH_SIZE), mOscIn(NULL)
  {
    root_ = this;
    registerObject(this);
    objectsHash_.set(std::string(""), this);
  }

  Root(size_t hashSize) : Object("",""), ground_(NULL), objectsHash_(hashSize), mOscIn(NULL)
  {
    root_ = this;
    objectsHash_.set(std::string(""), this);
  }
  
  Root(void * ground) : Object("",""), ground_(ground), objectsHash_(OBJECT_HASH_SIZE), mOscIn(NULL)
  {
    root_ = this;
    registerObject(this);
    objectsHash_.set(std::string(""), this);
  }

  Root(void * ground, size_t hashSize) : Object("",""), ground_(ground), objectsHash_(hashSize), mOscIn(NULL)
  {
    root_ = this;
    objectsHash_.set(std::string(""), this);
  }
  
  virtual ~Root();

  virtual void clear()
  {
    this->Object::clear();
  }
  
  void openPort(uint port)
  {
    if (mOscIn) delete mOscIn;
    mOscIn = new OscReceive(this, port);
  }
  
  /** Execute the default operation for an object. */
  const Value call (const char* pUrl)
  {
    return call(std::string(pUrl), "", gNilValue);
  }

  /** Execute the default operation for an object. */
  const Value call (std::string& pUrl)
  {
    return call(pUrl, "", gNilValue);
  }

  /** Execute the default operation for an object. */
  const Value call (const char* pUrl, const char * typeTag, const Value val)
  {
    return call(std::string(pUrl), typeTag, val);
  }

  /** Execute the default operation for an object. */
  const Value call (const std::string& pUrl, const char * typeTag, const Value val)
  {
    Object * target = NULL;
    size_t pos;
    std::string url = pUrl;
    
    bool not_found = false;
    
    while (!get(&target, url) && url != "") {
      not_found = true;
      pos = url.rfind("/");
      if (pos == std::string::npos)
        url = "";
      else
        url = url.substr(0, pos);
    
    }
  
    if (!target) return notFound(pUrl, typeTag, val);
    
    if (not_found) return target->notFound(pUrl, typeTag, val);
    
    // FIXME: return type... target->typeTagStr_ing.c_str()
    return target->typeTag_ == hashId(typeTag) ? target->trigger(val) : target->trigger(gNilValue);
  }

  /** Notification of name/parent change from an object. */
  void registerObject(Object * pObj)
  {
    // url/tree changed, make sure dict is in sync
    // std::cout << pObj->url() << " registered !\n";
  
    // 1. remove object
    if (pObj->root_) pObj->root_->unregisterObject(pObj);
  
    // 2. add with new key
    objectsHash_.set(pObj->url(), pObj);
  
    // 4. this is the new root
    pObj->setRoot(this);
  }

  /** Unregister an object from tree. */
  void unregisterObject(Object * pObj)
  {
    objectsHash_.remove_element(pObj);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const std::string& pUrl)
  {
    return objectsHash_.get(pResult, pUrl);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const char* pUrl)
  {
    return objectsHash_.get(pResult, std::string(pUrl));
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
  
  void receive(const std::string& pUrl, const char * typeTag, const Value val)
  {
    //FIX Value res;
    
    //FIX if (pParams.is_error()) {
    //FIX   res = pParams;
    //FIX } else {
    //FIX   res = call(pUrl,pParams);
    //FIX }
    //FIX send_reply(pUrl, res);
  }
  
  inline void send_reply(const std::string& pUrl, const char * typeTag, const Value val)
  {
    OscSend::send_all(mControllers, pUrl, typeTag, val);
  }
  
  void * ground_;                   /**< Context pointer for objects in the tree. */
protected:
  THash<std::string, Object*> objectsHash_;   /**< Hash to find any object in the tree from its url. */

private:
  OscReceive * mOscIn;              /**< Listening socket. */
  std::list<OscSend*> mControllers; /**< List of satellites that have registered to get return values back. */
};
  
} // namespace oscit
#endif // _ROOT_H_