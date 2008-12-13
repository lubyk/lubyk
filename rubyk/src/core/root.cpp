#include "root.h"
#include "class_finder.h"

/** Return the class listing Object (create one if needed). */
ClassFinder * Root::classes()
{
  Object * obj = find(CLASS_ROOT);
  if (!obj) {
    // create ClassFinder
    obj = adopt(new ClassFinder(std::string(CLASS_ROOT).substr(1, std::string::npos), DEFAULT_OBJECTS_LIB_PATH));
  }
  return TYPE_CAST(ClassFinder, obj);
}


/** Create a new object from a class name. Calls "/class/ClassName/new". */
const Value Root::new_object(const char * pUrl, const char * pClass, const Value& pParams)
{
  Hash h;
  h.set_key("url", String(pUrl));
  h.set_key("params", Hash(pParams));
  Value res = call(std::string(classes()->url()).append("/").append(pClass).append("/new"), h);
  if (!res.is_error()) create_pending_links();
  return res;
}

/** Create a new object from a class name. Calls "/class/ClassName/new". */
const Value Root::new_object(const std::string& pUrl, const std::string& pClass, const Value& pParams)
{  
  Hash h;
  h.set_key("url", String(pUrl));
  h.set_key("params", Hash(pParams));
  Value res = call(std::string(classes()->url()).append("/").append(pClass).append("/new"), h);
  if (!res.is_error()) create_pending_links();
  return res;
}

const Value Root::create_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo)
{
  //std::cout << "pending " << pFrom << "("<< pFromPort << ")" << " --> " << pTo << "("<< pToPort << ")" << std::endl;
  
  std::string url(pFrom);
  url.append("/out/").append(pFromPort).append("/link");
  String param(pTo);
  param.append("/in/").append(pToPort);
  
  // try to create link
  Value res = call(url, param);
  if (res.is_error()) mPendingLinks.push_back(Call(url,param));
  return res;
}

const Value Root::remove_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo)
{  
  std::string url(pFrom);
  url.append("/out/").append(pFromPort).append("/unlink");
  String param(pTo);
  param.append("/in/").append(pToPort);
  
  // try to remove link
  Value res = call(url, param);
  return res;
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Root::create_pending_links()
{
  std::list<Call>::iterator it,end;
  end = mPendingLinks.end();
  it  = mPendingLinks.begin();
  Value res;
  
  while(it != end) {
    res = call(it->mUrl, it->mParam);
    if (res.is_string()) 
      it = mPendingLinks.erase(it);  // call succeeded
    else
      it++;
  }
  // return list of created links ?
  return gNilValue;
}

/** Called by commands and other low priority threads. */
void Root::normal_priority()
{
  pthread_t id = pthread_self(); // this is a command thread

  if (pthread_setschedparam(id, mCommandSchedPoclicy, &mCommandThreadParam)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", mCommandThreadParam.sched_priority);
  }
}

/** Called during startup to increase thread priority. */
void Root::high_priority()
{
  struct sched_param param;
  int policy;
  pthread_t id = pthread_self();

  // save original scheduling parameters
  pthread_getschedparam(id, &mCommandSchedPoclicy, &mCommandThreadParam);
  
  // set to high priority
  param = mCommandThreadParam;
  param.sched_priority = 47; // magick number for Mac OS X
  policy = SCHED_RR;         // round robin
  
  
  if (pthread_setschedparam(id, policy, &param)) {
    fprintf(stderr, "Could not set thread priority to %i.\n", param.sched_priority);
  }
}
