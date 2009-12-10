#include "test_helper.h"
#include "oscit/location.h"

class LocationTest : public TestHelper
{
public:
  void test_create( void ) {
    Location loc1("oscit", "my place");
    Location loc2("oscit", 167772167, 45);
  }

  void test_to_stream( void ) {
    Location loc1("oscit", "my place");
    Location loc2("oscit", 167772167, 45);
    std::ostringstream out(std::ostringstream::out);
    out << loc1;
    assert_equal("oscit://\"my place\"", out.str());
    out.str("");
    out << loc2;
    assert_equal("oscit://10.0.0.7:45", out.str());
  }

  void test_hashId( void ) {
    Location loc1("oscit", "some place");
    Location loc2("oscit", "other place");
    assert_equal(1838689659, hashId(loc1));
  }

  void test_equal_operator( void ) {
    Location loc1("oscit", "some place");
    Location loc2("oscit", "other place");
    Location loc3("oscit", "some place");
    assert_false(loc1 == loc2);
    assert_true( loc1 == loc3);
  }

};
