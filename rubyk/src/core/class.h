
/** Pointer to a function to create nodes. */
typedef Node * (*create_method_t)(Class * pClass, const std::string& pName, Planet * pServer, const Hash& p, std::ostream * pOutput);
#define CLASS_ROOT CLASS_ROOT


class NewMethod : public ClassMethod
{
  virtual Value trigger(const Value& val)
  {
    Value res = (*mMethod)(val);
    
    if (!res.is_string()) return res;
    
    
    obj->set_class_url(mParent->url());  // used by osc (using url instead of name because we might have class folders/subfolders some day).
    obj->set_server(pServer);  // ??? where do we find this one ?
    obj->set_output(pOutput);  // ??? I think we won't need this anymore once we have implemented notifications ?
    
    // make inlets
    pClass->make_slots(obj);
    // make outlets
    // make accessors
    
    // initialize
    
    // ??? How do we get parameters ? Should val be a hash {url:"/one/two" params:{tempo:290 rubato:99}} ?
    obj->set_is_ok( obj->init() && obj->set(p) ); // if init or set returns false, the node goes into 'broken' mode.
    return (Node*)obj;
  }
}


/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public Object
{
public:
  
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
  
};