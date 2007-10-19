#ifndef _PARAMS_H_
#define _PARAMS_H_
#include "Hash.h"

class Params
{
public:
  Params (const std::string& pParams) : mParameters(20) { build_hash(pParams); }
  Params (const char * pParams)  : mParameters(20)
  { build_hash( std::string(pParams) ); }
  
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