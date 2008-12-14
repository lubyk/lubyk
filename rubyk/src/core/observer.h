#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <list>
#include "node.h"

/** An object that is notified of node changes. */
class Observer
{
public:
  virtual ~Observer()
  {
    for(std::list<Node*>::iterator it = mObservedNodes.begin(); it != mObservedNodes.end(); it++) {
      // to avoid notification to this dying object
      (*it)->unregister_observer(this);
    }
  }
  
  /** Notification trigger. */
  virtual void changed(Node * node, uint key, const Value& pVal)
  {}
  
  void observe(Node * node)
  {
    node->register_observer(this);
    mObservedNodes.push_back(node);
  }
  
  void stop_observing(Node * node, bool pUnregister = true)
  {
    if (pUnregister) node->unregister_observer(this);
    
    std::list<Node*>::iterator end = mObservedNodes.end();
    for(std::list<Node*>::iterator it = mObservedNodes.begin(); it != end; it++) {
      if (*it == node){
        mObservedNodes.erase(it);
        break;
      }
    }
  }
  
private:
  std::list<Node *> mObservedNodes; /**< Nodes observed. */
};

//struct ObservingLink
//{
//  ObservingLink(Observer * pObserver, Node * pNode, time_t pTime) : mObserver(pObserver), mNode(pNode), mSince(pTime) {}
//  ~ObservingLink()
//  {
//    if (mNode) mNode->remove_observer
//  }
//  time_t mSince;
//  Observer * mObserver;
//  Node     * mNode;
//};

#endif // _OBSERVER_H_