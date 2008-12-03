#include "class.h"
#include <dlfcn.h> // dylib load

const Value NewMethod::trigger (const Value& val)
{
  String url;
  Hash params;
  Value res;
  if (val.is_hash()) {
    url    = Hash(val)["url"];
    params = Hash(val)["params"];
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
  Class * parent = (Class*)mParent;  // FIXME: type checking !!
  res = parent->make_inlets(obj);
  if (res.is_error()) return res;
  
//FIX    // make outlets
//FIX    res = parent->make_outlets(obj);
//FIX    if (res.is_error()) return res;
//FIX    
//FIX    // make accessors
//FIX    res = parent->make_accessors(obj);
//FIX    if (res.is_error()) return res;
  
  // initialize
  obj->set_is_ok( obj->init() && obj->set(params) ); // if init or set returns false, the node goes into 'broken' mode.
  return url;
}


/** This trigger implements "/classes". It returns the list of objects in mObjectsPath. */
const Value ClassListing::trigger (const Value& val)
{
  return String(""); // TODO: 'lib' directory listing !
}

const Value ClassListing::not_found (const std::string& pUrl, const Value& val)
{
  std::string className = pUrl.substr(url().length() + 1);
  std::cout << "---> " << className << std::endl;
  className = className.substr(0, className.find("/"));
  Object * obj;
  
  // try to load dynamic lib
  std::string path = mObjectsPath;
  path.append("/").append(className).append(".rko");
  
  std::cout << "load lib from '" << path << "'" << std::endl;
  
  if (load(path.c_str(), "init")) {
    // FIXME: we should handle '#info' and '#inspect' here
    if (Object::get(&obj, pUrl)) {
      // Found object, we can trigger it.
      return obj->trigger(val);
    } else
      return Error("'").append(path).append("' should declare '").append(className).append("'.");
  } else
    return Error("Could not load '").append(path).append("'.");
}

// for help to create a portable version of this load function, read Ruby's dln.c file.
bool ClassListing::load(const char * file, const char * init_name)
{
  void *image;
  void (*function)(void);
  const char *error = 0;
  
  // load shared extension image into memory
  if ((image = (void*)dlopen(file, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
    printf("Could not open file '%s'.", file);
    if ( (error = dlerror()) ) 
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
    if ( (error = dlerror()) ) 
      printf(" %s\n", error);
    else
      printf("\n");
    return false;
  }
  
  // call 'init', passing the registration object
  (*function)();

  return true;
}

/** Get a Class object from it's std::string name ("Metro"). */
bool Class::get_class (Class ** pResult, const std::string& pName)
{
  Object * obj;
  if (Object::get(&obj, std::string(CLASS_ROOT).append("/").append(pName))) {
    *pResult = (Class*)obj; // FIXME: type checking !!
    return true;
  }
  return false;
}
