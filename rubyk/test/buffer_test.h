// loop buffer test
// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "buffer.h"

// FIXME: complete tests !
class BufferTest : public CxxTest::TestSuite
{
public:
  void test_advance( void )
  {
    Buffer b;
    b.set_sizes(2, 2);
    real_t * vector;
    
    for(size_t i = 0; i < 100; i += 2){
      vector = b.advance();
      vector[0] = i+1;
      vector[1] = i+2;
      assert_correct_values(b, i);
    }
  }

private:
  
  void assert_correct_values(const Buffer& b, size_t counter)
  {
    const CutMatrix mat = b.matrix();
    TS_ASSERT_EQUALS(mat.size(), 2 * 2);
    if (mat.size() == 2 * 2) {
      for(size_t i=0; i < mat.row_count(); i++) {
        for(size_t j=0; j < mat.col_count(); j++) {
          int val = counter - 3 + i * mat.row_count() + j;
          if (val < 0) val = 0;
          
          TS_ASSERT_EQUALS( (int)(mat[i][j]), val );
        }
      }
    }
  }
};