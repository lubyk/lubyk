#ifndef _NODE_H_
#define _NODE_H_

#include "Inlet.h"
#include "Outlet.h"
#include "Params.h"
#include "Hash.h"

#define MAX_SPY_MESSAGE      200

/** Pointer to a function to create nodes. */
typedef Node * (*class_creator_function_t)(const std::string& key, const std::string& pParams);

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */  
class Node
{
public:
  Node() : mTriggerPosition(0), mId(0) { mClassName[0] = '\0'; }
  virtual ~Node() {}
  
  bool init() {}
  
  /** Compute new values for each outlet and send values through connections. */
  void bang (void)
  {
    int i = mOutlets.size() - 1;
    // do not try to compute outlets if the node is in broken mode
    if (!mIsOK) return;
    
    while(i >= 0) {
      /** Send through rightmost outlet first. */
      mOutlets[i--].compute_and_send();
    }
  }
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  virtual const char * spy() {
    return '\0';
  }
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline float trigger_position() { return mTriggerPosition; }
  
  Inlet  * inlet  (int slot_id) { return &( mInlets[slot_id]); }
  Outlet * outlet (int slot_id) { return &(mOutlets[slot_id]); }
  
  void set_is_ok (bool pStatus) 
  { mIsOK = pStatus; }
  
  void set_class_name (const std::string& pName) 
  { mClassName = pName; }
  
  const char * class_name() { return mClassName.c_str(); }
  
  ////// class methods ///////
  
  /** Load an object stored in a dynamic library. */
  static bool load(const char * file, const char * init_name);
  
  static Node * create (const char * pKey, const std::string& pParams)
  { return Node::create(std::string(pKey), pParams); }
  
  static Node * create (const char * pKey, const char * pParams)
  { return Node::create(std::string(pKey), std::string(pParams)); }
  
  static Node * create (std::string& pKey, const char * pParams)
  { return Node::create(pKey, std::string(pParams)); }
  
  static Node * create (const std::string& pKey, const std::string& pParams)
  {
    class_creator_function_t * func = mClasses.get(pKey);
    if (func)
      return (**func)(pKey, pParams);
    else {
      // try to load dynamic lib
      std::string path = mObjectsPath;
      path.append("/").append(pKey).append(".rko");
      //printf("Loading '%s'\n", path.c_str());
      if (load(path.c_str(), "init")) {
        func = mClasses.get(pKey);
        if (func) {
          return (**func)(pKey, pParams);
        } else {
          printf("Error, '%s' should declare '%s'.\n", path.c_str(), pKey.c_str());
        }
      } 
      // load failed
      // dummy object in broken mode
      
      Node * obj = new Node;
      obj->set_class_name(pKey);
      obj->set_is_ok( false ); // if init returns false, the node goes into 'broken' mode.
      return obj;
    }
      
  }
  
  template<class T>
  static void declare(const char* name)
  {
    mClasses.set(std::string(name), &make_class<T>);
  }
  
  static void set_lib_path(const char* pPath)
  { mObjectsPath = pPath; }
  
  static void set_lib_path(const std::string& pPath)
  { mObjectsPath = pPath; }
  
protected:
  
  // ================ INLET / OUTLET ================= //
  /** Create a new inlet with the method as callback to set incoming value. */
  template <class T, void(T::*Tmethod)(float)>
  void make_inlet ()
  {
    Inlet s(this,&cast_inlet_method<T, Tmethod>);
    s.setId(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(s);
  }
  
  /** Create a new outlet with the method as callback to set compute the new value. */
  template <class T, float(T::*Tmethod)(void)>
  void make_outlet ()
  {
    Outlet s(this,&cast_outlet_method<T, Tmethod>);
    s.setId(mOutlets.size()); /* first outlet has id 0 */
    mOutlets.push_back(s);
  }
  
  
  // ================ MEMBER DATA    ================= //
  
  long  mId;
  bool  mIsOK; /**< If something bad arrived to the node during initialization or edit, the node goes into
                 *  broken mode and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                 
  float mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  
  std::vector<Inlet>  mInlets;
  std::vector<Outlet> mOutlets;
  
  char mSpy[MAX_SPY_MESSAGE + 1];       /**< Buffer used to transmit node status through 'spy'. */
  std::string mClassName; /**< Name of the current node's class (metro, counter, etc) */
  
private:
  
  // ================ REGISTER CLASSES =============== //
  
  static Hash<std::string, class_creator_function_t> mClasses; /**< Contains a dictionary of class names and functions to create objects of this class. For example, 'metro' => function to create a Metro. */
  
  static std::string mObjectsPath; /**< Where to load the librairies (objects). */
  
  /** This function is used to create an instance of class 'T'. If the instance could not be
    * properly initialized, this function returns NULL. */
  template<class T>
  static Node * make_class(const std::string& pClassName, const std::string& pParams)
  {
    T * obj = new T;
    Params p(pParams);
    obj->set_class_name(pClassName);
    obj->set_is_ok( obj->init(p) ); // if init returns false, the node goes into 'broken' mode.
    return (Node*)obj;
  }

  // ================ INLET / OUTLET ================= //
  /** Return a function pointer from a pointer to method. */
  template <class T, void(T::*Tmethod)(float)>
  static void cast_inlet_method (void * receiver, float value)
  {
    (((T*)receiver)->*Tmethod)(value);
  }
  
  /** Return a function pointer from a pointer to method. */
  template <class T, float(T::*Tmethod)(void)>
  static float cast_outlet_method (void * receiver)
  {
    return (((T*)receiver)->*Tmethod)();
  }
};

#endif