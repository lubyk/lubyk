namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QTabWidget.
 *
 * @see TabWidget
 * @see QTabWidget
 * @dub super: 'QWidget'
 */
class QTabWidget {
 public:
   /** Get the index of the currently selected tab.
    */
   int currentIndex();

   /** Get the number of tabs.
    */
   int count();

   /** Select a tab by position.
    */
   void setCurrentIndex(int pos);
};

} // mimas



