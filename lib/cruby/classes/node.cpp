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
    
  if (mSpy != NULL)
    free(mSpy);
  
  if (mInspect != NULL)
    free(mInspect);
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

void Node::spy_print(const char *fmt, ...)
{
  int n;
  char * np;
  va_list ap;
  
  if (mSpy == NULL) {
    mSpy = (char*)malloc(START_SPY_BUFFER);
    if (mSpy) mSpySize = START_SPY_BUFFER;
  }
  
  while (1) {
     /* Try to print in the allocated space. */
     va_start(ap, fmt);
     n = vsnprintf (mSpy, mSpySize, fmt, ap);
     va_end(ap);
     /* If that worked, return the string. */
     if (n > -1 && n < mSpySize)
        return; // OK
     /* Else try again with more space. */
     if (n > -1)    /* glibc 2.1 */
        mSpySize = n+1; /* precisely what is needed */
     else           /* glibc 2.0 */
        mSpySize *= 2;  /* twice the old size */
     if ((np = (char*)realloc (mSpy, mSpySize)) == NULL) {
         mSpy[mSpySize - 1] = '\0';
        return; // no more memory. fail.
     } else {
        mSpy = np;
     }
  }
}


void Node::inspect_print(const char *fmt, ...)
{
  int n;
  char * np;
  va_list ap;
  
  if (mInspect == NULL) {
    mInspect = (char*)malloc(START_SPY_BUFFER);
    if (mInspect) mInspectSize = START_SPY_BUFFER;
  }
  
  while (1) {
     /* Try to print in the allocated space. */
     va_start(ap, fmt);
     n = vsnprintf (mInspect, mInspectSize, fmt, ap);
     va_end(ap);
     /* If that worked, return the string. */
     if (n > -1 && n < mInspectSize)
        return; // OK
     /* Else try again with more space. */
     if (n > -1)    /* glibc 2.1 */
        mInspectSize = n+1; /* precisely what is needed */
     else           /* glibc 2.0 */
        mInspectSize *= 2;  /* twice the old size */
     if ((np = (char*)realloc (mInspect, mInspectSize)) == NULL) {
         mInspect[mInspectSize - 1] = '\0';
        return; // no more memory. fail.
     } else {
        mInspect = np;
     }
  }
}

const char * Node::inspect() {
  printf("A\n");
  if (mIsOK)
    inspect_print("#<%s:%s %s>", mClassName.c_str(), mVariableName.c_str(), get_spy());
  else
    inspect_print("#<%s:%s X>", mClassName.c_str(), mVariableName.c_str(), get_spy());
  
  printf("B\n");
  if (mInspect)
    return mInspect;
  else
    return "";
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