#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include "method.h"
#include <list>

/** Pointer to a function to create nodes. */
typedef Node * (*create_method_t)(Class * pClass, const std::string& pName, Planet * pServer, const Hash& p, std::ostream * pOutput);

#define CLASS_ROOT "/classes"
#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

class NewMethod : public ClassMethod
{
public:
  NewMethod(const std::string &pName, class_method_t pMethod) : ClassMethod(pName, pMethod) {}
  NewMethod(const char * pName, class_method_t pMethod) : ClassMethod(pName, pMethod) {}
  virtual ~NewMethod() {}
  
  /** This trigger method actually implements "new". The parameter can be either a String containing the url of the new object or
    * a Hash containing the "url" and "params": {url: "foo" params:{ bar:1 pico:2 }}. */
  virtual const Value trigger (const Value& val);
};

/** Special class to handle class listing from a directory. This usually responds at to the '/classes' url. */
class ClassListing : public Object
{
public:
  ClassListing(const std::string &pName) : Object(pName), mObjectsPath(DEFAULT_OBJECTS_LIB_PATH) {
    std::cout << "ClassListing: " << url() << std::endl;
  }
  ClassListing(const char * pName) : Object(pName), mObjectsPath(DEFAULT_OBJECTS_LIB_PATH) {}
  virtual ~ClassListing() {}
  
  /** This trigger implements "/classes". It returns the list of objects in mObjectsPath. */
  virtual const Value trigger (const Value& val);
  
  virtual const Value not_found (const std::string& pUrl, const Value& val);
  
  static ClassListing sClasses;
  
  static void set_lib_path(const char* pPath)
  { sClasses.mObjectsPath = pPath; }
 
  static void set_lib_path(const std::string& pPath)
  { sClasses.mObjectsPath = pPath; }
 
  static std::string get_lib_path()
  { return sClasses.mObjectsPath; }
   
private:
  
  /** Load an object stored in a dynamic library. */
  static bool load(const char * file, const char * init_name);
  
  std::string mObjectsPath; /**< Where to find objects in the filesystem. */
};

/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public Object
{
public:
  Class(const char* pName, const char* pInfo) : Object(pName) 
  {
    set_info(pInfo);
  }
  
  virtual ~Class()
  {
    std::list<InletPrototype*>::iterator it;
    std::list<InletPrototype*>::iterator end = mInletPrototypes.end();
    for (it = mInletPrototypes.begin(); it != end; it++) {
      delete *it;
    }
  }
  
  /** Add a new class method to the class. */
  void add_class_method(const char * pName, class_method_t pMethod, const char * pInfo)
  {
    ClassMethod * m = adopt(new ClassMethod(pName, pMethod));
    m->set_info(pInfo);
  }
  
  
  /** Declare an inlet, with an accessor method. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  void add_inlet (const char * pName, value_t pAcceptTypes, const char * pInfo)
  { 
    mInletPrototypes.push_back( new InletPrototype(pName, pAcceptTypes, pInfo, &cast_inlet_method<T, Tmethod>) );
  }
  
  /** Get a Class object from it's name ("Metro"). */
  static Class * find (const char* pName)
  {
    return (Class*)ClassListing::sClasses.child(pName); // FIXME: type checking !!
  }
  
  /** Get a Class object from it's std::string name ("Metro"). */
  static Class * find (const std::string& pName)
  {
    return (Class*)ClassListing::sClasses.child(pName);
  }
  
  /** Declare a new class. This template is responsible for generating the "new" method. */
  template<class T>
  static void declare(const char* pName, const char* pInfo)
  {
    Class * klass;
    if (Class::find(pName))
      delete klass; // remove existing class with same name.
    
    klass = ClassListing::sClasses.adopt(new Class(pName, pInfo));
    
    if (!klass) return;
    
    // build "new" method for this class
    klass->adopt(new NewMethod("new", &cast_create<T>));
  }
  
  /** Template used as a method to create new nodes. */
  template<class T>
  static const Value cast_create(const Value& val)
  {
    Object * parent;
    String url(val);
    std::string name;
    if (url.is_nil()) return Error("Invalid 'url' parameter.");
    
    // get parent ["/met", "met"] => "", "/grp/met" => "/grp"
    std::string str = url.string();
    if (str.at(0) == '/') {
      size_t pos = url.rfind("/");
      parent = Object::find( url.substr(0, pos) );
      if (!parent) return Error("Invalid parent '").append(url.substr(0,pos)).append("'.");
      name = url.substr(pos + 1, url.length());
    } else {
      parent = NULL;
      name = str;
    }
    
    T * obj;
    
    if (parent)
      obj = parent->adopt(new T());
    else
      obj = new T();
      
    obj->set_name(name);
    
    return String(obj->url());
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  static void cast_inlet_method (void * receiver, const Value& val)
  {
    (((T*)receiver)->*Tmethod)(val);
  }
  
  
private:
  friend class NewMethod;
  
  static std::string sObjectsPath; /**< Where to load the librairies (objects). */
  
  const Value make_inlets(Node * pObj)
  {
    std::list<InletPrototype*>::iterator it;
    std::list<InletPrototype*>::iterator end = mInletPrototypes.end();
    Object * inlets = pObj->adopt(new Object("inlets"));
    if (!inlets) return Error("Could not create 'inlets' folder for'").append(pObj->url()).append("'.");
    
    for (it = mInletPrototypes.begin(); it != end; it++) {
      inlets->adopt(new Inlet(pObj, **it));
    }
    
    return gNilValue;
  }
  
  std::list<InletPrototype*> mInletPrototypes;  /**< Prototypes to create inlets. */
};


// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info)         {Class::declare<klass>(#klass, info);}
#define CLASS_METHOD(klass,method, info) {Class::find(#klass)->add_class_method(#method, &klass::method, info);}
#endif // _CLASS_H_