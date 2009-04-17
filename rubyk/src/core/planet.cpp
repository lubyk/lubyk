#include "planet.h"
#include "class_finder.h"
#include "node.h"
#include "text_command.h"

void Planet::init() {
  // build application methods
  
  //          /class
  adopt(new ClassFinder(Url(CLASS_URL).name(), DEFAULT_OBJECTS_LIB_PATH));
  //          /rubyk
  Object * rubyk = adopt(new Object(Url(RUBYK_URL).name()));
  //          /rubyk/link
  rubyk->adopt(new TMethod<Planet, &Planet::link>(this, Url(LINK_URL).name(), H("ss"), "Create a link between to the two provided urls."));
  //          /rubyk/unlink
  rubyk->adopt(new TMethod<Planet, &Planet::unlink>(this, Url(UNLINK_URL).name(), H("ss"), "Remove link between to the two provided urls."));
  //          /rubyk/quit
  rubyk->adopt(new TMethod<Planet, &Planet::quit>(this, Url(QUIT_URL).name(), H(""), "Stop all operations and quit."));
}

// typetag: "ss" (inlet, outlet)
const Value Planet::link(const Value &val) {
  if (val.is_nil()) {
    return create_pending_links();
  }
  
  // link: "/met/out/tempo" ---> "/counter/in/bang"
  
  Value error;
  Object *source = object_at(Url(val[0].str()), &error);
  if (error.is_error()) return error;
  
  Slot   *slot = TYPE_CAST(Slot, source);
  Object *object;
  if (slot != NULL) {
    return slot->link(val[1]);
  } else if ( (object = source->first_child()) ) {
    // was a link default slots: /met/out --> /counter/in
    if ( (slot = TYPE_CAST(Slot, object)) ) {
      return slot->link(val[1]);
    } else {
      return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(slot->url()).append("' does not support links (using first child of '").append(source->url()).append("')."));
    }
  } else {
    return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (not an Outlet, Inlet or Node)."));
  }
}

const Value Planet::unlink(const Value &val) { 
  
  // unlink: "/met/out/tempo" // "/counter/in/bang"
  
  Value error;
  Object *source = object_at(Url(val[0].str()), &error);
  if (error.is_error()) return error;
  
  Slot *slot = TYPE_CAST(Slot, source);
  if (slot != NULL) {
    return slot->unlink(val[1]);
  } else {
    return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (not an Outlet, Inlet or Node)."));
  }
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Planet::create_pending_links() {
  std::list<Call>::iterator it  = pending_links_.begin();
  std::list<Call>::iterator end = pending_links_.end();
  
  Value res;
  Value list;
  
  while (it != end) {
    //std::cout << "PENDING " << it->mUrl << " => " << it->mParam << std::endl;
    res = it->trigger(this);
    if (res.is_string()) {
      list.push_back(res);
      it = pending_links_.erase(it);  // call succeeded
    } else {
      it++;
    }
  }
  // return list of created links
  return list;
}

