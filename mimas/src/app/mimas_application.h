#ifndef MIMAS_SRC_APP_MIMAS_APPLICATION_H_
#define MIMAS_SRC_APP_MIMAS_APPLICATION_H_
#include "mimas.h"
#include "MimasWindowContent.h"
#include "mimas_window.h"


/** This is the application object that is started up when Mimas starts. It handles
 * the initialisation and shutdown of the whole application.
 */
class MimasApplication : public JUCEApplication {
public:
  MimasApplication() : mimas_window_(NULL) {}
  ~MimasApplication() {}

  /** Application ready, this is where we create objects and build up the content.
    */
  void initialise (const String& commandLine) {
    mimas_window_ = new MimasWindow();
  }

  /** Cleanup method.
   */
  void shutdown() {
    if (mimas_window_ != NULL) delete mimas_window_;
  }

  const String getApplicationName() {
      return T("Mimas");
  }

  const String getApplicationVersion() {
      return T("0.1");
  }

  bool moreThanOneInstanceAllowed() {
      return true;
  }

  void anotherInstanceStarted(const String& commandLine) {
    // we don't care
  }

private:
  /** Main worspace window.
   * TODO: replace by multiple window setup and quit when last window is closed.
   */
  MimasWindow* mimas_window_;
};


#endif // MIMAS_SRC_APP_MIMAS_APPLICATION_H_