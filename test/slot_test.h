/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "test_helper.h"

// these receivers are complicated to make sure they work in the correct order and they are all called.
// x = 2*x + y + 1
static void SlotTest_receive_value1(Inlet *inlet, const Value &val) {
  Node *receiver = inlet->node();
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 1;
}

// x = 2*x + y + 2
static void SlotTest_receive_value2(Inlet *inlet, const Value &val) {
  Node *receiver = inlet->node();
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 2;
}

// x = 2*x + y + 4
static void SlotTest_receive_value4(Inlet *inlet, const Value &val) {
  Node *receiver = inlet->node();
  *(((DummyNode*)receiver)->SlotTest_value_) = (2*(*((DummyNode*)receiver)->SlotTest_value_)) + val.r + 4;
}

class SlotTest : public TestHelper
{
public:
  
  void test_out_no( void ) {
    Real value;
    DummyNode counter(&value);
    Outlet o_no(  &counter, NoIO("Not sending anything."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[[null,null],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // all should fail
    assert_false(o_no.connect(&i_no));
    assert_false(o_no.connect(&i_bang));
    assert_false(o_no.connect(&i_real));
    assert_false(o_no.connect(&i_str ));
    assert_false(o_no.connect(&i_hash));
    assert_false(o_no.connect(&i_mat ));
    assert_false(o_no.connect(&i_list));
    assert_false(o_no.connect(&i_any));
    
    // connection initiated by inlet
    
    // all should fail
    assert_false(i_no.connect(&o_no));
    assert_false(i_bang.connect(&o_no));
    assert_false(i_real.connect(&o_no));
    assert_false(i_str.connect(&o_no));
    assert_false(i_hash.connect(&o_no));
    assert_false(i_mat.connect(&o_no));
    assert_false(i_list.connect(&o_no));
    assert_false(i_any.connect(&o_no));
  }
  
  void test_out_bang( void ) {
    Real value;
    DummyNode counter(&value);
    Outlet o_bang(&counter, BangIO("Sends current counter value."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[[null,null],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // bang --> bang
    assert_true(o_bang.connect(&i_bang));
    // bang --> real
    assert_true(o_bang.connect(&i_real));
    // bang --> str
    assert_true(o_bang.connect(&i_str ));
    // bang --> hash
    assert_true(o_bang.connect(&i_hash));
    // bang --> mat
    assert_true(o_bang.connect(&i_mat ));
    // bang --> list
    assert_true(o_bang.connect(&i_list));
    // bang --> any
    assert_true(o_bang.connect(&i_any));
    
    // no should fail
    assert_false(o_bang.connect(&i_no));
    
    // connection initiated by inlet
    
    // bang <-- bang
    assert_true(i_bang.connect(&o_bang));
    // real <-- bang
    assert_true(i_real.connect(&o_bang));
    // str  <-- bang
    assert_true(i_str.connect(&o_bang));
    // hash <-- bang
    assert_true(i_hash.connect(&o_bang));
    // mat <-- bang
    assert_true(i_mat.connect(&o_bang));
    // list <-- bang
    assert_true(i_list.connect(&o_bang));
    // any  <-- bang
    assert_true(i_any.connect(&o_bang));
    
    // no should fail
    assert_false(i_no.connect(&o_bang));
  }
  
  void test_out_real( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_real(&counter, RealIO("any", "Receive real values."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[[0,0],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // real --> real
    assert_true(o_real.connect(&i_real));
    // real --> any
    assert_true(o_real.connect(&i_any));
    
    // others should fail
    assert_false(o_real.connect(&i_no));
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
    assert_false(i_no.connect(&o_real));
    assert_false(i_bang.connect(&o_real));
    assert_false(i_str.connect(&o_real));
    assert_false(i_hash.connect(&o_real));
    assert_false(i_mat.connect(&o_real));
    assert_false(i_list.connect(&o_real));
  }
  
  void test_out_str( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_str(&counter, StringIO("any", "Receive real values."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[['',''],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // str --> str
    assert_true(o_str.connect(&i_str));
    // str --> any
    assert_true(o_str.connect(&i_any));
    
    // others should fail
    assert_false(o_str.connect(&i_no));
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
    assert_false(i_no.connect(&o_str));
    assert_false(i_bang.connect(&o_str));
    assert_false(i_real.connect(&o_str));
    assert_false(i_hash.connect(&o_str));
    assert_false(i_mat.connect(&o_str));
    assert_false(i_list.connect(&o_str));
  }
  
  void test_out_hash( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_hash(&counter, HashIO("Any description."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[[{},{}],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // hash --> hash
    assert_true(o_hash.connect(&i_hash));
    // hash --> any
    assert_true(o_hash.connect(&i_any));
    
    // others should fail
    assert_false(o_hash.connect(&i_no));
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
    assert_false(i_no.connect(&o_hash));
    assert_false(i_bang.connect(&o_hash));
    assert_false(i_real.connect(&o_hash));
    assert_false(i_str.connect(&o_hash));
    assert_false(i_mat.connect(&o_hash));
    assert_false(i_list.connect(&o_hash));
  }
  
  void test_out_mat( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_mat(&counter, MatrixIO(2,3,"A matrix."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[[0,0],\"first\", \"second\", \"Info.\"]"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // mat --> mat
    assert_true(o_mat.connect(&i_mat));
    // mat --> any
    assert_true(o_mat.connect(&i_any));
    
    // others should fail
    assert_false(o_mat.connect(&i_no));
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
    assert_false(i_no.connect(&o_mat));
    assert_false(i_bang.connect(&o_mat));
    assert_false(i_real.connect(&o_mat));
    assert_false(i_str.connect(&o_mat));
    assert_false(i_hash.connect(&o_mat));
    assert_false(i_list.connect(&o_mat));
  }
  
  void test_out_list( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_list(&counter, JsonValue("[['',0,''], 'ho', 'ha', 'three', 'Info.']"));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[['',0,''],'first', 'second', 'three', 'Info.']"));
    Inlet  i_list2(&counter, SlotTest_receive_value1,JsonValue("[['',0], 'one', 'two', 'Info.']"));
    Inlet  i_list3(&counter, SlotTest_receive_value1,JsonValue("[['',0,'',''], 'one', 'two', 'Info.']"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // list --> list
    assert_true(o_list.connect(&i_list));
    // list --> any
    assert_true(o_list.connect(&i_any));
    // list --> shorter list
    assert_true(o_list.connect(&i_list2));
    
    // others should fail
    assert_false(o_list.connect(&i_no));
    assert_false(o_list.connect(&i_bang));
    assert_false(o_list.connect(&i_real));
    assert_false(o_list.connect(&i_str));
    assert_false(o_list.connect(&i_hash));
    assert_false(o_list.connect(&i_mat ));
    assert_false(o_list.connect(&i_list3)); // not same signature
    
    // connection initiated by inlet
    
    // list <-- list
    assert_true(i_list.connect(&o_list));
    // any  <-- list
    assert_true(i_any.connect(&o_list));
    // shorter list <-- shorter list
    assert_true(i_list2.connect(&o_list));
    
    // others should fail
    assert_false(i_no.connect(&o_list));
    assert_false(i_bang.connect(&o_list));
    assert_false(i_real.connect(&o_list));
    assert_false(i_str.connect(&o_list));
    assert_false(i_hash.connect(&o_list));
    assert_false(i_mat.connect(&o_list));
    assert_false(i_list3.connect(&o_list));
  }
  
  void test_out_any( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_any( &counter, AnyIO("blah."));
    Inlet  i_no(  &counter, SlotTest_receive_value1, NoIO("Don't hit me."));
    Inlet  i_bang(&counter, SlotTest_receive_value1, NilIO("Receives bang values."));
    Inlet  i_real(&counter, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  i_str( &counter, SlotTest_receive_value1, StringIO("any", "Receive string values."));
    Inlet  i_hash(&counter, SlotTest_receive_value1, HashIO("Any description."));
    Inlet  i_mat( &counter, SlotTest_receive_value1, MatrixIO(2,3,"A matrix."));
    Inlet  i_list(&counter, SlotTest_receive_value1, JsonValue("[['',0,''],'first', 'second', 'three', 'Info.']"));
    Inlet  i_list2(&counter, SlotTest_receive_value1,JsonValue("[['',0], 'one', 'two', 'Info.']"));
    Inlet  i_any( &counter, SlotTest_receive_value1, AnyIO("Blah."));
    
    // connection initiated by outlet
    
    // any --> any
    assert_true(o_any.connect(&i_any));
    
    // others should fail
    assert_false(o_any.connect(&i_no));
    assert_false(o_any.connect(&i_bang));
    assert_false(o_any.connect(&i_real));
    assert_false(o_any.connect(&i_str));
    assert_false(o_any.connect(&i_list));
    assert_false(o_any.connect(&i_hash));
    assert_false(o_any.connect(&i_mat ));
    
    // connection initiated by inlet
    
    // any  <-- any
    assert_true(i_any.connect(&o_any));
    
    // others should fail
    assert_false(i_no.connect(&o_any));
    assert_false(i_bang.connect(&o_any));
    assert_false(i_real.connect(&o_any));
    assert_false(i_str.connect(&o_any));
    assert_false(i_list.connect(&o_any));
    assert_false(i_hash.connect(&o_any));
    assert_false(i_mat.connect(&o_any));
    assert_false(i_list2.connect(&o_any));
  }
  
  void test_single_connection( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, RealIO("any", "Receive real values."));
    Inlet  inlet(&receiver, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    assert_true(outlet.connect(&inlet));
    assert_equal(0.0, value);
    outlet.send(Value(1.0));
    assert_equal(2.0, value);
  }
  
  void test_many_connections_same_node( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, RealIO("any", "Receive real values."));
    Inlet  inlet1(&receiver, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  inlet2(&receiver, SlotTest_receive_value2, RealIO("any", "Receive real values."));
    Inlet  inlet3(&receiver, SlotTest_receive_value4, RealIO("any", "Receive real values."));
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
    Outlet outlet(&sender, RealIO("any", "Receive real values."));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, RealIO("any", "Receive real values."));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, RealIO("any", "Receive real values."));
    
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
    Outlet outlet(&sender, RealIO("any", "Receive real values."));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, RealIO("any", "Receive real values."));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, RealIO("any", "Receive real values."));
    
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
    Outlet outlet(&sender, RealIO("any", "Receive real values."));
    Inlet  inlet1(&receiver1, SlotTest_receive_value1, RealIO("any", "Receive real values."));
    Inlet  inlet2(&receiver2, SlotTest_receive_value2, RealIO("any", "Receive real values."));
    Inlet  inlet3(&receiver2, SlotTest_receive_value4, RealIO("any", "Receive real values."));
    
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
  
  void test_find_inlet( void ) {
    Root base;
    Real value = 0;
    DummyNode *sender    = base.adopt(new DummyNode(&value));
    Object *out    = sender->adopt(new Object("out"));
    DummyNode *receiver1 = base.adopt(new DummyNode(&value));
    Object *in     = receiver1->adopt(new Object("in"));
    Outlet *outlet = out->adopt(new Outlet(sender, "ping", RealIO("any", "Receive real values.")));
    in->adopt(new Inlet(receiver1, "pong", SlotTest_receive_value1, RealIO("any", "Receive real values.")));
    
    Value res = outlet->link(Value(receiver1->url())); // should find /receiver1/in/pong
    assert_true(res.type_id() == H("sss"));
    assert_equal("/n/out/ping", res[0].str());
    assert_equal("=>", res[1].str());
    assert_equal("/n-1/in/pong", res[2].str());
  }
};

class SlotCommandTest : public ParseHelper
{
public:
  void test_pending_link( void ) {
    assert_result("# /a ? /b\n", "a => b\n");
    assert_result("# /a/out/value ? /b/in/print\n", "a~value => print~b\n");
  }
  
  void test_pending_removed_on_unlink( void ) {
    setup("a => b\na = Value()\n"); // pending a => b
    assert_result("# /a || /b\n", "a || b\n");
    assert_result("# <Print:/b prefix:\"b\">\n", "b = Print()\n");
  }
};