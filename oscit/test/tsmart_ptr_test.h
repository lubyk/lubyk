#include "test_helper.h"
#include "oscit/tsmart_ptr.h"

/** Simple class to test the smart pointer template. */
class TestSmartData
{
public:
  TestSmartData(int value, const char ** statusNotification = NULL) : value_(value), statusNotification_(statusNotification)
  {
    if (statusNotification_ != NULL)
      *statusNotification_ = "created";
  }
  
  ~TestSmartData()
  {
    if (statusNotification_ != NULL)
      *statusNotification_ = "deleted";
  }
  
  int value_;
  const char ** statusNotification_;
};

class TestSmart : public TSmartPtr<TestSmartData>
{
public:
  TestSmart(int value) : TSmartPtr<TestSmartData>(new TestSmartData(value)) {}
  TestSmart(int value, const char ** notification) : TSmartPtr<TestSmartData>(new TestSmartData(value, notification)) {}
};

class SmartPointerTest : public TestHelper
{  
public:
  void testCreate( void )
  { 
    TestSmart s(5);
    assertEqual(5, s.getData()->value_);
  }
  
  void testRefCount( void )
  {
    TestSmart s(5);
    assertEqual(1, s.getRefCount());
    assertEqual(5, s.getData()->value_);
    
    TestSmart * s2 =  new TestSmart(s);
    assertEqual(2, s.getRefCount());
    assertEqual(2, s2->getRefCount());
    assertEqual(5, s2->getData()->value_);
    
    s2->getData()->value_ = 3;
    assertEqual(3, s2->getData()->value_);
    assertEqual(3, s.getData()->value_);
    
    delete s2;
    assertEqual(1, s.getRefCount());
  }
  
  void testDelete( void )
  {
    const char * notification;
    TestSmart * s = new TestSmart(3, &notification);
    assertEqual("created", notification);
    notification = "";
    // should not create a new TestSmartData
    TestSmart * s2 = new TestSmart(*s);
    assertEqual("", notification);
    delete s;
    // should not delete the TestSmartData
    assertEqual("", notification);
    delete s2;
    assertEqual("deleted", notification);
  }
};