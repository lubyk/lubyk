/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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

#ifndef MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
#define MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
#include <string>

#include "mdns/location.h"
#include "rubyk/mutex.h"

namespace mdns {

/** This class let's you easily find applications providing a certain
 * service.
 */
class AbstractBrowser : public rubyk::Mutex {
  class Implementation;
  Implementation *impl_;
protected:
  /** This value is on if the browser is running (listening for new devices).
  */
  bool          running_;

  /** Protocol used in communication (usually 'rubyk').
  */
  std::string   protocol_;

  /** Service-type to browse.
  */
  std::string   service_type_;

public:
  AbstractBrowser(const char *service_type);

  virtual ~AbstractBrowser();

  /** This method is called just after a new proxy has been added to the list.
   */
  virtual void add_device(const Location &location) = 0;

  /** This method is called so that you have an opportunity to delete it cleanly.
   */
  virtual void remove_device(const char *name) = 0;

protected:

  /** This method should be called when the browser is ready.
   */
  virtual void start();

  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callbacks (add_device, remove_device) are not called in the
	 * middle of a class destruction.
	 */
  virtual void stop();

  /** Return true if the browser is running (searching for devices).
   */
  bool is_running() {
    return running_;
  }

  void set_running(bool is_running) {
    running_ = is_running;
  }

  void get_protocol_from_service_type();

};

} // mdns

#endif // MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
