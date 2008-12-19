#ifndef _PLANET_H_
#define _PLANET_H_
#include "group.h"
#include "root.h"
#include "command.h"
#include <csignal>
#include <fstream>

class OscReceive;

class Planet : public Group
{
public:
  Planet();
  
  Planet(int argc, char * argv[]);
  
  ~Planet ();
  
  void quit()
  {
    mRoot.quit();
  }
  
  bool init (const Value& p);
  
  virtual const Value inspect(const Value& val) ;
  
  virtual void bang(const Value& val);
  
  /** Run until quit (through a command or signal). */
  void run()
  {
    Root::set_thread_this(this);
    
    signal(SIGTERM, Planet::term); // register a SIGTERM handler
    signal(SIGINT,  Planet::term);

    while (mRoot.run());
  }
  
  void listen_to_command(Command& pCmd)
  {
    mRoot.listen_to_command(pCmd);
  }
  
  //Root * tree()
  //{
  //  return &mRoot;
  //}
  
protected:
  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void do_adopt(Object * pObject)
  {
    mRoot.do_adopt(pObject);
  }
  
private:
  static void term(int sig)
  {
    ((Planet*)Root::thread_this())->quit();
  }
  
  Root mRoot;
  OscReceive * mOscIn;
};

#endif // _PLANET_H_