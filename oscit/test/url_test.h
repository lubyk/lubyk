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
#include "oscit/url.h"

class UrlTest : public TestHelper
{
public:
  void test_create( void ) {
    Url url("http://example.com:7010/one/two");
    assert_equal("http", url.protocol());
    assert_true(  url.has_hostname() );
    assert_false( url.has_service_name() );
    assert_equal("example.com", url.hostname());
    assert_equal(7010, url.port());
    assert_equal("/one/two", url.path());
  }

  void test_empty( void ) {
    Url url;
    assert_equal("", url.str());
  }

  void test_create_copy( void ) {
    Url url("http://example.com:7010/one/two");
    Url copy(url);
    url.set("oscit://'funky thing'/a/b/c");
    assert_equal("http", copy.protocol());
    assert_true(  copy.has_hostname() );
    assert_false( copy.has_service_name() );
    assert_equal("example.com", copy.hostname());
    assert_equal(7010, copy.port());
    assert_equal("/one/two", copy.path());
  }

  void test_create_using_service_name_squote( void ) {
    Url url("oscit://'stage camera'/filter/contrast");
    assert_equal("oscit", url.protocol());
    assert_false( url.has_hostname() );
    assert_true(  url.has_service_name() );
    assert_equal("stage camera", url.service_name());
    assert_equal(Location::NO_PORT, url.port());
    assert_equal("/filter/contrast", url.path());
  }

  void test_create_using_service_name_dquote( void ) {
    Url url("oscit://\"my new stage camera\"/filter/contrast");
    assert_equal("oscit", url.protocol());
    assert_false( url.has_hostname() );
    assert_true(  url.has_service_name() );
    assert_equal("my new stage camera", url.service_name());
    assert_equal(Location::NO_PORT, url.port());
    assert_equal(Location::NO_IP, url.ip());
    assert_equal("/filter/contrast", url.path());
  }

  void test_create_using_service_name_without_protocol( void ) {
    Url url("'my new stage camera'/filter/contrast");
    assert_equal("oscit", url.protocol());
    assert_false( url.has_hostname() );
    assert_true(  url.has_service_name() );
    assert_equal("my new stage camera", url.service_name());
    assert_equal(Location::NO_PORT, url.port());
    assert_equal("/filter/contrast", url.path());
  }

  void test_create_using_host_without_dot_and_port( void ) {
    Url url("oscit://localhost:80/filter/contrast");
    assert_equal("oscit", url.protocol());
    assert_true( url.has_hostname() );
    assert_false(url.has_service_name() );
    assert_equal("localhost", url.hostname());
    assert_equal(80, url.port());
    assert_equal("/filter/contrast", url.path());
  }

  void test_create_local( void ) {
    Url url("/one/two");
    assert_equal("", url.protocol());
    assert_equal("", url.hostname());
    assert_equal(Location::NO_PORT, url.port());
    assert_equal("/one/two", url.path());
  }

  void test_create_relative( void ) {
    Url url("one/two");
    assert_equal("", url.protocol());
    assert_equal("", url.hostname());
    assert_equal(Location::NO_PORT, url.port());
    assert_equal("one/two", url.path());
  }

  void test_create_bad_url( void ) {
    Url url("one/two://foo/bar");
    assert_equal("", url.path());
  }

  void test_create_bad_path( void ) {
    Url url("http://www.example.com:80 /foo/bar");
    assert_equal("", url.path());
  }

  void test_name( void ) {
    Url url("http://www.example.com/ bad/url");
    assert_equal("", url.name());
    assert_equal("boy",   url.set("http://www.example.com:80/good/boy").name());
    assert_equal("foo",   url.set("foo").name());
    assert_equal("buzz",  url.set("/buzz").name());
    assert_equal("buzz_waga",  url.set("/buzz_waga").name());
    assert_equal("split", url.set("/banana/split").name());
  }

  void test_get_parent_url( void ) {
    Url url("http://'some place'/one/two/three");

    assert_true(url.get_parent_url(&url));
    assert_equal("http://\"some place\"/one/two", url.str());

    assert_true(url.get_parent_url(&url));
    assert_equal("http://\"some place\"/one", url.str());

    assert_true(url.get_parent_url(&url));
    assert_equal("http://\"some place\"", url.str());

    assert_false(url.get_parent_url(&url));
    assert_equal("http://\"some place\"", url.str());
  }

  void test_to_stream( void ) {
    Url url("oscit://'my place'/foo/bar/baz");
    std::ostringstream out(std::ostringstream::out);
    out << url;
    assert_equal("oscit://\"my place\"/foo/bar/baz", out.str());
  }

  void test_create_using_ip_port_path( void ) {
    Url url((10<<24)+4, 1432, "/this/is/a/path");
    assert_equal("oscit", url.protocol());
    assert_true( url.has_hostname() );
    assert_false(url.has_service_name() );
    assert_equal("10.0.0.4", url.hostname());
    assert_equal((10<<24)+4, url.ip());
    assert_equal(1432, url.port());
    assert_equal("/this/is/a/path", url.path());
  }

  void test_is_meta( void ) {
    Url meta_url("oscit://\"foobar\"/.list");
    Url url("oscit://\"foobar\"/list");
    assert_true(meta_url.is_meta());
    assert_false(url.is_meta());
  }

  void test_same( void ) {
    Url url1("oscit://\"foobar\"/a/b");
    Url url2("http://\"foobar\"/a/b");
    assert_false(url1 == url2);

    url2.set("oscit://\"foobar\"/a/b/c");
    assert_false(url1 == url2);

    url2.set("oscit://\"foobar\"/a/c");
    assert_false(url1 == url2);

    url2.set("oscit://\"bozon\"/a/b");
    assert_false(url1 == url2);

    url2.set("oscit://\"foobar\"/a/b");
    assert_true(url1 == url2);
  }
};
