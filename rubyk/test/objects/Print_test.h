#include "test_helper.h"

class PrintTest : public ParseHelper
{
public:
  
  void test_create( void ) {
    assert_result("# <Print:/p prefix:\"p\">\n", "p=Print()\n");
  }
  
  void test_prefix( void ) {
    assert_result("# <Print:/p1 prefix:\"Turing\">\n", "p1=Print('Turing')\n");
    assert_result("# <Print:/p2 prefix:\"Ada\">\n",    "p2=Print(prefix:'Ada')\n");
  }

  void test_print_method( void ) {
    setup_with_print("\n");
    assert_print("p: Haha\n", "p/print('Haha')\n");
  }

};