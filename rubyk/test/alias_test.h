// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "alias.h"
#include "test_helper.h"

class AliasTest : public ValueTestHelper
{
public:
  void test_trigger( void )
  {
    Root root;
    Object       * one = root.adopt(new Object("one")); //   [/one]
    DummyNumber  * sub = one->adopt(new DummyNumber("sub", 3)); //   [/one/sub]
    Alias        * ali = root.adopt(new Alias("ali", sub));
    DummyNumber  * two = root.adopt(new DummyNumber("two", 2)); //  [/two]
    Value res;
    
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
    TS_ASSERT_EQUALS( ali->url(),     std::string("/ali") );
    
    TS_ASSERT_EQUALS(sub->mCounter.value(), 3.00);
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 3.00);
    res = root.call("/ali");
    TS_ASSERT_EQUALS(Number(res).value(), 3.00);
    
    res = root.call("/one/sub", "4.50");
    TS_ASSERT_EQUALS(Number(res).value(), 4.50);
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 4.50);
    res = root.call("/ali");
    TS_ASSERT_EQUALS(Number(res).value(), 4.50);
    res = root.call("/two");
    TS_ASSERT_EQUALS(Number(res).value(), 2.00);
    
    res = root.call("/ali", "23");
    TS_ASSERT_EQUALS(Number(res).value(), 23.00);
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 23.00);
    
    
    ali->set_original(std::string("/two"));
    
    res = root.call("/ali", "7");
    TS_ASSERT_EQUALS(Number(res).value(), 7.00);
    res = root.call("/two");
    TS_ASSERT_EQUALS(Number(res).value(), 7.00);
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 23.00);
    
    delete two; // delete original first
    res = root.call("/ali", "23");
    TS_ASSERT_EQUALS(res.to_string(), "[8] #\"Object \'/ali\' not found.\"");
  }
  
  void test_delete_alias_first( void )
  {
    Root root;
    Object       * one = root.adopt(new Object("one")); //   [/one]
    DummyNumber  * sub = one->adopt(new DummyNumber("sub", 3)); //   [/one/sub]
    Alias        * ali = root.adopt(new Alias("ali", sub));
    Value res;
    
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
    TS_ASSERT_EQUALS( ali->url(),     std::string("/ali") );
    
    TS_ASSERT_EQUALS(sub->mCounter.value(), 3.00);
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 3.00);
    res = root.call("/ali");
    TS_ASSERT_EQUALS(Number(res).value(), 3.00);
    
    delete ali;
    res = root.call("/ali");
    TS_ASSERT_EQUALS(res.to_string(), "[2] #\"Object \'/ali\' not found.\"");
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(Number(res).value(), 3.00);
    
    delete one;
    res = root.call("/one/sub");
    TS_ASSERT_EQUALS(res.to_string(), "[3] #\"Object \'/one/sub\' not found.\"");
  }
  
};