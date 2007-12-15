#include "node.h"
#include "event.h"
#include "rubyk.h"
#include "class.h"


unsigned int Node::sIdCounter = 0;
pthread_key_t Node::sThisKey  = NULL;

Node::~Node()
{
  // we have to do this here before ~Node, because some events have to be triggered before the node dies (note off).
  remove_my_events();
  unloop_me();
  stop_my_threads();
  
  for(std::vector<Outlet*>::iterator it = mOutlets.begin(); it < mOutlets.end(); it++)
    delete *it;
  
  for(std::vector<Inlet*>::iterator it = mInlets.begin(); it < mInlets.end(); it++)
    delete *it;
    
  if (mSpy != NULL)
    free(mSpy);
  
  if (mInspect != NULL)
    free(mInspect);
  
  //if (mBuf != NULL)
  //  free(mBuf);
}

const char * Node::class_name() const
{ 
  return mClass->name().c_str(); 
}

const char * Node::bprint(char *& pBuffer, size_t& pBufferSize, const char *fmt, ...)
{
  int n;
  char * np;
  va_list ap;
  
  if (pBuffer == NULL) {
    pBuffer = (char*)malloc(START_PRINT_BUFFER * sizeof(char));
    if (pBuffer) pBufferSize = START_PRINT_BUFFER;
  }
  
  while (1) {
     /* try to print in the allocated space. */
     va_start(ap, fmt);
     n = vsnprintf (pBuffer, pBufferSize, fmt, ap);
     va_end(ap);
     /* if that worked, return the string. */
     if (n > -1 && n < (int)pBufferSize)
        return pBuffer; // OK
     /* else try again with more space. */
     if (n > -1)    /* glibc 2.1 */
        pBufferSize = n+1; /* precisely what is needed */
     else           /* glibc 2.0 */
        pBufferSize *= 2;  /* twice the old size */
     if ((np = (char*)realloc (pBuffer, pBufferSize * sizeof(char))) == NULL) {
         pBuffer[pBufferSize - 1] = '\0';
        return "could not print (no more memory)"; // no more memory. fail.
     } else {
        pBuffer = np;
     }
  }
}

/** Print message into string. */
void Node::bprint (std::string& pStr, const char *fmt, ...) const
{
  int n;
  char * np;
  va_list ap;

  size_t buf_size;
  char * buf = (char*)malloc(START_PRINT_BUFFER * sizeof(char));
  if (buf) buf_size = START_PRINT_BUFFER;
  else {
    pStr = "no more memory!";
    return;
  }
  
  while (1) {
     /* try to print in the allocated space. */
     va_start(ap, fmt);
     n = vsnprintf (buf, buf_size, fmt, ap);
     va_end(ap);
     /* if that worked, return the string. */
     if (n > -1 && n < (int)buf_size) {
       pStr = buf;
       free(buf);
       return; // OK
     }
     /* else try again with more space. */
     if (n > -1)    /* glibc 2.1 */
        buf_size = n+1; /* precisely what is needed */
     else           /* glibc 2.0 */
        buf_size *= 2;  /* twice the old size */
     if ((np = (char*)realloc (buf, buf_size * sizeof(char))) == NULL) {
       if (buf) free(buf);
       pStr = "no more memory!";
       return; // no more memory. fail.
     } else {
       buf = np;
     }
  }
  pStr = buf;
  free(buf);
}

const char * Node::inspect() {
  if (mIsOK)
    bprint(mInspect, mInspectSize, "#<%s:%s %s>", class_name(), mName.c_str(), get_spy());
  else
    bprint(mInspect, mInspectSize, "#<%s:%s ### (%s)>", class_name(), mName.c_str(), get_spy());
  
  if (mInspect)
    return mInspect;
  else
    return "";
}


void Node::execute_method (const std::string& pMethod, const Params& p)
{
  member_method_t method;
  if (pMethod == "help" ){
    help();
  } else if (pMethod == "debug" || pMethod == "d") {
    debug();
  } else if (pMethod == "set" || pMethod == "s") {
    mIsOK = set(p);
  } else if (pMethod == "b" || pMethod == "bang") {
    bang(p);
  } else if (mClass->get_member_method(&method, pMethod)) {
    (*method)(this, p);
  } else {
    // try to use an inlet
    
    
    *mOutput << "Unknown method '" << pMethod << "'\n";
  }
}