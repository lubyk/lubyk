#include "value_test_helper.h"
#include "matrix_value.h"

#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

class MatrixxTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    Matrixx n(2,3);
    TS_ASSERT_EQUALS( 6, n.size());
    assert_id(1, n);
    // anonymize
    Value v(n);
    assert_id(1, v);
    
    Matrixx n2;
    TS_ASSERT(v.set(&n2));
    assert_id(1, n2);
    TS_ASSERT_EQUALS( 6, n2.size());
  }
  
  void test_size( void )
  {
    Matrixx m;
    TS_ASSERT_EQUALS(0, m.size());
    Matrixx m2(1,1);
    assert_id(1, m2); // first object created
  }
  
  void test_copy( void )
  {
    Matrixx m1, m2;
    set_fixture(m1);
    TS_ASSERT(m2.copy(m1));
    assert_matrix_equal(m2, m1);
  }
  
  //void _copy_at( void )
  //{
  //  Matrix m1, m2;
  //  set_fixture(m1);
  //  m2.set_sizes(1,3);
  //  m2.clear();
  //  
  //  TS_ASSERT_EQUALS(m2.row_count(), 1);
  //  TS_ASSERT(m2.copy_at(1, m1));
  //  TS_ASSERT_EQUALS(m2.row_count(), 3);
  //  TS_ASSERT_EQUALS(m2.col_count(), 3);
  //  
  //  TS_ASSERT_EQUALS(m2[0][0], 0.0);
  //  TS_ASSERT_EQUALS(m2[0][1], 0.0);
  //  TS_ASSERT_EQUALS(m2[0][2], 0.0);
  //  
  //  TS_ASSERT_EQUALS(m2[1][0], m1[0][0]);
  //  TS_ASSERT_EQUALS(m2[1][1], m1[0][1]);
  //  TS_ASSERT_EQUALS(m2[1][2], m1[0][2]);
  //  
  //  TS_ASSERT_EQUALS(m2[2][0], m1[1][0]);
  //  TS_ASSERT_EQUALS(m2[2][1], m1[1][1]);
  //  TS_ASSERT_EQUALS(m2[2][2], m1[1][2]);
  //}
  
  void test_append( void )
  {
    Matrixx m1, m2;
    set_fixture(m1);
    TS_ASSERT(m2.set_sizes(2,3));
    m2.clear();
    
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    
    TS_ASSERT(m2.append(m1));
    TS_ASSERT_EQUALS(m2.row_count(), 4);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.0);
    TS_ASSERT_EQUALS(m2[0][1], 0.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 0.0);
    TS_ASSERT_EQUALS(m2[1][1], 0.0);
    TS_ASSERT_EQUALS(m2[1][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[2][0], m1[0][0]);
    TS_ASSERT_EQUALS(m2[2][1], m1[0][1]);
    TS_ASSERT_EQUALS(m2[2][2], m1[0][2]);
    
    TS_ASSERT_EQUALS(m2[3][0], m1[1][0]);
    TS_ASSERT_EQUALS(m2[3][1], m1[1][1]);
    TS_ASSERT_EQUALS(m2[3][2], m1[1][2]);
  }
  
  
private:
  
  char mBuf[256];
  
  void set_fixture(Matrixx& v)
  {
    MatrixData * m = v.mutable_data();
    
    m->set_sizes(2,3);
    
    /*
    1 2.4 3
    7 8   9.5 */
    m->data[0] = 1; m->data[1] = 2.4; m->data[2] = 3;
    m->data[3] = 7; m->data[4] = 8;   m->data[5] = 9.5;
  }
  
  void _assert_matrix_equal(const char * file, int lineno, const Matrixx& v2, const Matrixx& v1, int p = 10000) // p = precision
  {
    const MatrixData * m1 = v1.data();
    const MatrixData * m2 = v2.data();
    
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("rowCount")), m2->row_count(), m1->row_count());
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("colCount")), m2->col_count(), m1->col_count());
    if (m2->row_count() == m1->row_count() && m2->col_count() == m1->col_count()) {
      for(size_t i=0; i < m1->row_count(); i++) {
        for(size_t j=0; j < m1->col_count(); j++) {
          snprintf(mBuf, 256, "value[%lu][%lu]", i, j);
          _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(mBuf)), round(p * m2->value_at(i,j))/p, round(p * (*m1)[i][j])/p);
        }
      }
    }
  }
};