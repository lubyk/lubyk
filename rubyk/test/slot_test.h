#include "test_helper.h"

// these receivers are complicated to make sure they work in the correct order and they are all called.
// x = 2*x + y + 1
static void SlotTest_receive_value1(Node * receiver, const Value &val)
{
  ((DummyNode*)receiver)->value_ = (2*(((DummyNode*)receiver)->value_)) + val.r + 1;
}

// x = 2*x + y + 2
static void SlotTest_receive_value2(Node * receiver, const Value &val)
{
  ((DummyNode*)receiver)->value_ = (2*(((DummyNode*)receiver)->value_)) + val.r + 2;
}

// x = 2*x + y + 4
static void SlotTest_receive_value4(Node * receiver, const Value &val)
{
  ((DummyNode*)receiver)->value_ = (2*(((DummyNode*)receiver)->value_)) + val.r + 4;
}

class SlotTest : public TestHelper
{
public:
  
  void test_types( void ) {
    DummyNode counter(0);
    Outlet o_num(&counter, H("f"));
    Outlet o_str(&counter, H("s"));
    Outlet o_any(&counter, H("*"));
    Inlet  i_mat(&counter, SlotTest_receive_value1, H("M") );
    Inlet  i_any(&counter, SlotTest_receive_value1, H("*") );
    Inlet  i_num(&counter, SlotTest_receive_value1, H("f") );
    
    TS_ASSERT(!o_num.connect(&i_mat));
    TS_ASSERT( o_num.connect(&i_num) );
    TS_ASSERT( o_num.connect(&i_any) );
    
    TS_ASSERT(!o_any.connect(&i_mat));
    TS_ASSERT(!o_any.connect(&i_num) );
    TS_ASSERT( o_any.connect(&i_any) );
    
    TS_ASSERT(!o_str.connect(&i_num) );
    TS_ASSERT(!o_str.connect(&i_mat) );
    TS_ASSERT( o_str.connect(&i_any) );
  }
  
  // void test_single_connection( void ) {
  //   DummyNode counter(0);
  //   Outlet o(&counter); // counter behaves as the receiver ()
  //   Inlet  i(&counter, SlotTest_receive_value1 );
  //   i.set_id(3); // make sure it does not send a 'bang()' to our fake receiver.
  //   o.connect(&i);
  //   TS_ASSERT_EQUALS( 0.0, counter.value_);
  //   o.send(Number(1.0));
  //   TS_ASSERT_EQUALS( 2.0, counter.value_);
  // }
  // 
  // void test_many_connections( void )
  // {
  //   DummyNode counter(0);
  //   Outlet o(&counter);
  //   Inlet  i1(&counter, SlotTest_receive_value1 );
  //   Inlet  i2(&counter, SlotTest_receive_value2 );
  //   Inlet  i3(&counter, SlotTest_receive_value4 );
  //   // from their ids, these should be called starting by i1, then i2 and i3 as they share the same receiver (counter)
  //   i1.set_id(4); //         value + 1 = 2
  //   i2.set_id(2); // 2 * 2 + value + 2 = 7
  //   i3.set_id(1); // 2 * 7 + value + 4 = 19
  //   
  //   o.connect(&i2);
  //   o.connect(&i1);
  //   o.connect(&i3);
  //   
  //   TS_ASSERT_EQUALS( 0.0, counter.value_);
  //   
  //   o.send(Number(1.0));
  //   TS_ASSERT_EQUALS( 19.0, counter.value_);
  // }
};