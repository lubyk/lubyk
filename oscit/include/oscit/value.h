#ifndef OSCIT_INCLUDE_OSCIT_VALUE_H_
#define OSCIT_INCLUDE_OSCIT_VALUE_H_

#include <string.h> // strlen
#include <string>
#include "oscit/value_types.h"
#include "oscit/thash.h"
#include "oscit/string_data.h"
#include "oscit/error.h"
#include "oscit/list.h"
#include "oscit/hash.h"
#include "oscit/matrix.h"
#include "oscit/midi_message.h"

namespace oscit {

/** This is just a different typedef for std::string. */
class Json : public std::string
{
 public:
  explicit Json(const char *str) : std::string(str) {}
  explicit Json(const std::string &str) : std::string(str) {}
};


/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages.
 *
 *  Nil vs Empty: Empty means "not initialized", Nil means "initialized to 'no value'". If you pass
 *  Empty as a parameter it will be considered the same as Nil (root does a cast).
 */
class Value
{
public:

  /** =========================================================    Empty   */
  Value() : type_(EMPTY_VALUE) {}

  /** =========================================================    Real    */

  explicit Value(Real real) : type_(REAL_VALUE), r(real) {}

  explicit Value(int number) : type_(REAL_VALUE), r(number) {}

  /** =========================================================    String  */
  explicit Value(const char *string) : type_(STRING_VALUE) {
    set_string(string);
  }

  explicit Value(const std::string &string) : type_(STRING_VALUE) {
    set_string(string.c_str());
  }

  /** =========================================================    List     */
  explicit Value(const List *list) : type_(LIST_VALUE) {
    set_list(list);
  }

  explicit Value(const List &list) : type_(LIST_VALUE) {
    set_list(&list);
  }

  /** =========================================================    Error    */
  explicit Value(ErrorCode code, const char *str) : type_(ERROR_VALUE) {
    set_error(code, str);
  }

  explicit Value(ErrorCode code, const std::string& str) : type_(ERROR_VALUE) {
    set_error(code, str.c_str());
  }

  /** =========================================================    Hash    */
  explicit Value(const Hash *hash) : type_(HASH_VALUE) {
    set_hash(hash);
  }

  explicit Value(const Hash &hash) : type_(HASH_VALUE) {
    set_hash(&hash);
  }

  /** =========================================================    Matrix  */
  explicit Value(const Matrix *matrix) : type_(MATRIX_VALUE) {
    set_matrix(matrix);
  }

  explicit Value(const Matrix &matrix) : type_(MATRIX_VALUE) {
    set_matrix(&matrix);
  }

  /** =========================================================    Midi    */
  explicit Value(const MidiMessage *midi_message) : type_(MIDI_VALUE) {
    set_midi(midi_message);
  }

  explicit Value(const MidiMessage &midi_message) : type_(MIDI_VALUE) {
    set_midi(&midi_message);
  }

  /** =========================================================    Any     */
  /** Create a value from a TypeTag string. */
  explicit Value(TypeTag type_tag) : type_(EMPTY_VALUE) {
    set_type_tag(type_tag.str_);
  }

  /** Create a value by parsing a JSON string. */
  explicit Value(const Json &json) : type_(EMPTY_VALUE) {
    build_from_json(json.c_str());
  }

  /** Create a default value from a type character like 'f'. */
  explicit Value(char type_char) : type_(EMPTY_VALUE) {
    set_type(type_from_char(type_char));
  }

  /** Copy constructor (needed since many methods return a Value). */
  Value(const Value &value) : type_(EMPTY_VALUE) {
    *this = value;
  }

  ~Value() {
    clear();
  }

  /** Share the content of another Value. */
  Value &set(const Value &other) { // TODO: use 'const' to select between 'share' and 'copy' ? Do the same with Root::call return values.
    switch (other.type_) {
      case REAL_VALUE:
        set(other.r);
        break;
      case STRING_VALUE:
        share(other.string_);
        break;
      case LIST_VALUE:
        share(other.list_);
        break;
      case ERROR_VALUE:
        share(other.error_);
        break;
      case HASH_VALUE:
        share(other.hash_);
        break;
      case MATRIX_VALUE:
        // FIXME: share matrix header
        set(other.matrix_);
        break;
      case MIDI_VALUE:
        share(other.midi_message_);
        break;
      case ANY_VALUE:
        set_any();
        break;
      case EMPTY_VALUE:
        /* we consider that if you set a value with empty it means you want Nil.
         * This is very useful for return values. */
        /* continue */
      case NIL_VALUE:
        /* continue */
      default:
        set_nil();
    }
    return *this;
  }

