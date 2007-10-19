#include "Params.h"
#include <string>

void Params::build_hash (const std::string& pParams)
{
  std::string key, value;
  unsigned int size = pParams.size();
  unsigned int pos,key_start,key_end,value_start,value_end;
  pos = 0;
  // 1. loop
  while(pos < size)
  {  
    //    1.1 skip whitespace
    pos = pParams.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return;
    key_start = pos;
    
    //    1.2 read until ':' => key
    pos   = pParams.find(':',pos);
    if (pos == std::string::npos) return;
    key_end = pos;
    
    pos++;
    //    1.3 skip white space
    pos   = pParams.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return;
    
    //    1.4 if '"'
    if (pParams[pos] == '"') {
      pos++;
      //        1.4.1 read until '"' => value
      value_start = pos;
      pos         = pParams.find('"',pos);
      if (pos == std::string::npos) return;
      value_end = pos;
      pos++;
    } else {  
      //        1.4.2 read until ' ' or EOF => value
      value_start = pos;
      pos         = pParams.find(' ',pos);
      if (pos == std::string::npos)
        value_end = size; // EOF
      else
        value_end = pos;
    }
    //    1.5 mValues.set(key,value)
    mParameters.set(pParams.substr(key_start, key_end - key_start), pParams.substr(value_start, value_end - value_start));
  }
}

template<>
int Params::get(const char * pKey, int pDefault)
{  
  std::string * value = mParameters.get(std::string(pKey));
  if (value == NULL) {
    return pDefault;
  } else {  
    return atoi(value->c_str());
  }
}

template<>
float Params::get(const char * pKey, float pDefault)
{  
  return (float)get(pKey, (double)pDefault);
}

template<>
double Params::get(const char * pKey, double pDefault)
{  
  std::string * value = mParameters.get(std::string(pKey));
  if (value == NULL) {
    return pDefault;
  } else {  
    return atof(value->c_str());
  }
}