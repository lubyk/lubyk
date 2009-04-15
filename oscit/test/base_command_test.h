#include "test_helper.h"
#include "oscit/thread.h"
#include <sstream>

struct DummyCommand : public BaseCommand
{
 public:
  DummyCommand(std::string *string) : string_(string) {}
  
  void do_listen() {
    while (thread_.run()) {
      string_->append(".");
      microsleep(10);
    }
  }
  
  std::string * string_;
};

class BaseCommandTest : public TestHelper
{  
public:
  void test_create_delete( void ) {
    Root * root = new Root;
    std::string string;
    root->adopt_command(new DummyCommand(&string));
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    microsleep(15);
    delete root;
    // should join here
    assert_equal("..", string);
  }
  
  void test_many_root_many_commands( void ) {
    Root * root1 = new Root;
    Root * root2 = new Root;
    std::string string1;
    std::string string2;
    std::string string3;
    root1->adopt_command(new DummyCommand(&string1));
    root1->adopt_command(new DummyCommand(&string2));
    root2->adopt_command(new DummyCommand(&string3));
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    microsleep(15);
    delete root1;
    delete root2;
    
    // should join here
    assert_equal("..", string1);
    assert_equal("..", string2);
    assert_equal("..", string3);
  }
};