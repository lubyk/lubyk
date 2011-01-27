/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#include "lubyk/node.h"
#include "lubyk/class_finder.h"
#include "lubyk/text_command.h"
#include "lubyk/planet.h"
#include "lubyk/patch_method.h"

namespace rk {

void Planet::init() {
  set_context(&worker_);

  // set defaults
  // TODO: remove x,y coord once we store the "device composition" in the remote GUI.
  attributes_.set(Oscit::VIEW, Oscit::POS_X,  10);
  attributes_.set(Oscit::VIEW, Oscit::POS_Y,  10);

  attributes_.set(Oscit::VIEW, Oscit::WIDTH,  500);
  attributes_.set(Oscit::VIEW, Oscit::HEIGHT, 300);

  // build application methods

  //           /.inspect
  // FIXME: remove /.inspect...
  adopt(new TMethod<Planet, &Planet::inspect>(this, Url(INSPECT_URL).name(), Oscit::string_io("Returns inspect information on a given url.")));

  //          /class
  classes_ = adopt(new ClassFinder(Url(CLASS_URL).name(), DEFAULT_OBJECTS_LIB_PATH));

  //          /lubyk
  Object *lubyk = adopt(new Object(Url(LUBYK_URL).name()));

  //          /lubyk/link [[["","source url"],["", "target url"]], "Create a link between two urls."]
  lubyk->adopt(new TMethod<Planet, &Planet::link>(this, Url(LINK_URL).name(), Oscit::io("Update a link between the two provided urls. Arguments are (url, op, url). Operations are '=>' (link) '||' (unlink) or '?' (pending).", "link operation", "sss")));

  //          /lubyk/quit
  lubyk->adopt(new TMethod<Planet, &Planet::quit>(this, Url(QUIT_URL).name(), Oscit::bang_io("Stop all operations and quit.")));
}

bool Planet::expose_views(const std::string &path, Value *error) {
  if (this->Root::expose_views(path, error)) {
    // /views/patch
    ObjectHandle views;
    if (get_object_at(VIEWS_PATH, &views)) {
      views->adopt(new PatchMethod(Url(PATCH_VIEW_URL).name(), "patch.rks"));
    } else {
      std::cerr << "Could not expose patch view: " << VIEWS_PATH << " not found.\n";
    }
    return true;
  }

  return false;
}

// typetag: "ss" (inlet, outlet)
const Value Planet::link(const Value &val) {
  if (val.is_nil()) return gNilValue; // no current value ? return pending links ?
  // std::cout << "link: " << val << std::endl;
  if (val.is_bang()) {
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

  Outlet *source_outlet = source.type_cast<Outlet>();

  ObjectHandle out, outlet;

  if (source_outlet != NULL) {
    return val[1].str() == "||" ? source_outlet->unlink(val[2]) : source_outlet->link(val[2]);
  } else if (source->get_child(Lubyk::NODE_OUT_KEY, &out) && out->first_child(&outlet)) {
    // was a link to/from default slots: /met --> /counter
    if ( (source_outlet = outlet.type_cast<Outlet>()) ) {
      return val[1].str() == "||" ? source_outlet->unlink(val[2]) : source_outlet->link(val[2]);
    } else {
      return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (using first child of '").append(source->url()).append("')."));
    }
  } else {
    return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (not an Outlet)."));
  }
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
// FIXME: thread safety !
const Value Planet::create_pending_links() {
  static const uint LinkTypeTagId = hashId("sss");
  std::list<Call>::iterator it  = pending_links_.begin();
  std::list<Call>::iterator end = pending_links_.end();

  Value res;
  Value list;

  while (it != end) {
    res = it->trigger_call(this);
    if ((res.type_id() == LinkTypeTagId && res[1].str() == "=>") || res.is_error()) {
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

/** Return a hash representation of the whole system.
 */
const Value Planet::to_hash() {
  Value result(attributes_);
  HashValue patch;

  { ScopedRead lock(children_vector_);
    if (children_vector_.size() > 0) {
      std::vector<Object*>::iterator it, end = children_vector_.end();

      for(it = children_vector_.begin(); it != end; ++it) {
        Node *node = TYPE_CAST(Node, *it);
        if (node) {
          patch.set(node->name(), node->to_hash());
        }
      }
    }
  }

  result.set(Lubyk::NODES_KEY, patch);
  return result;
}

/** Set/get the patch from a hash representation. If the value is nil,
 * return the full view.
 */
const Value Planet::patch(const Value &hash) {
  if (!hash.is_hash()) {
    return to_hash();
  } else {
    // FIXME: set nodes from content...
    // clear and update

    clear_on_register();

    this->Object::clear();

    return set(hash);
  }
}

/** Update the content of the nodes by doing a deep merge.
 */
const Value Planet::set(const Value &hash) {
  if (!hash.is_hash()) return HashValue(); // empty hash

  Value nodes;
  Value all_but_nodes(hash); // copy
  all_but_nodes.remove(Lubyk::NODES_KEY);

  // Start by setting attributes
  Value result(Object::set(all_but_nodes));

  // Update nodes
  if (hash.get(Lubyk::NODES_KEY, &nodes) && nodes.is_hash()) {
    HashValue nodes_result;

    HashIterator it, end = nodes.end();
    ObjectHandle obj;
    Node *node;
    Value node_value;
    std::string key;

    for (it = nodes.begin(); it != end; ++it) {
      key = *it;
      if (!nodes.get(key, &node_value)) continue; // this should never happen, just in case ;-)

      if (get_child(key, &obj)) {
        // existing Object
        if (! (node = TYPE_CAST(Node, obj.ptr()))) continue;

        // It's a Node

        if (node_value.is_hash()) {
          // update Node
          nodes_result.set(key, node->set(node_value));
        } else if (node_value.is_nil()) {
          // delete Node
          node->release();
          nodes_result.set(key, gNilValue);
        } else {
          // invalid argument (can only create/update Node from Hash).
          // Ignore. Insert error ?
        }
      } else {
        // create Node
        if (!node_value.has_key(Lubyk::CLASS)) {
          // Create a widget ?
          Value widget(node_value[Oscit::VIEW][Oscit::WIDGET]);
          if (widget.is_nil()) {
            nodes_result.set(key, ErrorValue(BAD_REQUEST_ERROR, "Missing @class to create new Node."));
          } else {
            // just add in attributes ?
            nodes_result.set(key, node_value);
          }
          continue;
        }

        create_node(key, node_value[Lubyk::CLASS], node_value, &nodes_result);
      }
    }
    result.set(Lubyk::NODES_KEY, nodes_result);
  }
  return result;
}

void Planet::create_node(const std::string &name, const Value &class_url, const Value &create_params, Value *result) {
  if (!class_url.is_string()) return; // ignore
  Value params(create_params);
  Value view_params = params[Oscit::VIEW];

  if (!view_params.has_key(Oscit::POS_X)) {
    // get next location for new Node

    { ScopedRead lock(children_vector_);
      int size = children_vector_.size();
      Node *node = NULL;

      // get last child that is a Node
      for(int i = size - 1; i >= 0; --i) {
        if ( (node = TYPE_CAST(Node, children_vector_[i])) ) break;
      }

      if (node) {
        view_params.set(Oscit::POS_X, node->pos_x() + NEW_NODE_POS_X_DELTA);
        Value pos_y;
        if (node->attributes()[Oscit::VIEW].get(Oscit::POS_Y, &pos_y)) {
          view_params.set(Oscit::POS_Y, pos_y.r + NEW_NODE_POS_Y_DELTA);
        }
      }
    }
    params.set(Oscit::VIEW, view_params);
  }

  ObjectHandle obj;
  Node *node;

  // /class/Metro/new 'x' params
  Value args;
  args.push_back(name);
  args.push_back(params);

  Value name_or_error = call(std::string(class_url.str()).append("/new"), args);

  if (name_or_error.is_string() && get_object_at(name_or_error.str(), &obj)) {

    node = TYPE_CAST(Node, obj.ptr());

    if (!node) {
      // should never happen
      assert(false);
      return;
    }

  } else {
    result->set(name, name_or_error);
    return;
  }

  Value links = create_pending_links(); // create pending links

  result->set(Url(name_or_error.str()).name(), node->to_hash());
}

} // rk