  /** Share the content of another Value. */
  Value &copy(const Value &other) {
    switch (other.type_) {
      case REAL_VALUE:
        set(other.r);
        break;
      case STRING_VALUE:
        set(*other.string_);
        break;
      case LIST_VALUE:
        set(other.list_);
        break;
      case ERROR_VALUE:
        set(other.error_);
        break;
      case HASH_VALUE:
        set(other.hash_);
        break;
      case MATRIX_VALUE:
        set(other.matrix_);
        break;
      case MIDI_VALUE:
        set(other.midi_message_);
        break;
      case ANY_VALUE:
        set_any();
        break;
      case EMPTY_VALUE:
        /* we consider that if you set a value with empty it means you want Nil.
         * This is very useful for return values. */
        /* continue */
      case NIL_VALUE:
        /* continue */
      default:
        set_nil();
    }
    return *this;
  }

  /** Copy the content of the other value. */
  void operator=(const Value& other) {
    set(other);
  }

  const char *type_tag() const {
    switch (type_) {
      case NIL_VALUE:    return "N";
      case REAL_VALUE:   return "f";
      case ERROR_VALUE:  return "E";
      case STRING_VALUE: return "s";
      case HASH_VALUE:   return "H";
      case MATRIX_VALUE: return "M";
      case MIDI_VALUE:   return "m";
      case LIST_VALUE:   return list_->type_tag();
      case ANY_VALUE:    return "*";
      case EMPTY_VALUE:  /* continue */
      default:           return "";
    }
  }

  TypeTagID type_id() const {
    switch (type_) {
      case NIL_VALUE:    return H("N");
      case REAL_VALUE:   return H("f");
      case ERROR_VALUE:  return H("E");
      case STRING_VALUE: return H("s");
      case HASH_VALUE:   return H("H");
      case MATRIX_VALUE: return H("M");
      case MIDI_VALUE:   return H("m");
      case LIST_VALUE:   return list_->type_id();
      case ANY_VALUE:    return H("*");
      case EMPTY_VALUE:  /* continue */
      default:           return NO_TYPE_TAG_ID;  // EMPTY_VALUE
    }
  }

  ValueType type() const {
    return type_;
  }

  /** Build the content of the value from the value to nil.
   *  @return number of characters eaten in the buffer to build Value.
   */
  size_t build_from_json(const char *json, bool strict_mode = false);

  /** Return a string representation of the value. */
  Json to_json() const;

  /** Return a lazy json string representation of the value. */
  Json lazy_json() const;

  /** Change the Value into the specific type. Since a default value must be set,
    * it is better to use 'set'. */
  void set_type(ValueType type) {
    set_type_without_default(type);
    set_default();
  }

  /** Change the Value into something defined in a typeTag. */
  const char *set_type_tag(const char *type_tag) {
    if (strlen(type_tag) > 1) {
      set_type_without_default(LIST_VALUE);
      list_ = new List;
      return list_->set_type_tag(type_tag);
    } else {
      set_type(type_from_char(type_tag[0]));
      return type_tag+1; // eof
    }
  }


  static ValueType type_from_char(char c) {
    switch (c) {
      case REAL_TYPE_TAG:   return REAL_VALUE;
      case STRING_TYPE_TAG: return STRING_VALUE;
      // ERROR_TYPE_TAG == STRING_TYPE_TAG;
      case HASH_TYPE_TAG:   return HASH_VALUE;
      case MATRIX_TYPE_TAG: return MATRIX_VALUE;
      case MIDI_MESSAGE_TYPE_TAG: return MIDI_VALUE;
      case ANY_TYPE_TAG:    return ANY_VALUE;
      case NIL_TYPE_TAG:    return NIL_VALUE;
      default:              return EMPTY_VALUE;
    }
  }


  /** =========================================================    Empty   */
  bool is_empty() const { return type_ == EMPTY_VALUE; }

  /** Change the value to nil. */
  Value &set_empty() {
    set_type_without_default(EMPTY_VALUE);
    return *this;
  }

  /** =========================================================    Nil     */
  bool is_nil() const    { return type_ == NIL_VALUE; }

  /** Change the value to nil. */
  Value &set_nil() {
    set_type_without_default(NIL_VALUE);
    return *this;
  }

  /** =========================================================    Real    */
  bool is_real() const  { return type_ == REAL_VALUE; }
  bool is_true() const  { return type_ == REAL_VALUE && r == 1.0; }
  bool is_false() const { return type_ == REAL_VALUE && r == 0.0; }

