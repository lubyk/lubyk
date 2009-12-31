#ifndef MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
#define MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_

#include "mimas.h"
#include "m_look_and_feel.h"
#include "oscit/osc_command.h"
#include "device_browser.h"
#include "root_proxy_tree_item.h"
#include "m_root_proxy.h"
#include "m_proxy_factory.h"

class MimasWindowContent : public Component, public ButtonListener {
public:
  enum EditorMode {
    EditMode,
    ActionMode,
    FreezeMode
  };

  MimasWindowContent();
  ~MimasWindowContent();

  virtual void paint(Graphics& g);
  virtual void resized();
  virtual void buttonClicked(Button *button);

  juce_UseDebuggingNewOperator

private:
  /* ================== APP STATE    ================= */
  EditorMode editor_mode_;

  /* ================== MAIN WIDGETS ================= */

  /** Our main working tree (holds commands and proxies).
   */
  Root work_tree_;

  /* ================== MAIN WIDGETS ================= */

  /** TODO: remove: we use a custom "paint.cpp" and might make it a dylib.
   */
  MLookAndFeel mimas_look_and_feel_;

  /** Device browser (currently a TreeView, but should be replaced by a custom view
   * holding a device list, a breadcrumbs path and a method list).
   */
  DeviceBrowser *device_browser_;

  /** Border used to resize the window.
  */
  ResizableBorderComponent *border_;

  /** Main workspace and view port: this is where all the work
  * is done.
  */
  Component *workspace_;

  /** Scrollable container that contains the main workspace.
  */
  Viewport *workspace_port_;

  /** Top toolbar.
  */
  Component *toolbar_;

  /** Button to enter/exit edit mode.
  */
  TextButton *edit_mode_button_;

  // (prevent copy constructor and operator= being generated..)
  MimasWindowContent (const MimasWindowContent&);
  const MimasWindowContent& operator= (const MimasWindowContent&);
};


#endif // MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
