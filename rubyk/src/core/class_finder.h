#ifndef _CLASS_FINDER_H_
#define _CLASS_FINDER_H_
#include "object.h"
#include "new_method.h"
#include "class.h"

/** Special class to handle class listing from a directory. This usually responds at to the '/class' url. */
class ClassFinder : public Object
{
public:
  ClassFinder(const std::string &pName, const char * pObjPath) : Object(pName), mObjectsPath(pObjPath) {}
  ClassFinder(const std::string &pName, std::string& pObjPath) : Object(pName), mObjectsPath(pObjPath) {}
  ClassFinder(const char * pName,       const char * pObjPath) : Object(pName), mObjectsPath(pObjPath) {}
  ClassFinder(const char * pName,       std::string& pObjPath) : Object(pName), mObjectsPath(pObjPath) {}
  
  virtual ~ClassFinder() {}
  
  /** Class signature. */
  virtual uint type()
  {
    return H("ClassFinder");
  }
  
  /** This trigger implements "/class". It returns the list of objects in mObjectsPath. */
  virtual const Value trigger (const Value& val);
  
  virtual const Value not_found (const std::string& pUrl, const Value& val);
  
  void set_lib_path(const char* pPath)
  { mObjectsPath = pPath; }
 
  void set_lib_path(const std::string& pPath)
  { mObjectsPath = pPath; }
 
  std::string get_lib_path()
  { return mObjectsPath; }
  
  /** Declare a new class. This template is responsible for generating the "new" method. */
  template<class T>
  Class * declare(const char* pName, const char* pInfo)
  {
    Class * klass;
    
    if (find_class(pName))
      delete klass; // remove existing class with same name.
    
    klass = adopt(new Class(pName, pInfo));
    
    if (!klass) return NULL; // FIXME: this will crash !!!
    
    // build "new" method for this class
    klass->adopt(new NewMethod("new", &NewMethod::cast_create<T>));
    
    return klass;
  }
  
  /** Get a Class object from it's name ("Metro"). */
  Class * find_class (const char* pName)
  {
    return TYPE_CAST(Class, child(pName));
  }
  
  /** Get a Class object from it's std::string name ("Metro"). */
  Class * find_class (const std::string& pName)
  {
    return TYPE_CAST(Class, child(pName));
  }
  
private:
  
  /** Load an object stored in a dynamic library. */
  bool load(const char * file, const char * init_name);
  
  std::string mObjectsPath; /**< Where to find objects in the filesystem. */
};

#endif // _CLASS_FINDER_H_