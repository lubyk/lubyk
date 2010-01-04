/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef MIMAS_SRC_APP_MIMAS_APPLICATION_H_
#define MIMAS_SRC_APP_MIMAS_APPLICATION_H_
#include "mimas.h"
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