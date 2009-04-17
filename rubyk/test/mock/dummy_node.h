#ifndef _DUMMY_NODE_H_
#define _DUMMY_NODE_H_

class DummyNode : public Node
{
public:
  TYPED("Object.Node.DummyNode")
  
  DummyNode(Real *value) : SlotTest_value_(value) {}
  
  DummyNode(Real value) : value_(value) {}
  
  virtual void bang(const Value &val) {
    if (val.is_real()) {
      value_ = val.r;
    } else {
      ++value_;
    }
  }
  
  Real * SlotTest_value_;
  Real value_;
};


#endif // _DUMMY_NODE_H_