  /** Change the Value into a RealValue. */
  Value &set(Real real) {
    set_type_without_default(REAL_VALUE);
    r = real;
    return *this;
  }

  /** =========================================================    String  */
  bool is_string() const { return type_ == STRING_VALUE; }

  /** Change the Value into a StringValue. */
  Value &set(const char *string) {
    set_type_without_default(STRING_VALUE);
    set_string(string);
    return *this;
  }

  /** Change the Value into a StringValue. */
  Value &set(const std::string &string) {
    set_type_without_default(STRING_VALUE);
    set_string(string);
    return *this;
  }

  Value& append(const std::string &string) {
    return append(string.c_str());
  }

  Value& append(const char *str) {
    if (is_error()) {
      error_->append(str);
    } else if (is_string()) {
      string_->append(str);
    }
    return *this;
  }

  inline const std::string &str() const {
    return *string_;
  }

  inline std::string &str() {
    return *string_;
  }

  inline const char *c_str() const {
    return string_->c_str();
  }

  /** =========================================================    List    */
  bool is_list() const   { return type_ == LIST_VALUE; }

  /** Change the Value into a List by copying the content of the argument. */
  Value &set(const List *list) {
    set_type_without_default(LIST_VALUE);
    set_list(list);
    return *this;
  }

  /** Change the Value into a List by copying the content of the argument. */
  Value &set(const List &list) {
    set_type_without_default(LIST_VALUE);
    set_list(&list);
    return *this;
  }

  const Value &operator[](size_t pos) const {
    return *((*list_)[pos]);
  }

  Value &operator[](size_t pos) {
    return *((*list_)[pos]);
  }

  const Value &value_at(size_t pos) const {
    return *((*list_)[pos]);
  }

  Value &value_at(size_t pos) {
    return *((*list_)[pos]);
  }

  const Value &last() const {
    if (is_list()) {
      Value *last = list_->last();
      return last ? *last : gNilValue;
    } else {
      return *this;
    }
  }

  const Value &first() const {
    if (is_list()) {
      Value *first = list_->first();
      return first ? *first : gNilValue;
    } else {
      return *this;
    }
  }

  void set_value_at(size_t pos, const Value &val) {
    if (!is_list()) return;
    list_->set_value_at(pos, val);
  }

  size_t size() const {
    return type_ == LIST_VALUE ? list_->size() : 0;
  }

  template<class T>
  Value &push_back(const T& elem) {
    return push_back(Value(elem));
  }

  Value &push_back(const Value& val);

  template<class T>
  Value &push_front(const T& elem) {
    return push_front(Value(elem));
  }

  Value &push_front(const Value& val);

  /** =========================================================    Error   */
  bool is_error() const  { return type_ == ERROR_VALUE; }

  /** Change the Value into an ErrorValue. */
  Value &set(ErrorCode code, const char *string) {
    set_type_without_default(ERROR_VALUE);
    set_error(code, string);
    return *this;
  }

  /** Change the Value into an ErrorValue. */
  Value &set(ErrorCode code, const std::string &string) {
    set_type_without_default(ERROR_VALUE);
    set_error(code, string.c_str());
    return *this;
  }

  /** Change the Value into a ListValue by copying the content of the argument. */
  Value &set(const Error *error) {
    set_type_without_default(ERROR_VALUE);
    set_error(error);
    return *this;
  }

  /** Change the Value into a ListValue by copying the content of the argument. */
  Value &set(const Error &error) {
    set_type_without_default(ERROR_VALUE);
    set_error(&error);
    return *this;
  }

  const std::string& error_message() const {
    return error_->message();
  }

  ErrorCode error_code() const {
    return error_->code();
  }

  /** =========================================================    Hash    */
  bool is_hash() const   { return type_ == HASH_VALUE; }

  /** Change the Value into a HashValue by copying the content of the argument. */
  Value &set(const Hash *hash) {
    set_type_without_default(HASH_VALUE);
    set_hash(hash);
    return *this;
  }

  /** Change the Value into a HashValue by copying the content of the argument. */
  Value &set(const Hash &hash) {
    set_type_without_default(HASH_VALUE);
    set_hash(&hash);
    return *this;
  }

  template<class T>
  void set(const char *key, const T &val) {
    set(std::string(key), Value(val));
  }

  template<class T>
  void set(const std::string &key, const T &val) {
    set(key, Value(val));
  }

  void set(const std::string &key, const Value &val) {
    if (!is_hash()) set_type(HASH_VALUE);
    hash_->set(key, val);
  }

