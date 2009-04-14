#ifndef _INLET_H_
#define _INLET_H_
#include "slot.h"

class Node;

typedef void(*inlet_method_t)(Node * node, const Value &val);

/** Prototype constructor for Inlets. */
struct InletPrototype
{
  InletPrototype(const char *name, inlet_method_t method, TypeTagID type_tag_id, const char *info, const Value &type) :
      name_(name), type_tag_id_(type_tag_id), method_(method), info_(info), type_(type) {}
  std::string    name_;
  TypeTagID      type_tag_id_;
  inlet_method_t method_;
  std::string    info_;
  Value          type_;
};

class Inlet : public Slot {
public:
  /** Constructor used for testing. */
  Inlet(Node *node, inlet_method_t method, TypeTagID type_tag_id) : Slot(node, type_tag_id), method_(method) {
    register_in_node();
  }
  
  Inlet(Node *node, const char *name, inlet_method_t method, TypeTagID type_tag_id) : Slot(node, name, type_tag_id), method_(method) {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Inlet(Node *node, const InletPrototype& prototype) : Slot(node, prototype.name_, prototype.type_tag_id_), method_(prototype.method_) {
    set_info(prototype.info_);
    set_type(prototype.type_);
    register_in_node();
  }
  
  virtual ~Inlet() {
    unregister_in_node();
  }
  
  /** Class signature. */
  virtual uint class_type() {
    return H("Inlet");
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** The operation to be executed on call. If 'val' is not Nil, send to inlet. */
  virtual const Value trigger(const Value &val) {
    receive(val);
    return gNilValue;
  }
  
  /** Receive a value. */
  void receive(const Value &val) {
    (*method_)(node_, val);     // use functor
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  static void cast_method(Node *receiver, const Value &val) {
    (((T*)receiver)->*Tmethod)(val);
  }
  
private:
  inlet_method_t method_;        /**< Method to set a new value. */
};

#endif