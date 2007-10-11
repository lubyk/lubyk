#include "../rubyk.h"

void
dummy_function (void* receiver)
{
  int * val = (int*) receiver;
  *val = *val + gCurrentTime;
}

void 
setup_time_queue () 
{
  rk_init_queue();
}

void
test_add_to_queue () 
{
  int elem1, elem2, elem3;
  assert_equal(gQueue->next, NULL);
  rk_add_to_queue(10, (void*)&elem1, &dummy_function);
  assert_equal(gQueue->next->receiver, &elem1);

  rk_add_to_queue(10, (void*)&elem2, &dummy_function);
  assert_equal(gQueue->next->receiver, &elem1);
  assert_equal(gQueue->next->next->receiver, &elem2);

  rk_add_to_queue(4, (void*)&elem3, &dummy_function);
  assert_equal(gQueue->next->receiver, &elem3);
  assert_equal(gQueue->next->next->receiver, &elem1);
  assert_equal(gQueue->next->next->next->receiver, &elem2);
}

void
test_remove_from_queue () 
{
  int elem1, elem2, elem3;
  assert_equal(gQueue->next, NULL);
  rk_add_to_queue(10, (void*)&elem1, &dummy_function);
  rk_add_to_queue(10, (void*)&elem2, &dummy_function);
  rk_add_to_queue(4,  (void*)&elem3, &dummy_function);
  rk_add_to_queue(6,  (void*)&elem2, &dummy_function);
  assert_equal(gQueue->next->receiver, &elem3);
  assert_equal(gQueue->next->next->receiver, &elem2);
  assert_equal(gQueue->next->next->next->receiver, &elem1);
  assert_equal(gQueue->next->next->next->next->receiver, &elem2);
  
  rk_remove_from_queue((void*)&elem2);
  assert_equal(gQueue->next->receiver, &elem3);
  assert_equal(gQueue->next->next->receiver, &elem1);
  assert_equal(gQueue->next->next->next, NULL);
}

void
test_run_queue () 
{
  int elem1 = 0;
  int elem2 = 0;
  int elem3 = 0;
  
  assert_equal(gQueue->next, NULL);
  rk_add_to_queue(10, (void*)&elem1, &dummy_function);
  rk_add_to_queue(10, (void*)&elem2, &dummy_function);
  rk_add_to_queue(4,  (void*)&elem3, &dummy_function);
  rk_add_to_queue(6,  (void*)&elem2, &dummy_function);
  assert_equal(gQueue->next->receiver, &elem3);
  assert_equal(gQueue->next->next->receiver, &elem2);
  assert_equal(gQueue->next->next->next->receiver, &elem1);
  assert_equal(gQueue->next->next->next->next->receiver, &elem2);
  
  rk_run_queue(5);
  assert_equal(elem3, 4);
  assert_equal(gQueue->next->receiver, &elem2);
  assert_equal(gQueue->next->next->receiver, &elem1);
  assert_equal(gQueue->next->next->next->receiver, &elem2);
  
  rk_run_queue(8);
  assert_equal(elem3, 4);
  assert_equal(elem2, 6);
  assert_equal(gQueue->next->receiver, &elem1);
  assert_equal(gQueue->next->next->receiver, &elem2);
  
  rk_run_queue(10);
  assert_equal(elem3, 4);
  assert_equal(elem1, 10);
  assert_equal(elem2, 16);
  assert_equal(gQueue->next, NULL);
}

TestSuite *time_queue_test_suite() {
  TestSuite *suite = create_test_suite();

  setup(suite, setup_time_queue);

  add_test(suite, test_add_to_queue);
  add_test(suite, test_remove_from_queue);
  add_test(suite, test_run_queue);
  return suite;
}