#include "class_finder.h"
#include "planet.h"
#include <dlfcn.h> // dylib load


/** This trigger implements "/class". It returns the list of objects in objects_path_. */
const Value ClassFinder::trigger (const Value &val)
{
  return gNilValue; // TODO: 'lib' directory listing ?
}

BaseObject *ClassFinder::build_child(const std::string &class_name, Value *error) {
  BaseObject * obj;
  
  // try to load dynamic lib
  std::string path = objects_path_;
  path.append("/").append(class_name).append(".rko");
  
  if (load(path.c_str(), "init")) {
    obj = child(class_name);
    if ( obj != NULL ) {
      // Found object (everything went fine) !
      return obj;
    } else {
      error->set(INTERNAL_SERVER_ERROR, "'").append(path).append("' should declare '").append(name).append("'.");
      return NULL;
    }
  } else {
    error->set(INTERNAL_SERVER_ERROR, "Could not load '").append(path).append("'.");
    return NULL;
  }
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool ClassFinder::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(Worker&);
  const char *error = 0;
  
  // load shared extension image into memory
  // ---> 
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if ( (error = dlerror()) ) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // get 'init' function into the image
  function = (void(*)(Worker&))dlsym(image, init_name);
  if (function == 0) {
    dlclose(image);
    printf("Symbol '%s' not found in '%s'.",init_name,file);
    if ( (error = dlerror()) ) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // call 'init', passing the registration object
  (*function)(*worker_);

  return true;
}