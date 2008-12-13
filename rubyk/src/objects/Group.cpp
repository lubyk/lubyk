#include "rubyk.h"
#include "group.h"

// implementation is in "src/core/group.cpp"

extern "C" void init(Root& root)
{
  root.classes()->declare<Group>("Group", "Group functionalities together. Help organize all the mess.");
}