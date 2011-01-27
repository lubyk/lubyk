#include "lubyk.h"
#include "oscit/alias.h"

/** This objects either sends received data to a remote url through OSC or to an alias. */
class Send : public Node
{
public:
  Send() : alias_(NULL) {}
  
  // [1] send to remote url
  void bang(const Value &val) {
    if (alias_) alias_->trigger(val); // ignore return value
  }
  
  const Value remote_url(const Value &val) {
    if (val.is_string()) {
      if (set_remote_url(val.str())) {
        
      } else {
        return ErrorValue(BAD_REQUEST_ERROR, std::string("Could not connect to remote url '").append(val.str()).append("'."))
      }
    }
    if (alias_) {
      return Value(alias_->url());
    } else {
      return remote_url_;
    }
  }
private:
  void set_remote_url(const std::string &url) {
    
  }
  
  std::string remote_url_; /**< Fully qualified name of the remote url. */
  Alias *alias_; /**< Remote object is on the same planet. */
};

extern "C" void init(Planet &planet) {
  Class * c = planet.classes()->declare<AliasNode>("Alias", "Create an alias to a node's method.");
  ACCESSOR(AliasNode, original, "Set original method.")
}