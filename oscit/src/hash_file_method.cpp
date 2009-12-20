#include "oscit/hash_file_method.h"


namespace oscit {


void HashFileMethod::create_methods() {

}

const Value HashFileMethod::trigger(const Value &val) {
 Value h;
  if (!val.is_string()) {
    Value str(file_.read());
    h.set((Json)str.str());
    if (!h.is_hash()) {
      std::cerr << url() << ": error, hash file content '" << file_.path() << "' is not a Hash !\n";
    } else {
      hash_ = h;
    }
    return str;
  }

  h.set((Json)val.str());
  if (!h.is_hash()) {
    return Value(BAD_REQUEST_ERROR, "Invalid file content, should be a hash (json encoded).");
  } else {
    if (!file_.write(val.str())) {
      // could not write to file
      return file_.last_error();
    } else {
      hash_ = h;
    }
  }

  return val;
}

const Value HashFileMethod::update(const Value &hash) {
  if (!hash.is_hash()) return gNilValue;

  if (hash_.is_empty()) {
    Value res = trigger(gNilValue);
    if (res.is_error()) return res;
  }

  hash_ = hash_.deep_merge(hash);
  if (!file_.write(hash.to_json())) {
    // could not write to file
    return file_.last_error();
  } else {
    return hash;
  }
}


} // oscit
