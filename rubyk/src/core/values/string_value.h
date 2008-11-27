
/** StringValue contains a std::string (makes a copy). */
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Value> * meta;
  std::string * value;
  bool   free_me;
} StringValue_t;
