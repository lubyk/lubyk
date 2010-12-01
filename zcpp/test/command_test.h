/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#include "test_helper.h"

class CreateCommandTest : public TestHelper {
public:
  void test_create( void )
  {
    Planet planet;
    std::istringstream  input(std::istringstream::in);   // allow input operations
    std::ostringstream output(std::ostringstream::out);  // allow output  operations
    planet.adopt_command(new TextCommand(input, output));

    microsleep(10);

    assert_equal(PLANET_WELCOME, output.str());
  }
};

class ParseCommandTest : public ParseHelper {
public:

  void test_create_command_should_work( void ) {
    assert_result("# <Value:/v1 value:1.25>\n", "v1=Value(value:1.25)\n");
    assert_result("# 1.25\n", "/v1/value\n");
  }

  void test_argument_which_is_not_hash_should_go_to_first_inlet( void ) {
    assert_result("# <Value:/v1 value:2.52>\n", "v1=Value(2.52)\n");
  }

  void test_hash_argument_should_set( void ) {
    assert_result("# {\"prefix\":\"print\", \"foo\":\"404 foo\"}\n", "p = Print(prefix:'print' foo:34)\n");
  }

  void test_print( void ) {
    setup_with_print("n = Value(34)\n");
    assert_print("p: 34\n", "n/value(true)\n");
  }

  void test_parse_zero( void )
  { assert_result("# <Number:/v1 value:0>\n", "v1=Number(0)\n"); }

  void test_parse_zero_dot_int( void )
  { assert_result("# <Number:/v1 value:0.1>\n", "v1=Number(0.1)\n"); }

  void test_parse_command_with_spaces( void )
  { assert_result("# <Number:/v1 value:2>\n", "v1 = Number(2)\n\n"); }

  void test_parse_multiline_string( void )
  { assert_result("<Test:/t \'I love you (a lot)\nwhen you\'re naked...\' counter:0>\n", "t=Test(\"I love you (a lot)\nwhen you're naked...\")\n"); }

  void test_parse_integer( void )
  { assert_result("# <Number:/v1 value:235>\n", "v1 = Number(235)\n\n"); }

  void test_parse_real( void )
  { assert_result("# <Number:/v1 value:2.35>\n", "v1 = Number(2.35)\n\n"); }

  void test_parse_negative_integer( void )
  { assert_result("# <Number:/v1 value:-5>\n", "v1 = Number(-5)\n\n"); }

  void test_parse_negative_real( void )
  { assert_result("# <Number:/v1 value:-2.35>\n", "v1 = Number(-2.35)\n\n"); }

  // Do we want to keep this ?
  void test_execute_method( void )
  { assert_result("<Counter:/v1 3.00 (+1.00)>\n<Counter:/v1 4.00 (+1.00)>\n", "v1=Counter(3)\nv1.bang\nv1\n"); }

  void test_execute_command( void )
  { assert_result("<Counter:/v1 3.00 (+1.00)>\n<Counter:/v1 4.00 (+1.00)>\n", "v1=Counter(3)\nv1.bang\nv1\n"); }

  void test_execute_bang_command( void )
  {
    setup_with_print("n=Number(1)\n");
    assert_print("p: 1\n", "n/value!\n");
  }

  void test_inspect_command( void )
  { assert_result("<Counter:/i 14.00 (+1.00)>\n", "i=Counter(14)\ni\n"); }

  void test_syntax_error( void )
  { assert_result("<Counter:/i 1.00 (+1.00)>\nSyntax error near '\n4\ni\n'.\n<Counter:/i 1.00 (+1.00)>\n", "i=Counter(1)\n4\ni\n"); }

  void test_execute_accessor_with_params( void )
  { assert_result("<Counter:/i 1.00 (+1.00)>\n5.00\n<Counter:/i 6.00 (+5.00)>\n", "i=Counter(1)\ni.increment(5)\ni.bang\ni\n"); }

  void test_execute_set( void )
  { assert_result("<Counter:/i 1.00 (+1.00)>\n<Counter:/i 9.00 (+5.00)>\n", "i=Counter(1)\ni.set(counter:4 increment:5)\ni.bang\ni\n"); }

  void test_execute_set_value( void )
  { assert_result("<Counter:/i 1.00 (+1.00)>\n<Counter:/i 6.00 (+1.00)>\n", "i=Counter(1)\ni.set(5)\ni.bang\ni\n"); }

  void test_bang_shortcut( void )
  { assert_result("<Counter:/i 1.00 (+1.00)>\n<Counter:/i 2.00 (+1.00)>\n", "i=Counter(1)\ni.b\ni\n"); }

  void test_execute_class_method( void )
  { assert_result("Hello: Nil\n", "Test.hello\n"); }

  void test_parse_link( void )
  {
    assert_result("# /obj/out/output ? /other/input\n","obj~output => input~other\n"); // just make sure there is no parse error
  }

  void test_parse_unLink( void )
  {
    setup_with_print("n=Number(1)\n");
    // make sure connection is ok
    assert_print("p: 1\n", "n/value!\n");
    assert_print("","n || p\nn/value!\n");
  }

  void test_parse_first_links( void )
  { assert_result("# /n ? /p\n", "n=>p\n");
    assert_result("# <Number:/n value:3>\n", "n=Number(3)\n");
    // resolve links
    assert_result("# <Print:/p prefix:\"p\">\n# /n/out/value => /p/print\n", "p=Print()\n");
  }

  void test_parse_bad_links( void )
  { assert_result("# Syntax error near \'>\n\'.\n", "n=>\n"); }

  void test_create_same_name( void )
  { setup("v=Number()\n");
    assert_result("# <Print:/v-1 prefix:\"v-1\">\n", "v=Print()\n");
  }

  void test_comment( void )
  { assert_result("# <Number:/v1 value:null>\n", "v1=Number()    # this is a comment\n");
    assert_result("# <Number:/v2 value:null>\n", "v2=Number()    # this is a comment\n");
    assert_result("# <Number:/v3 value:4.02>\n", "v3=Number(4.02)#this is a comment\n# bobob\n");
  }
};