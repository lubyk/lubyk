#include <dlfcn.h>
#include "node.h"
#include "event.h"
#include "rubyk.h"

// definitions of static data members

Hash<std::string, class_creator_function_t> Node::sClasses(300);
std::string Node::sObjectsPath("lib");
unsigned int Node::sIdCounter = 0;

Node::~Node()
{
  for(std::vector<Outlet*>::iterator it = mOutlets.begin(); it < mOutlets.end(); it++)
    delete *it;
  
  for(std::vector<Inlet*>::iterator it = mInlets.begin(); it < mInlets.end(); it++)
    delete *it;
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool Node::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(void);
  const char *error = 0;
  
  // load shared extension image into memory
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if (error = dlerror()) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // get 'init' function into the image
  function = (void(*)(void))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    printf("Symbol '%s' not found in '%s'.",init_name,file);
    if (error = dlerror()) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // call 'init', passing the registration object
  (*function)();

  return true;
}

const char * Node::inspect() {
  // FIXME: buffer overflow from mClassName...
  if (mIsOK)
    sprintf(mInspect, "#<%s:%s %s>", mClassName.c_str(), mVariableName.c_str(), spy());
  else
    sprintf(mInspect, "#<%s:%s X>", mClassName.c_str(), mVariableName.c_str(), spy());
  return mInspect;
}


void Node::execute_method (const std::string& pMethod, const Params& pParams)
{
  if (pMethod == "bang") {
    bang();
  } else {
    // FIXME....
  }
}

void Node::bang_me_in (long double pTime)
{
  BaseEvent * e = (BaseEvent *) new CallEvent<Node, &Node::bang>(mServer->mCurrentTime + pTime, this);
  mServer->register_event( e );
}