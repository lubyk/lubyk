#include "rubyk.h"
#include "group.h"

extern "C" void init(Worker& planet)
{
  planet.classes()->declare<Group>("Group", "Group functionalities together. Help organize all the mess.");
}