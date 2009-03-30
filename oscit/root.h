#ifndef _ROOT_H_
#define _ROOT_H_
#include "oscit/object.h"
#include "oscit/call.h"
#include "oscit/receive.h"
#include "oscit/send.h"

#include "oscpack/ip/UdpSocket.h" // needed for IpEndpointName

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
  Root() : ground_(NULL), objects_(OBJECT_HASH_SIZE), osc_in_(NULL) {
    init();
  }

  Root(size_t hashSize) : ground_(NULL), objects_(hashSize), osc_in_(NULL) {
    init();
  }
  
  Root(void *ground) : ground_(ground), objects_(OBJECT_HASH_SIZE), osc_in_(NULL) {
    init();
  }

  Root(void *ground, size_t hashSize) : ground_(ground), objects_(hashSize), osc_in_(NULL) {
    init();
  }
  
  virtual ~Root();
  
  /** Start listening for incomming messages on a given port. */
  void open_port(uint port) {
    if (osc_in_) delete osc_in_;
    osc_in_ = new OscReceive(this, port);
  }
  
  /** Trigger the object located at the given url, passing nil as parameter. */
  const Value call(const char *url) {
    return call(std::string(url), gNilValue);
  }

  /** Trigger the object located at the given url, passing nil as parameter. */
  const Value call(std::string &url) {
    return call(url, gNilValue);
  }

  /** Trigger the object located at the given url with the given parameters. */
  const Value call(const char *url, const Value &val) {
    return call(std::string(url), val);
  }

  /** Trigger the object located at the given url with the given parameters. */
  const Value call(const std::string &url, const Value &val) {
    Object * target = find_or_build_object_at(url);
    
    if (!target) return ErrorValue(NOT_FOUND_ERROR, url);
    
    if (val.is_nil() || val.type_tag_id() == target->type_tag_id() || target->accept_any_type()) {
      return target->trigger(val);
    } else {
      return ErrorValue(BAD_REQUEST_ERROR, std::string("'").append(url).append("' (").append(target->info()).append(")."));
    }
  }

  /** Notification of name/parent change from an object. This method
   *  keeps the objects dictionary in sync. */
  void register_object(Object *obj) {
    // 1. remove object
    if (obj->root_) obj->root_->unregister_object(obj);
  
    // 2. add with new key
    objects_.set(obj->url(), obj);
  
    // 4. set 'this' as the object's new root
    obj->set_root(this);
  }

  /** Unregister an object from tree (forget about it). */
  void unregister_object(Object *obj) {
    objects_.remove_element(obj);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get_object_at(const std::string &url, Object **retval) {
    return objects_.get(retval, url);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get_object_at(const char *url, Object **retval) {
    return objects_.get(retval, std::string(url));
  }
  
  /** Return a pointer to the object located at a given url. NULL if not found. */
  Object * object_at(const std::string &url) {
    Object *res = NULL;
    get_object_at(url, &res);
    return res;
  }

  /** Return a pointer to the object located at a given url. NULL if not found. */
  Object * object_at(const char *url) {
    return object_at(std::string(url));
  }
  
  /** Find the object at the given url. Before raising a 404 error, we try to find a 'not_found'
   *  handler that could build the resource.
   */
  Object * find_or_build_object_at(const std::string &url) {
    Object * object = object_at(url);
    
    if (object == NULL) {
      size_t pos = url.rfind("/");
      if (pos != std::string::npos) {
        /** call 'not_found' handler in parent. */
        Object * parent = find_or_build_object_at(url.substr(0, pos));
        if (parent != NULL) {
          return parent->build_child(url.substr(pos+1));
        }
      }
    }
    return object;
  }
  
  /** Find the object at the given url. Before raising a 404 error, we try to find a 'not_found'
   *  handler that could build the resource.
   */
  Object * find_or_build_object_at(const char *url) {
    return find_or_build_object_at(std::string(url));
  }
  
  /* ======================= META METHODS HELPERS ===================== */
  
  /** Add a new satellite to the list of observers. This method is the implementation
   *  of "/.register".
   * TODO: how to get the IP without checking for "/.register" in the 'receive' method ?
   * FIXME: implement TTL (time to live)
   * FIXME: avoid duplicates (??)
   */
  void register_observer(const IpEndpointName &observer);
  
  void receive(const std::string &url, const Value &val) {
    //FIX Value res;
    
    //FIX if (pParams.is_error()) {
    //FIX   res = pParams;
    //FIX } else {
    //FIX   res = call(url,pParams);
    //FIX }
    //FIX send_reply(url, res);
  }
  
  /** Send reply to received message. 
   *  @param remote_endpoint can be NULL if this is triggered by an internal call.
   */
  void send_reply(UdpSocket *socket, const IpEndpointName *remote_endpoint, const std::string &url, const Value &val);
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   */
  void send(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val) {
    send(remote_endpoint, url.c_str(), val);
  }
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   */
  void send(const IpEndpointName &remote_endpoint, const char *url, const Value &val);
  
  void * ground_;                 /**< Context pointer for objects in the tree. */
 protected:
  THash<std::string, Object*> objects_;   /**< Hash to find any object in the tree from its url. */

 private:
  void init();
   
  OscReceive * osc_in_;           /**< Listening socket. */
  std::list<IpEndpointName> observers_; /**< List of satellites that have registered to get return values back. */
};
  
} // namespace oscit
#endif // _ROOT_H_