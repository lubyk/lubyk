struct Link
{
  Link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo) :
  mFrom(pFrom), mFromPort(pFromPort), mToPort(pToPort), mTo(pTo) {}
  std::string mFrom;
  std::string mTo;
  unsigned int mFromPort, mToPort;
};