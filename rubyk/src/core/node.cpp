#include "class.h"
#include "observer.h"

Node::~Node() 
{
  std::list<Observer*>::iterator end = mObservers.end();
  for(std::list<Observer*>::iterator it = mObservers.begin(); it != end; it++) {
    // to avoid notification to this dying object
    (*it)->stop_observing(this, false);
  }

}


void Node::notify(uint key, const Value& pValue)
{
  std::list<Observer*>::iterator end = mObservers.end();
  for(std::list<Observer*>::iterator it = mObservers.begin(); it != end; it++)
    (*it)->changed(this, key, pValue);
}