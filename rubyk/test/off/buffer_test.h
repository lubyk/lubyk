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
    Real * vector;
    
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