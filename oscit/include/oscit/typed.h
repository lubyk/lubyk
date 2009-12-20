#ifndef OSCIT_INCLUDE_OSCIT_TYPED_H_
#define OSCIT_INCLUDE_OSCIT_TYPED_H_
#include <stdio.h> // NULL

// Using RTTI does not seem to work with libs and such (rubyk does not recognize oscit objects...)
// #define USE_RTTI_TYPE

#include <iostream>

namespace oscit {

#define TYPED(str) \
  virtual const char *class_path() const { return str; } \
  static  const char *_path() { return str; } \

#ifdef USE_RTTI_TYPE

#define kind_of(klass) is_a<klass *>()

class Typed {
public:
  TYPED("")
  virtual ~Typed() {}

  template<class T>
  inline bool is_a() {
    return dynamic_cast<T>(this) != NULL;
  }

  template<class T>
  inline T type_cast() {
    return dynamic_cast<T>(this);
  }

  const std::string class_name() const {
    std::string path(class_path());
    size_t pos = path.find_last_of('.');
    return pos == std::string::npos ? path : path.substr(pos+1);
  }
};

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) dynamic_cast<klass *>(op)

#else

#define kind_of(klass) is_a(klass::_path())

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

  const std::string class_name() const {
    std::string path(class_path());
    size_t pos = path.find_last_of('.');
    return pos == std::string::npos ? path : path.substr(pos+1);
  }

};

/** Return a pointer to the object if the type match. */
#define TYPE_CAST(klass, op) oscit::Typed::type_cast<klass>(op)

#endif  // USE_RTTI_TYPE

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_TYPED_H_
