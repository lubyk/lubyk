// compile with
// g++ -o load_test.bundle -dynamic -bundle -undefined suppress -flat_namespace  -L/usr/lib -lgcc -lstdc++ load_test.cpp 


#include <cstdio>
#include "load_test.h"

extern "C" void init(Dummy& master)
{  
  master.declare("I come from load_test.bundle.");
}