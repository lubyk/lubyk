#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "rubyk_types.h"
#include "thash.h"
#include "value.h"
#include <string>

class Hash;
typedef std::list<std::string>::const_iterator Hash_iterator;

/* Holds the actual data of the Hash class. This is a wrapper around a THash. */
class HashData : public Data
{
public:
  HashData () : mHash(20) {}
  HashData (unsigned int size) : mHash(size)  {}
  
  virtual Data * clone()
  { return new HashData(*this); }
  virtual value_t type() const
  { return HashValue; }

  // copy constructor
  HashData(const HashData& other) : mHash(other.storage_size())
  {
    mHash = other.mHash;
  }

  virtual ~HashData() {}
  
  /** Display dictionary inside stream. */
  virtual void to_stream(std::ostream& pStream) const
  { 
#ifdef _TESTING_
    if (sShowId) pStream << "[" << mId << "] ";
#endif
    pStream << "{ " << mHash << " }";
  }
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    pStream << "{ " << mHash << " }";
  }
  
  //FIX template<class T>
  //FIX T val(const char * pKey, T pDefault, bool pUseAnon = false) const
  //FIX {
  //FIX   std::string value;
  //FIX   if (mHash.get(&value, std::string(pKey)))
  //FIX     return cast_param<T>(value);
  //FIX   else if (pUseAnon) {
  //FIX     T value;
  //FIX     if (get(&value)) return value;
  //FIX     else return pDefault;
  //FIX   } else
  //FIX     return pDefault;
  //FIX }
  //FIX 
  //FIX /** Try to get a parameter from a given key. Returns false if the key is not found. If the key is NULL, get the default value. */
  //FIX template<class T>
  //FIX bool get(T* pResult, const char * pKey, bool pUseAnon) const
  //FIX {
  //FIX   std::string value;
  //FIX   if (pKey == NULL) {
  //FIX     if (mListValues.size() == 1) {
  //FIX       // 'default' parameter
  //FIX       *pResult = cast_param<T>(mListValues[0]);
  //FIX       return true;
  //FIX     } else
  //FIX       return false;
  //FIX   }
  //FIX   if (mHash.get(&value, std::string(pKey))) {
  //FIX     *pResult = cast_param<T>(value);
  //FIX     return true;
  //FIX   } else if (pUseAnon) {
  //FIX     return get(pResult, NULL, false);
  //FIX   } else
  //FIX     return false;
  //FIX }
  //FIX 
  //FIX /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  //FIX template<class T>
  //FIX bool get(T* pResult, size_t pIndex) const
  //FIX {
  //FIX   std::string value;
  //FIX   if (pIndex < mListValues.size()) {
  //FIX     *pResult = cast_param<T>(mListValues[pIndex]);
  //FIX     return true;
  //FIX   } else
  //FIX     return false;
  //FIX }
  //FIX 
  //FIX /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  //FIX template<class T>
  //FIX bool get(T* pResult, int pIndex) const
  //FIX {
  //FIX   std::string value;
  //FIX   if ((size_t)pIndex < mListValues.size()) {
  //FIX     *pResult = cast_param<T>(mListValues[pIndex]);
  //FIX     return true;
  //FIX   } else
  //FIX     return false;
  //FIX }
  //FIX 
  //FIX /** Get default value. Return false if none found. */
  //FIX template<class T>
  //FIX bool get(T* pResult) const
  //FIX {
  //FIX   return get(pResult, (size_t)0);
  //FIX }
  //FIX 
  //FIX /** Try to get a parameter from a given key. Returns false if the key is not found (default value not used). If the key is NULL, get the default value. */
  //FIX template<class T>
  //FIX bool get(T* pResult, const char * pKey) const
  //FIX {
  //FIX   return get(pResult, pKey, false);
  //FIX }
  //FIX // FIX: remove
  //FIX template<class T>
  //FIX T cast_param(const std::string &value) const
  //FIX {
  //FIX   return (T)value;
  //FIX }
  //FIX 
  //FIX void add (const char * str)
  //FIX {
  //FIX   add(std::string(str));
  //FIX }
  //FIX 
  //FIX void add (const std::string &pValue) {
  //FIX   mListValues.push_back(pValue);
  //FIX }
  //FIX 
  //FIX void set (const std::string &pKey, const std::string &pValue) {
  //FIX   mHash.set(pKey,pValue);
  //FIX }
  //FIX void set (const std::string &pKey, const char *       pValue) {
  //FIX   mHash.set(pKey, std::string(pValue));
  //FIX }
  //FIX void set (const char *       pKey, const std::string &pValue) {
  //FIX   mHash.set(std::string(pKey), pValue);
  //FIX }
  //FIX void set (const char *       pKey, const char *       pValue) {
  //FIX   mHash.set(std::string(pKey), std::string(pValue));
  //FIX }
  
  void clear () 
  { mHash.clear(); }
  
  unsigned int storage_size() const
  {
    return mHash.storage_size();
  }
  
  friend std::ostream& operator<< (std::ostream& pStream, const HashData& p);
  
private:  
  friend class Hash;
  THash<std::string,Value>  mHash; /** Hash of std::string => Value. */
  
};

/** Value class to hold a single number (Real). */
class Hash : public Value
{
public:
  VALUE_METHODS(Hash, HashData, HashValue, Value)
  VALUE_FROM_STRING(Hash)
  
  /** Rebuild the whole dictionary from a single string. */
  void rebuild(const std::string &s)
  {
    *this = Value(s);
  }
  
  
  // ADD PROXY METHODS HERE...
  
  size_t size() const
  {
    return ptr_ ? data_pointer()->mHash.size() : 0;
  }
  
  /** Return a Value or Nil from a string key. */
  const Value operator[] (const std::string &pKey) const;
  
  /** Return a Value or Nil from a string key. */
  const Value operator[] (const char * c) const
  {
    return (*this)[std::string(c)];
  }
  
  /** Set Value for string key. */
  void set_key (const std::string &pKey, const Value& val) // FIXME: replace by 'set' and change Value.set for something else.
  {
    mutable_data()->mHash.set(pKey, val);
  }
  
  /** Return an iterator pointing at the first key in the dictionary. */
  Hash_iterator begin() const
  {
    return ptr_ ? (Hash_iterator)data_pointer()->mHash.begin() : (Hash_iterator)NULL;
  }
  
  /** Return an iterator pointing at the past-end key in the dictionary. */
  Hash_iterator end() const
  {
    return ptr_ ? (Hash_iterator)data_pointer()->mHash.end() : (Hash_iterator)NULL;
  }
  
  /** Clear hash (empty). */
  void clear()
  {
    mutable_data()->clear();
  }
  
  template<class T>
  static T cast_param(const std::string &value)
  {
    return (T)value;
  }
};

template<>
int Hash::cast_param (const std::string &value);

template<>
bool Hash::cast_param (const std::string &value);

template<>
unsigned int Hash::cast_param (const std::string &value);

template<>
unsigned char Hash::cast_param (const std::string &value);

template<>
time_t Hash::cast_param (const std::string &value);

template<>
size_t Hash::cast_param (const std::string &value);

template<>
Real Hash::cast_param (const std::string &value);

template<>
float Hash::cast_param (const std::string &value);

template<>
const char * Hash::cast_param (const std::string &value);

template<>
char Hash::cast_param (const std::string &value);

#endif // _DICTIONARY_H_