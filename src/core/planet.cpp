/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "rubyk/node.h"
#include "rubyk/class_finder.h"
#include "rubyk/text_command.h"
#include "rubyk/planet.h"

void Planet::init() {
  set_context(&worker_);

  // build application methods
  //           /.inspect
  adopt(new TMethod<Planet, &Planet::inspect>(this, Url(INSPECT_URL).name(), StringIO("Returns inspect information on a given url.")));
  //          /class
  classes_ = adopt(new ClassFinder(Url(CLASS_URL).name(), DEFAULT_OBJECTS_LIB_PATH));
  //          /rubyk
  Object *rubyk = adopt(new Object(Url(RUBYK_URL).name()));
  //          /rubyk/link [[["","source url"],["", "target url"]], "Create a link between two urls."]
  rubyk->adopt(new TMethod<Planet, &Planet::link>(this, Url(LINK_URL).name(), JsonValue("[['','', ''],'url','op','url','Update a link between the two provided urls. Operations are '=>' (link) '||' (unlink) or '?' (pending).']")));
  //          /rubyk/quit
  rubyk->adopt(new TMethod<Planet, &Planet::quit>(this, Url(QUIT_URL).name(), NilIO("Stop all operations and quit.")));
}

// typetag: "ss" (inlet, outlet)
const Value Planet::link(const Value &val) {
  // std::cout << "link: " << val << std::endl;
  if (val.is_nil()) {
    return create_pending_links();
  }

  Value error;
  ObjectHandle source, target;
  if (!get_object_at(Url(val[0].str()), &error, &source) || !get_object_at(Url(val[2].str()), &error, &target)) {
    // not found
    if (val[1].str() == "=>") {
      return add_pending_link(val);
    } else if (val[1].str() == "||") {
      // remove from pending links
      return remove_pending_link(val);
    }
    return val;
  }

  Slot   *slot = source.type_cast<Slot>();
  ObjectHandle out, outlet;
  if (slot != NULL) {
    return val[1].str() == "||" ? slot->unlink(val[2]) : slot->link(val[2]);
  } else if (source->get_child("out", &out) && out->first_child(&outlet)) {
    // was a link to/from default slots: /met/out --> /counter/in
    if ( (slot = outlet.type_cast<Slot>()) ) {
      return val[1].str() == "||" ? slot->unlink(val[2]) : slot->link(val[2]);
    } else {
      return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(slot->url()).append("' does not support links (using first child of '").append(source->url()).append("')."));
    }
  } else {
    return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (not an Outlet, Inlet or Node)."));
  }
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
// FIXME: thread safety !
const Value Planet::create_pending_links() {
  std::list<Call>::iterator it  = pending_links_.begin();
  std::list<Call>::iterator end = pending_links_.end();

  Value res;
  Value list;

  while (it != end) {
    res = it->trigger_call(this);
    if ((res.type_id() == SelectIOTypeId && res[1].str() == "=>") || res.is_error()) {
      list.push_back(res);
      it = pending_links_.erase(it);  // call succeeded or definitely failed
    } else {
      ++it;
    }
  }
  // return list of created links
  return list;
}


// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Planet::remove_pending_link(const Value &val) {
  std::list<Call>::iterator it  = pending_links_.begin();
  std::list<Call>::iterator end = pending_links_.end();

  Value res;
  res.set_nil();

  while (it != end) {

    if (it->param_[0].str() == val[0].str() && it->param_[2].str() == val[2].str()) {
      res = it->param_;
      res[1].set("||");
      it = pending_links_.erase(it);  // removed
      break;
    } else {
      ++it;
    }
  }
  return res;
}

const Value Planet::inspect(const Value &val) {
  if (!val.is_string()) return Value(BAD_REQUEST_ERROR, "Bad arguments:'inspect' should be called with an url.");
  Value res;
  ObjectHandle object;
  if (!find_or_build_object_at(val.str(), &res, &object)) return res;
  Node *node = object.type_cast<Node>();
  if (!node) return Value(BAD_REQUEST_ERROR, std::string("Bad target '").append(object->url()).append("':inspect only works on Nodes (class is '").append(object->class_path()).append("')."));
  return node->do_inspect();
}