#include <cxxtest/TestSuite.h>
#include "test_helper.h"
#include "oscit/alias.h"

//FIX class AliasTest : public TestHelper
//FIX {
//FIX public:
//FIX   void test_trigger( void )
//FIX   {
//FIX     oscit::Root root;
//FIX     oscit::Object       * one = root.adopt(new oscit::Object("one")); //   [/one]
//FIX     DummyNumber  * sub = one->adopt(new DummyNumber("sub", 3)); //   [/one/sub]
//FIX     oscit::Alias        * ali = root.adopt(new oscit::Alias("ali", sub));
//FIX     DummyNumber  * two = root.adopt(new DummyNumber("two", 2)); //  [/two]
//FIX     Value res;
//FIX     
//FIX     TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
//FIX     TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
//FIX     TS_ASSERT_EQUALS( ali->url(),     std::string("/ali") );
//FIX     
//FIX     TS_ASSERT_EQUALS(sub->mCounter.value(), 3.00);
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 3.00);
//FIX     res = root.call("/ali");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 3.00);
//FIX     
//FIX     res = root.call("/one/sub", "4.50");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 4.50);
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 4.50);
//FIX     res = root.call("/ali");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 4.50);
//FIX     res = root.call("/two");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 2.00);
//FIX     
//FIX     res = root.call("/ali", "23");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 23.00);
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 23.00);
//FIX     
//FIX     
//FIX     ali->set_original(std::string("/two"));
//FIX     
//FIX     res = root.call("/ali", "7");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 7.00);
//FIX     res = root.call("/two");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 7.00);
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 23.00);
//FIX     
//FIX     delete two; // delete original first
//FIX     res = root.call("/ali", "23");
//FIX     TS_ASSERT_EQUALS(res.to_string(), "[8] #Object '/ali' not found.");
//FIX   }
//FIX   
//FIX   void test_delete_alias_first( void )
//FIX   {
//FIX     oscit::Root root;
//FIX     oscit::Object       * one = root.adopt(new oscit::Object("one")); //   [/one]
//FIX     DummyNumber  * sub = one->adopt(new DummyNumber("sub", 3)); //   [/one/sub]
//FIX     oscit::Alias        * ali = root.adopt(new oscit::Alias("ali", sub));
//FIX     Value res;
//FIX     
//FIX     TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
//FIX     TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
//FIX     TS_ASSERT_EQUALS( ali->url(),     std::string("/ali") );
//FIX     
//FIX     TS_ASSERT_EQUALS(sub->mCounter.value(), 3.00);
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 3.00);
//FIX     res = root.call("/ali");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 3.00);
//FIX     
//FIX     delete ali;
//FIX     res = root.call("/ali");
//FIX     TS_ASSERT_EQUALS(res.to_string(), "[2] #Object '/ali' not found.");
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(Number(res).value(), 3.00);
//FIX     
//FIX     delete one;
//FIX     res = root.call("/one/sub");
//FIX     TS_ASSERT_EQUALS(res.to_string(), "[3] #Object '/one/sub' not found.");
//FIX   }
//FIX   
//FIX };