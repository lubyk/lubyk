#include "test_helper.h"

class KmeansTest : public CxxTest::TestSuite, public ParseTest
{
public:
  /*
    data means : [0.5, 2.5], [2.5, 0.5], [2.5, 2.5]
  */
  void test_single( void ) 
  { 
    setup_with_print("n=Kmeans(vector:2 data:\"test/fixtures/kmeans\")\nn.learn\nn.2=>p\n");
    
    assert_print("n.b(0.5,2)\n",   "0.12\n49\n"); // square distances 0.125, 2.125, 3.125
    assert_print("n.b(1.5,1.5)\n", "1.00\n49\n"); // square distances 1.0, 1.0, 1.0
    assert_print("n.b(2.5,0.5)\n", "0.00\n51\n"); // square distances 4.0, 2.0, 0.0
  }
  
};