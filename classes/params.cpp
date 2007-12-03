#include "params.h"
#include <sstream>

void Params::build_hash (const std::string& p)
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


std::ostream& operator<<(std::ostream& pStream, const Params& p)
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
  if(p.mListParams.size() > 0) {
    pStream << "(";
    end = p.mListParams.end();
    for(it = p.mListParams.begin(); it < end; it++) {
      if (it != p.mListParams.begin()) pStream << ", ";
      pStream << *it;
    }
    pStream << ")";
  }
  return pStream;
}

template<>
time_t Params::cast_param(const std::string& value) const
{
  return atoi(value.c_str());
}

template<>
unsigned int Params::cast_param(const std::string& value) const
{
  return atoi(value.c_str());
}

template<>
unsigned char Params::cast_param(const std::string& value) const
{
  return atoi(value.c_str());
}

template<>
int Params::cast_param(const std::string& value) const
{
  return atoi(value.c_str());
}

template<>
double Params::cast_param(const std::string& value) const
{
  return atof(value.c_str());
}

template<>
float Params::cast_param(const std::string& value) const
{
  return atof(value.c_str());
}

template<>
const char * Params::cast_param(const std::string& value) const
{
  return value.c_str();
}

template<>
char Params::cast_param (const std::string& value) const
{
  return value.c_str()[0];
}
