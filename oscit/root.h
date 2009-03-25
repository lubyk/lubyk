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
  Root() : ground_(NULL), objects_(OBJECT_HASH_SIZE), osc_in_(NULL) {
    root_ = this;
    register_object(this);
    objects_.set(std::string(""), this);
  }

  Root(size_t hashSize) : ground_(NULL), objects_(hashSize), osc_in_(NULL) {
    root_ = this;
    objects_.set(std::string(""), this);
  }
  
  Root(void *ground) : ground_(ground), objects_(OBJECT_HASH_SIZE), osc_in_(NULL) {
    root_ = this;
    register_object(this);
    objects_.set(std::string(""), this);
  }

  Root(void *ground, size_t hashSize) : ground_(ground), objects_(hashSize), osc_in_(NULL) {
    root_ = this;
    objects_.set(std::string(""), this);
  }
  
  virtual ~Root();

  virtual void clear() {
    this->Object::clear();
  }
  
  void openPort(uint port) {
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
    Object * target = NULL;
    size_t pos;
    std::string tmp_url(url);
    
    bool is_not_found = false;
    
    while (!get_object_at(tmp_url, &target) && tmp_url != "") {
      // Before raising a 404 error, we try to find a '404' handler that
      // could build the resource or do something better then just sending
      // an error back.
      is_not_found = true;
      pos = tmp_url.rfind("/");
      if (pos == std::string::npos) {
        tmp_url = "";
      } else {
        tmp_url = tmp_url.substr(0, pos);
      }
    
    }
  
    if (!target) return not_found(url, val);
    
    if (is_not_found) return target->not_found(url, val);
    
    return val.type_tag_id() == target->type_tag_id() ? target->trigger(val) : target->trigger(gNilValue);
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
  
  /* ======================= META METHODS ===================== */
  
  /** Add a new satellite to the list of observers. This method is the implementation
   *  of "/.register".
   * TODO: how to get the IP without checking for "/.register" in the 'receive' method ?
   * FIXME: implement TTL (time to live)
   * FIXME: avoid duplicates (??)
   */
  void register_observer(OscSend *observer) {
    observers_.push_back(observer);
  }
  
  void receive(const std::string &url, const Value &val) {
    //FIX Value res;
    
    //FIX if (pParams.is_error()) {
    //FIX   res = pParams;
    //FIX } else {
    //FIX   res = call(url,pParams);
    //FIX }
    //FIX send_reply(url, res);
  }
  
  inline void send_reply(const std::string &url, const Value &val) {
    OscSend::send_all(observers_, url, val);
  }
  
  void * ground_;                 /**< Context pointer for objects in the tree. */
 protected:
  THash<std::string, Object*> objects_;   /**< Hash to find any object in the tree from its url. */

 private:
  OscReceive * osc_in_;           /**< Listening socket. */
  std::list<OscSend*> observers_; /**< List of satellites that have registered to get return values back. */
};
  
} // namespace oscit
#endif // _ROOT_H_