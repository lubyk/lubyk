#ifndef OSCIT_INCLUDE_REFERENCE_COUNTED_H_
#define OSCIT_INCLUDE_REFERENCE_COUNTED_H_

namespace oscit {

class ReferenceCounted
{
 public:
  ReferenceCounted() : ref_count_(1) {}
  
  virtual ~ReferenceCounted() {}
  
  size_t ref_count() { return ref_count_; }
  
  static void acquire(ReferenceCounted *elem) {
    ++elem->ref_count_;
  }
  
  static void release(ReferenceCounted *elem) {
    if (--elem->ref_count_ == 0) delete elem;
  }
 protected:
  size_t ref_count_;
};

} // oscit

#endif // OSCIT_INCLUDE_REFERENCE_COUNTED_H_
