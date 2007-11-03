#ifndef _PARAMS_H_
#define _PARAMS_H_
#include "hash.h"
#include <iostream>

class Params
{
public:
  Params (const std::string& p) : mParameters(20) { build_hash(p); }
  Params (const char * p)  : mParameters(20)
  { build_hash( std::string(p) ); }
  Params () : mParameters(20) {}
  
  template<class T>
  T get(const char * pKey, T pDefault) const
  {
    std::string value;
    if (mParameters.get(&value, std::string(pKey)))
      return (T)value;
    else
      return pDefault;
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
int Params::get(const char * pKey, int pDefault) const;

template<>
double Params::get(const char * pKey, double pDefault) const;

template<>
float Params::get(const char * pKey, float pDefault) const;

template<>
const char * Params::get(const char * pKey, const char * pDefault) const;

#endif