namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QMenu.
 *
 * @see Menu
 * @see QWidget
 * @dub super: 'QWidget'
 */
class QMenu {
 public:
  void addAction(Action *action);
  /** Remove all actions.
   */
  void clear();
};

} // mimas


