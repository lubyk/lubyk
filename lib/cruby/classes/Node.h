#ifndef _NODE_H_
#define _NODE_H_

#include "Inlet.h"
#include "Outlet.h"

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */
  
class Node
{
public:
  Node() : mTriggerPosition(0), mId(0) {}
  
  /** Compute new values for each outlet and send values through connections. */
  void bang (void)
  {
    int i = mOutlets.size() - 1;
    
    while(i >= 0) {
      /** Send through rightmost outlet first. */
      ((Outlet)mOutlets[i--]).compute_and_send();
    }
  }
  
  inline float trigger_position() { return mTriggerPosition; }
  
  Inlet  * inlet  (int slot_id) { return &( mInlets[slot_id]); }
  Outlet * outlet (int slot_id) { return &(mOutlets[slot_id]); }
  
protected:
  long  mId;
  float mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  
  std::vector<Inlet>  mInlets;
  std::vector<Outlet> mOutlets;
  
private:
    
  template <class T, void(T::*Tmethod)(float)>
  void declare_inlet ()
  {
    Inlet s(this,&cast_inlet_method<T, Tmethod>);
    s.setId(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(s);
  }

  template <class T, float(T::*Tmethod)(void)>
  void declare_outlet ()
  {
    Outlet s(this,&cast_outlet_method<T, Tmethod>);
    s.setId(mOutlets.size()); /* first outlet has id 0 */
    mOutlets.push_back(s);
  }

  template <class T, void(T::*Tmethod)(float)>
  static void cast_inlet_method (void * receiver, float value)
  {
    (((T*)receiver)->*Tmethod)(value);
  }
  
  template <class T, float(T::*Tmethod)(void)>
  static float cast_outlet_method (void * receiver)
  {
    return (((T*)receiver)->*Tmethod)();
  }
};

#endif