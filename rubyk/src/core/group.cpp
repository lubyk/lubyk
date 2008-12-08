#include "class.h"

bool Group::init (const Value& p)
{
  return true;
}

bool Group::set (const Value& p)
{
  return true;
}

void Group::spy() 
{ bprint(mSpy, mSpySize,"");  }

void Group::bang(const Value& val)
{ }
