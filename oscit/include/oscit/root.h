#ifndef OSCIT_INCLUDE_OSCIT_ROOT_H_
#define OSCIT_INCLUDE_OSCIT_ROOT_H_
#include "oscit/object.h"
#include "oscit/command.h"

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
class Root : public Object
{
 public: 
  /** Class signature. */
  TYPED("Object.Root")
  
  Root() : objects_(OBJECT_HASH_SIZE) {
    init();
  }
  
  Root(const char *name) : Object(name), objects_(OBJECT_HASH_SIZE) {
    init();
  }
  
  Root(const Value &type) : Object(type), objects_(OBJECT_HASH_SIZE) {
    init();
  }
  
  Root(const char *name, const Value &type) : Object(name, type), objects_(OBJECT_HASH_SIZE) {
    init();
  }
  
  Root(size_t hashSize) : objects_(hashSize) {
    init();
  }
  
  Root(size_t hashSize, const char *name) : Object(name), objects_(hashSize) {
    init();
  }
  
  Root(size_t hashSize, const Value &type) : Object(type), objects_(hashSize) {
    init();
  }
  
  Root(size_t hashSize, const char *name, const Value &type) : Object(name, type), objects_(hashSize) {
    init();
  }
  
  virtual ~Root() {
    clear();
    root_ = NULL; // avoid call to unregister_object in ~Object
  }
  
  void clear() {
    while (!commands_.empty()) {
      Command *command = commands_.front();
      command->kill();
      command->set_root(NULL); // avoid call to unregister_command in ~Command
      
      delete command;
      commands_.pop_front();
    }
    this->Object::clear();
  }
  
  /** Start listening for incomming messages from the given command. */
  template<class T>
  T * adopt_command(T *command, bool start = true) {
    if (command_for_protocol(command->protocol()) != NULL) {
      // we already have a command for this protocol. Do not adopt.
      delete command;
      return NULL;
    }
    command->set_root(this);
    commands_.push_back(command);
    if (start) command->listen();
    return command;
  }
  
  void unregister_command(Command *command) {
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

  /** Trigger the object in the local tree located at the given url. */
  const Value call(const std::string &path, const Value &val) {
    Value error;
    Object * target = find_or_build_object_at(path, &error);
    
    // FIXME: possible problem here: target deleted by other thread before call..
    // a solution is to use a purgatory for suppressed objects where they are kept for a few seconds.
    
    if (!target) {
      return error;
    }
    
    return call(target, val);
  }
  
  /** Send value to given url (can be local or remote). You should use 'call' for local urls (faster). */
  const Value send(const Url &url, const Value &val) {
    Value error;
    Object * target = object_at(url, &error);
    
    // FIXME: possible problem here: target deleted by other thread before call..
    // a solution is to use a purgatory for suppressed objects where they are kept for a few seconds.
    
    if (!target) {
      return error;
    }
    
    return call(target, val);
  }
  
  /** Find any object (local or remoate). */
  Object *object_at(const Url &url, Value *error) {
    if (url.path() == "") {
      // bad url
      error->set(BAD_REQUEST_ERROR, std::string("Could not parse url '").append(url.str()).append("'."));
      return NULL;
    } else if (url.protocol() == "") {
      // local object
      return find_or_build_object_at(url.str(), error);
    } else {
      // remote
      // find command for given protocol
      Command * cmd = command_for_protocol(url.protocol());
      if (cmd) {
        return cmd->remote_object(url, error);
      } else {
        error->set(BAD_REQUEST_ERROR, std::string("No command to handle '").append(url.protocol()).append("' protocol."));
        return NULL;
      }
    }
  }
  
  inline const Value call(Object *target, const Value &val) {
    if (val.is_empty()) return call(target, gNilValue);
    if (target->can_receive(val)) {
      return target->safe_trigger(val);
    } else {
      Value type = call("/.type", Value(target->url()));
      return ErrorValue(BAD_REQUEST_ERROR, std::string("'").append(target->url()).append("' (").append(target->type().last().str()).append(")."));
    }
  }

  /** Notification of name/parent change from an object. This method
   *  keeps the objects dictionary in sync.
   */
  void register_object(Object *obj) {
    // add object to objects dictionary
    objects_.set(obj->url(), obj);
  }

  /** Unregister an object from tree (forget about it). */
  void unregister_object(Object *obj) {
    objects_.remove_element(obj);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get_object_at(const std::string &url, Object **retval) {
    return objects_.get(url, retval);
  }

  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get_object_at(const char *url, Object **retval) {
    return objects_.get(std::string(url), retval);
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
  Object *find_or_build_object_at(const std::string &url, Value *error) {
    Object *object = do_find_or_build_object_at(url, error);
    
    if (object == NULL && (error->is_empty() || (error->is_error() && error->error_code() == NOT_FOUND_ERROR))) {
      error->set(NOT_FOUND_ERROR, url);
    }
    
    return object;
  }
  
  /** Find the object at the given url. Before raising a 404 error, we try to find a 'not_found'
   *  handler that could build the resource.
   */
  inline Object * find_or_build_object_at(const char *url, Value *error) {
    return find_or_build_object_at(std::string(url), error);
  }
  
  /* ======================= META METHODS HELPERS ===================== */
  
  /** Send a reply to all commands so they pass it further to their observers. */
  void notify_observers(const char *url, const Value &val, const Command *skip_command = NULL) {
    std::list<Command*>::iterator it;
    std::list<Command*>::iterator end = commands_.end();
    for (it = commands_.begin(); it != end; ++it) {
      if (*it != skip_command) {
        (*it)->notify_observers(url, val);
      }
    }
  }
  
  Command *command_for_protocol(const std::string& protocol) {
    std::list<Command*>::iterator it;
    std::list<Command*>::iterator end = commands_.end();
    for (it = commands_.begin(); it != end; ++it) {
      if ((*it)->protocol() == protocol) {
        return *it;
      }
    }
    return NULL;
  }
  
 protected:
  THash<std::string, Object*> objects_;   /**< Hash to find any object in the tree from its url. */

 private:
  Object * do_find_or_build_object_at(const std::string &path, Value *error) {
    Object * object = object_at(path);
    
    if (object == NULL) {
      // ask parents to build
      size_t pos = path.rfind("/");
      if (pos != std::string::npos) {
        /** call 'build_child' handler in parent. */
        Object * parent = do_find_or_build_object_at(path.substr(0, pos), error);
        if (parent != NULL && (object = object_at(path)) == NULL) {
          return parent->build_child(path.substr(pos+1), error);
        }
      }
    }
    return object;
  }
   
  void init();
  
  std::list<Command *> commands_;    /**< Listening commands (only one allowed per protocol). */
};
  
} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ROOT_H_
