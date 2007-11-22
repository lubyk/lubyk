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
    int mode = 2;
    struct termios oldt, newt;
    mServer->unlock(); // Let the server breath. We are in a lock--unlock from Command
    
    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    
    while(1) {
      c = getchar();
      if (mode == 2 && c == '\e') {
        mode = 1;
        continue;
      } else if (mode == 1 && c == '\e') {
        break;
      } else if (mode == 1 && c == 91) {
        mode = 3;
        continue;
      } else if (mode == 1) {
        // send both \e and current
        mode = 2;
        
        mServer->lock();
          // protected resource
          send('\e');
        mServer->unlock();
      } else if (mode == 3 && c == 68) {
        // left arrow
        c = RK_LEFT_ARROW;
        mode = 2;
      } else if (mode == 3 && c == 67) {
        c = RK_RIGHT_ARROW;
        mode = 2;
      } else {
        mode = 2;
      }
      if (mode <= 0) break;  // esc+esc to leave
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