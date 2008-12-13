#include "test_helper.h"

class KmeansTest : public ParseHelper
{
public:
  /*
    data means : [0.5, 2.5], [2.5, 0.5], [2.5, 2.5]
  */
  void test_single( void ) 
  { 
    parse("n=Kmeans(vector:2 data:\"test/fixtures/kmeans\")\nn.learn\nn.2=>p\nn=>p\n");
    
    assert_print("n.b(0.5,2)\n",   "0.25\n49\n");
    assert_print("n.b(1.5,1.5)\n", "2.00\n49\n");
    assert_print("n.b(2.5,0.5)\n", "0.00\n51\n");
  }
  
  /*
    data means : [0.5, 2.5], [2.5, 0.5], [2.5, 2.5]
  */
  void test_mahalanobis( void ) 
  { 
    parse("n=Kmeans(vector:2 data:\"test/fixtures/kmeans\" distance:\"Mahalanobis\")\nn.learn\nn.2=>p\nn=>p\n");
    
    assert_print("n.b(0.5,2.0)\n", "0.08\n49\n"); // 0.0833
    assert_print("n.b(1.0,2.5)\n", "0.75\n49\n"); // 0.75
    assert_print("n.b(2.5,0.5)\n", "0.00\n51\n");
  }
  
  /*
    data means : [0.5, 2.5], [2.5, 0.5], [2.5, 2.5]
  */
  void test_closest( void ) 
  { 
    parse("n=Kmeans(vector:2 data:\"test/fixtures/kmeans\" distance:\"Closest\")\nn.learn\nn.2=>p\nn=>p\n");
    
    assert_print("n.b(0.5,2.0)\n", "1.25\n49\n");
    assert_print("n.b(1.0,2.5)\n", "1.25\n50\n");
    assert_print("n.b(2.5,0.5)\n", "0.50\n51\n");
    assert_print("n.b(2.0,0.0)\n", "0.00\n51\n");
  }
};