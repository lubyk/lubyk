#ifndef _OUTLET_H_
#define _OUTLET_H_
#include "slot.h"

/** Prototype constructor for Inlets. */
struct OutletPrototype
{
  OutletPrototype(const char *name, TypeTagID type, const char * info) : name_(name), type_(type), info_(info) {}
  std::string name_;
  TypeTagID   type_;
  std::string info_;
};


class Outlet : public Slot {
public:  
  Outlet(Node *node, TypeTagID type) : Slot(node, type) {
    register_in_node();
  }
  
  Outlet(Node *node, const std::string &name, TypeTagID type) : Slot(node, name, type) {
    register_in_node();
  }
  
  Outlet(Node *node, const char *name, TypeTagID type) : Slot(node, name, type) {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Outlet(Node *node, const OutletPrototype &prototype) : Slot(node, prototype.name_, prototype.type_) {
    set_info(prototype.info_);
    register_in_node();
  }
  
  virtual ~Outlet() {
    unregister_in_node();
  }
  
  /** Class signature. */
  virtual uint class_type() {
    return H("Outlet");
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** Send the signal to all connections. Do nothing if there are no connections. */
  void send(const Value &val);
};

#endif
