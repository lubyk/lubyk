#include "planet.h"

bool Planet::init (const Value& p)
{
  return true;
}

const Value Planet::inspect(const Value& val)
{ 
  return String("Planet");
}

void Planet::bang(const Value& val)
{ }