  bool get(const std::string &key, Value *retval) const {
    if (!is_hash()) return false;
    // TODO: how to make sure the retval is not changed ?
    return hash_->get(key, retval);
  }

  HashIterator begin() const {
    return is_hash() ? hash_->begin() : gEmptyHash.begin();
  }

  HashIterator end() const {
    return is_hash() ? hash_->end() : gEmptyHash.end();
  }

  Value operator[](const std::string &key) {
    if (!is_hash()) return gNilValue;
    Value res;
    if (get(key, &res)) {
      return res;
    } else {
      return gNilValue;
    }
  }

  Value operator[](const char * &key) {
    if (!is_hash()) return gNilValue;
    Value res;
    if (get(key, &res)) {
      return res;
    } else {
      return gNilValue;
    }
  }

  /** =========================================================    Matrix  */
  bool is_matrix() const   { return type_ == MATRIX_VALUE; }

  /** Change the Value into a MatrixValue by making a reference to the argument. */
  Value &set(const Matrix *matrix) {
    set_type_without_default(MATRIX_VALUE);
    set_matrix(matrix);
    return *this;
  }

  /** Change the Value into a MatrixValue by making a reference to the argument. */
  Value &set(const Matrix &matrix) {
    set_type_without_default(MATRIX_VALUE);
    set_matrix(&matrix);
    return *this;
  }

  size_t mat_size() const {
    return is_matrix() ? matrix_->rows() * matrix_->cols() : 0;
  }

  int mat_type() const {
    return is_matrix() ? matrix_->type() : 0;
  }

  Real * mat_data() const {
    return is_matrix() ? (Real*)matrix_->data() : NULL;
  }

  /** =========================================================    Midi  */
  bool is_midi() const   { return type_ == MIDI_VALUE; }

  /** Change the Value into a MidiValue by making a reference
   *  to the argument. */
  Value &set(const MidiMessage *midi_message) {
    set_type_without_default(MIDI_VALUE);
    set_midi(midi_message);
    return *this;
  }

  /** Change the Value into a MidiMessage by making a reference
   *  to the argument. */
  Value &set(const MidiMessage &midi_message) {
    set_type_without_default(MIDI_VALUE);
    set_midi(&midi_message);
    return *this;
  }

  /** Change the Value into a Midi note.
   */
  void set_as_note(unsigned char note, unsigned char velocity = 80,
    unsigned int length = 500, unsigned int channel = 1, time_t wait = 0) {
    if (!is_midi()) set_type(MIDI_VALUE);
    midi_message_->set_as_note(note, velocity, length, channel, wait);
  }

  /** Change the Value into a Midi control change.
   */
  void set_as_ctrl(unsigned char ctrl, unsigned char ctrl_value,
    unsigned int channel = 1, time_t wait = 0) {
    if (!is_midi()) set_type(MIDI_VALUE);
    midi_message_->set_as_ctrl(ctrl, ctrl_value, channel, wait);
  }

  /** =========================================================    Any     */
  bool is_any() const    { return type_ == ANY_VALUE; }

  /** Change the value to nil. */
  Value &set_any() {
    set_type_without_default(ANY_VALUE);
    return *this;
  }

  /** Create a value by parsing a JSON string. */
  Value &set(const Json &json) {
    set_type_without_default(NIL_VALUE);
    build_from_json(json.c_str());
    return *this;
  }

  /** Print value into the given stream.
   *  @param out_stream std stream to print value in
   *  @param lazy print mode (lazy = unparsable json but can be easier to read)
   */
  void to_stream(std::ostream &out_stream, bool lazy = false) const;

 private:
  /** Set the value to nil and release/free contained data. */
  void clear()  {
   switch (type_) {
   case LIST_VALUE:
     list_ = ReferenceCounted::release(list_);
     break;
   case STRING_VALUE:
     string_ = ReferenceCounted::release(string_);
     break;
   case ERROR_VALUE:
     error_ = ReferenceCounted::release(error_);
     break;
   case HASH_VALUE:
     hash_ = ReferenceCounted::release(hash_);
     break;
   case MATRIX_VALUE:
       // TODO: Same reference counting as others by using cv::Mat header counter ?
     if (matrix_ != NULL) delete matrix_;
     matrix_ = NULL;
     break;
   case MIDI_VALUE:
     midi_message_ = ReferenceCounted::release(midi_message_);
     break;
   default:
     ; // nothing to clear
   }
  }

  /** Change the Value into the specific type. Does not set any default value
    * so the object must be considered uninitialized. */
  void set_type_without_default(ValueType type) {
    clear();
    type_ = type;
  }

