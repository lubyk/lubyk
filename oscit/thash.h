/** Dictionary. The dictionary stores a copy of the data. Pointers to the copy are returned. 
  * We return pointers so that we can return NULL if the value is not found. */
#ifndef _HASH_H_
#define _HASH_H_

#include <cstdio>
#include <string>
#include <list>
#include <iostream>

///////////// MACRO FOR STATIC STRING HASH ////////////////
// macro hashing function taken from http://chrissavoie.com/index.php?option=com_content&task=view&id=14&Itemid=1
// the constant value to return at the end of the hashing
#define HASH_CONSTANT 5381
 
// The following is the guts of the compile-time hasher
#define HASH_RECURSE_00(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_01(string+1, *(string+1))), value)
#define HASH_RECURSE_01(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_02(string+1, *(string+1))), value)
#define HASH_RECURSE_02(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_03(string+1, *(string+1))), value)
#define HASH_RECURSE_03(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_04(string+1, *(string+1))), value)
#define HASH_RECURSE_04(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_05(string+1, *(string+1))), value)
#define HASH_RECURSE_05(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_06(string+1, *(string+1))), value)
#define HASH_RECURSE_06(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_07(string+1, *(string+1))), value)
#define HASH_RECURSE_07(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_08(string+1, *(string+1))), value)
#define HASH_RECURSE_08(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_09(string+1, *(string+1))), value)
#define HASH_RECURSE_09(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_10(string+1, *(string+1))), value)
#define HASH_RECURSE_10(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_11(string+1, *(string+1))), value)
#define HASH_RECURSE_11(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_12(string+1, *(string+1))), value)
#define HASH_RECURSE_12(string, value) HASH_CONSTANT
 
// The following is the function used for hashing
// Do NOT use NEXTHASH more than once, it will cause
// N-Squared expansion and make compilation very slow
// If not impossible
#define HASH_FUNCTION(NEXTHASH, VALUE) VALUE + (NEXTHASH << 2)
 
// finally the macro used to generate the hash
#define H(string) (uint)HASH_RECURSE_00(string, *string)

#ifndef uint
typedef unsigned int uint;
#endif

typedef std::list<std::string>::iterator string_iterator;
typedef std::list<std::string>::const_iterator const_string_iterator;

template<class K, class T>
struct THashElement
{
  THashElement() : obj(0), next(0) {}
  ~THashElement() { if (obj) delete obj; }
  K     id; 
  T*    obj;
  /// lookup collision chain
  THashElement<K,T> * next;
};

template<class K>
static uint hashId(K key);

// K is the key class, T is the object class
template <class K, class T>
class THash
{
public:
  THash(unsigned int size) : mSize(size) { 
    mTHashTable = new THashElement<K,T>[size];
  }
  
  // copy constructor
  THash(const THash& pOther) {  
    const_string_iterator it;
    const_string_iterator end = pOther.end();
    T value;
    
    mSize = pOther.mSize;
    mTHashTable = new THashElement<K,T>[mSize];
    
    for(it = pOther.begin(); it != end; it++) {
      if (pOther.get(&value, *it)) {
        set(*it, value);
      }
    }
  }
  
  THash& operator=(const THash& pOther) {
    const_string_iterator it;
    const_string_iterator end = pOther.end();
    T value;

    for(it = pOther.begin(); it != end; it++) {
      if (pOther.get(&value, *it)) {
        set(*it, value);
      }
    }
    return *this;
  }
  
  virtual ~THash() {
    THashElement<K,T> * current, * next;
    // remove collisions
    for(size_t i=0;i<mSize;i++) {
      current = mTHashTable[i].next;
      while (current) {
        next = current->next;
        delete current;
        current = next;
      }
    }
    // remove table
    delete[] mTHashTable;
  }
  
  void set(const K& pId, const T& pElement);
  
  /** Get an element of the dictionary and set the pResult to this element. Returns false if no element found. */
  bool get(const K& pId, T* pResult) const;
  
  /** Get an element's key. Returns false if the element could not be found. */
  bool get_key(const T& pElement, K* pResult) const;
  
  /** Get the default value (last value). */
  bool get(T* pResult) const;
  
  /** Remove object with the given key. */
  void remove(const K& pId);
  
  /** Remove the given element. */
  void remove_element(const T& pElement);
  
  /** Remove all objects. */
  void clear() {
    // TODO: optimize...
    while(mKeys.begin() != mKeys.end()) {
      remove(*mKeys.begin());
    }
  }
  
  /** Return true if the dictionary is empty. */
  bool empty() const { return size() == 0; }
  
  /** Return number of elements (distinct keys). */
  size_t size() const { return mKeys.size(); }
  
  /** Return size of storage (main hash table). */
  unsigned int storage_size() const { return mSize; }
  
  /** List of keys. */
  const std::list<K>& keys() { return mKeys; }
  
  /** Begin iterator over the keys of the dictionary (read-only). */
  typename std::list<K>::const_iterator begin() const { return mKeys.begin(); }
  
  /** Past end iterator over the keys of the dictionary (read-only). */
  typename std::list<K>::const_iterator end()   const { return mKeys.end(); }
  
  /** Begin iterator over the keys of the dictionary. */
  typename std::list<K>::iterator begin() { return mKeys.begin(); }
  
