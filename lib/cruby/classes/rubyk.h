#ifndef _RUBYK_H_
#define _RUBYK_H_
#include "node.h"
#include "link.h"
#include <list>

class Rubyk
{
public:
  Rubyk() : mInstances(200), mQuit(false) {}
  
  virtual ~Rubyk();
  
  Node * create_instance(const std::string& pVariable, const std::string& pClass, const Params& pParams);
  
  void create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
  
  Node * get_instance(const std::string& pVariable);
  
  void quit()
  { mQuit = true; }
  
  /** Main loop. Returns false when rubyk has to quit. */
  bool run();
  
private:
  void create_pending_links();
  bool mQuit;
  /** Reference to the objects in the system. */
  Hash<std::string, Node *> mInstances;
  
  /** List of pending connections waiting for variable assignements. */
  std::list<Link> mPendingLinks;
  //std::list<Link> mLinksSet;
  
};

#endif