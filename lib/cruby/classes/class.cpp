#include "class.h"
#include <dlfcn.h> // dylib load

// definitions of static data members
Hash<std::string, Class*> Class::sClasses(300);
std::string Class::sObjectsPath("lib");


void Class::execute_method (const std::string& pMethodName, const Params& p, std::ostream * pOutput)
{
  class_method_t method;
  if (mClassMethods.get(&method, pMethodName)) {
    (*method)(pOutput, p);
  } else {
    *pOutput << "Unknown method '" << pMethodName << "'\n";
  }
}

/////// Class methods ///////////

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

Node * Class::create (Rubyk * pServer, const std::string& pClassName, const Params& p)
{
  Class * klass;
  if (get(&klass, pClassName))
    return (*klass)(pServer, p);
    
  // load failed
  // dummy object in broken mode
  
  if (! sClasses.get(&klass, "Node"))
    Class::declare<Node>("Node");
  
  sClasses.get(&klass, "Node");
  Node * obj = (*klass)(pServer, p);
  obj->set_class(klass);
  obj->set_is_ok( false ); // if init returns false, the node goes into 'broken' mode.
  return obj;
}

inline Node * Class::operator() (Rubyk * pServer, const Params& p)
{  
  return (*mCreateFunction)(this, pServer, p);
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
    if (error = dlerror()) 
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
    if (error = dlerror()) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // call 'init', passing the registration object
  (*function)();

  return true;
}
