#include "values.h"

std::ostream& operator<< (std::ostream& pStream, const Value& val)
{
  if (val.mPtr && val.mPtr->mDataPtr) {
    val.mPtr->mDataPtr->to_stream(pStream);
  } else {
    pStream << "Nil";
  }
  return pStream;
}


/** Display number inside stream. */
template<>
void MatrixData::to_stream(std::ostream& pStream) const
{
  char buffer[20];
  if (size() == 0) {
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "] 0>";
#else
    pStream << "<" << type_name() << " 0>";
#endif
  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    snprintf(buffer, 20, "<%s[%lu] [ % .2f", type_name(), mId, data[start]);
#else
    snprintf(buffer, 20, "<%s [ % .2f", type_name(), data[start]);
#endif
    pStream << buffer;
    for (size_t i= start+1; i < start+sz; i++) {
      snprintf(buffer, 20, " % .2f", data[i]);
      pStream << buffer;
    }
    pStream << " ], " << mRowCount << "x" << mColCount << ">";
  }
}

/** Display number inside stream. */
template<>
void CharMatrixData::to_stream(std::ostream& pStream) const
{
  if (size() == 0) {
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "] 0>";
#else
    pStream << "<" << type_name() << " 0>";
#endif
  } else {
    size_t sz = 16;
    size_t start;
    if (sz > size()) sz = size();
    start = size() - sz;
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "]" << " [ " << data[start];
#else
    pStream << "<" << type_name() << "[" << mId << "]" << " [ " << data[start];
#endif
    for (size_t i= start+1; i < start+sz; i++) {
      pStream << " " << data[i];
    }
    pStream << " ], " << mRowCount << "x" << mColCount << ">";
  }
}