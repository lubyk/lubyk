namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QObject.
 *
 * @dub bind: 'QObject.yml'
 */
class QObject {
 public:
  // custom implementation in QObject.yml
  void object();
  void name();
  void setName(const char *name);
};

} // mimas
