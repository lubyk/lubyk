class StringValueTest : public TestHelper
{  
public:
  void test_create( void )
  {
    Value v("foo");
    
    assert_false(v.isNil());
    assert_false(v.isReal());
    assert_true (v.isString());
    assert_false(v.isError());
    
    assert_equal("foo", v.s);
    
    assert_equal("s", v.type_tag());
  }
  
  void test_createStdString( void )
  {
    std::string str("foo");
    Value v(str);
    
    assert_false(v.isNil());
    assert_false(v.isReal());
    assert_true (v.isString());
    assert_false(v.isError());
    
    assert_equal("foo", v.s);
    
    str.append("bar");
    assert_equal("foobar", str);
    assert_equal("foo", v.s);
    
    assert_equal("s", v.type_tag());
  }
  
  void test_createChar( void )
  {
    Value v('s');
    
    assert_true(v.isString());
    assert_equal("", v.s);
  }
  
  void testCopy( void )
  {
    Value v("foo");
    Value v2(v);
    Value v3;
    
    assert_true(v2.isString());
    assert_true(v3.isNil());
    
    v3 = v;
    
    assert_true(v3.isString());
    
    assert_equal("foo", v.s);
    assert_equal("foo", v2.s);
    assert_equal("foo", v3.s);
    
    // WARNING: never use v.s = "some string" (crash guaranteed !).
    v.set("bar");
    assert_equal("bar", v.s);
    assert_equal("foo", v2.s);
    assert_equal("foo", v3.s);
  }
  
  void testSet( void )
  {
    Value v;
    
    assert_true(v.isNil());
    v.set("foobar");
    assert_true(v.isString());
    assert_equal("foobar", v.s);
  }
  
  void testSetTag( void )
  {
    Value v;
    
    v.set_type_tag("s");
    assert_true(v.isString());
    assert_equal("", v.s);
  }
  
  void testSetType( void )
  {
    Value v;
    
    v.setType(STRING_VALUE);
    assert_true(v.isString());
    assert_equal("", v.s);
  }
};