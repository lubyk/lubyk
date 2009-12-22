/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
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

/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_OSC_ROOT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_OSC_ROOT_PROXY_H_

#include "oscit/root_proxy.h"

class IpEndpointName;

namespace oscit {
  
class OscCommand;

/** This is a root proxy that is used to communicate using an OscCommand. It manages a link
 * to a remote tree using OSC. In order to avoid dangling pointers, the OscCommand should be
 * deleted after all the associated OscRootProxy objects.
 */
class OscRootProxy : RootProxy {
public:
  OscRootProxy(OscCommand *command, const IpEndpointName *remote_endpoint);
  
  virtual ~OscRootProxy();
  
  /** This method is used to send a message to the remote tree.
   */
  virtual void send_to_remote(const std::string &url, const Value &val);
  
private:
  /** Reference to the command used to contact the real tree.
   * The command is already adopted by Root.
   */
  OscCommand *command_;
  
  /** Location of the remote tree.
   */
  IpEndpointName remote_endpoint_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_ROOT_PROXY_H_