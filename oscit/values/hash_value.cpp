#include "values.h"
#include <sstream>
#include <iostream>

const Value Hash::operator[] (const std::string& pKey) const
{
  if (!ptr_) return gNilValue;
  Value res;
  if (!data_pointer()->mHash.get(&res, pKey)) return gNilValue;
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
