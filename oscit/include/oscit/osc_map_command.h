#ifndef OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_

#include <list>
#include <stdexcept> // runtime_error

#include "oscit/osc_command.h"
#include "oscit/script.h"
#include "oscit/mapper.h"
#include "oscit/values.h"

namespace oscit {

struct TimeRef;

class OscMapCommand : public Script, public OscCommand {
public:
  OscMapCommand();

  OscMapCommand(uint16_t port, uint16_t reply_port);

  virtual ~OscMapCommand();

  // {4} set/change input port
  const Value port(const Value &val) {
    if (val.is_real() && val.r > 0 && val.r != port_) {
      lock();
        change_port(val.r);
      unlock();
    }

    return Value(port_);
  }

  // {5} set/change input port
  const Value reply_port(const Value &val) {
    if (val.is_real() && val.r > 0) {
      lock();
        reply_port_ = val.r;
      unlock();
    }

    return Value(reply_port_);
  }

 protected:
  /** Executed within mutex lock from own thread.
   */
  virtual void receive(const Url &url, const Value &ext_val);

  /** Executed within mutex lock from root.
   */
  virtual void notify_observers(const char *path, const Value &val);

  /** Mappings compilation.
   */
  virtual const Value eval_script() {
    Value res;
    lock();
      if (mapper_.parse(script_.c_str())) {
        res = Value(script_);
      } else {
        res = Value(BAD_REQUEST_ERROR, mapper_.error());
      }
    unlock();
    return res;
  }

 private:

  uint16_t reply_port_;

  Mapper mapper_;
  TimeRef *time_ref_;
};

}  // oscit

#endif  // OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_