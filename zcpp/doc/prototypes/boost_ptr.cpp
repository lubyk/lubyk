/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include <cxxtest/TestSuite.h>
#include "test_helper.h"

#include "boost/smart_ptr.hpp"

// BT is for BoostTest in case you wonder...
class DummyBT {
public:
  DummyBT(const char *id, std::ostringstream *oss) : id_(id), oss_(oss) {}

  ~DummyBT() {
    *oss_ << "[" << id_ << ": deleted]";
  }

  void append(const char *text) {
    *oss_ << "[" << id_ << ": " << text << "]";
  }

  const char *id_;
  std::ostringstream *oss_;
};

typedef boost::shared_ptr<DummyBT> DummyBTHandle;
typedef boost::weak_ptr<DummyBT> DummyBTPtr;

/** The goal of this test is to make sure boost compiles and behaves as expected (and we
 * understand how to use it).
 */
class BoostTest : public TestHelper {
public:

  void test_shared_ptr( void ) {
    std::ostringstream oss;
    {
      std::list<DummyBTHandle> list;
      {
        // Create a shared pointer
        // ref count = 1
        DummyBTHandle ptr(new DummyBT("a", &oss));

        assert_equal(1, ptr.use_count());

        // Insert in a list twice
        list.push_back(ptr);
        list.push_back(ptr);

        assert_equal(3, ptr.use_count());

        // Remove from list once
        list.pop_front();

        assert_equal(2, ptr.use_count());
      } // ptr scope

      // ptr out of scope: removed once
      assert_equal(1, list.front().use_count());
    } // list scope

    // list deleted, should be removed from list ==> delete
    assert_equal("[a: deleted]", oss.str());
  }

  void test_weak_ptr( void ) {
    std::ostringstream oss;
    DummyBTPtr weak;
    {
      // ref count = 1
      DummyBTHandle ptr(new DummyBT("a", &oss));

      assert_equal(1, ptr.use_count());

      weak = ptr;

      assert_equal(1, ptr.use_count());

      if (DummyBTHandle obj = weak.lock()) {
        // should be ok.
        assert_equal(2, ptr.use_count());
        obj->append("weak");
      } else {
        // should never execute here
        assert_true(false);
      }

      assert_equal(1, ptr.use_count());
    }

    if (DummyBTHandle obj = weak.lock()) {
      // should never execute here
      assert_true(false);
    }

    // out of scope for ptr and list: DummyBT should be dead
    assert_equal("[a: weak][a: deleted]", oss.str());
  }

};

