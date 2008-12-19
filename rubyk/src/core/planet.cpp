#include "planet.h"
#include "osc.h"

Planet::Planet()
{
  mOscIn = new OscReceive(&mRoot);
  // force build of "/class"
  mRoot.classes();
}

Planet::Planet(int argc, char * argv[])
{
  mOscIn = new OscReceive(&mRoot);
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

Planet::~Planet()
{
  delete mOscIn;
}

bool Planet::init (const Value& p)
{
  return true;
}

const Value Planet::inspect(const Value& val)
{ 
  return String("Planet");
}

void Planet::bang(const Value& val)
{ }
