#ifndef _INLET_H_
#define _INLET_H_
#include "slot.h"

class Node;

typedef void(*inlet_method_t)(Node * node, const Value &val);

/** Prototype constructor for Inlets. */
struct InletPrototype
{
  InletPrototype(const char *name, inlet_method_t method, const Value &type) : name_(name), method_(method), type_(type) {}
  
  std::string    name_;
  inlet_method_t method_;
  Value          type_;
};

class Inlet : public Slot {
public:
  TYPED("Object.Slot.Inlet")
  
  /** Constructor used for testing. */
  Inlet(Node *node, inlet_method_t method, const Value &type) : Slot(node, type), method_(method) {
    register_in_node();
  }
  
  Inlet(Node *node, const char *name, inlet_method_t method, const Value &type) : Slot(node, name, type), method_(method) {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Inlet(Node *node, const InletPrototype &prototype) : Slot(node, prototype.name_, prototype.type_), method_(prototype.method_) {
    register_in_node();
  }
  
  virtual ~Inlet() {
    unregister_in_node();
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** The operation to be executed on call. If 'val' is not Nil, send to inlet. */
  virtual const Value trigger(const Value &val);
    
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