/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#include "rubyk/patch_method.h"

#include "rubyk/planet.h"

namespace rk {

PatchMethod::~PatchMethod() {
  // do not save
}

/** Read/write the patch content as string.
 */
const Value PatchMethod::trigger(const Value &hash) {
  return planet_ ? planet_->patch(hash) : gNilValue;
}

/** Save the patch to the filesystem. The argument is a filepath or nil.
 */
const Value PatchMethod::save(const Value &val) {
  // TODO
  return gNilValue;
}

void PatchMethod::adopted() {
  std::cout << "adopted\n";
  if ( (planet_ = TYPE_CAST(Planet, root_)) ) {
    std::cout << "adding update method\n";
    adopt(new TMethod<Planet, &Planet::update>(planet_, "update", HashIO("Hash to update patch.")));
//    adopt(new TMethod<PatchMethod, &PatchMethod::save>(this, "save", StringIO("Filepath to save patch to.")));
  } else {
    // error
  }
}

} // rk