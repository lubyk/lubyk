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
  void get()
  { 
    struct termios oldt, newt;
    mServer->unlock(); // Let the server breath. We are in a lock--unlock from Command
    
    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    
    while((mChar = getchar()) != '\e') { // until escape
      mServer->lock();
        // protected resource
        bang_me_in(0);
      mServer->unlock();
    }
    
    // restore old settings
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
    
    mServer->lock();
  }
  
  void send(Signal& sig)
  {
    sig.set(mChar);
  }
private:
  int mChar;
};

extern "C" void init()
{
  CLASS (Keyboard)
  OUTLET(Keyboard,send)
  METHOD(Keyboard,get)
}