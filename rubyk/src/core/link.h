#include <string>

struct Link
{
  Link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo) :
  mFrom(pFrom), mTo(pTo), mFromPort(pFromPort), mToPort(pToPort) {}
  std::string mFrom;
  std::string mTo;
  unsigned int mFromPort, mToPort;
};