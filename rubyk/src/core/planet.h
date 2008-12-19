#ifndef _PLANET_H_
#define _PLANET_H_
#include "group.h"
#include "root.h"
#include "command.h"
#include <csignal>
#include <fstream>
#include "osc_receive.h"
#include "osc_send.h"

#define DEFAULT_RECEIVE_PORT 7000

class Planet : public Group
{
public:
  Planet()
  {
    mOscIn = new OscReceive(this, DEFAULT_RECEIVE_PORT);
    // force build of "/class"
    mRoot.classes();
  }
  
  Planet(uint pPort)
  {
    mOscIn = new OscReceive(this, pPort);
    // force build of "/class"
    mRoot.classes();
  }
  
  Planet(int argc, char * argv[])
  {
    // TODO get port from command line
    mOscIn = new OscReceive(this, DEFAULT_RECEIVE_PORT);
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
  
  ~Planet ()
  {
    std::list<OscSend*>::iterator it  = mSatellites.begin();
    std::list<OscSend*>::iterator end = mSatellites.end();
    
    delete mOscIn;
    
    while (it != end) delete *it++;
  }
  
  void quit()
  {
    mRoot.quit();
  }
  
  bool init (const Value& p)
  {
    return true;
  }
  
  virtual const Value inspect(const Value& val)
  { 
    return String("Planet");
  }
  
  virtual void bang(const Value& val) {}
  
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
  
  void receive(const std::string& pUrl, const Value& pParams)
  {
    Value res;
    
    if (pParams.is_error()) {
      res = pParams;
    } else {
      res = mRoot.call(pUrl,pParams);
    }
    send_reply(pUrl, res);
  }
  
  inline void send_reply(const std::string& pUrl, const Value& pVal)
  {
    OscSend::send_all(mSatellites, pUrl, pVal);
  }
  
  void register_satellite(OscSend * pSatellite)
  {
    // FIXME: avoid duplicates, implement TTL...
    mSatellites.push_back(pSatellite);
  }
  
  Root * root()
  {
    return &mRoot;
  }
  
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
  
  Root mRoot;                      /**< Processing tree. */
  OscReceive * mOscIn;             /**< Listening socket. */
  std::list<OscSend*> mSatellites; /**< List of satellites that have registered to get return values back. */
};

#endif // _PLANET_H_