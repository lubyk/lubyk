class StringValueTest : public TestHelper
{  
public:
  void testCreate( void )
  {
    Value v("foo");
    
    assertFalse(v.isNil());
    assertFalse(v.isReal());
    assertTrue (v.isString());
    assertFalse(v.isError());
    
    assertEqual("foo", v.s);
    
    assertEqual("s", v.getTypeTag());
  }
  
  void testCreateStdString( void )
  {
    std::string str("foo");
    Value v(str);
    
    assertFalse(v.isNil());
    assertFalse(v.isReal());
    assertTrue (v.isString());
    assertFalse(v.isError());
    
    assertEqual("foo", v.s);
    
    str.append("bar");
    assertEqual("foobar", str);
    assertEqual("foo", v.s);
    
    assertEqual("s", v.getTypeTag());
  }
  
  void testCreateChar( void )
  {
    Value v('s');
    
    assertTrue(v.isString());
    assertEqual("", v.s);
  }
  
  void testCopy( void )
  {
    Value v("foo");
    Value v2(v);
    Value v3;
    
    assertTrue(v2.isString());
    assertTrue(v3.isNil());
    
    v3 = v;
    
    assertTrue(v3.isString());
    
    assertEqual("foo", v.s);
    assertEqual("foo", v2.s);
    assertEqual("foo", v3.s);
    
    // WARNING: never use v.s = "some string" (crash guaranteed !).
    v.set("bar");
    assertEqual("bar", v.s);
    assertEqual("foo", v2.s);
    assertEqual("foo", v3.s);
  }
  
  void testSet( void )
  {
    Value v;
    
    assertTrue(v.isNil());
    v.set("foobar");
    assertTrue(v.isString());
    assertEqual("foobar", v.s);
  }
  
  void testSetTag( void )
  {
    Value v;
    
    v.setTypeTag("s");
    assertTrue(v.isString());
    assertEqual("", v.s);
  }
  
  void testSetType( void )
  {
    Value v;
    
    v.setType(StringValueType);
    assertTrue(v.isString());
    assertEqual("", v.s);
  }
};