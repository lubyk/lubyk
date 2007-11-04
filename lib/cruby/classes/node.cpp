#include "node.h"
#include "event.h"
#include "rubyk.h"
#include "class.h"


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

const char * Node::class_name() const
{ 
  return mClass->mName.c_str(); 
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
  if (mIsOK)
    inspect_print("#<%s:%s %s>", class_name(), mName.c_str(), get_spy());
  else
    inspect_print("#<%s:%s X>", class_name(), mName.c_str(), get_spy());
  
  if (mInspect)
    return mInspect;
  else
    return "";
}


void Node::execute_method (const std::string& pMethod, const Params& p, std::ostream * pOutput)
{
  mOutput = pOutput;
  member_method_t method;
  if (pMethod == "bang") {
    bang();
    *mOutput << inspect() << std::endl;
  } else if (pMethod == "help" ){
    help();
  } else if (mClass->mMethods.get(&method, pMethod)) {
    (*method)(this, p);
  } else {
    *mOutput << "Unknown method '" << pMethod << "'\n";
  }
}

void Node::bang_me_in (long double pTime)
{
  BaseEvent * e = (BaseEvent *) new CallEvent<Node, &Node::bang>(mServer->mCurrentTime + pTime, this);
  
  mServer->register_event( e );
}


// This would be better inside 'signal.cpp', but it breaks... when I include <iostream> in signal.h ?
std::ostream& operator<< (std::ostream& pStream, const Signal& sig)
{
  char buffer[50];
  switch(sig.type) {
  case Bang:
    pStream << "Bang!\n";
    break;
  case Integer:
    pStream << sig.i.value << std::endl;
    break;
  case Float:
    snprintf(buffer, 50, "%.2f", sig.f.value);
    pStream << buffer << std::endl;
    break;
  case FloatArray:
    snprintf(buffer, 50, "{%i,%i}",sig.floats.value, sig.floats.size);
    pStream << buffer << std::endl;
    break;
  case VoidPointer:
    snprintf(buffer, 50, "[%p,%i]",sig.ptr.value, sig.ptr.free_me);
    pStream << buffer << std::endl;
    break;
  default:
    pStream << "??\n";
  }
}
