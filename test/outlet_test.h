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
static const Value OutletTest_receive_value1(void *receiver, const Value &val) {
  DummyNode *node = (DummyNode*)receiver;
  *(node->OutletTest_value_) = (2*(*node->OutletTest_value_)) + val.r + 1;
  return gNilValue;
}

// x = 2*x + y + 2
static const Value OutletTest_receive_value2(void *receiver, const Value &val) {
  DummyNode *node = (DummyNode*)receiver;
  *(node->OutletTest_value_) = (2*(*node->OutletTest_value_)) + val.r + 2;
  return gNilValue;
}

// x = 2*x + y + 4
static const Value OutletTest_receive_value4(void *receiver, const Value &val) {
  DummyNode *node = (DummyNode*)receiver;
  *(node->OutletTest_value_) = (2*(*node->OutletTest_value_)) + val.r + 4;
  return gNilValue;
}

class OutletTest : public TestHelper
{
public:

  void test_out_no( void ) {
    Real value;
    DummyNode counter(&value);
    Outlet o_no(  &counter, Attribute::no_io("Not sending anything."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "NN"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

    // all should fail
    assert_false(o_no.connect(&i_no));
    assert_false(o_no.connect(&i_bang));
    assert_false(o_no.connect(&i_real));
    assert_false(o_no.connect(&i_str ));
    assert_false(o_no.connect(&i_hash));
    assert_false(o_no.connect(&i_mat ));
    assert_false(o_no.connect(&i_list));
    assert_false(o_no.connect(&i_any));
  }

  void test_out_bang( void ) {
    Real value;
    DummyNode counter(&value);
    Outlet o_bang(&counter, Attribute::bang_io("Sends current counter value."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "NN"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

    // connection initiated by outlet

    // bang --> bang
    assert_true(o_bang.connect(&i_bang));

    // bang --> any
    assert_true(o_bang.connect(&i_any));

    // bang connects to everything
    assert_true(o_bang.connect(&i_no  ));
    assert_true(o_bang.connect(&i_real));
    assert_true(o_bang.connect(&i_str ));
    assert_true(o_bang.connect(&i_hash));
    assert_true(o_bang.connect(&i_mat ));
    assert_true(o_bang.connect(&i_list));
  }

  void test_out_real( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_real(&counter, Attribute::real_io("Receive real values."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "ff"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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
  }

  void test_out_str( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_str(&counter, Attribute::string_io("Receive real values."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "ff"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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
  }

  void test_out_hash( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_hash(&counter, Attribute::hash_io("Any description."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "ff"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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
  }

  void test_out_mat( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_mat(&counter, Attribute::matrix_io("A matrix."));
    Inlet  i_no(  &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang(&counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real(&counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str( &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash(&counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat( &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "ff"));
    Inlet  i_any( &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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
  }

  void test_out_list( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_list( &counter, JsonValue("[['',0,''], 'ho', 'ha', 'three', 'Info.']"));
    Inlet  i_no(   &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang( &counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real( &counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str(  &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash( &counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat(  &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list( &counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "sfs"));  // should connection match take signature or name ?
    Inlet  i_list2(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list2", "sf"));   // should connection match take signature or name ?
    Inlet  i_list3(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list3", "sfss")); // should connection match take signature or name ?
    Inlet  i_any(  &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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

  }

  void test_out_any( void ) {
    Real value = 0;
    DummyNode counter(&value);
    Outlet o_any(  &counter, Attribute::any_io("blah."));
    Inlet  i_no(   &counter, "", OutletTest_receive_value1, Attribute::no_io("Don't hit me."));
    Inlet  i_bang( &counter, "", OutletTest_receive_value1, Attribute::bang_io("Receives bang values."));
    Inlet  i_real( &counter, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  i_str(  &counter, "", OutletTest_receive_value1, Attribute::string_io("Receive string values."));
    Inlet  i_hash( &counter, "", OutletTest_receive_value1, Attribute::hash_io("Any description."));
    Inlet  i_mat(  &counter, "", OutletTest_receive_value1, Attribute::matrix_io("A matrix."));
    Inlet  i_list( &counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list1", "sfs"));
    Inlet  i_list2(&counter, "", OutletTest_receive_value1, Attribute::io("Info.", "list2", "sf"));
    Inlet  i_any(  &counter, "", OutletTest_receive_value1, Attribute::any_io("Blah."));

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

  }

  void test_single_connection( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, Attribute::real_io("Receive real values."));
    Inlet  inlet(&receiver, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    assert_true(outlet.connect(&inlet));
    assert_equal(0.0, value);
    outlet.send(Value(1.0));
    assert_equal(2.0, value);
  }

  void test_many_connections_same_node( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver(&value);
    Outlet outlet(&sender, Attribute::real_io("Receive real values."));
    Inlet  inlet1(&receiver, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  inlet2(&receiver, "", OutletTest_receive_value2, Attribute::real_io("Receive real values."));
    Inlet  inlet3(&receiver, "", OutletTest_receive_value4, Attribute::real_io("Receive real values."));
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
    Outlet outlet(&sender, Attribute::real_io("Receive real values."));
    Inlet  inlet1(&receiver1, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  inlet2(&receiver2, "", OutletTest_receive_value2, Attribute::real_io("Receive real values."));
    Inlet  inlet3(&receiver2, "", OutletTest_receive_value4, Attribute::real_io("Receive real values."));

    receiver1.set(Value(Json("{@view:{x:2.0}}"))); // should trigger first
    inlet1.set_id(1); // should not sort with this id               value + 1 = 2

    receiver2.set(Value(Json("{@view:{x:1.0}}"))); // should trigger last
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
    Outlet outlet(&sender, Attribute::real_io("Receive real values."));
    Inlet  inlet1(&receiver1, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  inlet2(&receiver2, "", OutletTest_receive_value2, Attribute::real_io("Receive real values."));
    Inlet  inlet3(&receiver2, "", OutletTest_receive_value4, Attribute::real_io("Receive real values."));

    receiver1.set(Value(Json("{@view:{x:2.0}}"))); // should trigger first
    inlet1.set_id(1); // should not sort with this id               value + 1 = 2

    receiver2.set(Value(Json("{@view:{x:1.0}}"))); // should trigger last
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

  void test_update_node_position( void ) {
    Real value = 0;
    DummyNode sender(&value);
    DummyNode receiver1(&value);
    DummyNode receiver2(&value);
    Outlet outlet(&sender, Attribute::real_io("Receive real values."));
    Inlet  inlet1(&receiver1, "", OutletTest_receive_value1, Attribute::real_io("Receive real values."));
    Inlet  inlet2(&receiver2, "", OutletTest_receive_value2, Attribute::real_io("Receive real values."));
    Inlet  inlet3(&receiver2, "", OutletTest_receive_value4, Attribute::real_io("Receive real values."));

    inlet1.set_id(1); // should not sort with this id       2 * 0 + x + 1 = 2
    inlet2.set_id(8); // sub-sorting by id                  2 * 2 + x + 2 = 7
    inlet3.set_id(7); //                                    2 * 7 + x + 4 = 19

    assert_true(outlet.connect(&inlet1)); // since receiver1 and receiver2 have same position, sorting is done
    assert_true(outlet.connect(&inlet2)); // using connection order
    assert_true(outlet.connect(&inlet3));

    assert_equal(0.0, value);

    outlet.send(Value(1.0));
    assert_equal(24.0, value); // order was inlet2, inlet3, inlet1

    receiver1.set(Value(Json("{@view:{x:2.0}}"))); // should trigger first
    receiver2.set(Value(Json("{@view:{x:1.0}}"))); // should trigger last

    value = 0.0;

    outlet.send(Value(1.0));
    assert_equal(19.0, value); // order is now inlet1, inlet2, inlet3
  }

  void test_find_inlet( void ) {
    Root base;
    Real value = 0;
    DummyNode *sender    = base.adopt(new DummyNode(&value));
    Object    *out       = sender->adopt(new Object(NODE_OUT_KEY));
    DummyNode *receiver1 = base.adopt(new DummyNode(&value));
    Outlet    *outlet    = out->adopt(new Outlet(sender, "ping", Attribute::real_io("Receive real values.")));
    receiver1->adopt(new Inlet(receiver1, "pong", OutletTest_receive_value1, Attribute::real_io("Receive real values.")));

    Value res = outlet->link(Value(receiver1->url())); // should find /receiver1/in/pong
    assert_equal((int)res.type_id(), (int)hashId("sss"));
    assert_equal("/n/out/ping", res[0].str());
    assert_equal("=>", res[1].str());
    assert_equal("/n-1/pong", res[2].str());
  }

  void test_to_hash( void ) {
    Root base;
    Real value = 0;
    DummyNode *sender    = base.adopt(new DummyNode(&value));
    Object    *out       = sender->adopt(new Object(NODE_OUT_KEY));
    DummyNode *receiver1 = base.adopt(new DummyNode(&value));
    Outlet    *outlet    = out->adopt(new Outlet(sender, "ping", Attribute::real_io("Receive real values.")));
    receiver1->adopt(new Inlet(receiver1, "mic", OutletTest_receive_value1, Attribute::real_io("Receive real values.")));
    receiver1->adopt(new Inlet(receiver1, "mac", OutletTest_receive_value1, Attribute::real_io("Receive real values.")));

    outlet->link(Value(receiver1->url())); // should find /receiver1/pong
    outlet->link(Value("/n-1/mac"));

    assert_equal("{\"@type\":0, \"/n-1/mac\":{}, \"/n-1/mic\":{}}", outlet->to_hash().to_json());
  }
};

class OutletCommandTest : public ParseHelper
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