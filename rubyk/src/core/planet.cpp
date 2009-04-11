#include "planet.h"
#include "class_finder.h"
#include "node.h"
#include "command.h"

Planet::~Planet() {
  Command * child;

  while(!commands_.empty()) {
    child = commands_.front();
    // join
    child->close(); // joins pthread
    commands_.pop();
  }
  
  pending_links_.clear();
  clear();
}

/** Return the class listing Object (create one if needed). */
ClassFinder * Planet::classes() {
  BaseObject * obj = object_at(CLASS_ROOT);
  if (!obj) {
    // create ClassFinder
    ClassFinder * cf = adopt(new ClassFinder(std::string(CLASS_ROOT).substr(1, std::string::npos), DEFAULT_OBJECTS_LIB_PATH));
    return cf;
  }
  return TYPE_CAST(ClassFinder, obj);
}

/** Start listening to a command. */
void Planet::listen_to_command (Command &command)
{
  int ret;
  pthread_t id;
  command.set_worker(this);

  ret = pthread_create( &id, NULL, &Command::call_do_listen, &command);
  command.set_thread_id(id);
  // FIXME: check for error from 'ret'

  commands_.push(&pCommand);
}

/** Create a new object from a class name. Calls "/class/ClassName/new URL PARAMS". */
const Value Planet::new_object(const char * url, const char * class_name, const Value &params) {
  ListValue list;
  list.push_back(url);
  list.push_back(params);
  return call(std::string(classes()->url()).append("/").append(class_name).append("/new"), list);
}

const Value Planet::create_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_object) {
  //std::cout << "pending " << from << "("<< from_port << ":" << from_port.length() << ")" << " --> " << to_object << "("<< to_port << ")" << std::endl;
  
  std::string url(from);
  if (from_port != "")
    url.append("/out/").append(from_port).append("/link");
  else
    url.append("/out/link"); // link from first outlet
    
  Value param(to_object);
  
  if (to_port != "")
    param.append("/in/").append(to_port);
  else
    param.append("/in"); // link to first inlet
  
  // try to create link
  Value res = call(url, param);
  if (res.is_error()) pending_links_.push_back(Call(url,param));
  return res;
}

const Value Planet::remove_link(const std::string &from, const std::string &from_port, const std::string &to_port, const std::string &to_object) { 
  
  std::string url(from);
  if (from_port != "")
    url.append("/out/").append(from_port).append("/unlink");
  else
    url.append("/out/unlink"); // unlink first outlet
    
  String param(to_object);
  
  if (to_port != "")
    param.append("/in/").append(to_port);
  else
    param.append("/in"); // unlink first inlet
    
  // try to remove link
  Value res = call(url, param);
  return res;
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Planet::create_pending_links() {
  std::list<oscit::Call>::iterator it,end;
  end = pending_links_.end();
  it  = pending_links_.begin();
  Value res;
  
  while(it != end) {
    //std::cout << "PENDING " << it->mUrl << " => " << it->mParam << std::endl;
    res = call(it->mUrl, it->mParam);
    if (res.is_string()) {
      it = pending_links_.erase(it);  // call succeeded
    } else
      it++;
  }
  // return list of created links ?
  return gNilValue;
}
