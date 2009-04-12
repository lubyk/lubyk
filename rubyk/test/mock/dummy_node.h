#ifndef _DUMMY_NODE_H_
#define _DUMMY_NODE_H_

class DummyNode : public Node
{
public:
  DummyNode(Real value) : value_(value) {}
  
  virtual void bang(const Value &val) {
    if (val.is_real()) {
      value_ = val.r;
    } else {
      ++value_;
    }
  }
  
  Real value_;
};


#endif // _DUMMY_NODE_H_