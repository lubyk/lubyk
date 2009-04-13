#include "test_helper.h"

// these receivers are complicated to make sure they work in the correct order and they are all called.
// x = 2*x + y + 1
static void SlotTest_receive_value1(Node * receiver, const Value &val) {
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 1;
}

// x = 2*x + y + 2
static void SlotTest_receive_value2(Node * receiver, const Value &val) {
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 2;
}

// x = 2*x + y + 4
static void SlotTest_receive_value4(Node * receiver, const Value &val) {
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 4;
}

class SlotTest : public TestHelper
{
public:
  
  void test_out_bang( void ) {
    Real value;
    DummyNode counter(&value);
    Outlet o_bang(&counter, H(""));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("fsf"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // bang --> bang
    assert_true(o_bang.connect(&i_bang));
    // bang --> any
    assert_true(o_bang.connect(&i_any));
    
    // others should fail
    assert_false(o_bang.connect(&i_real));
    assert_false(o_bang.connect(&i_str ));
    assert_false(o_bang.connect(&i_hash));
    assert_false(o_bang.connect(&i_mat ));
    assert_false(o_bang.connect(&i_list));
    
    // connection initiated by inlet
    
    // bang <-- bang
    assert_true(i_bang.connect(&o_bang));
    // any  <-- bang
    assert_true(i_any.connect(&o_bang));
    
    // others should fail
    assert_false(i_real.connect(&o_bang));
    assert_false(i_str.connect(&o_bang));
    assert_false(i_hash.connect(&o_bang));
    assert_false(i_mat.connect(&o_bang));
    assert_false(i_list.connect(&o_bang));
  }
  
  void test_out_real( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_real(&counter, H("f"));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("ff"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // real --> real
    assert_true(o_real.connect(&i_real));
    // real --> any
    assert_true(o_real.connect(&i_any));
    
    // others should fail
    assert_false(o_real.connect(&i_bang));
    assert_false(o_real.connect(&i_str ));
    assert_false(o_real.connect(&i_hash));
    assert_false(o_real.connect(&i_mat ));
    assert_false(o_real.connect(&i_list));
    
    // connection initiated by inlet
    
    // real <-- real
    assert_true(i_real.connect(&o_real));
    // any  <-- real
    assert_true(i_any.connect(&o_real));
    
    // others should fail
    assert_false(i_bang.connect(&o_real));
    assert_false(i_str.connect(&o_real));
    assert_false(i_hash.connect(&o_real));
    assert_false(i_mat.connect(&o_real));
    assert_false(i_list.connect(&o_real));
  }
  
  void test_out_str( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_str(&counter, H("s"));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("ss"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // str --> str
    assert_true(o_str.connect(&i_str));
    // str --> any
    assert_true(o_str.connect(&i_any));
    
    // others should fail
    assert_false(o_str.connect(&i_bang));
    assert_false(o_str.connect(&i_real));
    assert_false(o_str.connect(&i_hash));
    assert_false(o_str.connect(&i_mat ));
    assert_false(o_str.connect(&i_list));
    
    // connection initiated by inlet
    
    // str <-- str
    assert_true(i_str.connect(&o_str));
    // any <-- str
    assert_true(i_any.connect(&o_str));
    
    // others should fail
    assert_false(i_bang.connect(&o_str));
    assert_false(i_real.connect(&o_str));
    assert_false(i_hash.connect(&o_str));
    assert_false(i_mat.connect(&o_str));
    assert_false(i_list.connect(&o_str));
  }
  
  void test_out_hash( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_hash(&counter, H("H"));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("HH"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // hash --> hash
    assert_true(o_hash.connect(&i_hash));
    // hash --> any
    assert_true(o_hash.connect(&i_any));
    
    // others should fail
    assert_false(o_hash.connect(&i_bang));
    assert_false(o_hash.connect(&i_real));
    assert_false(o_hash.connect(&i_str));
    assert_false(o_hash.connect(&i_mat ));
    assert_false(o_hash.connect(&i_list));
    
    // connection initiated by inlet
    
    // hash <-- hash
    assert_true(i_hash.connect(&o_hash));
    // any  <-- hash
    assert_true(i_any.connect(&o_hash));
    
    // others should fail
    assert_false(i_bang.connect(&o_hash));
    assert_false(i_real.connect(&o_hash));
    assert_false(i_str.connect(&o_hash));
    assert_false(i_mat.connect(&o_hash));
    assert_false(i_list.connect(&o_hash));
  }
  
  void test_out_mat( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_mat(&counter, H("M"));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("MM"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // mat --> mat
    assert_true(o_mat.connect(&i_mat));
    // mat --> any
    assert_true(o_mat.connect(&i_any));
    
    // others should fail
    assert_false(o_mat.connect(&i_bang));
    assert_false(o_mat.connect(&i_real));
    assert_false(o_mat.connect(&i_str));
    assert_false(o_mat.connect(&i_hash));
    assert_false(o_mat.connect(&i_list));
    
    // connection initiated by inlet
    
    // mat <-- mat
    assert_true(i_mat.connect(&o_mat));
    // any <-- mat
    assert_true(i_any.connect(&o_mat));
    
    // others should fail
    assert_false(i_bang.connect(&o_mat));
    assert_false(i_real.connect(&o_mat));
    assert_false(i_str.connect(&o_mat));
    assert_false(i_hash.connect(&o_mat));
    assert_false(i_list.connect(&o_mat));
  }
  
  void test_out_list( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_list(&counter, H("sfs"));
    Inlet  i_bang(&counter, SlotTest_receive_value1, H(""));
    Inlet  i_real(&counter, SlotTest_receive_value1, H("f"));
    Inlet  i_str( &counter, SlotTest_receive_value1, H("s"));
    Inlet  i_hash(&counter, SlotTest_receive_value1, H("H"));
    Inlet  i_mat( &counter, SlotTest_receive_value1, H("M"));
    Inlet  i_list(&counter, SlotTest_receive_value1, H("sfs"));
    Inlet  i_list2(&counter, SlotTest_receive_value1, H("sf"));
    Inlet  i_any( &counter, SlotTest_receive_value1, H("*"));
    
    // connection initiated by outlet
    
    // list --> list
    assert_true(o_list.connect(&i_list));
    // list --> any
    assert_true(o_list.connect(&i_any));
    
    // others should fail
    assert_false(o_list.connect(&i_bang));
    assert_false(o_list.connect(&i_real));
    assert_false(o_list.connect(&i_str));
    assert_false(o_list.connect(&i_hash));
    assert_false(o_list.connect(&i_mat ));
    assert_false(o_list.connect(&i_list2)); // not same signature
    
    // connection initiated by inlet
    
    // list <-- list
    assert_true(i_list.connect(&o_list));
    // any  <-- list
    assert_true(i_any.connect(&o_list));
    
    // others should fail
    assert_false(i_bang.connect(&o_list));
    assert_false(i_real.connect(&o_list));
    assert_false(i_str.connect(&o_list));
    assert_false(i_hash.connect(&o_list));
    assert_false(i_mat.connect(&o_list));
    assert_false(i_list2.connect(&o_list));
  }
  
  void test_single_connection( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, H("f"));
    Inlet  inlet(&receiver, SlotTest_receive_value1, H("f"));
    assert_true(outlet.connect(&inlet));
    assert_equal(0.0, value);
    outlet.send(Value(1.0));
    assert_equal(2.0, value);
  }
  
  void test_many_connections_same_node( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, H("f"));
    Inlet  inlet1(&receiver, SlotTest_receive_value1, H("f"));
    Inlet  inlet2(&receiver, SlotTest_receive_value2, H("f"));
    Inlet  inlet3(&receiver, SlotTest_receive_value4, H("f"));
    // from their ids, these should be called starting by inlet1, then inlet2 and inlet3 as they share the same receiver (counter)
    inlet1.set_id(4); //         value + 1 = 2
    inlet2.set_id(2); // 2 * 2 + value + 2 = 7
    inlet3.set_id(1); // 2 * 7 + value + 4 = 19
    
    assert_true(outlet.connect(&inlet2));
    assert_true(outlet.connect(&inlet1));
    assert_true(outlet.connect(&inlet3));
    
    assert_equal(0.0, value);
    
    outlet.send(Value(1.0));
    assert_equal(19.0, value);
  }
  
  void test_many_connections_different_nodes( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver1(&value);
    DummyNode receiver2(&value);
    Outlet outlet(&sender, H("f"));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, H("f"));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, H("f"));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, H("f"));
    
    receiver1.set_trigger_position(2.0); // should trigger first
    inlet1.set_id(1); // should not sort with this id               value + 1 = 2
    
    receiver2.set_trigger_position(1.0); // should trigger last
    inlet2.set_id(8); // sub-sorting by id                  2 * 2 + value + 2 = 7
    inlet3.set_id(7); //                                    2 * 7 + value + 4 = 19
    
    assert_true(outlet.connect(&inlet2));
    assert_true(outlet.connect(&inlet1));
    assert_true(outlet.connect(&inlet3));
    
    assert_equal(0.0, value);
    
    outlet.send(Value(1.0));
    assert_equal(19.0, value);
  }
  
