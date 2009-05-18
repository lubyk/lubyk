#ifndef OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_
#define OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_
#include <string>

#include "oscit/script.h"

class DummyScript : public Script {
public:
  DummyScript() {}
  
  virtual const Value eval_script() {
    size_t pos = script_.find("compilation error");
    if (pos != std::string::npos) {
      return Value(BAD_REQUEST_ERROR, std::string("Compilation error near '").append(script_.substr(pos - 4, 23)).append("'."));
    }
    return Value(script_);
  }
  
  void reload_script(time_t current_time) {
    this->Script::reload_script(current_time);
  }
};

#endif // OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_