#ifndef _CALL_H_
#define _CALL_H_

#include <string>

struct Call
{
  Call(const std::string& pUrl, const Value& param) : mUrl(pUrl), mParam(param) {}
  std::string mUrl;
  Value mParam;
};

#endif // _CALL_H_