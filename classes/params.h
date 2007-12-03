#ifndef _PARAMS_H_
#define _PARAMS_H_
#include "hash.h"
#include <string>


class Params
{
public:
  Params (const std::string& p) : mParameters(20) { build_hash(p); }
  Params (const char * p)  : mParameters(20)      { build_hash( std::string(p) ); }
  Params () : mParameters(20) {}
  
  template<class T>
  T val(const char * pKey, T pDefault, bool pUseAnon = false) const
  {
    std::string value;
    if (mParameters.get(&value, std::string(pKey)))
      return cast_param<T>(value);
    else if (pUseAnon) {
      T value;
      if (get(&value)) return value;
      else return pDefault;
    } else
      return pDefault;
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found. If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey, bool pUseAnon) const
  {
    std::string value;
    if (pKey == NULL) {
      if (mListParams.size() > 0) {
        *pResult = cast_param<T>(mListParams[0]);
        return true;
      } else
        return false;
    }
    if (mParameters.get(&value, std::string(pKey))) {
      *pResult = cast_param<T>(value);
      return true;
    } else if (pUseAnon) {
      return get(pResult, NULL, false);
    } else
      return false;
  }
  
  /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  template<class T>
  bool get(T* pResult, size_t pIndex) const
  {
    std::string value;
    if (pIndex < mListParams.size()) {
      *pResult = cast_param<T>(mListParams[pIndex]);
      return true;
    } else
      return false;
  }
  
  /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  template<class T>
  bool get(T* pResult, int pIndex) const
  {
    std::string value;
    if ((size_t)pIndex < mListParams.size()) {
      *pResult = cast_param<T>(mListParams[pIndex]);
      return true;
    } else
      return false;
  }
  
  /** Get default value. Return false if none found. */
  template<class T>
  bool get(T* pResult) const
  {
    return get(pResult, (size_t)0);
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found (default value not used). If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey) const
  {
    return get(pResult, pKey, false);
  }
  
  template<class T>
  T cast_param(const std::string& value) const
  {
    return (T)value;
  }
  
  void add (const char * str)
  {
    add(std::string(str));
  }
  
  void add (const std::string& pValue) {
    mListParams.push_back(pValue);
  }

  void set (const std::string& pKey, const std::string& pValue) {
    mParameters.set(pKey,pValue);
  }
  void set (const std::string& pKey, const char *       pValue) {
    mParameters.set(pKey, std::string(pValue));
  }
  void set (const char *       pKey, const std::string& pValue) {
    mParameters.set(std::string(pKey), pValue);
  }
  void set (const char *       pKey, const char *       pValue) {
    mParameters.set(std::string(pKey), std::string(pValue));
  }
  
  void clear () 
  { mParameters.clear(); mListParams.clear();  }
  
  size_t size () const
  { return mListParams.size(); }
  
  friend std::ostream& operator<< (std::ostream& pStream, const Params& p);
  
private:
  Hash<std::string,std::string> mParameters;
  
  std::vector<std::string> mListParams;
  
  void build_hash(const std::string& p);
};

template<>
int Params::cast_param (const std::string& value) const;

template<>
unsigned int Params::cast_param (const std::string& value) const;

template<>
unsigned char Params::cast_param (const std::string& value) const;

template<>
time_t Params::cast_param (const std::string& value) const;

template<>
size_t Params::cast_param (const std::string& value) const;

template<>
double Params::cast_param (const std::string& value) const;

template<>
float Params::cast_param (const std::string& value) const;

template<>
const char * Params::cast_param (const std::string& value) const;

template<>
char Params::cast_param (const std::string& value) const;

std::ostream& operator<< (std::ostream& pStream, const Params& p);

#endif