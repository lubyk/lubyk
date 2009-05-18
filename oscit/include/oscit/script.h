#ifndef OSCIT_INCLUDE_OSCIT_SCRIPT_H_
#define OSCIT_INCLUDE_OSCIT_SCRIPT_H_
#include <sys/types.h>  // time_t
#include <string>

#include "oscit/conf.h"

namespace oscit {

class Value;

/** Abstract class to manage script load, reload and eval. */
class Script {
public:
  Script();
  
  virtual ~Script();
  
  /** Set script content.
   * If there is a file path, the content is save to
   * this path.
   */
  const Value script(const Value &val);
  
  /** Load a script from a given file path.
   */
  const Value file(const Value &val);
  
  /** Reload time in seconds.
   * This is the value for the minimum amount of time between
   * two file modification time verifications.
   */
  const Value reload(const Value &val);
  
  /** Set hash to script content.
   * @param hash should be a pointer to a HashValue.
   */
  void inspect(Value *hash);
  
  bool script_ok() {
    return script_ok_;
  }
  
 protected:
  
  /** Ask for a script reload.
   * If reload_every_ time has not elapsed, the call is ignored.
   * If the file's modification time has not changed, nothing
   * happens. If the file content changed, it is evaluated.
   */
  void reload_script(time_t current_time);
  
  /** Evaluat the content of the script.
   * This method must be overwritten in sub-classes.
   */
  virtual const Value eval_script() = 0;
  
  /** Sets 'script_ok' flag.
   * This can be overwritten by subclasses in case they need to
   * react on state changes.
   */
  virtual void set_script_ok(bool state);
  
protected:
  /** Simple flag that is set to true when the last
   * compilation was successful. This flag can be used
   * to avoid using compilation results if something
   * went wrong.
   */
  bool script_ok_;
  
  /** Script text.
   */
  std::string script_;
  
  /** Path to script file when the text is stored in a file.
   */
  std::string script_file_;
  
  /** Script file's modification time when it was last loaded.
   */
  time_t script_mod_time_;
  
  /** How often we should check for a modification in the source file (in seconds).
   */
  Real reload_every_;
  
  /** Next scheduled reload.
   */
  time_t next_reload_;
  
  /** Current logical time.
   */
  time_t current_time_;
private:
  
  /** Load script and evaluate content.
   */
  const Value load_script_from_file(bool is_new);
  
  /** Save script content to file.
   * This is done automatically when the script content changes.
   */
  const Value save_script();
     
  /** Define next reload time.
   */
  void set_next_reload();
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_SCRIPT_H_