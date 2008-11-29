#include "class.h"
#include <dlfcn.h> // dylib load
#include <iostream>

// definitions of static data members
Hash<std::string, Class*> Class::sClasses(300);

// FIXME: change this for other platforms:
std::string Class::sObjectsPath("/usr/local/lib/rubyk");

void Class::set_lib_path(const char* pPath)
{ sObjectsPath = pPath; }

void Class::set_lib_path(const std::string& pPath)
{ sObjectsPath = pPath; }

std::string Class::get_lib_path()
{ return sObjectsPath; }

void Class::execute_method (const std::string& pMethodName, const Value& p, std::ostream * pOutput)
{
  class_method_t method;
  if (mClassMethods.get(&method, pMethodName)) {
    (*method)(pOutput, p);
  } else {
    *pOutput << "Unknown method '" << pMethodName << "'\n";
  }
}

bool Class::get (Class ** pClass, const std::string& pClassName)
{
  if (sClasses.get(pClass, pClassName)) {
    return true;
  } else {
    // try to load dynamic lib
    std::string path = sObjectsPath;
    path.append("/").append(pClassName).append(".rko");
    
    if (load(path.c_str(), "init")) {
      if (sClasses.get(pClass, pClassName))
        return true;
      else
        printf("Error, '%s' should declare '%s'.\n", path.c_str(), pClassName.c_str());
    }
  }
  return false;
}

Node * Class::create (Planet * pServer, const std::string& pName, const std::string& pClassName, const Value& p, std::ostream * pOutput)
{
  Class * klass;
  if (get(&klass, pClassName))
    return klass->new_obj(pName, pServer, p, pOutput);
    
  // load failed
  // dummy object in broken mode
  
  if (! sClasses.get(&klass, "NotFound"))
    Class::declare<NotFound>("NotFound");
  
  sClasses.get(&klass, "NotFound");
  Node * obj = klass->new_obj(pName, pServer, p, pOutput);
  obj->set_class(klass);
  obj->set_name(pName);
  klass->make_slots(obj);
  obj->set_is_ok( false ); // if init returns false, the node goes into 'broken' mode.
  return obj;
}

inline Node * Class::new_obj (const std::string& pName, Planet * pServer, const Value& p, std::ostream * pOutput)
{  
  return (*mCreateFunction)(this, pName, pServer, p, pOutput);
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool Class::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(void);
  const char *error = 0;
  
  // load shared extension image into memory
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if ( (error = dlis_error()) ) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // get 'init' function into the image
  function = (void(*)(void))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    printf("Symbol '%s' not found in '%s'.",init_name,file);
    if ( (error = dlis_error()) ) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // call 'init', passing the registration object
  (*function)();

  return true;
}
