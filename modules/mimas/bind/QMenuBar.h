namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QMenuBar.
 *
 * @see Menu
 * @see QWidget
 * @dub super: 'QWidget'
 */
class QMenuBar {
 public:
  void addMenu(Menu *menu);
};

} // mimas

