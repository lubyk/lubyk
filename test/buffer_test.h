// loop buffer test
// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "buffer.h"


class BufferTest : public CxxTest::TestSuite
{
public:
  void test_advance( void )
  {
    Buffer b;
    b.set_sizes(2, 2);
    double v[14] = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double * vector = b.advance();
    vector[0] = 1;
    vector[1] = 2;
    assert_equal(b, v, 4);
  }

private:
  
  void assert_equal(const Buffer& b, const double * data, size_t dataSize)
  {
    const CutMatrix mat = b.matrix();
    TS_ASSERT_EQUALS(mat.size(), dataSize);
    if (mat.size() == dataSize) {
      for(size_t i=0; i < mat.row_count(); i++) {
        for(size_t j=0; j < mat.col_count(); j++) TS_ASSERT_EQUALS( (int)(10000 * mat[i][j]), (int)(10000 * data[i * mat.row_count() + j]));
      }
    }
  }
};