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
  T val(const char * pKey, T pDefault) const
  {
    std::string value;
    if (mParameters.get(&value, std::string(pKey)))
      return cast_param<T>(value);
    else
      return pDefault;
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found. If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey, bool pTryDefault) const
  {
    std::string value;
    if (pKey == NULL) {
      if (mParameters.get(&value)) {
        *pResult = cast_param<T>(value);
        return true;
      } else
        return false;
    }
    if (mParameters.get(&value, std::string(pKey))) {
      *pResult = cast_param<T>(value);
      return true;
    } else if (pTryDefault) {
      return get(pResult, NULL, false);
    } else
      return false;
  }
  
  /** Get default value. Return false if none found. */
  template<class T>
  bool get(T* pResult) const
  {
    return get(pResult, NULL, false);
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found (default value not used). If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey) const
  {
    return get(pResult, NULL, true);
  }
  
  template<class T>
  T cast_param(const std::string& value) const
  {
    return (T)value;
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
  void clear () {
    mParameters.clear();
  }
  
  friend std::ostream& operator<< (std::ostream& pStream, const Params& p);
  
private:
  // FIXME: we could store only pointers to the values...
  Hash<std::string,std::string> mParameters;
  
  void build_hash(const std::string& p);
};

template<>
int Params::cast_param (const std::string& value) const;

template<>
double Params::cast_param (const std::string& value) const;

template<>
float Params::cast_param (const std::string& value) const;

template<>
const char * Params::cast_param (const std::string& value) const;

std::ostream& operator<< (std::ostream& pStream, const Params& p);

#endif