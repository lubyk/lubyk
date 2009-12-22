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

#ifndef OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_

#include "oscit/location.h"
#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "mock/logger.h"

template<class T, class U>
class TProxyFactoryLogger : public ProxyFactory, protected MockLogger {
public:
  TProxyFactoryLogger(const char *name,std::ostringstream *stream) :
                     ProxyFactory(),
                     MockLogger(name, stream) {}


  virtual RootProxy *build_root_proxy(const Location &end_point) {
    log("build_root_proxy", end_point);
    return new T(end_point, this);
  }

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type) {
    if (is_meta_method(name)) return NULL;

    log("build_object_proxy", name, type);
    return new U(name, type);
  }
};

typedef TProxyFactoryLogger<RootProxy, ObjectProxy> ProxyFactoryLogger;

#endif // OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_