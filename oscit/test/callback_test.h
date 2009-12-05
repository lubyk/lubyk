#include "test_helper.h"
#include "mock/callback_logger.h"

class CallbackTest : public TestHelper
{
public:
  void test_create( void ) {
    Logger logger;
    CallbackObserverLogger a("a", &logger);
    CallbackObservableMock b;
    const char *data = "foo";
    b.adopt_callback_for_event_xyz(new TCallbackLogger<CallbackObserverLogger, &CallbackObserverLogger::my_callback>(&a, (void*)data, "on xyz", &logger));
    b.event_xyz();
    assert_equal("[on xyz: trigger][a: my_callback foo]", logger.str());
  }

  void test_observer_dies_before_trigger( void ) {
    Logger logger;
    CallbackObserverLogger *a = new CallbackObserverLogger("a", &logger);
    CallbackObservableMock b;
    const char *data = "foo";
    b.adopt_callback_for_event_xyz(new TCallbackLogger<CallbackObserverLogger, &CallbackObserverLogger::my_callback>(a, (void*)data, "on xyz", &logger));
    delete a;
    b.event_xyz();
    assert_equal("[on xyz: destroy]", logger.str());
  }

  void test_observable_dies_before_observer( void ) {
    Logger logger;
    CallbackObserverLogger *a = new CallbackObserverLogger("a", &logger);
    CallbackObservableMock *b = new CallbackObservableMock;
    const char *data = "foo";
    b->adopt_callback_for_event_xyz(new TCallbackLogger<CallbackObserverLogger, &CallbackObserverLogger::my_callback>(a, (void*)data, "on xyz", &logger));
    delete b;
    delete a;
    assert_equal("[on xyz: destroy]", logger.str());
  }

};

