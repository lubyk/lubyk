#include "class.h"

bool Group::init (const Params& p)
{
  return true;
}

bool Group::set (const Params& p)
{
  return true;
}

void Group::spy() 
{ bprint(mSpy, mSpySize,"");  }

void Group::bang(const Signal& sig)
{ }
