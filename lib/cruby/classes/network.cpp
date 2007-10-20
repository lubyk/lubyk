#include "network.h"

Network::Network(const std::string& pPath)
{
   
}

Network::~Network()
{
  for(std::list<Node*>::iterator it = mNodesList.begin(); it != mNodesList.end(); it++) {
    delete *it;
  }
}
 
void Network::run();