#include "group.h"

bool Group::init (const Value& p)
{
  return true;
}

const Value Group::inspect(const Value val)
{ 
  return String("Group");
}

void Group::bang(const Value val)
{ }
