#include "values.h"
#include <sstream>
#include <iostream>

size_t HashData::build_hash (const std::string& p)
{ 
  std::string key;
  Value value;
  unsigned int size = p.size();
  unsigned int pos,key_start,key_end;
  pos = 0;
  
  // 1. loop
  while(pos < size)
  {  
    //    1.1 skip whitespace
    pos = p.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return size;
    
    if (p[pos] == '}') {
      // end of hash
      return pos + 1;
    }
    key_start = pos;
    
    //    1.2 read until ':' => key
    pos   = p.find(':',pos);
    if (pos == std::string::npos) return size;
    key_end = pos;
    
    pos++;
    //    1.3 skip white space
    pos   = p.find_first_not_of(' ',pos);
    if (pos == std::string::npos) return size;
    
    // get value
    pos += value.from_string(p.substr(pos, size - pos));
    
    // set dictionary
    mParameters.set(p.substr(key_start, key_end - key_start), value);
  }  
  return size; // reached end of string
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
