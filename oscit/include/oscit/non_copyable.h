#ifndef OSCIT_INCLUDE_OSCIT_NON_COPYABLE_H_
#define OSCIT_INCLUDE_OSCIT_NON_COPYABLE_H_

class NonCopyable {
 protected:
  NonCopyable() {}
  ~NonCopyable() {}
 private: 
  NonCopyable(const NonCopyable &other);
  NonCopyable &operator=(const NonCopyable &other);
};

#endif  // OSCIT_INCLUDE_OSCIT_NON_COPYABLE_H_