/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "test_helper.h"

#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

class CharMatrixTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    CharMatrix n(2,3);
    TS_ASSERT_EQUALS(n.size(), 6);
    assert_id(n, 1);
    // anonymize
    Value v(n);
    assert_id(v, 1);
    
    CharMatrix n2;
    TS_ASSERT(v.set(n2));
    assert_id(n2, 1);
    TS_ASSERT_EQUALS(n2.size(), 6);
  }
  
  void test_is_a( void )
  {
    CharMatrix e;
    CharMatrix e2(1,1);
    Value v;
    TS_ASSERT(!e.is_char_matrix());
    TS_ASSERT(e2.is_char_matrix());
    TS_ASSERT(!v.is_char_matrix());
    e2.set(v);
    TS_ASSERT(v.is_char_matrix());
  }
  
  void test_size( void )
  {
    CharMatrix m;
    TS_ASSERT_EQUALS(m.size(), 0);
    CharMatrix m2(1,1);
    assert_id(m2, 1); // first object created
  }
  
  void test_copy( void )
  {
    CharMatrix m1, m2;
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
    CharMatrix m1, m2;
    set_fixture(m1);
    TS_ASSERT(m2.set_sizes(2,3));
    m2.clear();
    
    TS_ASSERT_EQUALS(m2.row_count(), 2);
    
    TS_ASSERT(m2.append(m1));
    TS_ASSERT_EQUALS(m2.row_count(), 4);
    TS_ASSERT_EQUALS(m2.col_count(), 3);
    
    TS_ASSERT_EQUALS(m2[0][0], 0);
    TS_ASSERT_EQUALS(m2[0][1], 0);
    TS_ASSERT_EQUALS(m2[0][2], 0);
                                
    TS_ASSERT_EQUALS(m2[1][0], 0);
    TS_ASSERT_EQUALS(m2[1][1], 0);
    TS_ASSERT_EQUALS(m2[1][2], 0);
    
    TS_ASSERT_EQUALS(m2[2][0], m1[0][0]);
    TS_ASSERT_EQUALS(m2[2][1], m1[0][1]);
    TS_ASSERT_EQUALS(m2[2][2], m1[0][2]);
    
    TS_ASSERT_EQUALS(m2[3][0], m1[1][0]);
    TS_ASSERT_EQUALS(m2[3][1], m1[1][1]);
    TS_ASSERT_EQUALS(m2[3][2], m1[1][2]);
  }
  
  
private:
  
  char mBuf[256];
  
  void set_fixture(CharMatrix& v)
  {
    CharMatrixData * m = v.mutable_data();
    
    m->set_sizes(2,3);
    
    /*
    1 2.4 3
    7 8   9.5 */
    m->data[0] = 1; m->data[1] = 24;  m->data[2] = 3;
    m->data[3] = 7; m->data[4] = 8;   m->data[5] = 95;
  }
  
  void _assert_matrix_equal(const char * file, int lineno, const CharMatrix& v2, const CharMatrix& v1, int p = 10000) // p = (not used)
  {
    const CharMatrixData * m1 = v1.data();
    const CharMatrixData * m2 = v2.data();
    
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("rowCount")), m2->row_count(), m1->row_count());
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("colCount")), m2->col_count(), m1->col_count());
    if (m2->row_count() == m1->row_count() && m2->col_count() == m1->col_count()) {
      for(size_t i=0; i < m1->row_count(); i++) {
        for(size_t j=0; j < m1->col_count(); j++) {
          snprintf(mBuf, 256, "value[%lu][%lu]", i, j);
          _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(mBuf)), m2->value_at(i,j), (*m1)[i][j]);
        }
      }
    }
  }
};