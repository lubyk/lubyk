#ifndef _NODE_H_
#define _NODE_H_

#include "inlet.h"
#include "outlet.h"
#include "params.h"
#include "hash.h"
#include "buf.h"
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
  Node() : mClass(NULL), mServer(NULL), mId(0), mSpySize(0), mInspectSize(0), mBuf(NULL), mSpy(NULL), mInspect(NULL), mOutput(&std::cout), mLooped(false), mDebug(false)
  { 
    char buf[50];
    sIdCounter++;
    mTriggerPosition = sIdCounter; // last created is further on the right
    sprintf(buf,"_%i",sIdCounter);
    mName = std::string(buf);  // default variable name is 'id'
  }
  
  virtual ~Node();
  
  bool init( const Params& p) { return true; }
  
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
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  const char* get_spy() {
    spy();
    if (mSpy)
      return mSpy;
    else
      return "--";
  }
  
  ///////// send signals ///////////////
  
  
  /** Send a bang. */
  inline void send(rubyk_signal_t pType, int pPort = 1) 
  { 
    if (!pType) return; // do nothing if NilSignal
    mS.type = pType;
    send(mS, pPort);
  }
  
  /** Send an integer. */
  inline void send(int pInt, int pPort = 1)
  {
    mS.type = IntegerSignal;
    mS.i.value = pInt;
    send(mS, pPort);
  }
  
  /** Send a char. */
  inline void send(char pChar, int pPort = 1)
  {
    mS.type = CharSignal;
    mS.c.value = pChar;
    send(mS, pPort);
  }
  
  /** Send an unsigned integer. */
  inline void send(unsigned int pInt, int pPort = 1)
  {
    mS.type = IntegerSignal;
    mS.i.value = pInt;
    send(mS, pPort);
  }
  
  /** Send an unsigned long. */
  inline void send(long pInt, int pPort = 1)
  {
    mS.type = IntegerSignal;
    mS.i.value = (int)pInt;
    send(mS, pPort);
  }
  
  /** Send a double. */
  inline void send(double pDouble, int pPort = 1)
  {
    mS.type = DoubleSignal;
    mS.d.value = pDouble;
    send(mS, pPort);
  }
  
  /** Send a float. */
  inline void send(float pFloat, int pPort = 1)
  { 
    mS.type = DoubleSignal;
    mS.d.value = (double)pFloat;   
    send(mS, pPort);
  }
  
  /** Send a MidiMessage ptr. */
  inline void send(MidiMessage * pPtr, bool pFree = false, int pPort = 1)
  {
    mS.type = MidiSignal;
    mS.midi_ptr.value = pPtr;
    mS.midi_ptr.free_me = pFree;
    send(mS, pPort);
  }
  
  inline void send(const MidiMessage& pMsg, int pPort = 1)
  {
    MidiMessage * msg = new MidiMessage(pMsg);
    mS.type = MidiSignal;
    mS.midi_ptr.value = msg;
    mS.midi_ptr.free_me = true;
    send(mS, pPort);
  }
  
  /** Send a midi note with the parameters provided.
    * pNote: midi note from 0 to 96. 
    * pVelocity: velocity from 0 (note off) to 127. 
    * pLength: note duration in milliseconds. 
    * pChannel: midi channel from 1 to 16.
    * pTime: time to wait before playing this note.
    * pPort: outlet id.
    */
  inline void send_note(unsigned char pNote, unsigned char pVelocity = 80, unsigned int pLength = 500, unsigned int pChannel = 1, time_t pTime = 0, int pPort = 1)
  {
    MidiMessage * msg = new MidiMessage(3);
    msg->set_as_note(pNote, pVelocity, pLength, pChannel, pTime);
    mS.type = MidiSignal;
    mS.midi_ptr.value = msg;
    mS.midi_ptr.free_me = true;
    send(mS, pPort);
  }
  
  /** Send a void ptr. */
  inline void send(void * pPtr, bool pFree, int pPort = 1)
  {
    mS.type = VoidPointerSignal;
    mS.ptr.value = pPtr;
    mS.ptr.free_me = pFree;
    send(mS, pPort);
  }
  
  //////////////////////////////////////
  
  inline void send (const Signal& sig, int pOutletId = 1)
  {
    if (pOutletId < 1 || pOutletId > mOutlets.size() || !sig.type) return;
    mOutlets[pOutletId - 1]->send(sig);
  }
  
  
  /** Display the node's class, id and current state using 'spy'. */
  const char * inspect ();
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline float trigger_position() { return mTriggerPosition; }
  
  /** Return inlet at the given position. First inlet is '1', not '0'. */
  Inlet  * inlet  (int pInletId) 
  {   
    if (pInletId < 1 || pInletId > mInlets.size()) return NULL;
    return mInlets[pInletId - 1]; 
  }
  
  /** Return outlet at the given position. First outlet is '1', not '0'. */
  Outlet * outlet (int pOutletId) 
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
  const char * name () { return mName.c_str(); }
  
  const char * class_name() const;
  
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
    mDebug = !mDebug;
    if (mDebug)
      *mOutput << mName << ": debug on\n";
    else
      *mOutput << mName << ": debug off\n";
  }
  
  /** Print message into buffer. */
  void bprint (char *& pBuffer, int& pBufferSize, const char *fmt, ...);
  
  // FIXME: replace all uses of alloc_doubles with Buf.
  /** Allocate doubles and print an error message if it fails. */
  bool alloc_doubles (double ** pBuffer, int pSize, const char * pName);
  
  /** Reallocate doubles and print an error message if it fails. */
  bool realloc_doubles  (double ** pBuffer, int pSize, const char * pName);
  
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
  
  // ================ MEMBER DATA    ================= //
  
  long  mId;
  bool  mLooped; /**< True if the node is banged on every loop. */
  bool  mIsOK; /**< If something bad arrived to the node during initialization or edit, the node goes into
                 *  broken mode and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                 
  float mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  /** Host server. */
  Rubyk * mServer;
  Class * mClass;
  
  std::vector<Inlet*>  mInlets;
  std::vector<Outlet*> mOutlets;
  
  char * mSpy;       /**< Buffer used to transmit node status through 'spy'. */
  int  mSpySize;
  char * mInspect; /**< Buffer used to transmit 'inspect'. 16=ptr info, 5= characters in format. */
  int  mInspectSize;
  char * mBuf; /**< Buffer helper for nodes. */
  int  mBufSize;
  
  std::string mName; /**< Global identifier ('v1', 'x', 'm43') */
  
  std::ostream * mOutput; /**< Used to pass command result. */
  
  Signal mS; /**< To send through outlets when cast needed. */
  
  
  bool   mDebug; /**< Subclasses can implement conditional output based on debug mode (set with 'debug' command). */
  
private:
  static unsigned int sIdCounter;  /**< Each object has a unique id. */
};

/** This class is used when a new object's class is not found. We return an object of type 'NotFound'. */
class NotFound : public Node
{
public:
  
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

#endif