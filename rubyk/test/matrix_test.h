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
  
  void test_append_single_value_to_vector( void )
  {
    Matrix m2;
    m2.set_sizes(1,3);
    m2.fill(4.0);
    
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    TS_ASSERT(m2.append(3.5));
    TS_ASSERT_EQUALS(m2.col_count(), 4);
    TS_ASSERT(m2.append(9.0));
    TS_ASSERT_EQUALS(m2.col_count(), 5);
    
    
    TS_ASSERT_EQUALS(m2[0][0], 4.0);
    TS_ASSERT_EQUALS(m2[0][1], 4.0);
    TS_ASSERT_EQUALS(m2[0][2], 4.0);
    TS_ASSERT_EQUALS(m2[0][3], 3.5);
    TS_ASSERT_EQUALS(m2[0][4], 9.0);
  }
  
  void test_append_single_fail_if_not_vector( void )
  {
    Matrix m2;
    m2.set_sizes(2,3);

    TS_ASSERT(!m2.append(3.5));
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
  
  void test_add_AB( void )
  {
    Matrix m, A, B;
    A.set_sizes(2,3);
    B.set_sizes(2,3);
    A.data[0] = 0; A.data[1] = 1; A.data[2] = 2;
    A.data[3] = 2; A.data[4] = 3; A.data[5] = 5;
    
    B.data[0] = 4; B.data[1] = 6; B.data[2] = 5;
    B.data[3] = 2; B.data[4] = 8; B.data[5] = 3;
    
    TS_ASSERT(m.add(A,B,-1.0,0.5));
    TS_ASSERT_EQUALS(m.row_count(), 2);
    TS_ASSERT_EQUALS(m.col_count(), 3);
    
    TS_ASSERT_EQUALS(m[0][0], 2.0);
    TS_ASSERT_EQUALS(m[0][1], 2.0);
    TS_ASSERT_EQUALS(m[0][2], 0.5);
    
    TS_ASSERT_EQUALS(m[1][0], -1.0);
    TS_ASSERT_EQUALS(m[1][1], 1.0);
    TS_ASSERT_EQUALS(m[1][2], -3.5);
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
  
  void test_add_doubles( void )
  {
    Matrix m2;
    double d[6] = {1, 2, 3, 4, 5, 6};
    m2.set_sizes(2,3);
    m2.fill(0.5);
    
    TS_ASSERT(m2.add(d, 6));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 1.5);
    TS_ASSERT_EQUALS(m2[0][1], 2.5);
    TS_ASSERT_EQUALS(m2[0][2], 3.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 4.5);
    TS_ASSERT_EQUALS(m2[1][1], 5.5);
    TS_ASSERT_EQUALS(m2[1][2], 6.5);
  }
  
  void test_add_doubles_vector( void )
  {
    Matrix m2;
    double d[3] = {0.5, 1, 0.5};
    m2.set_sizes(2,3);
    m2.fill(1.0);
    m2.data[3] = 2.5;
    m2.data[4] = 3.5;
    
    TS_ASSERT(m2.add(d, 3));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 1.5);
    TS_ASSERT_EQUALS(m2[0][1], 2.0);
    TS_ASSERT_EQUALS(m2[0][2], 1.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 3.0);
    TS_ASSERT_EQUALS(m2[1][1], 4.5);
    TS_ASSERT_EQUALS(m2[1][2], 1.5);
  }
  
  void test_add_doubles_column( void )
  {
    Matrix m2;
    double d[2] = {10, 20};
    m2.set_sizes(2,3);
    m2.fill(0.5);
    m2.data[2] = 3.5;
    m2.data[4] = 7.0;
    
    TS_ASSERT(m2.add(d, 2));
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 10.5);
    TS_ASSERT_EQUALS(m2[0][1], 10.5);
    TS_ASSERT_EQUALS(m2[0][2], 13.5);
    
    TS_ASSERT_EQUALS(m2[1][0], 20.5);
    TS_ASSERT_EQUALS(m2[1][1], 27.0);
    TS_ASSERT_EQUALS(m2[1][2], 20.5);
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
  
  
  void test_to_file_not_isMatrix( void )
  {
    Matrix m1, m2, m3;
    m1.set_sizes(1,3);
    m1.fill(2);
    m2.set_sizes(3,3);
    m2.fill(5);
    
    FILE * file = fopen("matrix_test_11tmp.txt", "wb");
      m1.to_file(file, false);
      m2.to_file(file, true);
    fclose(file);
    
    
    file = fopen("matrix_test_11tmp.txt", "rb");
      m3.from_file(file);
    fclose(file);
    
    TS_ASSERT_EQUALS(m3.row_count(), 4);
    TS_ASSERT_EQUALS(m3.col_count(), 3);
    
    TS_ASSERT_EQUALS(m3.data[0], 2.0);
    TS_ASSERT_EQUALS(m3.data[1], 2.0);
    TS_ASSERT_EQUALS(m3.data[2], 2.0);
    for(size_t i = 3; i < 12; i++)
      TS_ASSERT_EQUALS(m3.data[i], 5.0);
    
    remove("matrix_test_11tmp.txt");
  }
  
  void test_from_file_integer( void )
  {
    IntMatrix m1, m2;
    m1.set_sizes(1,3);
    m2.set_sizes(1,0); // detect column count
    
    m1.data[0] = 1;
    m1.data[1] = 5;
    m1.data[2] = 7;
    TS_ASSERT(m1.to_file("matrix_test_tmp.txt"));
    
    TS_ASSERT(m2.from_file("matrix_test_tmp.txt"));
    
    assert_equal(m2, m1);
    remove("matrix_test_tmp.txt");
  }
  
  void test_many_from_file_integer( void )
  {
    IntMatrix m1, m2, m3, m4;
    m1.set_sizes(1,3);
    m2.set_sizes(1,0); // detect column count
    
    m3.set_sizes(1,3);
    m4.set_sizes(1,0); // detect column count
    
    m1.data[0] = 1;
    m1.data[1] = 5;
    m1.data[2] = 7;
    
    m3.data[0] = 4;
    m3.data[1] = 2;
    m3.data[2] = 12;
    
    TS_ASSERT(m1.to_file("matrix_test_tmp.txt"));
    TS_ASSERT(m3.to_file("matrix_test_tmp.txt", "ab"));
    
    FILE * file = fopen("matrix_test_tmp.txt", "rb");
      TS_ASSERT(m2.from_file(file));
      TS_ASSERT(m4.from_file(file));
    fclose(file);
    
    assert_equal(m2, m1);
    assert_equal(m4, m3);
    
    remove("matrix_test_tmp.txt");
  }
  
  void test_from_file_readall( void )
  {
    Matrix m1, m2, m3, m4;
    set_fixture(m1);
    m3.identity(7);
    TS_ASSERT_EQUALS(m2.row_count(), 0);
    TS_ASSERT_EQUALS(m2.col_count(), 0);
    TS_ASSERT_EQUALS(m4.row_count(), 0);
    TS_ASSERT_EQUALS(m4.col_count(), 0);
    
    FILE * file = fopen("matrix_test_tmp.txt", "wb");
      m1.to_file(file);
      m3.to_file(file);
    fclose(file);
    
    file = fopen("matrix_test_tmp.txt", "rb");
      m2.from_file(file);
      m4.from_file(file);
    fclose(file);
    
    assert_equal(m2, m1);
    assert_equal(m4, m3);
    
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
  
  void test_mat_multiply_transA( void )
  {
    Matrix a,b,c;
    a.set_sizes(3,1);
    a[0][0] = 1.0; a[1][0] = 2.0; a[2][0] = 3.0;
    
    b.set_sizes(3,2);
    b[0][0] = 1.0; b[0][1] = 2.0;
    b[1][0] = 0.5; b[1][1] = 2.0;
    b[2][0] = 2.0; b[2][1] = 3.0;
    
    /*                  | 1   2 |
       A'B =  [ 1 2 3 ] | 0.5 2 | = [8 15]
                        | 2   3 |
    */
    TS_ASSERT( c.mat_multiply(a, b, CblasTrans));

    TS_ASSERT_EQUALS(c.row_count(), 1);
    TS_ASSERT_EQUALS(c.col_count(), 2);
    
    TS_ASSERT_EQUALS(c[0][0], 8.0);
    TS_ASSERT_EQUALS(c[0][1], 15.0);
  }
  
  void test_mat_multiply_transA_transB( void )
  {
    Matrix a,b,c;
    a.set_sizes(3,1);
    a[0][0] = 1.0; a[1][0] = 2.0; a[2][0] = 3.0;
    
    b.set_sizes(2,3);
    b[0][0] = 1.0; b[1][0] = 2.0;
    b[0][1] = 0.5; b[1][1] = 2.0;
    b[0][2] = 2.0; b[1][2] = 3.0;
    
    /*                   | 1   2 |
       A'B' =  [ 1 2 3 ] | 0.5 2 | = [8 15]
                         | 2   3 |
    */
    TS_ASSERT( c.mat_multiply(a, b, CblasTrans, CblasTrans));

    TS_ASSERT_EQUALS(c.row_count(), 1);
    TS_ASSERT_EQUALS(c.col_count(), 2);
    
    TS_ASSERT_EQUALS(c[0][0], 8.0);
    TS_ASSERT_EQUALS(c[0][1], 15.0);
  }

  void test_symmetric( void )
  {
    Matrix a,b;
    a.set_sizes(2,3);
    a[0][0] = 1.0; a[0][1] = 2.0; a[0][2] = 3.0;
    a[1][0] = 4.0; a[1][1] = 5.0; a[1][2] = 6.0;
    
    TS_ASSERT(b.symmetric(a));
    
    TS_ASSERT_EQUALS(b.row_count(), 3);
    TS_ASSERT_EQUALS(b.col_count(), 3);
    
    TS_ASSERT_EQUALS(b[0][0], 17.0);
    TS_ASSERT_EQUALS(b[0][1], 22.0);
    TS_ASSERT_EQUALS(b[0][2], 27.0);
    
    TS_ASSERT_EQUALS(b[1][0], 22.0);
    TS_ASSERT_EQUALS(b[1][1], 29.0);
    TS_ASSERT_EQUALS(b[1][2], 36.0);
    
    TS_ASSERT_EQUALS(b[2][0], 27.0);
    TS_ASSERT_EQUALS(b[2][1], 36.0);
    TS_ASSERT_EQUALS(b[2][2], 45.0);
  }
  
  void test_eigenvectors( void )
  {
    Matrix a,b,c;
    a.set_sizes(3,3);
    a[0][0] = 1.0; a[0][1] = 2.0; a[0][2] = 3.0;
    a[1][0] = 2.0; a[1][1] = 4.0; a[1][2] = 2.0;
    a[2][0] = 3.0; a[2][1] = 2.0; a[2][2] = 1.0;
    
    TS_ASSERT(c.eigenvectors(b, a));

    TS_ASSERT_EQUALS(c.row_count(), 3);
    TS_ASSERT_EQUALS(c.col_count(), 3);
    TS_ASSERT_EQUALS(b.row_count(), 3);
    
    // eigenvalues
    TS_ASSERT_EQUALS(round(b[0][0] * 100000)/100000, -2.0);
    TS_ASSERT_EQUALS(round(b[1][0] * 100000)/100000, 1.17157);
    TS_ASSERT_EQUALS(round(b[2][0] * 100000)/100000, 6.82843);
    
    // eigenvectors
    if (c[0][0] < 0) {
      TS_ASSERT_EQUALS(round(c[0][0] * 100)/100, -0.71);
      TS_ASSERT_EQUALS(round(c[0][1] * 100)/100, 0.00);
      TS_ASSERT_EQUALS(round(c[0][2] * 100)/100, 0.71);
    } else {
      TS_ASSERT_EQUALS(round(c[0][0] * 100)/100, 0.71);
      TS_ASSERT_EQUALS(round(c[0][1] * 100)/100, 0.00);
      TS_ASSERT_EQUALS(round(c[0][2] * 100)/100, -0.71);
    }
    
    TS_ASSERT_EQUALS(round(c[1][0] * 100)/100, 0.50);
    TS_ASSERT_EQUALS(round(c[1][1] * 100)/100, -0.71);
    TS_ASSERT_EQUALS(round(c[1][2] * 100)/100, 0.50);
    
    TS_ASSERT_EQUALS(round(c[2][0] * 100)/100, -0.50);
    TS_ASSERT_EQUALS(round(c[2][1] * 100)/100, -0.71);
    TS_ASSERT_EQUALS(round(c[2][2] * 100)/100, -0.50);
    
  }
  
  void test_inverse( void )
  {
    Matrix a, b;
    a.set_sizes(3,3);
    b.set_sizes(3,3);
    a[0][0] =  1.0; a[0][1] =  1.0; a[0][2] =  3.0;
    a[1][0] =  2.0; a[1][1] =  4.0; a[1][2] =  2.0;
    a[2][0] =  1.0; a[2][1] =  3.0; a[2][2] =  1.0;
    
    b[0][0] = -0.5; b[0][1] =  2.0; b[0][2] = -2.5;
    b[1][0] =  0.0; b[1][1] = -0.5; b[1][2] =  1.0;
    b[2][0] =  0.5; b[2][1] = -0.5; b[2][2] =  0.5;
    
    TS_ASSERT(a.inverse());

    assert_equal(a,b);
  }
  
  void test_inverse_tiny_det( void )
  {
    Matrix a, b;
    a.set_sizes(2,2);
    b.set_sizes(2,2);
    a[0][0] =  1.28; a[0][1] =  1.599999999;
    a[1][0] =  1.6;  a[1][1] =  2.0;
    
    // abs(det) = abs(1.6e-9) > 1e-13 (MATRIX_MIN_DET)
    TS_ASSERT(a.inverse());
    
    a[0][0] =  -1.28; a[0][1] =  -1.5999999999;
    a[1][0] =  1.6;  a[1][1] =  2.0;
    
    // abs(det) = abs(-1.6e-12) > 1e-13 (MATRIX_MIN_DET)
    TS_ASSERT(a.inverse());
    
    a[0][0] =  1.28; a[0][1] =  1.599999999999;
    a[1][0] =  1.6;  a[1][1] =  2.0;
    
    // abs(det) = abs(-1.6e-14) < 1e-13 (MATRIX_MIN_DET)
    TS_ASSERT(!a.inverse());
  }
  
  void test_inverse_null_det( void )
  {
    Matrix a;
    a.set_sizes(2,2);
    a[0][0] =  1.28; a[0][1] =  1.6;
    a[1][0] =  1.6;  a[1][1] =  2.0;
    
    TS_ASSERT(!a.inverse());
  }
  
  void test_identity( void )
  {
    Matrix a, b;
    b.set_sizes(3,3);
    
    b[0][0] =  1.0; b[0][1] =  0.0; b[0][2] =  0.0;
    b[1][0] =  0.0; b[1][1] =  1.0; b[1][2] =  0.0;
    b[2][0] =  0.0; b[2][1] =  0.0; b[2][2] =  1.0;
    
    TS_ASSERT(a.identity(3));

    assert_equal(a,b);
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
  
  template<typename T>
  void assert_equal(const TMatrix<T>& m2, const TMatrix<T>& m1, int p = 10000) // p = precision
  {
    TS_ASSERT_EQUALS(m2.row_count(), m1.row_count());
    TS_ASSERT_EQUALS(m2.col_count(), m1.col_count());
    if (m2.row_count() == m1.row_count() && m2.col_count() == m1.col_count()) {
      for(size_t i=0; i < m1.row_count(); i++) {
        for(size_t j=0; j < m1.col_count(); j++) TS_ASSERT_EQUALS( round(p * m2[i][j])/p, round(p * m1[i][j])/p);
      }
    }
  }
};