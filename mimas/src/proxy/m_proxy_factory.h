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

#ifndef MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#define MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#include "oscit/proxy_factory.h"
#include "m_object_proxy.h"
#include "m_views_builder.h"
#include "m_view_proxy.h"

class MimasWindowContent;

class MProxyFactory : public ProxyFactory {
public:
  MProxyFactory(MimasWindowContent *mimas, Component *main_view)
      : mimas_(mimas) {}

  virtual RootProxy *build_root_proxy(const Location &end_point) {
    return new RootProxy(end_point);
  }

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type);

private:
  MimasWindowContent *mimas_;
};

#endif // MIMAS_SRC_PROXY_M_PROXY_FACTORY_