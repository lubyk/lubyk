#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "rubyk_types.h"
#include "thash.h"
#include "value.h"
#include <string>

class Hash;
typedef std::vector<std::string>::const_iterator Hash_iterator;

/* Holds the actual data of the Hash class. This is a wrapper around a THash. */
class HashData : public Data
{
public:
  HashData (const std::string& p) : mParameters(20) { build_hash(p); }
  HashData (const char * p)  : mParameters(20)      { build_hash( std::string(p) ); }
  HashData () : mParameters(20) {}
  HashData (unsigned int size) : mParameters(size)  {}
  
  virtual Data * clone()
  { return new HashData(*this); }
  virtual value_t type() const
  { return HashValue; }

  // copy constructor
  HashData(const HashData& pOther) : mParameters(pOther.storage_size())
  {
    mParameters = pOther.mParameters;
    mListValues = pOther.mListValues; // FIXME: what is this ?
  }

  virtual ~HashData() {}
  
  /** Display dictionary inside stream. */
  virtual void to_stream(std::ostream& pStream) const
  { 
#ifdef _TESTING_
    pStream << "[" << mId << "] ";
#endif
    pStream << "{ " << mParameters << " }";
  }
  
  void rebuild(const std::string& s)
  {
    clear();
    build_hash(s);
  }
  
  //FIX template<class T>
  //FIX T val(const char * pKey, T pDefault, bool pUseAnon = false) const
  //FIX {
  //FIX   std::string value;
  //FIX   if (mParameters.get(&value, std::string(pKey)))
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
  //FIX   if (mParameters.get(&value, std::string(pKey))) {
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
  //FIX T cast_param(const std::string& value) const
  //FIX {
  //FIX   return (T)value;
  //FIX }
  //FIX 
  //FIX void add (const char * str)
  //FIX {
  //FIX   add(std::string(str));
  //FIX }
  //FIX 
  //FIX void add (const std::string& pValue) {
  //FIX   mListValues.push_back(pValue);
  //FIX }
  //FIX 
  //FIX void set (const std::string& pKey, const std::string& pValue) {
  //FIX   mParameters.set(pKey,pValue);
  //FIX }
  //FIX void set (const std::string& pKey, const char *       pValue) {
  //FIX   mParameters.set(pKey, std::string(pValue));
  //FIX }
  //FIX void set (const char *       pKey, const std::string& pValue) {
  //FIX   mParameters.set(std::string(pKey), pValue);
  //FIX }
  //FIX void set (const char *       pKey, const char *       pValue) {
  //FIX   mParameters.set(std::string(pKey), std::string(pValue));
  //FIX }
  
  void clear () 
  { mParameters.clear(); mListValues.clear();  }
  
  //FIX size_t size () const
  //FIX { return mListValues.size(); }
  
  unsigned int storage_size() const
  {
    return mParameters.storage_size();
  }
  
  friend std::ostream& operator<< (std::ostream& pStream, const HashData& p);
  
private:  
  friend class Hash;
  THash<std::string,Value>  mParameters; /** Hash of std::string => Value. */
  
  std::vector<std::string>  mListValues; //TODO: remove this and store values in a matrix as first element in dictionary.
  
  void build_hash (const std::string& p);
};

/** Value class to hold a single number (real_t). */
class Hash : public Value
{
public:
  VALUE_METHODS(Hash, HashData, HashValue, Value)
    
  /** Set Hash from a string. */
  Hash& operator= (const std::string& s)
  {
    if (!mPtr) {
      mPtr = new Ptr(new HashData(s));
    } else {
      copy_if_shared();
      *(data_pointer()) = s;
    }
    return *this;
  }
  
  /** Rebuild the whole dictionary from a single string. */
  void rebuild(const std::string& s)
  {
    mutable_data()->rebuild(s);
  }
  
  size_t size() const
  {
    return mPtr ? data_pointer()->mParameters.size() : 0;
  }
  
  /** Return a Value or Nil from a string key. */
  const Value operator[] (const std::string& pKey) const;
  
  const Value operator[] (const char * c) const
  {
    return (*this)[std::string(c)];
  }
  
  // ADD PROXY METHODS HERE...
  
  /** Return an iterator pointing at the first key in the dictionary. */
  Hash_iterator begin() const
  {
    return mPtr ? (Hash_iterator)data_pointer()->mParameters.begin() : (Hash_iterator)NULL;
  }
  
  /** Return an iterator pointing at the past-end key in the dictionary. */
  Hash_iterator end() const
  {
    return mPtr ? (Hash_iterator)data_pointer()->mParameters.end() : (Hash_iterator)NULL;
  }
  
  
  template<class T>
  static T cast_param(const std::string& value)
  {
    return (T)value;
  }
};

template<>
int Hash::cast_param (const std::string& value);

template<>
bool Hash::cast_param (const std::string& value);

template<>
unsigned int Hash::cast_param (const std::string& value);

template<>
unsigned char Hash::cast_param (const std::string& value);

template<>
time_t Hash::cast_param (const std::string& value);

template<>
size_t Hash::cast_param (const std::string& value);

template<>
real_t Hash::cast_param (const std::string& value);

template<>
float Hash::cast_param (const std::string& value);

template<>
const char * Hash::cast_param (const std::string& value);

template<>
char Hash::cast_param (const std::string& value);

#endif // _DICTIONARY_H_