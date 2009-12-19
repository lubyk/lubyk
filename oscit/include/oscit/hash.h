#ifndef OSCIT_INCLUDE_OSCIT_HASH_H_
#define OSCIT_INCLUDE_OSCIT_HASH_H_
#include "oscit/values.h"

namespace oscit {
class Value;
typedef std::list<std::string>::const_iterator HashIterator;

/** A Hash is just a reference counted THash<std::string,Value>. */
class Hash : public ReferenceCounted, public THash<std::string,Value>
{
 public:
  typedef std::list<std::string>::const_iterator const_iterator;
  explicit Hash(size_t size) : THash<std::string,Value>(size) {}
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_HASH_H_
