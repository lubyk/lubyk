#include "class.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

class Keyboard : public Node
{
public:
  
  bool init (const Params& p)
  {
    return true;
  }
  
  // capture method, called from a command
  void bang(const Signal& sig)
  { 
    // do nothing
  }
  
  void get(const Params& p)
  {
    int c;
    struct termios oldt, newt;
    mServer->unlock(); // Let the server breath. We are in a lock--unlock from Command
    
    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    
    while((c = getchar()) != '\e') { // until escape
      mServer->lock();
        // protected resource
        send(c);
      mServer->unlock();
    }
    
    // restore old settings
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
    
    mServer->lock();
  }
  
};

extern "C" void init()
{
  CLASS (Keyboard)
  METHOD(Keyboard,get)
  OUTLET(Keyboard,send)
}