/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef LUBYK_INCLUDE_LUBYK_PATCH_METHOD_H_
#define LUBYK_INCLUDE_LUBYK_PATCH_METHOD_H_

#include <list>

#include "oscit.h"

namespace rk {

class Planet;

/** This class implements '/views/patch' method that is a proxy for the CRUD operations on
 * the Planet. The class also implements 'save' operations.
 *
 */
class PatchMethod : public Object {
public:
  TYPED("Object.PatchMethod")

  PatchMethod(const char *name, const char *path)
      : Object(name, Oscit::hash_io("Read/write patch content.")),
        planet_(NULL),
        file_(path) {}

  PatchMethod(const std::string &name, const std::string &path)
      : Object(name, Oscit::hash_io("Read/write patch content.")),
        planet_(NULL),
        file_(path) {}

  virtual ~PatchMethod();

  /** Read/write the patch content as a hash.
   */
  virtual const Value trigger(const Value &hash);

  /** Save the patch to the filesystem. The argument is a filepath or nil.
   */
  const Value save(const Value &val);

  /** Finish initialization when the object is adopted.
   */
  virtual void adopted();

private:
  /** Direct access to planet.
   */
  Planet *planet_;

  /** File handle to save patch content.
   */
  File file_;
};

} // rk

#endif LUBYK_INCLUDE_LUBYK_PATCH_METHOD_H_