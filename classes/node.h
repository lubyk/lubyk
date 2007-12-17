#ifndef _NODE_H_
#define _NODE_H_

#include "inlet.h"
#include "outlet.h"
#include "params.h"
#include "hash.h"
#include "matrix.h"
#include "rubyk.h"
#include "rubyk_signal.h"
#include "event.h"

#include <sstream>
#include <cstdio>
#include <vector>
#include <string>


#define START_PRINT_BUFFER   20
#define MAX_CLASS_NAME       50
#define START_INSPECT_BUFFER (START_SPY_BUFFER + MAX_CLASS_NAME + 5 + 16)

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */  
class Node
{
public:
  Node() : mClass(NULL), mServer(NULL), mId(0), mLooped(false), mIsOK(true), mSpy(NULL), mSpySize(0), mInspect(NULL), mInspectSize(0), mOutput(&std::cout), mDebug(false)
  { 
    char buf[50];
    sIdCounter++;
    mTriggerPosition = sIdCounter; // last created is further on the right
    sprintf(buf,"_%i",sIdCounter);
    mName = std::string(buf);  // default variable name is 'id'
  }
  
  virtual ~Node();
  
  bool init(const Params& p) { return true; }
  
  /** Add an inlet with the given callback (used by Class during instantiation). */
  void add_inlet(inlet_method_t pCallback)
  {
    Inlet * s = new Inlet(this,pCallback);
    s->setId(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(s);
  }
  
  /** Add an outlet. (used by Class during instantiation).
    * Name not used for the moment. */
  void add_outlet(std::string& pName)
  {
    Outlet * s = new Outlet(this);
    s->setId(mOutlets.size()); /* first inlet has id 0 */
    mOutlets.push_back(s);
  }
  
  void set_server(Rubyk * pServer)
  { mServer = pServer; }
  
  void set_class(Class * pClass)
  { mClass = pClass; }
  
  void set_output(std::ostream * pOutput)
  { mOutput = pOutput; }
  
  std::ostream& output()
  { return *mOutput; }
  
  void execute_method (const std::string& pMethod, const Params& p);
  
  /** This method must be implemented in subclasses. It's the place where most
    * of the work should be done. This method is responsible for sending the signals out. */
  virtual void bang (const Signal& sig) = 0;
  
  void bang (const Params& p)
  {
    Signal sig;
    Matrix buf;
    sig.set(p, buf);
    bang(sig);
  }
  
  /** This method must be implemented in subclasses. It is used to set parameters that
    * can be changed during runtime. */
  virtual bool set (const Params& p) = 0;
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  const char* get_spy() {
    spy();
    if (mSpy)
      return mSpy;
    else
      return "--";
  }
  
  ///////// send signals ///////////////
  
  template<typename T>
  inline void send(const T& data)
  { send(1, data); }
  
  /** Send a bang. */
  inline void send(size_t pPort, rubyk_signal_t pType) 
  { 
    if (!pType) return; // do nothing if NilSignal
    mS.type = pType;
    send(pPort, mS);
  }
  
  /** Send an integer. */
  inline void send(size_t pPort, int pInt)
  {
    mS.type = IntegerSignal;
    mS.i.value = pInt;
    send(pPort, mS);
  }
  
  /** Send a char. */
  inline void send(size_t pPort, char pChar)
  {
    mS.type = CharSignal;
    mS.c.value = pChar;
    send(pPort, mS);
  }
  
  /** Send an unsigned integer. */
  inline void send(size_t pPort, unsigned int pInt)
  {
    mS.type = IntegerSignal;
    mS.i.value = pInt;
    send(pPort, mS);
  }
  
  /** Send an unsigned long. */
  inline void send(size_t pPort, long pInt)
  {
    mS.type = IntegerSignal;
    mS.i.value = (int)pInt;
    send(pPort, mS);
  }
  
  /** Send a double. */
  inline void send(size_t pPort, double pDouble)
  {
    mS.type = DoubleSignal;
    mS.d.value = pDouble;
    send(pPort, mS);
  }
  
  /** Send a float. */
  inline void send(size_t pPort, float pFloat)
  { 
    mS.type = DoubleSignal;
    mS.d.value = (double)pFloat;   
    send(pPort, mS);
  }
  
  /** Send a MidiMessage ptr. */
  inline void send(size_t pPort, MidiMessage * pPtr, bool pFree = false)
  {
    mS.type = MidiSignal;
    mS.midi_ptr.value = pPtr;
    mS.midi_ptr.free_me = pFree;
    send(pPort, mS);
  }
  
  /** Send a Matrix. */
  inline void send(size_t pPort, const Matrix& pMat)
  {
    mS.type = MatrixSignal;
    mS.matrix.value = &pMat;
    send(pPort, mS);
  }
  
  inline void send(size_t pPort, const MidiMessage& pMsg)
  {
    send(pPort, Signal(pMsg));
  }
  
  /** Send a midi note with the parameters provided.
    * pNote: midi note from 0 to 96. 
    * pVelocity: velocity from 0 (note off) to 127. 
    * pLength: note duration in milliseconds. 
    * pChannel: midi channel from 1 to 16.
    * pTime: time to wait before playing this note.
    * pPort: outlet id.
    */
  inline void send_note(size_t pPort, unsigned char pNote, unsigned char pVelocity = 80, unsigned int pLength = 500, unsigned int pChannel = 1, time_t pTime = 0)
  {
    MidiMessage * msg = new MidiMessage(3);
    msg->set_as_note(pNote, pVelocity, pLength, pChannel, pTime);
    mS.set(msg, true);
    send(pPort, mS);
  }
  
  /** Send a void ptr. */
  inline void send(size_t pPort, void * pPtr, bool pFree)
  {
    mS.type = VoidPointerSignal;
    mS.ptr.value = pPtr;
    mS.ptr.free_me = pFree;
    send(pPort, mS);
  }
  
  //////////////////////////////////////
  
  inline void send (size_t pPort, const Signal& sig)
  {  
    if (mDebug) *mOutput << "[" << mName << ":" << pPort << "] " << sig << std::endl;
    if (pPort < 1 || pPort > mOutlets.size() || !sig.type) return;
    mOutlets[pPort - 1]->send(sig);
  }
  
  
  /** Display the node's class, id and current state using 'spy'. */
  const char * inspect ();
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline float trigger_position() { return mTriggerPosition; }
  
  /** Return inlet at the given position. First inlet is '1', not '0'. */
  Inlet  * inlet  (size_t pInletId) 
  {   
    if (pInletId < 1 || pInletId > mInlets.size()) return NULL;
    return mInlets[pInletId - 1]; 
  }
  
  /** Return outlet at the given position. First outlet is '1', not '0'. */
  Outlet * outlet (size_t pOutletId) 
  { 
    if (pOutletId < 1 || pOutletId > mOutlets.size()) return NULL;
    return mOutlets[pOutletId - 1];
  }
  
  void set_is_ok (bool pStatus) 
  { mIsOK = pStatus; }
  
  Class * klass () 
  { return mClass; }
  
  void set_name (const char* pName) 
  { if (pName[0] != '\0') mName = pName; }
  
  void set_name (const std::string& pName) 
  { if (pName != "") mName = pName; }
  
  
  //const std::string& variable_name () { return mName; }
  const char * c_name () { return mName.c_str(); }
  
  const std::string& name () { return mName; }
  
  const char * class_name() const;
  
  /** Halt all running threads for the node. */
  virtual void stop_my_threads()
  {
    std::list<pthread_t>::iterator it  = mThreadIds.begin();
    std::list<pthread_t>::iterator end = mThreadIds.end();

    int i = 0;
    while(it != end) {
      pthread_t id = *it;
      it = mThreadIds.erase(it); // tell thread to stop
      pthread_join(id, NULL);  // wait
      i++;
    }
  }
  
  /** Set 'this' value for the current thread (used by Rubyk when starting a new thread). */
  void set_thread_this() const
  {
    pthread_setspecific(sThisKey, (void*)this);
  }
protected:
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  virtual void spy() {
    // do nothing
  }
  
  /** Display help message. */
  virtual void help() {
    // do nothing
    *mOutput << "Does nothing.\n";
  }
  
  void debug()
  {
    if (!this) {
      printf("BAD: debug\n");
    }
    mDebug = !mDebug;
    if (mDebug)
      *mOutput << mName << ": debug on\n";
    else
      *mOutput << mName << ": debug off\n";
  }
  
  /** Print message into buffer. */
  const char * bprint (char *& pBuffer, size_t& pBufferSize, const char *fmt, ...);
  
  /** Print message into string. */
  void bprint (std::string& pStr, const char *fmt, ...) const;
  
  /** Allocate/Reallocate T and print an error message if it fails. */
  template<class T>
  bool allocate (T ** pBuffer, size_t pSize, const char * pName, const char * pTypeName)
  {
    T * buf = (T*)realloc(*pBuffer, pSize * sizeof(T));        
    if (!buf) {
      *mOutput << mName << ": could not allocate " << pSize << " " << pTypeName << " for " << pName << ".\n";
      return false;
    }
    *pBuffer = buf;
    return true;
  }
  
  /** Helper to display error messages. */
  template<typename T>
  void error(TMatrix<T>& pMat, const char * pMsg, const char * pFilename, int pLine);
  
  /** Helper to display error messages. */
  void error(const char * pMsg)
  {
    *mOutput << mName << ": " << pMsg << ".\n";
  }
  
  /** Allocate/reallocate doubles. Print an error on failure. */
  bool alloc_doubles(double ** pBuffer, size_t pSize, const char * pName)
  {
    return allocate<double>(pBuffer, pSize, pName, "doubles");
  }
  
  /** Allocate/reallocate doubles. Print an error on failure. */
  bool alloc_ints(int ** pBuffer, size_t pSize, const char * pName)
  {
    return allocate<int>(pBuffer, pSize, pName, "ints");
  }
  
  // time in [ms]
  void bang_me_in (time_t pTime)
  {
    BaseEvent * e = new BaseEvent(mServer->mCurrentTime + pTime, this); // sets mIsBang to true

    mServer->register_event( e );
  }
  
  void loop_me()
  {
    mServer->register_looped_node(this);
    mLooped = true;
  }
  
  void unloop_me()
  {
    if (mLooped && mServer) {
      mServer->free_looped_node(this);
      mLooped = false;
    }
  }
  
  void remove_my_events()
  {
    if (mServer)
      mServer->free_events_for(this);
  }
  
  /** Used by sub-threads to detect if they should stop. */
  bool run_thread()
  {
    pthread_t id = pthread_self();
    std::list<pthread_t>::iterator it  = mThreadIds.begin();
    std::list<pthread_t>::iterator end = mThreadIds.end();

    while(it != end) {
      if (id == *it) return true; // if thread_id is in mThreadIds, it means it can stay alive
      it++;
    }
    return false; // current thread id is not in mThreadIds. It should stop.
  }
  
  template <class T, void(T::*Tmethod)(void *)>
  void register_event (time_t pTime, void * data)
  {
    BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
    e->mForced = false;
    mServer->register_event( e );
  }
  
  /** Register an event that must be run when the server quits. */
  template <class T, void(T::*Tmethod)(void *)>
  void register_forced_event (time_t pTime, void * data)
  {
    BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
    e->mForced = true;
    mServer->register_event( e );
  }
  
  /** Create a new thread that will run the method given as template parameter. Use NEW_THREAD(klass, method) if you prefer. */
  template <class T, void(T::*Tmethod)(void)>
  pthread_t new_thread ()
  {
    pthread_t id;
    BaseEvent * e = (BaseEvent*)new CallEvent<T, Tmethod>(mServer->mCurrentTime, (T*)this);
    if (!mServer) {
      error("you need an attached Rubyk server to create threads");
      return false;
    }
    id = mServer->create_thread(e);
    if (id) {
      // store thread id so rubyk server can later access running threads
      mThreadIds.push_back(id);
      return id;
    }
    return NULL;
  }
  
  void join_thread (pthread_t pId)
  {
    pthread_join( pId, NULL);
    mThreadIds.remove(pId);
  }
  
  static void * thread_this()
  {
    return pthread_getspecific(sThisKey);
  }
  
  
  // ================ MEMBER DATA    ================= //
  /** Host server. */
  Class * mClass;  /**< Pointer to the class of the node. Used by the node to know which methods it owns, etc (inspection). */
  Rubyk * mServer; /**< Pointer to the current server. Used to register event and get information on the running application. */
  long  mId;       /**< Unique id (in server scope). */
  bool  mLooped;   /**< True if the node is banged on every loop. */
  bool  mIsOK;     /**< If something bad arrived to the node during initialization or edit, the node goes into
                     *  broken mode and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                 
  float mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  
  
  std::vector<Inlet*>  mInlets;
  std::vector<Outlet*> mOutlets;
  
  char * mSpy;       /**< Buffer used to transmit node status through 'spy'. */
  size_t mSpySize;
  char * mInspect;   /**< Buffer used to transmit 'inspect'. 16=ptr info, 5= characters in format. */
  size_t mInspectSize;
//  char * mBuf;       /**< Buffer helper for nodes. What for ? */
//  int  mBufSize;
  
  std::string mName; /**< Global identifier ('v1', 'x', 'm43') */
  
  std::ostream * mOutput; /**< Used to pass command result. */
  
  Signal mS; /**< To send through outlets when cast needed. */
  
  std::list<pthread_t> mThreadIds; /**< Store running thread ids (used by rubyk server on quit). */
  
  bool   mDebug; /**< Subclasses can implement conditional output based on debug mode (set with 'debug' command). */
  
private:
  static unsigned int sIdCounter;  /**< Each object has a unique id. */
public:
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
};

/** This class is used when a new object's class is not found. We return an object of type 'NotFound'. */
class NotFound : public Node
{
public:
  bool set(const Params& p)
  {
    return false;
  }
  
  void bang(const Signal& sig)
  {
    // do nothing
    *mOutput << "I'm a dead node ! Don't hit me !\n";
  }
  
  void spy()
  {
    bprint(mSpy, mSpySize,"not found");
  }
};

// FIXME: these, with buffer alloc could go into an "utils" header
inline double absval(double d)
{
  if (d < 0)
    return -d;
  else
    return d;
}

#define ERROR(name) {error(name, #name)}
#define TRY(obj, meth) { if (!obj.meth) {error(obj,#obj,__FILE__,__LINE__); return false;}}
#define TRY_RET(obj, meth) { if (!obj.meth) {error(obj,#obj,__FILE__,__LINE__); return;}}
#define TRY_OR(obj, meth, action) { if (!obj.meth) {error(obj,#obj,__FILE__,__LINE__); action;}}


#define REGISTER_EVENT(klass, method, time, param) { register_event<klass,&klass::method>(time, param); }
#define NEW_THREAD(klass, method) { new_thread<klass,&klass::method>(); }
#endif