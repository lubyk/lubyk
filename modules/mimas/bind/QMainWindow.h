namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QMenu.
 *
 * @see QWidget
 * @dub super: 'QWidget'
 *
 */
class QMainWindow {
 public:
  setMenuBar(MenuBar *bar);
};

} // mimas



