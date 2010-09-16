// A simple demonstration of OpenGL lighting
// g++ sigslot.cpp

#include <ting/Signal.hpp>


//using namespace sigslot;

// BT is for BoostTest in case you wonder...
// class SenderBT {
// public:
//   void message(const char *message) {
//     some_event_.emit(message);
//   }
//
//   signal1<const char*, multi_threaded_local> some_event_;
// };

//class ReceiverBT : has_slots<multi_threaded_local> {
//public:
//  ReceiverBT(const char *id, std::ostringstream *oss) : id_(id), oss_(oss) {}
//
//  ~ReceiverBT() {
//    *oss_ << "[" << id_ << ": deleted]";
//  }
//
//  void event(const char *text) {
//    *oss_ << "[" << id_ << ": " << text << "]";
//  }
//
//  const char *id_;
//  std::ostringstream *oss_;
//};
//

int main() {
  //  SenderBT root;
  //  std::ostringstream oss;
  //  ReceiverBT *observer = new ReceiverBT("a", &oss);
  //
  //  // nothing send (slot empty)
  //  root.message("message A");
  //
  //  root.some_event_.connect(observer, &ReceiverBT::event);
  //
  //  // message sent
  //  root.message("message B");
  //
  //  delete observer;
  //
  //  // no message sent
  //  root.message("message C");
  //
  //  assert_equal("", oss.str());
  return 0;
}