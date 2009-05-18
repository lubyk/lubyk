#ifndef RUBYK_SRC_CORE_INLET_H_
#define RUBYK_SRC_CORE_INLET_H_
#include "slot.h"

class Inlet;
class Node;

typedef void(*inlet_method_t)(Inlet *node, const Value &val);

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
  inline void receive(const Value &val) {
    (*method_)(this, val);     // use functor
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((T*)inlet->node_)->*Tmethod)(val);
  }
  
  /** Create a callback for an inlet based on a normal accessor. */
  template <class T, const Value(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((T*)inlet->node_)->*Tmethod)(val);
    // ignore return value
  }
  
  /** Create a callback for an inlet (method in superclass). */
  template <class R, class T, void(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((R*)inlet->node_)->*Tmethod)(val);
  }
  
  /** Create a callback for an inlet based on a normal accessor (method in superclass). */
  template <class R, class T, const Value(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((R*)inlet->node_)->*Tmethod)(val);
    // ignore return value
  }
private:
  inlet_method_t method_;        /**< Method to set a new value. */
};

#endif // RUBYK_SRC_CORE_INLET_H_