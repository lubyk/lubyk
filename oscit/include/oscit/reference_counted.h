#ifndef OSCIT_INCLUDE_REFERENCE_COUNTED_H_
#define OSCIT_INCLUDE_REFERENCE_COUNTED_H_

namespace oscit {

class ReferenceCounted {
 public:
  ReferenceCounted() : ref_count_(1) {}
  
  virtual ~ReferenceCounted() {}
  
  size_t ref_count() { return ref_count_; }
  
  template<class T>
  static T* acquire(T *elem) {
    ++elem->ref_count_;
    return elem;
  }
  
  template<class T>
  static T* release(T *elem) {
    if (elem) {
      if (--elem->ref_count_ == 0) delete elem;
    }
    return NULL;
  }
 protected:
  size_t ref_count_;
};

} // oscit

#endif // OSCIT_INCLUDE_REFERENCE_COUNTED_H_