  /** Properly initialize the type with a default value. */
  inline void set_default() {
    switch (type_) {
      case REAL_VALUE:
        r = 0.0;
        break;
      case STRING_VALUE:
        set_string("");
        break;
      case LIST_VALUE:
        list_ = new List;
        break;
      case ERROR_VALUE:
        error_ = new Error;
        break;
      case HASH_VALUE:
        hash_ = new Hash(DEFAULT_HASH_TABLE_SIZE);
        break;
      case MATRIX_VALUE:
        matrix_ = new Matrix;
        break;
      case MIDI_VALUE:
        midi_message_ = new MidiMessage;
        break;
      default:
        ; // nothing to set
    }
  }


  /** =========================================================    String  */
  void share(const StringData *string) {
    if (type_ == STRING_VALUE && string_ == string) return;
    set_type_without_default(STRING_VALUE);
    // FIXME: there should be a way to deal with shared content
    // that is protected from changes... Any solution welcome !!
    string_ = ReferenceCounted::acquire(const_cast<StringData*>(string));
  }

  void set_string(const char *string) {
    string_ = new StringData(string);
  }

  void set_string(const std::string &string) {
    string_ = new StringData(string);
  }

  /** =========================================================    Error   */
  void share(const Error *error) {
    if (type_ == ERROR_VALUE && error_ == error) return;
    set_type_without_default(ERROR_VALUE);
    // FIXME: there should be a way to deal with shared content
    // that is protected from changes... Any solution welcome !!
    error_ = ReferenceCounted::acquire(const_cast<Error*>(error));
  }

  /** Set error content. */
  void set_error(ErrorCode code, const char *string) {
    error_ = new Error(code, string);
  }

  /** Set error content. */
  void set_error(const Error *error) {
    error_ = new Error(*error);
  }

  /** =========================================================    List    */
  void share(const List *list) {
    if (type_ == LIST_VALUE && list_ == list) return;
    set_type_without_default(LIST_VALUE);
    // FIXME: there should be a way to deal with shared content
    // that is protected from changes... Any solution welcome !!
    list_ = ReferenceCounted::acquire(const_cast<List*>(list));
  }

  /** Set List content by making a copy. */
  void set_list(const List *list) {
    list_ = new List(*list);
  }

  /** =========================================================    Hash    */
  void share(const Hash *hash) {
    if (type_ == HASH_VALUE && hash_ == hash) return;
    set_type_without_default(HASH_VALUE);
    // FIXME: there should be a way to deal with shared content
    // that is protected from changes... Any solution welcome !!
    hash_ = ReferenceCounted::acquire(const_cast<Hash*>(hash));
  }

  /** Set hash content. */
  void set_hash(const Hash *hash) {
    hash_ = new Hash(*hash);
  }

  /** =========================================================    Matrix  */

  /** Set matrix content. */
  void set_matrix(const Matrix *matrix) {
    matrix_ = new Matrix(*matrix);
  }

  /** =========================================================    Midi    */
  void share(const MidiMessage *midi_message) {
    if (type_ == MIDI_VALUE && midi_message_ == midi_message) return;
    set_type_without_default(MIDI_VALUE);
    // FIXME: there should be a way to deal with shared content
    // that is protected from changes... Any solution welcome !!
    midi_message_ = ReferenceCounted::acquire(const_cast<MidiMessage*>(midi_message));
  }

  /** Set hash content. */
  void set_midi(const MidiMessage *midi_message) {
    midi_message_ = new MidiMessage(*midi_message);
  }

  ValueType type_;

 public:
  union {
    /** Store a single Real number.
     *  The value of 'Real' can be a float or double depending on compilation.
     */
    Real r;

    /** Pointer to a reference counted std::string.
     */
    StringData *string_;

    /** Pointer to a list of values (class List).
     *  The list is reference counted.
     */
    List *list_;

    /** Pointer to an Error (contains an error code and message).
     *  The error is reference counted.
     */
    Error *error_;

    /** Pointer to a reference counted dictionary.
     *  Use std::string as keys for the dictionary.
     */
    Hash *hash_;

    /** Pointer to an opencv matrix (cv::Mat).
     *  TODO: reference count matrix_ !
     */
    Matrix *matrix_;

    /** Pointer to a reference counted MidiMessage.
     */
    MidiMessage *midi_message_;
  };
};

class JsonValue : public Value
{
public:
  JsonValue(const char *json) : Value(Json(json)) {}
};

std::ostream &operator<< (std::ostream &out_stream, const Value &val);
} // oscit

#endif // OSCIT_INCLUDE_OSCIT_VALUE_H_
