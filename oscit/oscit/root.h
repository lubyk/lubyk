#ifndef _OSCIT_ROOT_H_
#define _OSCIT_ROOT_H_
#include "oscit/base_object.h"
#include "oscit/base_command.h"

namespace oscit {
  
/** Size of the object hash table. */
#define OBJECT_HASH_SIZE 10000

/** Root object. You can only start new trees with root objects. 

In case you intend to call elements in the object tree from different
threads, you should either manage your own mutex locks on each objects
or use a context (Mutex class or subclass). Contexts ease inter-object
communication be requiring locks/unlocks between groups of objects. Not
for every object.

*/
class Root : public BaseObject
{
 public:
  Root() : objects_(OBJECT_HASH_SIZE) {
    init();
  }

  Root(size_t hashSize) : objects_(hashSize) {
    init();
  }
  
  virtual ~Root() {
    clear();
    root_ = NULL; // avoid call to unregister_object in ~BaseObject
  }
  
  void clear() {
    while (!commands_.empty()) {
      BaseCommand *command = commands_.front();
      command->kill();
      command->set_root(NULL); // avoid call to unregister_command in ~BaseCommand
      delete command;
      commands_.pop_front();
    }
    this->BaseObject::clear();
  }
  
  /** Start listening for incomming messages from the given command. */
  template<class T>
  T * adopt_command(T *command) {
    if (command_for_protocol(command->protocol()) != NULL) {
      // we already have a command for this protocol. Do not adopt.
      delete command;
      return NULL;
    }
    command->set_root(this);
    commands_.push_back(command);
    command->listen();
    return command;
  }
  
  void unregister_command(BaseCommand *command) {
    commands_.remove(command);
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
    Value error;
    BaseObject * target = find_or_build_object_at(url, &error);
    
    // FIXME: possible problem here: target deleted by other thread before call..
    // a solution is to use a purgatory for suppressed objects where they are kept for a few seconds.
    
    if (!target) {
      return error;
    }
    
    return call(target, val);
  }
  
  inline const Value call(BaseObject *target, const Value &val) {
    if (val.is_nil() || val.type_tag_id() == target->type_tag_id() || target->accept_any_type()) {
      return target->safe_trigger(val);
    } else {
      return ErrorValue(BAD_REQUEST_ERROR, std::string("'").append(target->url()).append("' (").append(target->info()).append(")."));
    }
  }

  /** Notification of name/parent change from an object. This method
   *  keeps the objects dictionary in sync. */
  void register_object(BaseObject *obj) {
    // 1. remove object
    if (obj->root_) obj->root_->unregister_object(obj);
  
    // 2. add with new key
    objects_.set(obj->url(), obj);
  
    // 4. set 'this' as the object's new root
    obj->set_root(this);
  }

  /** Unregister an object from tree (forget about it). */
  void unregister_object(BaseObject *obj) {
    objects_.remove_element(obj);
  }

  /** Find a pointer to an BaseObject from its url. Return false if the object is not found. */
  bool get_object_at(const std::string &url, BaseObject **retval) {
    return objects_.get(url, retval);
  }

  /** Find a pointer to an BaseObject from its url. Return false if the object is not found. */
  bool get_object_at(const char *url, BaseObject **retval) {
    return objects_.get(std::string(url), retval);
  }
  
  /** Return a pointer to the object located at a given url. NULL if not found. */
  BaseObject * object_at(const std::string &url) {
    BaseObject *res = NULL;
    get_object_at(url, &res);
    return res;
  }

  /** Return a pointer to the object located at a given url. NULL if not found. */
  BaseObject * object_at(const char *url) {
    return object_at(std::string(url));
  }
  
  /** Find the object at the given url. Before raising a 404 error, we try to find a 'not_found'
   *  handler that could build the resource.
   */
  BaseObject *find_or_build_object_at(const std::string &url, Value *error) {
    BaseObject *object = do_find_or_build_object_at(url, error);
    
    if (object == NULL && (error->is_nil() || (error->is_error() && error->error_code() == NOT_FOUND_ERROR))) {
      error->set(NOT_FOUND_ERROR, url);
    }
    
    return object;
  }
  
  /** Find the object at the given url. Before raising a 404 error, we try to find a 'not_found'
   *  handler that could build the resource.
   */
  inline BaseObject * find_or_build_object_at(const char *url, Value *error) {
    return find_or_build_object_at(std::string(url), error);
  }
  
  /* ======================= META METHODS HELPERS ===================== */
  
  /** Send a reply to all commands so they pass it further to their observers. */
  void notify_observers(const char *url, const Value &val, const BaseCommand *skip_command = NULL) {
    std::list<BaseCommand*>::iterator it;
    std::list<BaseCommand*>::iterator end = commands_.end();
    for (it = commands_.begin(); it != end; ++it) {
      if (*it != skip_command) {
        (*it)->notify_observers(url, val);
      }
    }
  }
  
  BaseCommand *command_for_protocol(const std::string& protocol) {
    std::list<BaseCommand*>::iterator it;
    std::list<BaseCommand*>::iterator end = commands_.end();
    for (it = commands_.begin(); it != end; ++it) {
      if ((*it)->protocol() == protocol) {
        return *it;
      }
    }
    return NULL;
  }
  
 protected:
  THash<std::string, BaseObject*> objects_;   /**< Hash to find any object in the tree from its url. */

 private:
  BaseObject * do_find_or_build_object_at(const std::string &path, Value *error) {
    BaseObject * object = object_at(path);
    
    if (object == NULL) {
      // is it a local object ?
      Url url(path);
      if (url.path() == "") {
        // bad url
        error->set(BAD_REQUEST_ERROR, std::string("Could not parse url '").append(path).append("'."));
      } else if (url.protocol() == "") {
        // find locally
        size_t pos = path.rfind("/");
        if (pos != std::string::npos) {
          /** call 'build_child' handler in parent. */
          BaseObject * parent = find_or_build_object_at(path.substr(0, pos), error);
          if (parent != NULL && (object = object_at(path)) == NULL) {
            return parent->build_child(path.substr(pos+1), error);
          }
        }
      } else {
        std::cout << "Remote: " << url << std::endl;
        // remote object
        // find command for given protocol
        BaseCommand * cmd = command_for_protocol(url.protocol());
        if (cmd) {
          return cmd->remote_object(url);
        } else {
          error->set(BAD_REQUEST_ERROR, std::string("No command to handle '").append(url.protocol()).append("' protocol."));
          return NULL;
        }
      }
    }
    return object;
  }
   
  void init();
  
  std::list<BaseCommand *> commands_;    /**< Listening commands (only one allowed per protocol). */
};
  
} // namespace oscit
#endif // _OSCIT_ROOT_H_