/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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
#ifndef LUBYK_INCLUDE_MDNS_CONTEXT_H_
#define LUBYK_INCLUDE_MDNS_CONTEXT_H_

#include "dub/dub.h"
#include "lk/SelectCallback.h"

#include <stdlib.h> // atoi

namespace mdns {

/** Singleton used to handle mdns queries on some platforms.
 *
 * @dub push: pushobject
 *      register: Context_core
 *      ignore: context, addSelectCallback
 */
class Context : public dub::Thread {
  class Implementation;
  Implementation *impl_;
public:
  Context();

  ~Context();

  /** @internal. Push a select callback into Lua and insert it into the event
   * loop (lk.Scheduler). This is called from C++, not from Lua.
   */
  void addSelectCallback(lk::SelectCallback *clbk, bool gc) {
    if (!dub_pushcallback("addSelectCallback")) {
      // Should never happen (callback defined in Context.lua)
      throw dub::Exception("Missing 'addSelectCallback' !");
    }
    clbk->pushobject(dub_L, clbk, "lk.SelectCallback", gc);
    // <func> <self> <clbk>
    dub_call(2, 0);
  }


  /** @internal. Platform specific context.
   */
  void *context();

  /** @internal. Used to perform initialization once Lua context
   * is properly initialized.
   */
  virtual void pushobject(lua_State *L, void *ptr, const char *type_name, bool gc = true);
};
} // mdns

#endif // LUBYK_INCLUDE_MDNS_CONTEXT_H_

