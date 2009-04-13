#include "test_helper.h"
#include "ordered_list.h"

struct OrderedListTest_Object
{
  OrderedListTest_Object(Real value) : value_(value) {}
  
  bool operator>=(const OrderedListTest_Object &object) const {
    return value_ >= object.value_;
  }
  
  Real value_;
};

class OrderedListTest : public TestHelper
{
public:
  void test_push_pop( void ) {
    OrderedList<int> list;
    int i;
    
    assert_equal( list.size(), 0);
    
    list.push(2);
    assert_equal( list.size(), 1);
    assert_equal( list.front(), 2);
    assert_true( list.get(&i) );
    assert_equal( i, 2);
    list.pop();
    assert_true( list.empty() );
    assert_true( !list.get(&i) );
    
    // adding keeps element sorted
    list.push(3);
    list.push(2);
    assert_equal( list.front(), 2);
    list.push(1);
    assert_equal( list.front(), 1);
    list.push(1);
    
    assert_equal( list.front(), 1);
    
    // pop to make sure we have a single copy, ordered
    list.pop();
    assert_equal( list.front(), 2);
    list.pop();
    assert_equal( list.front(), 3);
    list.pop();
    assert_true( list.empty());
  }
  
  void test_reorder( void ) {
    OrderedList<OrderedListTest_Object*> list;
    OrderedListTest_Object a(1), b(2), c(3);
    OrderedListTest_Object * res;
    
    // elements are sorted
    list.push(&c);
    list.push(&a);
    list.push(&b);
    assert_equal(3, list.size());
    assert_equal(&a,list.front());
    assert_true(list.get(&res));
    assert_equal(&a, res);
    
    // reordering
    a.value_ = 2.5;
    // not reordered yet
    assert_equal(&a, list.front());
    
    list.sort_object(&a);
    assert_equal(&b, list.front());
    list.pop();
    assert_equal(&a, list.front());
    list.pop();
    assert_equal(&c, list.front());
  }
};