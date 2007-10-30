#ifndef _PARAMS_H_
#define _PARAMS_H_
#include "hash.h"
#include <iostream>

class Params
{
public:
  Params (const std::string& pParams) : mParameters(20) { build_hash(pParams); }
  Params (const char * pParams)  : mParameters(20)
  { build_hash( std::string(pParams) ); }
  Params () : mParameters(20) {}
  
  template<class T>
  T get(const char * pKey, T pDefault) // const member function ?
  {
    std::string * value = mParameters.get(std::string(pKey));
    if (value == NULL) {
      return pDefault;
    } else {  
      return (T)*value;
    }
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
  
  friend std::ostream& operator<< (std::ostream& pStream, const Params& pParams);
  
private:
  Hash<std::string,std::string> mParameters;
  
  void build_hash(const std::string& pParams);
};

template<>
int Params::get(const char * pKey, int pDefault);

template<>
double Params::get(const char * pKey, double pDefault);

template<>
float Params::get(const char * pKey, float pDefault);

#endif