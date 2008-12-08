// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "outlet.h"
#include "inlet.h"
#include "values.h"

class DummyNode : public Node
{
public:
  DummyNode(real_t pVal) : mValue(pVal) {}
  
  virtual void bang(const Value& val)
  {}
  
  real_t mValue;
};

// these receivers are complicated to make sure they work in the correct order and they are all called.
// x = 2*x + y + 1
static void receive_value1(Node * receiver, const Value& val)
{
  ((DummyNode*)receiver)->mValue = (2*(((DummyNode*)receiver)->mValue)) + Number(val).value() + 1;
}

// x = 2*x + y + 2
static void receive_value2(Node * receiver, const Value& val)
{
  ((DummyNode*)receiver)->mValue = (2*(((DummyNode*)receiver)->mValue)) + Number(val).value() + 2;
}

// x = 2*x + y + 4
static void receive_value4(Node * receiver, const Value& val)
{
  ((DummyNode*)receiver)->mValue = (2*(((DummyNode*)receiver)->mValue)) + Number(val).value() + 4;
}

class SlotTest : public CxxTest::TestSuite
{
public:
  
  void test_types( void )
  {
    DummyNode counter(0);
    Outlet o_num(&counter, NumberValue);
    Outlet o_str(&counter, StringValue);
    Inlet  i_mat(&counter, receive_value1, MatrixValue );
    Inlet  i_any(&counter, receive_value1, AnyValue );
    Inlet  i_str_mat(&counter, receive_value1, StringValue | MatrixValue );
    Inlet  i_num(&counter, receive_value1, NumberValue );
    
    TS_ASSERT(!o_num.connect(&i_mat));
    TS_ASSERT( o_num.connect(&i_num) );
    TS_ASSERT( o_num.connect(&i_any) );
    
    TS_ASSERT(!o_str.connect(&i_num) );
    TS_ASSERT(!o_str.connect(&i_mat) );
    TS_ASSERT( o_str.connect(&i_any) );
    TS_ASSERT( o_str.connect(&i_str_mat) );
  }
  
  void test_single_connection( void )
  {
    DummyNode counter(0);
    Outlet o(&counter); // counter behaves as the receiver ()
    Inlet  i(&counter, receive_value1 );
    i.set_id(3); // make sure it does not send a 'bang()' to our fake receiver.
    o.connect(&i);
    TS_ASSERT_EQUALS( 0.0, counter.mValue);
    o.send(Number(1.0));
    TS_ASSERT_EQUALS( 2.0, counter.mValue);
  }
  
  void test_many_connections( void )
  {
    DummyNode counter(0);
    Outlet o(&counter);
    Inlet  i1(&counter, receive_value1 );
    Inlet  i2(&counter, receive_value2 );
    Inlet  i3(&counter, receive_value4 );
    // from their ids, these should be called starting by i1, then i2 and i3 as they share the same receiver (counter)
    i1.set_id(4); //         value + 1 = 2
    i2.set_id(2); // 2 * 2 + value + 2 = 7
    i3.set_id(1); // 2 * 7 + value + 4 = 19
    
    o.connect(&i2);
    o.connect(&i1);
    o.connect(&i3);
    
    TS_ASSERT_EQUALS( 0.0, counter.mValue);
    
    o.send(Number(1.0));
    TS_ASSERT_EQUALS( 19.0, counter.mValue);
  }
};