
/** Message pointed by value gets freed with the Signal if free_me is true. */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  MidiMessage * value;
  bool   free_me;
} MidiSignal_t;
