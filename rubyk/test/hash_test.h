#include "value_test_helper.h"


class HashTest : public ValueTestHelper
{
public:
  void test_dictionary_create( void )
  {
    Hash dict("first:1 second:2");
    TS_ASSERT_EQUALS( dict.size(), 2);
  }
  
  void test_dictionary_get( void )
  {
    Hash dict("first:1 second:2 name:\"foo\"");
    TS_ASSERT_EQUALS( dict["first"].data_type(), NumberValue);
    TS_ASSERT( dict["bad key"].is_nil());
    TS_ASSERT_EQUALS( dict["name"].data_type(), StringValue);
  }
  
//  void test_dictionary_data_type( void )
//  {
//    HashData dict("first:1 second:2 name:\"yoba\"");
//    TS_ASSERT_EQUALS( (int)dict.val("first", 3), 1 );
//  }
  
  
  
//  void testGetKey( void )
//  {
//    real_t res;
//    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
//    TS_ASSERT_EQUALS( (int)p.val("first",   3), 1 );
//    TS_ASSERT_EQUALS( (int)p.val("default", 3), 3 );
//    
//    TS_ASSERT_EQUALS( p.val("name", std::string("no-name"))    , std::string("Gaspard Buma"));
//    TS_ASSERT_EQUALS( p.val("surnname", std::string("no-name")), std::string("no-name")     );
//    
//    TS_ASSERT_EQUALS( (real_t)p.val("joy", 1.1), (real_t)3.5 );
//    TS_ASSERT_EQUALS( (real_t)p.val("bad", 1.1), (real_t)1.1 );
//    
//    res = 0.0;
//    TS_ASSERT( p.get(&res, "first"));
//    TS_ASSERT_EQUALS( res, 1.0);
//  }
//  
//  // FIXME: test bool
//  
//  void testGetIndex( void )
//  {
//    real_t res;
//    Params p;
//    p.add("1");
//    p.add("5");
//    p.add("3");
//    p.add("2");
//    TS_ASSERT_EQUALS( p.size(), 4);
//    
//    TS_ASSERT(p.get(&res,0));
//    TS_ASSERT_EQUALS( res, 1.0);
//    
//    TS_ASSERT(p.get(&res,1));
//    TS_ASSERT_EQUALS( res, 5.0);
//    
//    TS_ASSERT(p.get(&res,3));
//    TS_ASSERT_EQUALS( res, 2.0);
//    
//    TS_ASSERT(!p.get(&res,4));
//  }
//  
//  void testSet( void )
//  {
//    Params p;
//    p.set("first","1");
//    TS_ASSERT_EQUALS( (int)p.val("first",   3), 1 );
//    TS_ASSERT_EQUALS( (int)p.val("second",  7), 7 );
//    
//    p.set("second","2");
//    TS_ASSERT_EQUALS( (int)p.val("second",  7), 2 );
//    
//    p.set("name", "Gaspard Buma");
//    p.set("joy", "3.5");
//    TS_ASSERT_EQUALS( (int)p.val("default", 3), 3 );
//    
//    TS_ASSERT_EQUALS( p.val("name", std::string("no-name"))    , std::string("Gaspard Buma"));
//    TS_ASSERT_EQUALS( p.val("surnname", std::string("no-name")), std::string("no-name")     );
//    
//    TS_ASSERT_EQUALS( (real_t)p.val("joy", 1.1), (real_t)3.5 );
//    TS_ASSERT_EQUALS( (real_t)p.val("bad", 1.1), (real_t)1.1 );
//  }
//  
//  void testOstream( void )
//  {
//    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
//    std::ostringstream res(std::ostringstream::out); // allow output operations
//    res << p;
//    TS_ASSERT_EQUALS( res.str(), std::string("first:1 second:2 name:\"Gaspard Buma\" joy:3.5") );
//  }
//  
//  void testClear( void )
//  {
//    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
//    std::ostringstream res1(std::ostringstream::out); // allow output operations
//    std::ostringstream res2(std::ostringstream::out); // allow output operations
//    res1 << p;
//    TS_ASSERT_EQUALS( res1.str(), std::string("first:1 second:2 name:\"Gaspard Buma\" joy:3.5") );
//    p.clear();
//    res2 << p;
//    TS_ASSERT_EQUALS( res2.str(), std::string("") );
//  }
//  
//  void testCastParam( void )
//  {
//    int i = 0;
//    real_t d = 0.0;
//    Params p;
//    i = p.cast_param<int>(std::string("1"));
//    TS_ASSERT_EQUALS( i, 1);
//    d = p.cast_param<real_t>(std::string("1.234"));
//    TS_ASSERT_EQUALS( d, 1.234);
//  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Hash v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
    v.rebuild("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
    out.str(std::string(""));
    out << v;
    TS_ASSERT_EQUALS(out.str(), "<Hash[1] first:1 second:2 name:\"Gaspard Buma\" joy:3.5>");
  }

};