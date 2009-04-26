#include "test_helper.h"

class LuaTest : public ParseHelper
{
public:
  
  void test_send( void ) 
  { assert_print("n=Lua(\"function bang(sig)\nsend(1,{1,2,3})\nend\")\np=Print()\nn=>p\nn.bang\n","<Matrix [  1.00  2.00  3.00 ], 1x3>\n"); }

  void test_create_load( void ) 
  { assert_print("n=Lua(load:\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang(sig)\n\treturn sig+12\nend\n\n"); }

  void test_load( void ) 
  { assert_print("n=Lua()\nn.load(\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang(sig)\n\treturn sig+12\nend\n\n"); }

  void test_input_matrix( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend(1,{sig[1],sig[3]})\nend\")\nn=>p\n");
    
    assert_print("n.bang(1,2,3,4)\n",   "<Matrix [  1.00  3.00 ], 1x2>\n");
    assert_print("n.bang(-1,5,0)\n",    "<Matrix [ -1.00  0.00 ], 1x2>\n");
  }
  
  void test_send_note( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend_note(1,sig,66,500,1,3)\nend\")\nn=>p\n");
    
    assert_print("n.bang(60)\n",   "<Midi +1:C3(66), 3/500>\n");
    assert_print("n.bang(61)\n",   "<Midi +1:C#3(66), 3/500>\n");
  }
  
  void test_send_note_through( void ) 
  { 
    parse("no=NoteOut(note:29 velocity:66 channel:3 length:300)\nno=>n\nn=Lua(\"function bang(sig)\nsend(1,sig)\nend\")\nn=>p\n");
    
    assert_print("no.bang(60)\n",   "<Midi +3:C3(66), 0/300>\n");
    assert_print("no.bang(61)\n",   "<Midi +3:C#3(66), 0/300>\n");
  }
  
  void test_send_chord( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend_note(1,{sig,sig+7},66,500,1,3)\nend\")\nn=>p\n");

    assert_print("n.bang(60)\n",   "<Midi +1:C3(66), 3/500>\n<Midi +1:G3(66), 3/500>\n");
    assert_print("n.bang(61)\n",   "<Midi +1:C#3(66), 3/500>\n<Midi +1:G#3(66), 3/500>\n");
  }
  
  void test_send_ctrl( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend_ctrl(1,20,sig,7,5)\nend\")\nn=>p\n");

    assert_print("n.bang(60)\n",   "<Midi ~7:20(60), 5>\n");
    assert_print("n.bang(61)\n",   "<Midi ~7:20(61), 5>\n");
  }
  
  
  void test_inputs( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend(1,{sig or 0,in2 or 0,in3 or 0,in4 or 0,in5 or 0,in6 or 0,in7 or 0,in8 or 0,in9 or 0,in10 or 0})\nend\")\nn=>p\n");
    
    assert_print("n.b(11)\n",              "<Matrix [  11.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in2(22)\nn.b(11)\n",   "<Matrix [  11.00  22.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in3(33)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in4(44)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in5(55)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in6(66)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in7(77)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in8(88)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in9(9.9)\nn.b(11)\n",  "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  0.00 ], 1x10>\n");
    assert_print("n.in10(1.1)\nn.b(11)\n", "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  1.10 ], 1x10>\n");
  }
  
  void test_outputs( void ) 
  { 
    parse("n=Lua(\"function bang(sig)\nsend(6,sig)\nend\")\nn.6 => v\nv = Value()\nn // p\nv => p\nn=>p\n");
    
    assert_print("n.b(123.34)\n",              "123.34\n");
  }
  
  void test_current_time( void )
  { 
    parse("n=Lua(\"function bang(sig)\nsend(1,current_time)\nend\")\nn=>p\n");
    
    assert_print("n.b\n",              "0.00\n");
    worker_->current_time_ = 1234;
    assert_print("n.b\n",              "1234.00\n");
  }
};