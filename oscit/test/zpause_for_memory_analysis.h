#include <unistd.h>

class PauseTest : public TestHelper
{
public:

  void test_pause( void ) {
    std::cout << "\nPausing for memory leaks analysis.\nYou can now run:\n> leaks test_runner\n";
    pause();
  }
};