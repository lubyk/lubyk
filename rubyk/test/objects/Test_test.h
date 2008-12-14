#include "test_helper.h"

class TestTest : public NodeTestHelper
{    
public:
  
  void test_create( void )
  { 
    // create always produces 3 values ([2] {url:[1] params:[3]})
    create("/n", "Test", "counter: 5 message:foo", "[6] /n"); // 2 values + 3 => 5 + 1 => [6]. Ok !
    assert_inspect("/n", "<Test:/n '[3] foo' counter:5>");
    assert_info("/class/Test", "Object used for testing. Does not do anything really useful.");
  }
  
  void test_set_default_argument( void )
  { 
    create("/n", "Test", "\"Single text argument\"", "[4] /n"); // 2 values + 3 => 5 + 1 => [6]. Ok !
    assert_call("/n/message", "", "Single text argument");
    assert_inspect("/n", "<Test:/n '[1] Single text argument' counter:0>");
  }
  
  void test_class_method( void )
  { 
    assert_call("/class/Test/hello", "", "Hello: Nil");
    assert_call("/class/Test/hello", "1.35", "Hello: [2] 1.35");
    assert_info("/class/Test/hello", "If the input value is 0: stop. If it is greater the 0: start. Bang toggles on/off.");
  }
  
  void test_method( void )
  {   
    create("/t", "Test", "counter: 5 message:hopla", "[6] /t");
    assert_call("/t", ""     , "message,counter,in/,out/");     // get method list
    assert_call("/t/message", ""     , "hopla"); // get
    assert_call("/t/message", "yoba" , "yoba"); // set
    assert_call("/t/message", ""     , "yoba"); // get again
    assert_call("/t/message/#info","", "Example of value storage (String)."); // get info on method
  }
  
  void test_inlet( void )
  {   
    create("/t", "Test", "counter: 5 message:hopla", "[6] /t");
    assert_call("/t/in",           "", "bang/,counter/,info/");
    assert_call("/t/in/bang",      "", "link,unlink");
    assert_call("/t/in/bang/#info","", "Set counter | increment and send.");
  }
  
  void test_outlet( void )
  {   
    create("/t", "Test", "counter: 5 message:hopla", "[6] /t");
    assert_call("/t/out", ""      , "counter/,nil/");
    assert_call("/t/out/counter", ""      , "link,unlink");
    assert_call("/t/out/counter/#info","", "Increasing counter.");
  }
  
  void test_link( void )
  {   
    create("/t",  "Test", "counter: 5 message:hopla",  "[6] /t");
    create("/t2", "Test", "counter: 2 message:second", "[15] /t2");
    assert_call("/t/in/bang/link/#info", ""      , "Create a link / list links.");
    
    // should not allow creating a link from inlet to inlet:
    assert_call("/t/in/bang/link", "/t2/in/bang", "[21] #Could not update link ([20] /t2/in/bang: incompatible).");
    
    // should allow creating a link from an inlet to an outlet
    assert_call("/t/in/bang/link", "/t2/out/counter", "/t2/out/counter");
    // should allow creating a link from an outlet to an inlet
    assert_call("/t/out/counter/link", "/t2/in/bang", "/t2/in/bang");
  }
  
  void test_link_and_bang( void )
  {   
    create("/t",  "Test", "counter: 5 message:hopla",  "[6] /t");
    create("/t2", "Test", "counter: 2 message:second", "[15] /t2");
    
    // should allow creating a link from an outlet to an inlet
    assert_call("/t/out/counter/link", "/t2/in/bang", "/t2/in/bang");
    assert_call("/t2/counter", "", "[20] 2.00");
    assert_call("/t/in/bang", "Bang!", "Nil"); // t++ --> t2
    assert_call("/t2/counter", "", "[23] 6.00");
  }
  
  void test_unregister_inlet( void )
  {
    create("/t",  "Test", "counter: 5 message:hopla",  "[6] /t");
    create("/t2", "Test", "counter: 2 message:second", "[15] /t2");

    // should allow creating a link from an outlet to an inlet
    assert_call("/t/out/counter/link", "/t2/in/bang", "/t2/in/bang");
    assert_call("/t/counter",  "", "[20] 5.00");
    assert_call("/t2/counter", "", "[21] 2.00");
    assert_call("/t/in/bang", "Bang!", "Nil"); // t++ --> t2
    assert_call("/t2/counter", "", "[24] 6.00");
    
    Object * obj;
    TS_ASSERT( mRoot.get(&obj, "/t/out/counter") );
    // destroy first outlet ===> [2] becomes [1] ==> first output is now /out/nil
    // this is not a normal situation but it needs testing (dynamic outlets may be used with Lua and Group).
    delete obj;
    TS_ASSERT( !mRoot.get(&obj, "/t/out/counter") );
    TS_ASSERT( !mRoot.get(&obj, "/t/out/counter/link") );
    TS_ASSERT(  mRoot.get(&obj, "/t/out/nil") );
    
    assert_call("/t/in/bang", "Bang!", "Nil"); // t++ --> nil outlet
    assert_call("/t/counter",  "", "[27] 7.00");
    assert_call("/t2/counter", "", "[28] 6.00");
    // destroy 'nil' outlet
    delete obj;
    TS_ASSERT( !mRoot.get(&obj, "/t/out/nil") );
    TS_ASSERT( !mRoot.get(&obj, "/t/out/nil/link") );
    assert_call("/t/in/bang", "Bang!", "Nil"); // t++ --> not send (no more outlets).
  }
  // TODO: test loop (t => t2 => t).
  // TODO: make sure links cannot be made twice.
};

// class TestParseHelper : public ParseHelper
// {
// public:
// 
//   void test_send_nil( void ) 
//   { 
//     parse("n=Test()\nn.2=>p\n");
//     assert_print("n.b\n","sending nil=><=done.\n");
//   }
//   
//   void test_try( void )
//   {
//     parse("n=Test()\nn=>p\n");
//     assert_print("n.test_fail\n", "n: n size error (copy): bad start row 10 (10) of matrix 5x1 in src/objects/Test.cpp:58.\ntry failed");
//   }
// };