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
  case ArraySignal:
    if (sig.array.size == 0) {
      snprintf(buffer, 50, "<%s %i>",sig.type_name(), sig.array.size);
      pStream << buffer;
    } else {
      int sz = 16;
      int start;
      if (sz > sig.array.size) sz = sig.array.size;
      start = sig.array.size - sz;
      snprintf(buffer, 50, "<%s [ % .2f", sig.type_name(), sig.array.value[start]);
      pStream << buffer;
      for (int i= start+1; i < start+sz; i++) {
        snprintf(buffer, 50, " % .2f", sig.array.value[i]);
        pStream << buffer;
      }
      pStream << " ], " << sig.array.size << ">";
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
    pStream << "<" << sig.type_name() << ">";
  }
  return pStream;
}
