#include "values.h"
#include <sstream>
#include <iostream>

void DictionaryData::build_hash (const std::string& p)
{ 
  std::string key, value;
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
    
    //    1.4 if '"'
    if (p[pos] == '"') {
      pos++;
      //        1.4.1 read until '"' => value
      value_start = pos;
      pos         = p.find('"',pos);
      if (pos == std::string::npos) return;
      value_end = pos;
      pos++;
    } else {  
      //        1.4.2 read until ' ' or EOF => value
      value_start = pos;
      pos         = p.find(' ',pos);
      if (pos == std::string::npos)
        value_end = size; // EOF
      else
        value_end = pos;
    }
    //    1.5 mValues.set(key,value)
    mParameters.set(p.substr(key_start, key_end - key_start), p.substr(value_start, value_end - value_start));
  }
}

const Value Dictionary::operator[] (const std::string& pKey) const
{
  if (!mPtr) return gNilValue;
  std::string str;
  if (!data_pointer()->mParameters.get(&str, pKey)) return gNilValue;
  // FIXME: this is not very robust ! (store "\"blah\"" for strings instead of "blah").
  switch (str.at(0)) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return Number(cast_param<real_t>(str));
    default:
      return String(str);
  }
  // Matrix ?
}

template<>
bool Dictionary::cast_param (const std::string& value)
{
  return (value == "true" || value == "yes" || value == "y" || atoi(value.c_str()) != 0);
}

template<>
int Dictionary::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}
template<>
unsigned int Dictionary::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
unsigned char Dictionary::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
time_t Dictionary::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
size_t Dictionary::cast_param(const std::string& value)
{
  return atoi(value.c_str());
}

template<>
real_t Dictionary::cast_param(const std::string& value)
{
  return atof(value.c_str());
}

template<>
float Dictionary::cast_param(const std::string& value)
{
  return atof(value.c_str());
}

template<>
const char * Dictionary::cast_param(const std::string& value)
{
  return value.c_str();
}

template<>
char Dictionary::cast_param (const std::string& value)
{
  return value.c_str()[0];
}

std::ostream& operator<<(std::ostream& pStream, const DictionaryData& p)
{
  std::string str;
  std::vector<std::string>::const_iterator it;
  std::vector<std::string>::const_iterator end = p.mParameters.end();
  for(it = p.mParameters.begin(); it < end; it++) {
    if (it != p.mParameters.begin()) pStream << " ";
    pStream << *it << ":";
    if (p.mParameters.get(&str, *it)) {
      if (str.find_first_of(" :\"") != std::string::npos) {
        std::string value = "\"";
        value.append(str);
        value.append("\"");
        // TODO: escape "
        pStream << value;
      } else
        pStream << str;
      
    } else
      pStream << "(null)";
  }
  // list anonymous parameters
  if(p.mListValues.size() > 0) {
    pStream << "(";
    end = p.mListValues.end();
    for(it = p.mListValues.begin(); it < end; it++) {
      if (it != p.mListValues.begin()) pStream << ", ";
      pStream << *it;
    }
    pStream << ")";
  }
  return pStream;
}
