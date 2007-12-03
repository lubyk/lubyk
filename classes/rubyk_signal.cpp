#include "rubyk_signal.h"

std::ostream& operator<< (std::ostream& pStream, const Signal& sig)
{
  char buffer[50];
  switch(sig.type) {
  case BangSignal:
    pStream << "Bang!";
    break;
  case IntegerSignal:
    pStream << sig.i.value;
    break;
  case DoubleSignal:
    snprintf(buffer, 50, "%.2f", sig.d.value);
    pStream << buffer;
    break;
  case MatrixSignal:
    if (sig.matrix.value->size() == 0) {
      snprintf(buffer, 50, "<%s 0>",sig.type_name());
      pStream << buffer;
    } else {
      size_t sz = 16;
      size_t start;
      if (sz > sig.matrix.value->size()) sz = sig.matrix.value->size();
      start = sig.matrix.value->size() - sz;
      snprintf(buffer, 50, "<%s [ % .2f", sig.type_name(), sig.matrix.value->data[start]);
      pStream << buffer;
      for (size_t i= start+1; i < start+sz; i++) {
        snprintf(buffer, 50, " % .2f", sig.matrix.value->data[i]);
        pStream << buffer;
      }
      pStream << " ], " << sig.matrix.value->size() << ">";
    }
    break;
  case VoidPointerSignal:
    snprintf(buffer, 50, "<%s %p, %i>",sig.type_name(), sig.ptr.value, sig.ptr.free_me);
    pStream << buffer;
    break;
  case MidiSignal:
    pStream << "<" << sig.type_name() << " " << *(sig.midi_ptr.value) << ">";
    break;
  default:
    pStream << sig.type_name();
  }
  return pStream;
}
