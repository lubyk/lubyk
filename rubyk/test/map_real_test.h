#include "test_helper.h"
#include "map_real.h"

class MapRealTest : public TestHelper
{
public:
  void test_map_in_bounds( void ) {
    MapReal mapper(1, 3, 0, 1);
    assert_equal(0.0, mapper.scale(1));
    assert_equal(0.5, mapper.scale(2));
    assert_equal(1.0, mapper.scale(3));
  }
  
  void test_map_out_of_bounds_should_clip( void ) {
    MapReal mapper(1, 3, 0, 1);
    assert_equal(0.0, mapper.scale(-12));
    assert_equal(0.0, mapper.scale(0));
    assert_equal(1.0, mapper.scale(100));
  }
};