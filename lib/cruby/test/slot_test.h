// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "outlet.h"
#include "inlet.h"
#include "rubyk_signal.h"

void send_value(void * receiver, Signal& sig)
{ sig.set(1.0); }

// these receivers are complicated to make sure they work in the correct order and they are all called.
static void receive_value1(void * receiver, const Signal& sig)
{
  (*((float*)receiver)) = (2*(*((float*)receiver))) + sig.f.value + 1;
}

static void receive_value2(void * receiver, const Signal& sig)
{
  (*((float*)receiver)) = (2*(*((float*)receiver))) + sig.f.value + 2;
}

static void receive_value4(void * receiver, const Signal& sig)
{
  (*((float*)receiver)) = (2*(*((float*)receiver))) + sig.f.value + 4;
}

class TestSlot : public CxxTest::TestSuite
{
public:
  void testSingleConnection( void )
  {
    float counter = 0;
    Outlet o(&counter, send_value    );
    Inlet  i(&counter, receive_value1 );
    i.setId(3); // make sure it does not send a 'bang()' to our fake receiver.
    o.connect(&i);
    TS_ASSERT_EQUALS( 0.0, counter);
    o.compute_and_send();
    TS_ASSERT_EQUALS( 2.0, counter);
  }
  
  void testManyConnections( void )
  {
    float counter = 0;
    Outlet o(&counter, send_value    );
    Inlet  i1(&counter, receive_value1 );
    Inlet  i2(&counter, receive_value2 );
    Inlet  i3(&counter, receive_value4 );
    // from their ids, these should be called starting by i1, then i2 and i3 as they share the same receiver (counter)
    i1.setId(4); //         value + 1 = 2
    i2.setId(2); // 2 * 2 + value + 2 = 7
    i3.setId(1); // 2 * 7 + value + 4 = 19
    
    o.connect(&i2);
    o.connect(&i1);
    o.connect(&i3);
    
    TS_ASSERT_EQUALS( 0.0, counter);
    o.compute_and_send();
    TS_ASSERT_EQUALS( 19.0, counter);
  }
  
  void testNilSignal( void )
  {
    // Nil signal should not be sent
    TS_ASSERT( 0 ); // TODO
  }
};