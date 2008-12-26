#include "class_finder.h"
#include "planet.h"
#include <dlfcn.h> // dylib load


/** This trigger implements "/class". It returns the list of objects in mObjectsPath. */
const Value ClassFinder::trigger (const Value& val)
{
  return String(""); // TODO: 'lib' directory listing !
}

const Value ClassFinder::not_found (const std::string& pUrl, const Value& val)
{
  std::string className = pUrl.substr(url().length() + 1);
  className = className.substr(0, className.find("/"));
  oscit::Object * obj;
  
  // try to load dynamic lib
  std::string path = mObjectsPath.string();
  path.append("/").append(className).append(".rko");
  
  if (load(path.c_str(), "init")) {
    // FIXME: we should handle '#info' and '#inspect' here
    if (mRoot->get(&obj, pUrl)) {
      // Found object, we can trigger it.
      return obj->trigger(val);
    } else
      return Error("'").append(path).append("' should declare '").append(pUrl).append("'.");
  } else
    return Error("Could not load '").append(path).append("'.");
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool ClassFinder::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(oscit::Root&);
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
  function = (void(*)(oscit::Root&))dlsym(image, init_name);
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
  (*function)(*mRoot);

  return true;
}