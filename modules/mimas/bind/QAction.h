namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QAction.
 *
 * @see Action
 * @see QObject
 * @dub super: 'QObject'
 *      bind:  'QAction.yml'
 */
class QAction {
 public:
  void text();
  void setText(const char *text);
};

} // mimas