  /** Begin iterator over the keys of the dictionary. */
  typename std::list<K>::iterator end()   { return mKeys.end(); }
private:  
  /* data */
  THashElement<K,T> * mTHashTable;
  std::list<K>        mKeys;  // FIXME: this should be a list !
  
  unsigned int mSize;
};

template <class K, class T>
void THash<K,T>::set(const K& pId, const T& pElement) {
  THashElement<K,T> *  found;
  THashElement<K,T> ** set_next;
  uint key = hashId(pId) % mSize;
  found    = &(mTHashTable[key]);  // pointer to found element
  set_next = &(found->next);      // where to write the new inserted value if there is one
  
  while (found && found->obj && found->id != pId) { // found->obj is for the case where pId == 0
    set_next = &(found->next);
    found    = found->next;
  }
  if (found && !found->obj) {
    // in table value
  } else if (!found) {
    // collision
    found = new THashElement<K,T>;
    if (!found) {
      // FIXME: alloc error. Raise ?
      fprintf(stderr, "Could not allocate new hash pointer.\n");
      return;
    }
    *set_next = found;
  }
  if (found->obj) {
    // replace value for given key
    delete found->obj;
  } else {
    // new key
    mKeys.push_back(pId);
  }
  found->obj  = new T(pElement);
  found->id   = pId;
}

template <class K, class T>
bool THash<K,T>::get(const K& pId, T* pResult) const {
  THashElement<K,T> * found;
  uint key = hashId(pId) % mSize;
  
  found = &(mTHashTable[key]);
  while (found && found->obj && found->id != pId)
    found = found->next;
    
  if (found && found->obj && found->id == pId) {
    *pResult = *(found->obj);
    return true;
  } else {
    return false;
  }
}

template <class K, class T>
bool THash<K,T>::get_key(const T& pElement, K* pResult) const {
  typename std::list<K>::const_iterator it;
  typename std::list<K>::const_iterator end = mKeys.end();
  
  T element;
  
  for(it = mKeys.begin(); it != end; it++) {
    if (get(&element, *it) && element == pElement) {
      *pResult = *it;
      return true;
    }
  }
  return false;
}


template <class K, class T>
void THash<K,T>::remove(const K& pId) {
  THashElement<K,T> *  found, * next;
  THashElement<K,T> ** set_next;
  uint key = hashId(pId) % mSize;
  found    = &(mTHashTable[key]);  // pointer to found element
  set_next = NULL;                // where to write removed element's next if there is one
  
  while (found && found->obj && found->id != pId) {
    set_next = &(found->next);
    found    = found->next;
  }
  if (found && found->obj) {
    typename std::list<K>::iterator it  = mKeys.begin();
    typename std::list<K>::iterator end = mKeys.end();
    while(it != end) {
      if (*it == pId)
        it = mKeys.erase(it);
      else
        it++;
    }
    
    if (set_next) {
      // link previous to next
      *set_next = found->next;
      delete found;
    } else {
      // in table
      if ( (next = found->next) ) {
        // There was a collision. Move chain up.
        if (found->obj) {
          delete found->obj;
        }
        found->obj  = next->obj;
        next->obj   = NULL;  // to avoid Real delete
        found->id   = next->id;
        found->next = next->next;
        delete next;
      } else {
        // no collision. free.
        delete found->obj;
        found->obj  = NULL;
      }
    }
  }
}


template <class K, class T>
void THash<K,T>::remove_element(const T& pElement) {
  K key;
  while (get_key(&key, pElement)) {
    remove(key);
  }
}

template <class K, class T>
std::ostream& operator<< (std::ostream& pStream, const THash<K,T>& hash) {
  typename std::list<K>::const_iterator it,begin,end;
  end   = hash.end();
  begin = hash.begin();
  T value;
  for( it = begin; it != end; it++) {
    if (it != begin) pStream << " ";
    if (hash.get(&value, *it))
      pStream << *it << ":" << value;
    else
      pStream << *it << ":" << "/error/";
  }
  return pStream;
}

/////// HASH FUNCTIONS  ////////

// ===== uint =====
// Thomas Wang's 32 Bit Mix Function: http://www.cris.com/~Ttwang/tech/inthash.htm
template<>
inline uint hashId(const uint key) {
  uint res = key;
  res += ~(res << 15);
  res ^= (res >> 10);
  res += (res << 3);
  res ^= (res >> 6);
  res += ~(res << 11);
  res ^= (res >> 16);
  return res;
}

// ===== char *      =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
// template<>
// inline uint hashId (const char * str)
// {
//   unsigned long hash = 0;
//   int c;
// 
//   while ( (c = *str++) )
//     hash = c + (hash << 6) + (hash << 16) - hash;
// 
//   return hash;
// }
//
// We use the simpler hash to avoid too long compile times in the static string hash macro.

template<>
inline uint hashId(const char c) {
  return HASH_FUNCTION(HASH_CONSTANT, c);
}

template<>
inline uint hashId(const char * str) {
  return H(str);
}

// ===== std::string& =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
template<>
inline uint hashId(const std::string &key) {
  const char *str = key.c_str();
  return hashId(str);
}

// ===== std::string =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
template<>
inline uint hashId(const std::string key) {
  return hashId<const std::string&>(key);
}


#endif