
/** StringValue contains a std::string (makes a copy). */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  std::string * value;
  bool   free_me;
} StringSignal_t;
