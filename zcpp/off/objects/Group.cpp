#include "lubyk.h"
#include "group.h"

extern "C" void init(Planet &planet) {
  planet.classes()->declare<Group>("Group", "Group functionalities together. Help organize all the mess.");
}