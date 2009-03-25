#ifndef _OSCIT_CONF_H_
#define _OSCIT_CONF_H_

#define real_t double

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

#ifndef uint
#define uint unsigned int
#endif

#endif // _OSCIT_CONF_H_