/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
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
#include "oscit/thash.h"


class StringTHashTest : public TestHelper
{
public:
  void test_set_get( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    assert_true(hash.get("hello", &res));
    assert_equal("world", res);
    assert_true(hash.get("my", &res));
    assert_equal("mum", res);
    assert_false(hash.get("money", &res));
  }

  void test_set_get_empty( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string(""), std::string("empty"));
    hash.set(std::string("other"),    std::string("string"));

    assert_true(hash.get("", &res));
    assert_equal("empty", res);
    assert_true(hash.get("other", &res));
    assert_equal("string", res);
  }

  void test_get_key( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    assert_true(hash.get_key("world", &res));
    assert_equal("hello", res);
    assert_true(hash.get_key("mum", &res));
    assert_equal("my", res);
    assert_false(hash.get_key("money", &res));
  }

  void test_has_key( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    assert_true(hash.has_key("hello"));
    assert_true(hash.has_key("my"));
    assert_false(hash.has_key("bad key"));
    assert_false(hash.has_key("world"));
  }

  void test_keys( void ) {
    THash<std::string, std::string> hash(10);
    const std::list<std::string> * keys = &hash.keys();

    hash.set(std::string("hello"), std::string("world"));
    assert_equal(1, keys->size());
    assert_equal("hello", keys->front());

    hash.set(std::string("my"),    std::string("mum"));
    assert_equal(2, keys->size());
    assert_equal("hello", keys->front());
    assert_equal("my", keys->back());

    hash.remove(std::string("bob")); // does nothing
    assert_equal("hello", keys->front());
    assert_equal("my", keys->back());

    hash.remove(std::string("hello"));
    assert_equal(1, keys->size());
    assert_equal("my", keys->front());
  }

  void test_clear( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string("a"), std::string("world"));
    hash.set(std::string("b"), std::string("mum"));



    assert_equal(2, hash.keys().size());

    assert_true(hash.get("a", &res));
    assert_equal( res,  std::string("world") );
    assert_true(hash.get("b", &res));
    assert_equal( res,  std::string("mum"  ) );

    hash.clear();
    assert_equal(0, hash.keys().size());
    assert_false(hash.get("a", &res));
    assert_false(hash.get("b", &res));
  }

  void test_clear_int( void ) {
    THash<std::string, int> hash(10);
    int res;

    hash.set(std::string("a"), 1);
    hash.set(std::string("b"), 2);
    hash.set(std::string("c"), 3);


    assert_equal(3, hash.size());

    assert_true(hash.get("a", &res));
    assert_equal(1, res);
    assert_true(hash.get("b", &res));
    assert_equal(2, res);
    assert_true(hash.get("c", &res));
    assert_equal(3, res);

    hash.clear();
    assert_equal(0,  hash.size());
    assert_false(hash.get("a", &res));
    assert_false(hash.get("b", &res));
    assert_false(hash.get("c", &res));
  }

  void test_H( void ) {
    assert_false(H("a") == H("b"));
    assert_false(H("one") == H("two"));
    assert_false(H("longname_a") == H("longname_b"));
    assert_false(H("longname_a") == H("longname_c"));
    // this is the upper limit for H() macro
    assert_true(H("longnamelong_a") == H("longnamelong_c"));
    // no upper limit for hashId() function
    assert_false(hashId("longnamelong_a") == hashId("longnamelong_c"));
  }

  void test_N_type_id( void ) {
    assert_false(H("N") == H("f"));
    assert_false(H("N") == H("s"));
    assert_false(H("N") == H("H"));
    assert_false(H("N") == H("M"));
    assert_false(H("N") == H("m"));
    assert_false(H("N") == H("*"));
  }

  void test_f_type_id( void ) {
    assert_false(H("f") == H("N"));
    assert_false(H("f") == H("s"));
    assert_false(H("f") == H("H"));
    assert_false(H("f") == H("M"));
    assert_false(H("f") == H("m"));
    assert_false(H("f") == H("*"));
  }

  void test_s_type_id( void ) {
    assert_false(H("s") == H("N"));
    assert_false(H("s") == H("f"));
    assert_false(H("s") == H("H"));
    assert_false(H("s") == H("M"));
    assert_false(H("s") == H("m"));
    assert_false(H("s") == H("*"));
  }

  void test_H_type_id( void ) {
    assert_false(H("H") == H("N"));
    assert_false(H("H") == H("f"));
    assert_false(H("H") == H("s"));
    assert_false(H("H") == H("M"));
    assert_false(H("H") == H("m"));
    assert_false(H("H") == H("*"));
  }

  void test_M_type_id( void ) {
    assert_false(H("M") == H("N"));
    assert_false(H("M") == H("f"));
    assert_false(H("M") == H("s"));
    assert_false(H("M") == H("H"));
    assert_false(H("M") == H("m"));
    assert_false(H("M") == H("*"));
  }

  void test_m_type_id( void ) {
    assert_false(H("m") == H("N"));
    assert_false(H("m") == H("f"));
    assert_false(H("m") == H("s"));
    assert_false(H("m") == H("H"));
    assert_false(H("m") == H("M"));
    assert_false(H("m") == H("*"));
  }

  void test_any_type_id( void ) {
    assert_false(H("*") == H("N"));
    assert_false(H("*") == H("f"));
    assert_false(H("*") == H("s"));
    assert_false(H("*") == H("H"));
    assert_false(H("*") == H("m"));
    assert_false(H("*") == H("M"));
  }

  void test_copy( void ) {
    THash<std::string, std::string> hash(10);
    THash<std::string, std::string> hash2(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    // copy
    hash2 = hash;

    // change hash
    hash.set(std::string("hello"), std::string("planet"));

    assert_true(hash.get("hello", &res));
    assert_equal("planet", res);
    assert_true(hash.get("my", &res));
    assert_equal("mum", res);

    assert_true(hash2.get("hello", &res));
    assert_equal("world", res);
    assert_true(hash2.get("my", &res));
    assert_equal("mum", res);
  }

  void test_set_get_pointer( void ) {
    THash<std::string, std::string> hash(10);
    const std::string *res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    assert_true(hash.get("hello", &res));
    assert_equal("world", *res);
    assert_true(hash.get("my", &res));
    assert_equal("mum", *res);
    assert_false(hash.get("money", &res));
  }


  void test_equal( void ) {
    THash<std::string, int> a(10);
    THash<std::string, int> b(10);
    assert_equal(a, b);
    a.set(std::string("one"), 1);
    assert_false(a == b);
    b.set(std::string("one"), 1);
    assert_equal(a, b);
    a.set(std::string("two"), 2);
    assert_false(a == b);
    b.set(std::string("two"), 2);
    assert_equal(a, b);
    a.set(std::string("two"), 4);
    assert_false(a == b);
  }
};