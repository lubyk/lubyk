
/** Message pointed by value gets freed with the Value if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  THash<uint, Value> * meta;
  MidiMessage * value;
  bool   free_me;
} MidiValue_t;


//  case MidiValue:
//    pStream << "<" << val.data_type_name() << " " << *(val.midi_ptr.value) << ">";
//    break;