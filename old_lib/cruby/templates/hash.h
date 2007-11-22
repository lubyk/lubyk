/** Dictionary. The dictionary stores a copy of the data. Pointers to the copy are returned. 
  * We return pointers so that we can return NULL if the value is not found. */
#ifndef _HASH_H_
#define _HASH_H_

#include <cstdio>
#include <string>
#include <vector>

typedef unsigned int uint;

template<class K, class T>
struct HashElement {
  HashElement() : obj(0), next(0) {}
  ~HashElement() { if (obj) delete obj; }
  K     id; 
  T*    obj;
  /// lookup collision chain
  HashElement<K,T> * next;
};

template<class K>
static uint hashId(K key);

// K is the key class, T is the object class
template <class K, class T>
class Hash {
public:
  Hash(unsigned int size) : mSize(size) { mHashTable = new HashElement<K,T>[size]; }
  virtual ~Hash() {
    int i;
    HashElement<K,T> * current, * next;
    // remove collisions
    for(i=0;i<mSize;i++) {
      current = mHashTable[i].next;
      while (current) {
        next = current->next;
        delete current;
        current = next;
      }
    }
    // remove table
    delete[] mHashTable;
  }
  
  void set(const K& pId, const T& pElement);
  
  /** Get an element of the dictionary and set the pResult to this element. Returns false if no element found. */
  bool get(T* pResult, const K& pId) const;
  
  /** Get the default value (last value). */
  bool get(T* pResult) const;
  
  /** Remove object with the given key. */
  void remove(const K& pId);
  
  /** Remove all objects. */
  void clear() 
  {
    typename std::vector<K>::iterator it;
    typename std::vector<K>::iterator end = mKeys.end();
    for(it=mKeys.begin(); it < end; it++) {
      remove(*it);
    }
    mKeys.clear();
  }
  
  size_t size() const
  { return mKeys.size(); }
  
  /** List of keys. */
  const std::vector<K> * keys() { return &mKeys; }
  
  /** Begin iterator over the keys of the dictionary (read-only). */
  typename std::vector<K>::const_iterator begin() const { return mKeys.begin(); }
  
  /** Past end iterator over the keys of the dictionary (read-only). */
  typename std::vector<K>::const_iterator end()   const { return mKeys.end(); }
  
  /** Begin iterator over the keys of the dictionary. */
  typename std::vector<K>::iterator begin() { return mKeys.begin(); }
  
  /** Begin iterator over the keys of the dictionary. */
  typename std::vector<K>::iterator end()   { return mKeys.end(); }
private:  
  /* data */
  HashElement<K,T> * mHashTable;
  std::vector<K>     mKeys;
  
  unsigned int mSize;
};

template <class K, class T>
void Hash<K,T>::set(const K& pId, const T& pElement) {
  HashElement<K,T> *  found;
  HashElement<K,T> ** set_next;
  uint key = hashId(pId) % mSize;
  found    = &(mHashTable[key]);  // pointer to found element
  set_next = &(found->next);      // where to write the new inserted value if there is one
  
  while (found && found->obj && found->id != pId) { // found->obj is for the case where pId == 0
    set_next = &(found->next);
    found    = found->next;
  }
  if (found && !found->obj) {
    // in table value
  } else if (!found) {
    // collision
    found = new HashElement<K,T>;
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
bool Hash<K,T>::get(T* pResult, const K& pId) const 
{
  HashElement<K,T> * found;
  uint key = hashId(pId) % mSize;
  
  found = &(mHashTable[key]);
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
bool Hash<K,T>::get(T* pResult) const 
{
  if (mKeys.size() > 0)
    return get(pResult, mKeys[mKeys.size() - 1]);
  else
    return false;
}

template <class K, class T>
void Hash<K,T>::remove(const K& pId) {
  HashElement<K,T> *  found, * next;
  HashElement<K,T> ** set_next;
  uint key = hashId(pId) % mSize;
  
  found    = &(mHashTable[key]);  // pointer to found element
  set_next = NULL;                // where to write removed element's next if there is one
  
  while (found && found->obj && found->id != pId) {
    set_next = &(found->next);
    found    = found->next;
  }
  if (found && found->obj) {
    typename std::vector<K>::iterator it;
    typename std::vector<K>::iterator end = mKeys.end();
    for(it = mKeys.begin(); it < end; it++) {
      if (*it == pId) {
        mKeys.erase(it);
        break;
      }
    }
    
    if (set_next) {
      // link previous to next
      *set_next = found->next;
      delete found;
    } else {
      // in table
      if (next = found->next) {
        // There was a collision. Move chain up.
        if (found->obj) {
          delete found->obj;
        }
        found->obj  = next->obj;
        next->obj   = NULL;  // to avoid double delete
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

// ===== std::string& =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
template<>
inline uint hashId(const std::string& key)
{
  const char *str = key.c_str();
  unsigned long hash = 0;
  int c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

// ===== std::string =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
template<>
inline uint hashId(const std::string key)
{
  return hashId<const std::string&>(key);
}

// ===== char *      =====
// sdbm function: taken from http://www.cse.yorku.ca/~oz/hash.html
template<>
inline uint hashId (const char * str)
{
  unsigned long hash = 0;
  int c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

#endif