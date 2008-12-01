#ifndef _NODE_H_
#define _NODE_H_
#include "object.h"
#include "event.h"
#include "inlet.h"
#include "outlet.h"
#include "thash.h"
#include "values.h"

#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <list>
#include <pthread.h>

class Planet;


#define START_PRINT_BUFFER   20
#define MAX_CLASS_NAME       50
#define START_INSPECT_BUFFER (START_SPY_BUFFER + MAX_CLASS_NAME + 5 + 16)

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */  
class Node : public Object
{
public:
  Node() : mServer(NULL), mId(0), mLooped(false), mIsOK(true), mSpy(NULL), mSpySize(0), mInspect(NULL), mInspectSize(0), mOutput(&std::cout), mDebug(false)
  { 
    mTriggerPosition = sIdCounter; // last created is further on the right
  }
  
  virtual ~Node();
  
  /** This method must be implemented in subclasses. It is used to do a basic setup with default parameters before these
    * are changed during runtime. */
  virtual bool init()
  { return true; }
  
  /** Shortcut to call multiple methods on an object.
    * Using "obj.set(foo:4 bar:5)" is equivalent to calling "obj.foo(4)" and "obj.bar(5)". */
  bool set(const Hash& pParams)
  {
    Hash p(pParams);
    Object * obj;
    Hash_iterator it;
    Hash_iterator end = p.end();
    
    for(it = p.begin(); it != end; it++) {
      if ( (obj = child(*it)) ) {
        obj->trigger(p[*it]);
      }
    }
    return true;
  }
  
  /** Add an inlet with the given callback (used by Class during instantiation). */
  void add_inlet(obj_method_t pCallback)
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
  
  void set_server(Planet * pServer)
  { mServer = pServer; }
  
  void set_class_url(const std::string& pClass)
  { mClassUrl = pClass; }
  
  void set_output(std::ostream * pOutput)
  { mOutput = pOutput; }
  
  std::ostream& output()
  { return *mOutput; }
  
  void execute_method (const std::string& pMethod, const Value& p);
  
  /** This method must be implemented in subclasses. It's the place where most
    * of the work should be done. This method is responsible for sending the signals out. */
  virtual void bang (const Value& val) = 0;
    
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
  
  /** Send an integer. */
  inline void send(size_t pPort, int pInt)
  {
    send(pPort, Number((real_t)pInt));
  }
  
  /** Send a char. */
  inline void send(size_t pPort, char pChar)
  {
    send(pPort, Number((real_t)pChar));
  }
  
  /** Send an unsigned integer. */
  inline void send(size_t pPort, unsigned int pInt)
  {
      send(pPort, Number((real_t)pInt));
  }
  
  /** Send an unsigned long. */
  inline void send(size_t pPort, long pInt)
  {
      send(pPort, Number((real_t)pInt));
  }
  
  /** Send a real_t. */
  inline void send(size_t pPort, real_t pReal)
  {
    send(pPort, Number(pReal));
  }
  
  /** Send a float. */
  inline void send(size_t pPort, float pFloat)
  { 
    send(pPort, Number((real_t)pFloat));
  }
  
//FIX  /** Send a MidiMessage ptr. */
//FIX  inline void send(size_t pPort, MidiMessage * pPtr, bool pFree = false)
//FIX  {
//FIX    mS.type = MidiValue;
//FIX    mS.midi_ptr.value = pPtr;
//FIX    mS.midi_ptr.free_me = pFree;
//FIX    send(pPort, mS);
//FIX  }
//FIX  
//FIX  /** Send a Matrix. */
//FIX  inline void send(size_t pPort, const Matrix& pMat)
//FIX  {
//FIX    mS.type = MatrixValue;
//FIX    mS.matrix.value = &pMat;
//FIX    send(pPort, mS);
//FIX  }
//FIX  
//FIX  inline void send(size_t pPort, const MidiMessage& pMsg)
//FIX  {
//FIX    send(pPort, Value(pMsg));
//FIX  }
//FIX  
//FIX  /** Send a midi note with the parameters provided.
//FIX    * pPort: outlet id.
//FIX    * pNote: midi note from 0 to 96. 
//FIX    * pVelocity: velocity from 0 (note off) to 127. 
//FIX    * pLength: note duration in milliseconds. 
//FIX    * pChannel: midi channel from 1 to 16.
//FIX    * pTime: time to wait before playing this note.
//FIX    */
//FIX  inline void send_note(size_t pPort, unsigned char pNote, unsigned char pVelocity = 80, unsigned int pLength = 500, unsigned int pChannel = 1, time_t pTime = 0)
//FIX  {
//FIX    MidiMessage * msg = new MidiMessage(3);
//FIX    msg->set_as_note(pNote, pVelocity, pLength, pChannel, pTime);
//FIX    mS.set(msg, true);
//FIX    send(pPort, mS);
//FIX  }
//FIX  
//FIX  /** Send a midi control change with the parameters provided.
//FIX    * pPort: outlet id.
//FIX    * pCtrl: control number from 0 to 127. (see http://tomscarff.tripod.com/midi_analyser/control_and_mode_changes.htm)
//FIX    * pCtrlValue: velocity from 0 (note off) to 127.
//FIX    * pChannel: midi channel from 1 to 16.
//FIX    * pTime: time to wait before playing this note.
//FIX    */
//FIX  inline void send_ctrl(size_t pPort, unsigned char pCtrl, unsigned char pCtrlValue, unsigned int pChannel = 1, time_t pTime = 0)
//FIX  {
//FIX    MidiMessage * msg = new MidiMessage(3);
//FIX    msg->set_as_ctrl(pCtrl, pCtrlValue, pChannel, pTime);
//FIX    mS.set(msg, true);
//FIX    send(pPort, mS);
//FIX  }
//FIX  
//FIX  /** Send a void ptr. */
//FIX  inline void send(size_t pPort, void * pPtr, bool pFree)
//FIX  {
//FIX    mS.type = VoidPointerValue;
//FIX    mS.ptr.value = pPtr;
//FIX    mS.ptr.free_me = pFree;
//FIX    send(pPort, mS);
//FIX  }
  
