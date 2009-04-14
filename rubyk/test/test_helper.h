#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_

#include "rubyk.h"
#include "mock/dummy_node.h"

#include <cxxtest/TestSuite.h>
#include <sys/timeb.h> // ftime
#include <ostream>

void microsleep(size_t microseconds) {
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = microseconds * 1000000;
  nanosleep (&sleeper, NULL);
}

#define assert_equal(x,y) _assert_equal(__FILE__,__LINE__,#y,x,y)
#define assert_true(e) _TS_ASSERT(__FILE__,__LINE__,e)
#define assert_false(e) _TS_ASSERT(__FILE__,__LINE__,!(e))

#define _OSCIT_ASSERT_EQUALS(f,l,cmd,x,y) { _TS_TRY { CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (0) ); } __TS_CATCH(f,l) }

class TestHelper : public CxxTest::TestSuite
{
protected:
  
  void _assert_equal(const char * file, int lineno, const char * descr, Real expected, Real found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), found, expected);
  }
  
  void _assert_equal(const char * file, int lineno, const char * descr, int expected, int found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), found, expected);
  }
  
  void _assert_equal(const char * file, int lineno, const char * descr, const char * expected, const char * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), std::string(found), std::string(expected));
  }
  
  void _assert_equal(const char * file, int lineno, const char * descr, const char * expected, const std::string &found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), found, std::string(expected));
  }
  
  void _assert_equal(const char * file, int lineno, const char * descr, const std::string &expected, const std::string &found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), found, expected);
  }
  
  void _assert_equal(const char * file, int lineno, const char * descr, void * expected, void * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), found, expected);
  }
};



// ========================== ParseHelper  ====================== //

class ParseHelper : public CxxTest::TestSuite
{
public:
  ParseHelper() : output_(std::ostringstream::out), input_(std::istringstream::in) { 
    cmd_  = new Command(input_, output_);
    cmd_->set_planet(&planet_);
  }
  
  ~ParseHelper() {
    delete cmd_;
  }
  
  void setUp() {
    planet_.clear();
    planet_.classes()->set_lib_path("lib");
    output_.str(std::string("")); // clear output
  }
  
protected:
  Planet planet_;
  Command * cmd_;
  std::ostringstream output_;
  std::istringstream input_;
  
protected:
  
  void setup_with_print(const char *input) {
    Node * print;
    worker_.unlock();
      cmd_->parse("p=Print()\nn=>p\n");
      print = TYPE_CAST(Node, worker_.find("/p"));
      if (print) cmd_->observe(print);
      cmd_->parse(input);
      output_.str(std::string("")); // clear output
    worker_.lock();
  }
  
//FIX  void clean_assert_result(const char * input, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_result(input, pOutput);
//FIX  }

  void _assert_result(const char * file, int lineno, const char * input, const char * pOutput)
  {
    output_.str(std::string("")); // clear output
    worker_.unlock();
      cmd_->parse(input);
    worker_.lock();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(input)), output_.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_bang(const char * input, const char * pOutput)
//FIX  {
//FIX    setup_with_print(input);
//FIX    assert_bang("\n", pOutput);
//FIX  }
//FIX  
//FIX  void assert_bang(const char * input, const char * pOutput)
//FIX  { 
//FIX    worker_.unlock();
//FIX      output_.str(std::string("")); // clear output
//FIX      cmd_->set_silent(true);
//FIX      cmd_->parse(input);
//FIX      cmd_->parse("\nn.bang\n");
//FIX      cmd_->set_silent(false);
//FIX    worker_.lock();
//FIX    TS_ASSERT_EQUALS( output_.str(), std::string(pOutput));
//FIX  }
//FIX  
//FIX  void clean_assert_print(const char * input, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_print(input, pOutput);
//FIX    
//FIX  }
  
  void _assert_print(const char * file, int lineno, const char * input, const char * pOutput)
  { 
    worker_.unlock();
      output_.str(std::string("")); // clear output
      cmd_->set_silent(true);
      cmd_->parse(input);
      cmd_->set_silent(false);
    worker_.lock();
    
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(input)), output_.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_run(time_t pLength, const char * input, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_run(pLength, input, pOutput);
//FIX  }

  void _assert_run(const char * file, int lineno, time_t pLength, const char * pOutput)
  {
    time_t start = worker_.current_time_;
    output_.str(std::string("")); // clear output
    cmd_->set_silent(true);
    while(worker_.current_time_ <= start + pLength && worker_.do_run())
      ;
    cmd_->set_silent(false);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING("running"), output_.str(), std::string(pOutput));
  }
};

#endif // _TEST_HELPER_H_