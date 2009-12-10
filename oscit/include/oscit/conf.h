#ifndef OSCIT_INCLUDE_OSCIT_CONF_H_
#define OSCIT_INCLUDE_OSCIT_CONF_H_

namespace oscit {
  typedef double Real;
}
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

#endif // OSCIT_INCLUDE_OSCIT_CONF_H_
