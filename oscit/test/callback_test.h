#include "test_helper.h"
#include "mock/observer_logger.h"

class CallbackTest : public TestHelper
{
public:
  void test_create( void ) {
    Logger logger;
    ObserverLogger a("a", &logger);
    ObserverLogger b("b", &logger);
    const char *data = "foo";
    b.adopt_callback_for_event_xyz(new TCallbackLogger<ObserverLogger, &ObserverLogger::my_callback>(&a, (void*)data, "on xyz", &logger));
    b.event_xyz();
    assert_equal("[on xyz: trigger][a: my_callback foo]", logger.str());
  }

  void test_observer_dies_before_trigger( void ) {
    Logger logger;
    ObserverLogger *a = new ObserverLogger("a", &logger);
    ObserverLogger b("b", &logger);
    const char *data = "foo";
    b.adopt_callback_for_event_xyz(new TCallbackLogger<ObserverLogger, &ObserverLogger::my_callback>(a, (void*)data, "on_xyz_callback", &logger));
    delete a;
    b.event_xyz();
    assert_equal("[b: lock][on_xyz_callback: destroy][b: unlock]", logger.str());
  }

  void test_observable_dies_before_observer( void ) {
    Logger logger;
    ObserverLogger *a = new ObserverLogger("a", &logger);
    ObserverLogger *b = new ObserverLogger("b", &logger);
    const char *data = "foo";
    b->adopt_callback_for_event_xyz(new TCallbackLogger<ObserverLogger, &ObserverLogger::my_callback>(a, (void*)data, "on_xyz_callback", &logger));
    delete b;
    assert_equal("[on_xyz_callback: destroy][a: lock][a: unlock]", logger.str());
    logger.str("");
    delete a;
    assert_equal("", logger.str());
  }

};

