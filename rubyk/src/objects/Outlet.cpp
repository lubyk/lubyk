#include "rubyk.h"

class OutletNode : public Node
{
public:
  bool init ()
  {
    // FIXME: set type ?
    // Build outlet in parent
    
    if (!mParent) return false;
    
    oscit::Object * out = mParent->child("out");
    
    Node * node = TYPE_CAST(Node,mParent);
    
    if (!out || !node) return false;
    
    mOutlet = out->adopt(new Outlet(name_, node, AnyValue));
    // No need to register as Alias for parent because this is destroyed with the parent anyway.
    return mOutlet != NULL;
  }
  
  virtual const Value inspect(const Value &val)  
  { return String("Outlet"); }
  
  // [1] pass value to the related parent outlet
  void bang(const Value &val)
  { 
    mOutlet->send(val);
  }
private:
  Outlet * mOutlet;
};

extern "C" void init(Planet &planet) {
  Class * c = planet.classes()->declare<OutletNode>("Outlet", "Create an inlet in the parent object. Sends values received in the parent's inlet.");
  INLET(OutletNode, bang, AnyValue, "Send value received out of parent's outlet.")
}