  void test_update_id( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver1(&value);
    DummyNode receiver2(&value);
    Outlet outlet(&sender, H("f"));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, H("f"));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, H("f"));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, H("f"));
    
    receiver1.set_trigger_position(2.0); // should trigger first
    inlet1.set_id(1); // should not sort with this id               value + 1 = 2
    
    receiver2.set_trigger_position(1.0); // should trigger last
    inlet3.set_id(7); //                                    2 * 2 + value + 4 = 9
    inlet2.set_id(6); // sub-sorting by id                  2 * 9 + value + 2 = 21
    
    assert_true(outlet.connect(&inlet2));
    assert_true(outlet.connect(&inlet1));
    assert_true(outlet.connect(&inlet3));
    
    assert_equal(0.0, value);
    
    outlet.send(Value(1.0));
    assert_equal(21.0, value); // order was inlet1, inlet3, inlet2
    
    inlet2.set_id(8);
    value = 0.0;
    
    outlet.send(Value(1.0));
    assert_equal(19.0, value); // order is now inlet1, inlet2, inlet3
  }
  
  void test_update_node_trigger_position( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver1(&value);
    DummyNode receiver2(&value);
    Outlet outlet(&sender, H("f"));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, H("f"));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, H("f"));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, H("f"));
    
    inlet1.set_id(1); // should not sort with this id       2 * 0 + x + 1 = 2
    inlet2.set_id(8); // sub-sorting by id                  2 * 2 + x + 2 = 7
    inlet3.set_id(7); //                                    2 * 7 + x + 4 = 19
    
    assert_true(outlet.connect(&inlet2));
    assert_true(outlet.connect(&inlet1));
    assert_true(outlet.connect(&inlet3));
    
    assert_equal(0.0, value);
    
    outlet.send(Value(1.0));
    assert_equal(24.0, value); // order was inlet2, inlet3, inlet1
    
    receiver1.set_trigger_position(2.0); // should trigger first
    receiver2.set_trigger_position(1.0); // should trigger last
    value = 0.0;
    
    outlet.send(Value(1.0));
    assert_equal(19.0, value); // order is now inlet1, inlet2, inlet3
  }
};