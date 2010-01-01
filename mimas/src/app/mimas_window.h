#ifndef MIMAS_SRC_APP_MIMAS_WINDOW_H_
#define MIMAS_SRC_APP_MIMAS_WINDOW_H_
#include "mimas_window_content.h"

/** Main window holding workspace.
 */
class MimasWindow : public DocumentWindow {
public:
  MimasWindow()
      : DocumentWindow (T("mimas"), Colours::lightgrey, DocumentWindow::allButtons, true) {

    setContentComponent(new MimasWindowContent(), true, true);

    centreWithSize(getWidth(), getHeight());

    setVisible(true);
  }

  ~MimasWindow() {}

  void closeButtonPressed() {
    // This window will be deleted by our HelloWorldApplication::shutdown() method
    // TODO: replace with a multi-window setup...
    JUCEApplication::quit();
  }
};

#endif // MIMAS_SRC_APP_MIMAS_WINDOW_H_