#ifndef _OSCIT_TYPED_H_
#define _OSCIT_TYPED_H_
#include <stdio.h> // NULL

namespace oscit {

#define kind_of(klass) is_a(klass::_path())
#define TYPED(str) \
  virtual const char *class_path() { return str; } \
  static  const char *_path() { return str; } \
  
class Typed
{
public:
  TYPED("")
  virtual ~Typed() {}
  
  template<class T>
  static inline T * type_cast(Typed * obj) {
    if (!obj) return NULL;
    if (obj->is_a(T::_path())) {
      return (T*)obj;
    } else {
      return NULL;
    }
  }

  bool is_a(const char *path) {
    const char * c  = path;
    const char * cl = class_path();
    while (*c) {
      if (*c != *cl) return false;
      ++c;
      ++cl;
    }
    return (*cl == '\0') || (*cl == '.');
  }

};

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) oscit::Typed::type_cast<klass>(op)

} // oscit
#endif // _OSCIT_TYPED_H_