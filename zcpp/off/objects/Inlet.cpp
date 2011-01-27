#include "lubyk.h"

class InletNode : public Node
{
public:
  bool init ()
  {
    // FIXME: set type ?
    // Build inlet in parent
    
    if (!mParent) return false;
    
    oscit::Object * in = mParent->child("in");
    if (!in) return false;
    
    Inlet * inlet = in->adopt(new Inlet(name_, this, &Inlet::cast_method<InletNode, &InletNode::bang>, AnyValue));
    return inlet != NULL;
  }
  
  virtual const Value inspect(const Value &val)  
  { return String("Inlet"); }
  
  // [1]
  void bang(const Value &val)
  { 
    send(val);
  }
};

extern "C" void init(Planet &planet) {
  Class * c = planet.classes()->declare<InletNode>("Inlet", "Create an inlet in the parent object. Sends values received in the parent's inlet.");
  OUTLET(InletNode, port, AnyValue, "Value received in parent's inlet.")
}