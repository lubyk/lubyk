#include "group.h"

/** This is a group with it's own worker.

TODO: implement ! (inlets should lock/unlock when coming from another context)*/
class LiveGroup : public Group
{
 public: 
  /** Do not inherit the worker from the parent. */
  virtual void parent_changed() {}
};

// extern "C" void init(Worker& planet)
// {
//   planet.classes()->declare<Group>("Group", "Group functionalities together. Help organize all the mess.");
// }