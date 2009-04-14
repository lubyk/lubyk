#ifndef _CLASS_FINDER_H_
#define _CLASS_FINDER_H_
#include "object.h"
#include "new_method.h"
#include "class.h"

/** Special class to handle class listing from a directory. This usually responds at to the '/class' url. */
class ClassFinder : public Object
{
public:
  ClassFinder(const std::string &name, const char *objects_path) : Object(name), objects_path_(objects_path) {
    init();
  }
  
  ClassFinder(const std::string &name, std::string &objects_path) : Object(name), objects_path_(objects_path) {
    init();
  }
  
  ClassFinder(const char *name, const char *objects_path) : Object(name), objects_path_(objects_path) {
    init();
  }
  
  ClassFinder(const char *name, std::string &objects_path) : Object(name), objects_path_(objects_path) {
    init();
  }
  
  void init() {
    adopt(new Method(this, "lib_path", &Method::cast_method<ClassFinder, &ClassFinder::lib_path>, H("s"), "Get/set path to load objects files (*.rko)."));
  }
  
  virtual ~ClassFinder() {}
  
  /** Class signature. */
  virtual uint class_type() {
    return H("ClassFinder");
  }
  
  /** This trigger implements "/class". It returns the list of objects in objects_path_. */
  virtual const Value trigger (const Value &val);
  
  virtual BaseObject *build_child(const std::string &class_name, Value *error);
  
  const Value lib_path(const Value &val) { 
    if (val.is_string()) objects_path_ = val.str();
    return Value(objects_path_);
  }
 
  void set_lib_path(const std::string &path) { objects_path_ = path; }
  
  /** Declare a new class. This template is responsible for generating the "new" method. */
  template<class T>
  Class * declare(const char *name, const char *info, const char *new_info)
  {
    Class * klass;
    
    if (find_class(name))
      delete klass; // remove existing class with same name.
    
    klass = adopt(new Class(name, info));
    
    if (!klass) return NULL; // FIXME: this will crash !!!
    
    // build "new" method for this class
    klass->adopt(new NewMethod("new", &NewMethod::cast_create<T>, new_info));
    
    return klass;
  }
  
  /** Get a Class object from it's name ("Metro"). */
  Class * find_class (const char *name) {
    return TYPE_CAST(Class, child(name));
  }
  
  /** Get a Class object from it's std::string name ("Metro"). */
  Class * find_class (const std::string &name) {
    return TYPE_CAST(Class, child(name));
  }
  
private:
  
  /** Load an object stored in a dynamic library. */
  bool load(const char * file, const char * init_name);
  
  std::string objects_path_; /**< Where to find objects in the filesystem. */
};

#endif // _CLASS_FINDER_H_