  //////////////////////////////////////
  
  inline void send (size_t pPort, const Value& val)
  {  
    if (mDebug) *mOutput << "[" << mName << ":" << pPort << "] " << val << std::endl;
    if (pPort < 1 || pPort > mOutlets.size() || val.is_nil()) return;
    mOutlets[pPort - 1]->send(val);
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
  
  const String klass () const
  { return mClassUrl; }
  
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
  
  /** Set 'this' value for the current thread (used by Planet when starting a new thread). */
  void set_thread_this() const
  {
    pthread_setspecific(sThisKey, (void*)this);
  }
protected:
  
  /** Used to display some information on the current node. Should be overwridden by subclasses. */
  virtual void spy() = 0;
    
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
  void error(T& pMat, const char * pMsg, const char * pFilename, int pLine);
  
  /** Helper to display error messages. */
  void error(const char * pMsg)
  {
    *mOutput << mName << ": " << pMsg << ".\n";
  }
  
  /** Allocate/reallocate reals. Print an error on failure. */
  bool alloc_reals(real_t ** pBuffer, size_t pSize, const char * pName)
  {
    return allocate<real_t>(pBuffer, pSize, pName, "reals");
  }
  
  /** Allocate/reallocate reals. Print an error on failure. */
  bool alloc_ints(int ** pBuffer, size_t pSize, const char * pName)
  {
    return allocate<int>(pBuffer, pSize, pName, "ints");
  }
  
  /** Send be a bang in pTime in [ms]. */
  inline void bang_me_in (time_t pTime);
  
  /** Registers this object as needing independant runtime (Serial input, Midi input, etc).
   * It will receive a gNilValue in the 'bang' method on every tick.
  */
  inline void loop_me ();
  
  inline void unloop_me ();
  
  inline void remove_my_events ();
  
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
  inline void register_event (time_t pTime, void * data);
  
  /** Register an event that must be run when the server quits. */
  template <class T, void(T::*Tmethod)(void *)>
  inline void register_forced_event (time_t pTime, void * data);
  
  /** Create a new thread that will run the method given as template parameter. Use NEW_THREAD(klass, method) if you prefer. */
  template <class T, void(T::*Tmethod)(void)>
  pthread_t new_thread ();
  
  void join_thread (pthread_t pId)
  {
    pthread_join( pId, NULL);
    mThreadIds.remove(pId);
  }
  
  static void * thread_this()
  {
    return pthread_getspecific(sThisKey);
  }
  
  inline bool is_opengl_thread ()
  {
    return pthread_getspecific(sGLThreadKey) != NULL;
  }
  
  static void set_is_opengl_thread ()
  {
    pthread_setspecific(sGLThreadKey, (void*)true);
  }
  
  // ================ MEMBER DATA    ================= //
  /** Host server. */
  String mClassUrl; /**< String representation of the url to the node's class. Only used by osc. Inspection is done through the Object tree. */
  Planet * mServer; /**< Pointer to the current server. Used to register event and get information on the running application. */
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
  
  std::ostream * mOutput; /**< Used to pass command result. */
  
  Value mS; /**< To send through outlets when cast needed. */
  
  std::list<pthread_t> mThreadIds;    /**< Store running thread ids (used by rubyk server on quit). */
  
  bool   mDebug; /**< Subclasses can implement conditional output based on debug mode (set with 'debug' command). */
  
public:
  static pthread_key_t sThisKey;      /**< Key to retrieve 'this' value from a running thread. */
  static pthread_key_t sGLThreadKey;  /**< Key to test if the running thread is an OpenGL thread. */  
};

/** This class is used when a new object's class is not found. We return an object of type 'NotFound'. */
class NotFound : public Node
{
public:
  virtual bool set(const Hash& p)
  {
    return false;
  }
  
  void bang(const Value& val)
  {
    // do nothing
    *mOutput << "I'm a dead node ! Don't hit me !\n";
  }
  
  void spy()
  {
    bprint(mSpy, mSpySize,"not found");
  }
};

////////// FIXME: these, with buffer alloc could go into an "utils" header
inline real_t absval(real_t d)
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