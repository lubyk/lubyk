// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "matrix.h"


class MatrixTest : public CxxTest::TestSuite
{
public:
  void test_copy( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    TS_ASSERT(m2.copy(m1));
    assert_equal(m2, m1);
  }
  
  void test_copy_at( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(1,3);
    m2.clear();
    
    TS_ASSERT_EQUALS(m2.row_count(), 1);
    TS_ASSERT(m2.copy_at(1, m1));
    TS_ASSERT_EQUALS(m2.row_count(), 3);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.0);
    TS_ASSERT_EQUALS(m2[0][1], 0.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[1][0], m1[0][0]);
    TS_ASSERT_EQUALS(m2[1][1], m1[0][1]);
    TS_ASSERT_EQUALS(m2[1][2], m1[0][2]);
    
    TS_ASSERT_EQUALS(m2[2][0], m1[1][0]);
    TS_ASSERT_EQUALS(m2[2][1], m1[1][1]);
    TS_ASSERT_EQUALS(m2[2][2], m1[1][2]);
  }
  
  void test_append( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
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
  
  void test_append_partial( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.clear();
    
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT(m2.append(m1, -1));
    TS_ASSERT_EQUALS(m2.row_count(), 3);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.0);
    TS_ASSERT_EQUALS(m2[0][1], 0.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 0.0);
    TS_ASSERT_EQUALS(m2[1][1], 0.0);
    TS_ASSERT_EQUALS(m2[1][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[2][0], m1[1][0]);
    TS_ASSERT_EQUALS(m2[2][1], m1[1][1]);
    TS_ASSERT_EQUALS(m2[2][2], m1[1][2]);
  }
  
  void test_append_doubles( void )
  {
    Matrix m2;
    m2.set_sizes(2,3);
    m2.clear();
    double test_data[3] = {10,20,30};
    
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT(m2.append(test_data, 3));
    TS_ASSERT_EQUALS(m2.row_count(), 3);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.0);
    TS_ASSERT_EQUALS(m2[0][1], 0.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 0.0);
    TS_ASSERT_EQUALS(m2[1][1], 0.0);
    TS_ASSERT_EQUALS(m2[1][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[2][0], 10.0);
    TS_ASSERT_EQUALS(m2[2][1], 20.0);
    TS_ASSERT_EQUALS(m2[2][2], 30.0);
  }
  
  void test_append_more_doubles( void )
  {
    Matrix m2;
    m2.set_sizes(2,3);
    m2.clear();
    double test_data[6] = {10,20,30,40,50,60};
    
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT(m2.append(test_data, 6));
    TS_ASSERT_EQUALS(m2.row_count(), 4);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.0);
    TS_ASSERT_EQUALS(m2[0][1], 0.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 0.0);
    TS_ASSERT_EQUALS(m2[1][1], 0.0);
    TS_ASSERT_EQUALS(m2[1][2], 0.0);
    
    TS_ASSERT_EQUALS(m2[2][0], 10.0);
    TS_ASSERT_EQUALS(m2[2][1], 20.0);
    TS_ASSERT_EQUALS(m2[2][2], 30.0);
    
    TS_ASSERT_EQUALS(m2[3][0], 40.0);
    TS_ASSERT_EQUALS(m2[3][1], 50.0);
    TS_ASSERT_EQUALS(m2[3][2], 60.0);
  }
  
  void test_add( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.fill(1.0);
    
    TS_ASSERT(m2.add(m1));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 2.0);
    TS_ASSERT_EQUALS(m2[0][1], 3.4);
    TS_ASSERT_EQUALS(m2[0][2], 4.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 8.0);
    TS_ASSERT_EQUALS(m2[1][1], 9.0);
    TS_ASSERT_EQUALS(m2[1][2], 10.5);
  }
  
  void test_add_scale( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.fill(1.0);
    
    TS_ASSERT(m2.add(m1, 0, -1, 0.5));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 1.5);
    TS_ASSERT_EQUALS(m2[0][1], 2.2);
    TS_ASSERT_EQUALS(m2[0][2], 2.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 4.5);
    TS_ASSERT_EQUALS(m2[1][1], 5.0);
    TS_ASSERT_EQUALS(m2[1][2], 5.75);
  }
  
  void test_add_scalar( void )
  {
    Matrix m2;
    m2.set_sizes(2,3);
    m2.fill(1.0);
    
    TS_ASSERT(m2 += 0.3);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 1.3);
    TS_ASSERT_EQUALS(m2[0][1], 1.3);
    TS_ASSERT_EQUALS(m2[0][2], 1.3);
    
    TS_ASSERT_EQUALS(m2[1][0], 1.3);
    TS_ASSERT_EQUALS(m2[1][1], 1.3);
    TS_ASSERT_EQUALS(m2[1][2], 1.3);
  }
  
  void test_add_column_vector( void )
  {
    Matrix m1, m2;
    set_fixture(m2);
    m1.set_sizes(2,1);
    m1.data[0] = 10.0;
    m1.data[1] = 20.0;
    
    TS_ASSERT(m2 += m1);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 11.0);
    TS_ASSERT_EQUALS(m2[0][1], 12.4);
    TS_ASSERT_EQUALS(m2[0][2], 13.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 27.0);
    TS_ASSERT_EQUALS(m2[1][1], 28.0);
    TS_ASSERT_EQUALS(m2[1][2], 29.5);
  }
  
  void test_divide( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    set_fixture(m2);
    m1[0][2] = 6.0;
    
    TS_ASSERT(m2.divide(m1));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 1.0);
    TS_ASSERT_EQUALS(m2[0][1], 1.0);
    TS_ASSERT_EQUALS(m2[0][2], 0.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 1.0);
    TS_ASSERT_EQUALS(m2[1][1], 1.0);
    TS_ASSERT_EQUALS(m2[1][2], 1.0);
  }
  
  void test_divide_scale( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    set_fixture(m2);
    m1[0][2] = 6.0;

    TS_ASSERT(m2.divide(m1, 0, -1, 0.5));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);

    TS_ASSERT_EQUALS(m2[0][0], 2.0);
    TS_ASSERT_EQUALS(m2[0][1], 2.0);
    TS_ASSERT_EQUALS(m2[0][2], 1.0);

    TS_ASSERT_EQUALS(m2[1][0], 2.0);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 2.0);
  }
  
  void test_divide_scalar( void )
  {
    Matrix m2;
    set_fixture(m2);
    
    TS_ASSERT(m2 /= 4.0);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.25);
    TS_ASSERT_EQUALS(m2[0][1], 0.6);
    TS_ASSERT_EQUALS(m2[0][2], 0.75);

    TS_ASSERT_EQUALS(m2[1][0], 1.75);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 2.375);
  }
  
  void test_divide_column_vector( void )
  {
    Matrix m1, m2;
    set_fixture(m2);
    m1.set_sizes(2,1);
    m1.data[0] = 2.0;
    m1.data[1] = 4.0;
    
    TS_ASSERT(m2 /= m1);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.5);
    TS_ASSERT_EQUALS(m2[0][1], 1.2);
    TS_ASSERT_EQUALS(m2[0][2], 1.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 1.75);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 2.375);
  }
  
  
  void test_multiply( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.data[0] = 2;  m2.data[1] = 1;   m2.data[2] = 7;
    m2.data[3] = 4;  m2.data[4] = 0.5; m2.data[5] = 2;
    
    TS_ASSERT(m2.multiply(m1));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 2.0);
    TS_ASSERT_EQUALS(m2[0][1], 2.4);
    TS_ASSERT_EQUALS(m2[0][2], 21.0);
    
    TS_ASSERT_EQUALS(m2[1][0], 28.0);
    TS_ASSERT_EQUALS(m2[1][1], 4.0);
    TS_ASSERT_EQUALS(m2[1][2], 19.0);
  }
  
  void test_multiply_scale( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.data[0] = 2;  m2.data[1] = 1;   m2.data[2] = 7;
    m2.data[3] = 4;  m2.data[4] = 0.5; m2.data[5] = 2;

    TS_ASSERT(m2.multiply(m1, 0, -1, 0.5));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);

    TS_ASSERT_EQUALS(m2[0][0], 1.0);
    TS_ASSERT_EQUALS(m2[0][1], 1.2);
    TS_ASSERT_EQUALS(m2[0][2], 10.5);

    TS_ASSERT_EQUALS(m2[1][0], 14.0);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 9.5);
  }
  
  void test_multiply_scalar( void )
  {
    Matrix m2;
    set_fixture(m2);
    
    TS_ASSERT(m2 *= 0.25);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.25);
    TS_ASSERT_EQUALS(m2[0][1], 0.6);
    TS_ASSERT_EQUALS(m2[0][2], 0.75);

    TS_ASSERT_EQUALS(m2[1][0], 1.75);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 2.375);
  }
  
  void test_multiply_column_vector( void )
  {
    Matrix m1, m2;
    set_fixture(m2);
    m1.set_sizes(2,1);
    m1.data[0] = 0.5;
    m1.data[1] = 0.25;
    
    TS_ASSERT(m2 *= m1);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.5);
    TS_ASSERT_EQUALS(m2[0][1], 1.2);
    TS_ASSERT_EQUALS(m2[0][2], 1.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 1.75);
    TS_ASSERT_EQUALS(m2[1][1], 2.0);
    TS_ASSERT_EQUALS(m2[1][2], 2.375);
  }
  
  void test_substract_scalar( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.fill(1.0);
    
    TS_ASSERT(m2 -= 0.3);
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0.7);
    TS_ASSERT_EQUALS(m2[0][1], 0.7);
    TS_ASSERT_EQUALS(m2[0][2], 0.7);

    TS_ASSERT_EQUALS(m2[1][0], 0.7);
    TS_ASSERT_EQUALS(m2[1][1], 0.7);
    TS_ASSERT_EQUALS(m2[1][2], 0.7);
  }
  
  void test_to_file( void )
  {
    Matrix m1, m2;
    set_fixture(m1);
    m2.set_sizes(2,3);
    m2.clear();
    
    FILE * file = fopen("matrix_test_tmp.txt", "wb");
      m1.to_file(file);
    fclose(file);
    
    
    file = fopen("matrix_test_tmp.txt", "rb");
      m2.from_file(file);
    fclose(file);
    
    assert_equal(m2, m1);
    remove("matrix_test_tmp.txt");
  }

  void test_mat_multiply( void )
  {
    Matrix a,b,c;
    a.set_sizes(1,3);
    a[0][0] = 1.0; a[0][1] = 2.0; a[0][2] = 3.0;
    
    b.set_sizes(3,2);
    b[0][0] = 1.0; b[0][1] = 2.0;
    b[1][0] = 0.5; b[1][1] = 2.0;
    b[2][0] = 2.0; b[2][1] = 3.0;
    
    /*                 | 1   2 |
       AB =  [ 1 2 3 ] | 0.5 2 | = [8 15]
                       | 2   3 |
    */
    TS_ASSERT( c.mat_multiply(a, b));

    TS_ASSERT_EQUALS(c.row_count(), 1);
    TS_ASSERT_EQUALS(c.col_count(), 2);
    
    TS_ASSERT_EQUALS(c[0][0], 8.0);
    TS_ASSERT_EQUALS(c[0][1], 15.0);
  }
private:
  void set_fixture(Matrix& m)
  {
    m.set_sizes(2,3);
    
    /*
    1 2.4 3
    7 8   9.5 */
    m.data[0] = 1; m.data[1] = 2.4; m.data[2] = 3;
    m.data[3] = 7; m.data[4] = 8;     m.data[5] = 9.5;
  }
  
  void assert_equal(const Matrix& m2, const Matrix& m1)
  {
    TS_ASSERT_EQUALS(m2.row_count(), m1.row_count());
    TS_ASSERT_EQUALS(m2.col_count(), m1.col_count());
    if (m2.row_count() == m1.row_count() && m2.col_count() == m1.col_count()) {
      for(int i=0; i < m1.row_count(); i++) {
        for(int j=0; j < m1.col_count(); j++) TS_ASSERT_EQUALS( (int)(10000 * m2[i][j]), (int)(10000 * m1[i][j]));
      }
    }
  }
};