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
    logger.str("");

    delete o;
    assert_equal("", logger.str());
    delete a; // should not call o
    assert_equal("", logger.str());
  }

  void test_append_and_hold_should_append_value( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    MockObservable a("a");
    MockObservable b("b");
    assert_equal((MockObservable*)NULL, o.x_);
    assert_equal((MockObservable*)NULL, o.y_);

    o.append_x(&a);
    assert_equal(&a, o.list_x_.front());

    o.append_y(&b);
    assert_equal(&b, o.list_y_.front());
  }

  void test_destroy_observed_should_remove_value_from_list( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    o.mode_ = ObserverLogger::ListXTest;
    MockObservable *a = new MockObservable("a");

    o.append_x(a);
    logger.str("");

    delete a;
    assert_equal("[observer: lock 1][observer: unlock 0]", logger.str());
    assert_equal((MockObservable*)NULL, o.x_);
  }

  void test_destroy_observer_before_observed_should_not_notify_list_owner( void ) {
    Logger logger;
    ObserverLogger *o = new ObserverLogger("observer", &logger);
    MockObservable *a = new MockObservable("a");

    o->append_x(a);
    logger.str("");

    delete o;
    assert_equal("", logger.str());
    delete a; // should not call o
    assert_equal("", logger.str());
  }
};