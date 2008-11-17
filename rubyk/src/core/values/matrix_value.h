
typedef struct {
  rubyk_signal_t    type;
  Hash<uint, Signal> * meta;
  const Matrix * value;
} MatrixSignal_t;
