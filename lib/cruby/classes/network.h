#ifndef _NETWORK_H_
#define _NETWORK_H_
#include "node.h"


/** A Network is a group of nodes connected together and doing their job. The network is responsible for
  * putting the nodes to life (instanciating) and managing their connections. It is also the gateway from/to
  * editors. There may only be one network per thread. */
class Network
{
public:
  /** Instantiate a network from a text file. */
  Network(const std::string& pPath);
  ~Network();
  
  void run();
private:
  Hash<uint, Node*> mNodesById; /**< All nodes in the system, indexed by 'id'. */
  std::list<Node *> mNodesList; /**< All nodes in the system, list. */
};

#endif