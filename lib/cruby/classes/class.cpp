#include "class.h"
#include <dlfcn.h> // dylib load

// definitions of static data members
Hash<std::string, Class*> Class::sClasses(300);
std::string Class::sObjectsPath("lib");

Node * Class::create (Rubyk * pServer, const std::string& pKey, const Params& pParams)
{
  Class ** klass_ptr = sClasses.get(pKey);
  if (klass_ptr) {
    return (**klass_ptr)(pServer, pParams);
  } else {
    // try to load dynamic lib
    std::string path = sObjectsPath;
    path.append("/").append(pKey).append(".rko");
    //printf("Loading '%s'\n", path.c_str());
    if (load(path.c_str(), "init")) {
      klass_ptr = sClasses.get(pKey);
      if (klass_ptr)
        return (**klass_ptr)(pServer, pParams);
    } else {
      printf("Error, '%s' should declare '%s'.\n", path.c_str(), pKey.c_str());
    }
  } 
    // load failed
    // dummy object in broken mode
  if ((klass_ptr = sClasses.get("Node")) == NULL)
    Class::declare<Node>("Node");
  
  klass_ptr = sClasses.get("Node");
  Node * obj = (**klass_ptr)(pServer, pParams);
  obj->set_class(*klass_ptr);
  obj->set_is_ok( false ); // if init returns false, the node goes into 'broken' mode.
  return obj;
}

inline Node * Class::operator() (Rubyk * pServer, const Params& pParams)
{
  return (*mCreateFunction)(this, pServer, pParams);
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
