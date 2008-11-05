#ifndef _METHOD_H_
#define _METHOD_H_

/** Pointer to an inlet method that can be called from the command line with "obj.method(Params)" */
typedef void (*inlet_method_t)(void * pReceiver, const Signal& sig);

/** Pointer to an inlet method that can be called from the command line with "obj.method(Params)" */
typedef void (*outlet_method_t)(void * pReceiver, Signal& sig);

class Method : public Object
{
public:
  virtual operator() (const std::string& pName, Planet * pServer, const Params& p, std::ostream * pOutput)
  {  
    return (*mCreateFunction)(this, pName, pServer, p, pOutput);
  }
  
private:
  void *   mObject;   /**< Object containing the method. */
  method_t mMethod;   /**< Pointer on a static cast of the object's method. */ 
};

#endif