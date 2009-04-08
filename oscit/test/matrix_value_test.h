#include "test_helper.h"
#include "oscit/values.h"

class MatrixValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(TypeTag("M"));
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_true (v.is_matrix());
    
    Matrix * m = v.matrix();
    assert_equal(0, m->rows());
    assert_equal(0, m->cols());
    assert_equal(0, m->type());
    
    assert_equal("M", v.type_tag());
  }
  
  void test_create_matrix_value( void ) {
    MatrixValue v(3,2);
    MatrixValue v2;
    
    assert_true(v.is_matrix());
    assert_true(v2.is_matrix());
    
    
    assert_equal(6, v.mat_size());
    assert_equal(0, v2.mat_size());
  }
  
  void test_create_with_char( void ) {
    Value v('M');
    
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("M"));
    
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }
  
  void test_copy( void )
  {
    MatrixValue v(2,3);
#if Real == double
    assert_true(v.mat_type() == CV_64FC1);
#else
    assert_true(v.mat_type() == CV_32FC1);
#endif
    Real * mat = v.mat_data();
    
    mat[0] = 0.0; mat[1] = 1.0; mat[2] = 2.0;
    mat[3] = 3.0; mat[4] = 4.0; mat[5] = 5.0;
    
    Value v2(v);
    Value v3;
    
    assert_true(v2.is_matrix());
    assert_true(v3.is_nil());
    
    assert_equal(3.0, v2.mat_data()[3]);
    assert_equal(6, v.mat_size());
    assert_equal(6, v2.mat_size());
    
    mat[0] = 3.5;
    
    // shared data
    assert_equal(3.5,  v.mat_data()[3]);
    assert_equal(3.5, v2.mat_data()[3]);
    
    v3 = v;
    
    assert_true(v3.is_matrix());
    assert_equal(6, v3.mat_size());
    assert_equal(3.5, v3.mat_data()[3]);
    
    mat[0] = 3.8;
    
    assert_equal(3.8,  v.mat_data()[3]);
    assert_equal(3.8, v2.mat_data()[3]);
    assert_equal(3.8, v3.mat_data()[3]);
  }
  
  void test_set( void )
  {
    Value v;
    Matrix m(2,2);
    assert_true(v.is_nil());
    v.set(m);
    assert_true(v.is_matrix());
    assert_equal(4, v.mat_size());
  }
  
  void test_set_tag( void )
  {
    Value v;
    
    v.set_type_tag("M");
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }
  
  void test_set_type( void )
  {
    Value v;
    
    v.set_type(MATRIX_VALUE);
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }
  
  void test_stream( void ) {
    MatrixValue v(2,3);
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("...", os.str());
  }
};