/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <list>
#include "node.h"


///// This should be changed for a link based observer:
///// To observe metro/out/bang, you create a "send" Node and a link to listen to an outlet:
///// metro/out/bang -----> sender ---> observer's url
///// Something like the following:

class Send : public Node
{
 public:
  /** [1] sends received data to the local (same planet) or remote (through OSC) node. */
  virtual void bang (const Value &val) {
    call(spy_url_, val);
  }
  
 private:
  std::string spy_url_; /**< Url of the (local or remoate) observing node. */
};

////// Debugging: 
class Print : public Node
{
 public:
  /** [1] prints received data to stdout. */
  virtual void bang (const Value &val) {
    std::cout << prefix_ << val << std::endl;
  }
  
  /** Set the prefix shown before printed data. Should be "name:port" for debugging. */
  void set_prefix(const std::string &string) {
    prefix_ = std::string("[").append(string).append("] ");
  }
 private:
   std::string prefix_;
};
  

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
  virtual void changed(Node * node, uint key, const Value &pVal)
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
//  ObservingLink(Observer * pObserver, Node * node, time_t pTime) : mObserver(pObserver), mNode(node), mSince(pTime) {}
//  ~ObservingLink()
//  {
//    if (mNode) mNode->remove_observer
//  }
//  time_t mSince;
//  Observer * mObserver;
//  Node     * mNode;
//};

#endif // _OBSERVER_H_