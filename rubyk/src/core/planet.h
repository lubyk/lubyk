#ifndef _PLANET_H_
#define _PLANET_H_
#include "group.h"
#include "root.h"
#include "osc.h"
#include "command.h"
#include <csignal>
#include <fstream>

class Planet : public Group
{
public:
  Planet() : mOscIn(&mRoot)
  {
    // force build of "/class"
    mRoot.classes();
  }
  
  Planet(int argc, char * argv[]) : mOscIn(&mRoot)
  {
    mRoot.classes();
    // TODO: all this should go into Planet...
    if (argc > 1) {
      std::ifstream in(argv[1], std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
      in.close();

      Command  * fCmd;
      fCmd = new Command(std::cin, std::cout); // we use new because cleanup code is not executed for this thread due to opengl glutMainLoop
      fCmd->set_root(&mRoot);
      fCmd->set_silent(true);
      mRoot.unlock(); // so the commands are directly processed
        oss << "\n";
        fCmd->parse(oss.str());
      mRoot.lock();

      fCmd->close();
      delete fCmd;
    }

  }
  
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
  OscReceive mOscIn;
};

#endif // _PLANET_H_