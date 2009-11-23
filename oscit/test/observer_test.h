#include "test_helper.h"
#include "oscit/thash.h"
#include "mock/observer_logger.h"

class ObserverTest : public TestHelper
{
public:
  void test_set_and_hold_should_set_value( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    MockObservable a("a");
    MockObservable b("b");
    assert_equal((MockObservable*)NULL, o.x_);
    assert_equal((MockObservable*)NULL, o.y_);

    o.set_x(&a);
    assert_equal(&a, o.x_);

    o.set_y(&b);
    assert_equal(&b, o.y_);
  }

  void test_destroy_observed_should_set_value_to_null( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    MockObservable *a = new MockObservable("a");

    o.set_x(a);
    assert_equal(a, o.x_);
    assert_equal("", logger.str());

    delete a;
    assert_equal("[observer: lock a NULL][observer: unlock NULL NULL]", logger.str());
    assert_equal((MockObservable*)NULL, o.x_);
  }

  void test_destroy_observer_before_observed_should_not_notify( void ) {
    Logger logger;
    ObserverLogger *o = new ObserverLogger("observer", &logger);
    MockObservable *a = new MockObservable("a");

    o->set_x(a);
    assert_equal(a, o->x_);
    assert_equal("", logger.str());

    delete o;
    assert_equal("", logger.str());
    delete a; // should not call o
    assert_equal("", logger.str());
  }
};