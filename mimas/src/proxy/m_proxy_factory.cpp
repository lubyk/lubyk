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

#include "mimas.h"
#include "m_proxy_factory.h"

#include "mimas_window_content.h"
#include "m_device_view.h"

ObjectProxy *MProxyFactory::build_object_proxy(Object *parent, const std::string &name, const Value &type) {
  std::cout << "BUILD: " << name << " : " << type.type_tag() << "\n";
  if (parent->url() == "" && name == Url(VIEWS_PATH).name()) {
    // build views container
    return new MViewsBuilder(mimas_, name, type);
  } else if (parent->url() == VIEWS_PATH && type.type_id() == H("Hs")) {
    // build view proxy
    // TODO: move this as build_child in MViewsBuilder.
    return new MViewProxy(mimas_, name, type);
  } else if (!is_meta_method(name) || parent->url() != "") {
    return new MObjectProxy(mimas_, name, type);
  } else {
    return NULL;
  }
}