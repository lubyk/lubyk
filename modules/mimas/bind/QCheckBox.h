namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QCheckBox.
 *
 * @see CheckBox
 * @see QWidget
 * @dub super: 'QWidget'
 */
class QCheckBox {
 public:
  bool isCheckable();
  void setCheckable(bool check);

  bool isChecked();
  void setChecked(bool check);
};

} // mimas

