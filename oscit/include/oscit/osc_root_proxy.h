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