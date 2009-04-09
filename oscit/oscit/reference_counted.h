#ifndef _REFERENCE_COUNTED_H_
#define _REFERENCE_COUNTED_H_

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
#endif // _REFERENCE_COUNTED_H_