#include "oscit/hash_file_method.h"

#include "oscit/method.h"

namespace oscit {


void HashFileMethod::create_methods() {
  adopt(new TMethod<HashFileMethod, &HashFileMethod::update>(this, "update", HashIO("Hash to deep merge in current content.")));
}

const Value HashFileMethod::trigger(const Value &val) {
  Value h;
  if (!val.is_hash()) {
    if (hash_.is_empty()) {
      Value str(file_.read());
      h.set((Json)str.str());
      if (!h.is_hash()) {
        std::cerr << url() << ": error, hash file content '" << file_.path() << "' is not a Hash !\n";
      } else {
        hash_ = h;
      }
    }
    return hash_;
  }

  if (!file_.write(val.to_json())) {
    // could not write to file
    return file_.last_error();
  } else {
    hash_ = val;
  }

  return hash_;
}

const Value HashFileMethod::update(const Value &val) {
  if (!val.is_hash()) return gNilValue;

  if (hash_.is_empty()) {
    // force file loading
    Value res = trigger(gNilValue);
    if (res.is_error()) return res;
  }

  hash_.deep_merge(val);
  if (!file_.write(hash_.to_json())) {
    // could not write to file
    return file_.last_error();
  } else {
    return val;
  }
}


} // oscit
