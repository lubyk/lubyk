#include "node.h"
#include <list>

/** Pointer to a function to create nodes. */
typedef Node * (*create_method_t)(Class * pClass, const std::string& pName, Planet * pServer, const Hash& p, std::ostream * pOutput);

#define CLASS_ROOT CLASS_ROOT


class NewMethod : public ClassMethod
{
  /** This trigger method actually implements "new". The parameter can be either a String containing the url of the new object or
    * a Hash containing the "url" and "params": {url: "foo" params:{ bar:1 pico:2 }}. */
  virtual Value trigger(const Value& val)
  {
    String url;
    Hash params;
    Value res;
    if (val.is_hash()) {
      url    = val["url"];
      params = val["params"];
    } else {
      url = val;
    }
    
    if (url.is_nil()) return Error("Invalid parameters (missing 'url').");
    
    res = (*mMethod)(url);
    
    if (!res.is_string()) return res;
    url = res;
    
    Node * obj = (Node*)Object::find(url.string()); // FIXME: test type (= virtual for type signature) of returned Object before casting to Node * !!!
    if (!obj) {
      return Error("New Object at '").append(url.string()).append("' not found !");
    }
    
    obj->set_class_url(mParent->url());  // used by osc (using url instead of name because we might have class folders/subfolders some day).
    
    //FIX obj->set_server(pServer);  // ??? where do we find this one ?
    //FIX obj->set_output(pOutput);  // ??? I think we won't need this anymore once we have implemented notifications ?
    
    // make inlets
    res = mParent->make_inlets(obj);
    if (res.is_error()) return res;
    
    // make outlets
    res = mParent->make_outlets(obj);
    if (res.is_error()) return res;
    
    // make accessors
    res = mParent->make_accessors(obj);
    if (res.is_error()) return res;
    
    // initialize
    obj->set_is_ok( obj->init() && obj->set(params) ); // if init or set returns false, the node goes into 'broken' mode.
    return url;
  }
};


/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public Object
{
public:
  virtual ~Class()
  {
    std::list<InletPrototype*>::iterator it;
    std::list<InletPrototype*>::iterator end = mInletPrototypes.end();
    for (it = mInletPrototypes.begin(); it != end; it++) {
      delete *it;
    }
  }
  
  /** Declare an inlet, with an accessor method. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  void add_inlet (const char * pName, value_t pAcceptTypes, const char * pInfo)
  { 
    mInletPrototypes.push_back( new InletPrototype(pName, pAcceptTypes, pInfo, &cast_inlet_method<T, Tmethod>) );
  }
  
  
  /** Get a Class object from it's std::string name ("Metro"). */
  static bool get_class (Class ** pResult, const std::string& pName)
  {
    return Object::get(pResult, std::string(CLASS_ROOT).append(pName));
  }
  
  /** Get a Class object from it's name ("Metro"). */
  static bool get_class (Class ** pResult, const char* pName)
  {
    return Object::get(pResult, std::string(CLASS_ROOT).append(pName));
  }
  
  /** Declare a new class. This template is responsible for generating the "new" method. */
  template<class T>
  static Class * declare(const char* name)
  {
    Class * klass;
    if (get_class(&klass, name))
      delete klass; // remove existing class with same name.
    
    Object * class_folder;
    if (!Object::get(&class_folder, CLASS_ROOT)) {
      // create default class folder
      class_folder = new Object(CLASS_ROOT);
      if (!class_folder) return;
    }
    
    klass = new Class(class_folder, name);
    
    // build "new" method for this class
    ClassMethod * new_method = new ClassMethod(this, "new", &cast_create<T>);
    
    return klass;
  }
  
  /** Template used as a method to create new nodes. */
  template<class T>
  static const Value cast_create(const Value& val)
  {
    Object * parent;
    String url(val);
    std::string name;
    if (url.is_nil()) return Error("Invalid 'url' parameter.");
    
    // get parent ["/met", "met"] => NULL, "/venus/grp/met" => "/venus/grp"
    std::string str = url.string();
    if (str.at(0) == '/') {
      size_t pos = url.rfind("/");
      parent = Object::find( url.substr(0, pos) )
      if (!parent) return Error("Invalid parent '").append(url.substr(0,pos)).append("'.");
      name = url.substr(pos + 1, url.length);
    } else {
      parent = NULL;
      name = str;
    }
    
    T * obj = new T(parent, name);
    return String(obj->url());
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  static void cast_inlet_method (void * receiver, const Signal& sig)
  {
    (((T*)receiver)->*Tmethod)(sig);
  }
  
private:
  
  const Value make_inlets(Node * pObj)
  {
    std::list<InletPrototype*>::iterator it;
    std::list<InletPrototype*>::iterator end = mInletPrototypes.end();
    Object * inlets = pObj->new_child<Object>("inlets");
    if (!inlets) return Error("Could not create 'inlets' folder for'").append(pObj->url).append("'.");
    for (it = mInletPrototypes.begin(); it != end; it++) {
      Inlet * inl = new Inlet (Object * pParent, void * pNode, const InletPrototype& pProto)
    }
  }
  
  std::list<InletPrototype*> mInletPrototypes;  /**< Prototypes to create inlets. */
};