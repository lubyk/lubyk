#include "values.h"
#include <sstream>
#include <iostream>

void HashData::build_hash (const std::string& p)
{ 
  std::string key;
  Value value;
  unsigned int size = p.size();
  unsigned int pos,key_start,key_end,value_start,value_end;
  pos = 0;
  // 1. loop
  while(pos < size)
  {  
    //    1.1 skip whitespace
    pos = p.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return;
    key_start = pos;
    
    //    1.2 read until ':' => key
    pos   = p.find(':',pos);
    if (pos == std::string::npos) return;
    key_end = pos;
    
    pos++;
    //    1.3 skip white space
    pos   = p.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return;
    
    value_start = pos;

    //    1.4 switch type
    switch (p[pos]) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        // Number
        pos = p.find(' ',pos);
        value_end = (pos == std::string::npos) ? size : pos;
        Number(p.substr(value_start, value_end - value_start)).set(value);
        break;
      case '"':
        // String
        pos++;
        value_start = pos;
        pos         = p.find('"',pos);
        if (pos == std::string::npos) return; // bad format, abort
        value_end = pos;
        String(p.substr(value_start, value_end - value_start)).set(value);
        pos++;
        break;
        // FIXME: matrix, error, nil, bang, etc
      default:
        return; // abort, error
    }
    //    1.5 mValues.set(key,value)
    mParameters.set(p.substr(key_start, key_end - key_start), value);
  }
}

const Value Hash::operator[] (const std::string& pKey) const
{
  if (!mPtr) return gNilValue;
  Value res;
  if (!data_pointer()->mParameters.get(&res, pKey)) return gNilValue;
  return res;
}

template<>
bool Hash::cast_param (const std::string& value)
{
  return (value == "true" || value == "yes" || value == "y" || atoi(value.c_str()) != 0);
}

template<>
int Hash::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}
template<>
unsigned int Hash::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
unsigned char Hash::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
time_t Hash::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
size_t Hash::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
real_t Hash::cast_param(const std::string& value)
{
  return atof(value.c_str());
}

template<>
float Hash::cast_param(const std::string& value)
{
  return atof(value.c_str());
}

template<>
const char * Hash::cast_param(const std::string& value)
{
  return value.c_str();
}

template<>
char Hash::cast_param (const std::string& value)
{
  return value.c_str()[0];
}
