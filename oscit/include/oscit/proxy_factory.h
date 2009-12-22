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

#ifndef OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_
#define OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_

#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"

namespace oscit {

class ObjectProxy;
class RootProxy;

/** This class is used to bridge between oscit and a GUI when building a
 * 'ghost' tree that mirrors a remote 'real' tree.
 */
class ProxyFactory
{
public:
  ProxyFactory() {}

  virtual ~ProxyFactory() {}

  /** Called by ZeroConfBrowser to create a new RootProxy and pass
   *  it this object as factory.
   */
  RootProxy *build_and_init_root_proxy(const Location &location);

  /** Root proxy factory. This method should be overwritten in subclasses in order to
   *  create custom RootProxy objects. This is called by build_and_init_root_proxy.
   */
  virtual RootProxy *build_root_proxy(const Location &location) {
    return new RootProxy(location);
  }

  /** Object proxy factory. This method should be overwritten in subclasses in order to
   * create custom ObjectProxy objects.
   */
  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type) {
    return new ObjectProxy(name, type);
  }

  /** A RootProxy is using this object as factory, keep a link in case it is removed.
   */
  void register_proxy(RootProxy *proxy);

  /** Forget about this root proxy.
   */
  void unregister_proxy(RootProxy *proxy);

  static bool is_meta_method(const std::string &name) {
    return (name.size() < 1 || name.at(0) == '.');
  }

 private:

  /** List of RootProxies that use this factory. The list simply serves to remove the dependency
   *  when the ProxyFactory is deleted.
   */
  std::list<RootProxy *> root_proxies_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_