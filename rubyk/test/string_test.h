#include "value_test_helper.h"


class StringTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    String n("Hello World!");
    TS_ASSERT_EQUALS( *n.string(), "Hello World!");
    // anonymize
    Value v(n);
    
    String n2;
    TS_ASSERT(v.set(n2));
    TS_ASSERT_EQUALS( *n2.string(), "Hello World!");
    
    n2 = "foo";
    TS_ASSERT_EQUALS( *n2.string(), "foo");
    TS_ASSERT(n.set(n2));
  }
  
  void test_is_a( void )
  {
    String e;
    String e2("Blah");
    Value v;
    TS_ASSERT(!e.is_string());
    TS_ASSERT(e2.is_string());
    TS_ASSERT(!v.is_string());
    e2.set(v);
    TS_ASSERT(v.is_string());
  }
  
  void test_operator_equal( void )
  {
    String n;
    n = "My name is Bob.";
    TS_ASSERT_EQUALS( *n.string(), "My name is Bob.");
    n = "My name is Joe.";
    TS_ASSERT_EQUALS( *n.string(), "My name is Joe.");
  }
  
  void test_append( void )
  {
    String n;
    assert_id(n, 0);
    
    String n1("lala"); // first StringData created
    assert_id(n1, 1);
    
    n = "My name is Bob."; // second StringData created
    assert_id(n, 2);   
    
    String n3("lala"); // third StringData created
    assert_id(n3, 3);
    
    TS_ASSERT_EQUALS( "My name is Bob.", *n.string());
    n.append(" My friend is Joe.");
    assert_id(n, 2); // same old id
    TS_ASSERT_EQUALS( "My name is Bob. My friend is Joe.", *n.string());
    
    String n4("lala"); // fourth StringData created
    assert_id(n4, 4);
  }
  
  // 
  // void test_data( void )
  // {
  //   String n1(123.45);
  //   const StringData * d1 = n1.data();
  //   TS_ASSERT_EQUALS( d1, n1.data());
  //   // alter n1 (no need to copy)
  //   n1 = 555.23;
  //   TS_ASSERT_EQUALS( d1, n1.data());
  //   
  //   String n2(n1); // light copy
  //   const StringData * d2 = n2.data();
  //   TS_ASSERT_EQUALS( d1, d2);
  //   TS_ASSERT_EQUALS( 555.23, n1.value());
  //   TS_ASSERT_EQUALS( 555.23, n2.value());
  //   // alter n1
  //   n1 = 888.12;
  //   d1 = n1.data(); // pointer to new data
  //   TS_ASSERT_EQUALS( 888.12, n1.value());
  //   TS_ASSERT_EQUALS( 555.23, n2.value());
  //   TS_ASSERT_DIFFERS( d1, d2);
  // }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    String v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
    v = "Good day, Sir !";
    out.str(std::string(""));
    out << v;
    TS_ASSERT_EQUALS(out.str(), "[1] \"Good day, Sir !\"");
  }
};