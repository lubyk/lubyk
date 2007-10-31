#ifndef _RUBYK_H_
#define _RUBYK_H_
#include "node.h"
#include "link.h"
#include <list>

class Rubyk
{
public:
  Rubyk() : mInstances(200) {}
  virtual ~Rubyk();
  Node * create_instance(const std::string& pVariable, const std::string& pClass, const Params& pParams);
  void create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
private:
  void create_pending_links();
  
  /** Reference to the objects in the system. */
  Hash<std::string, Node *> mInstances;
  
  /** List of pending connections waiting for variable assignements. */
  std::list<Link> mPendingLinks;
  //std::list<Link> mLinksSet;
  
};

#endif