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

#include "rubyk.h"

/** The Stem script contains lua methods to implement the actual behavior of the
 * plant. We call these methods, passing every StemElement in turn with its
 * current context (surrounding array):
 *
 * The methods in the script are:
 *   <tt>move(stem)</tt>: give some life to the stem and allow it to move and grow.
 *
 * The callbacks that the script can use are:
 *   <tt>append(new_stem)</tt>: append a new stem (next_ must be NULL)
 *   <tt>branch(new_stem)</tt>: start a new branch
 *   <tt>entropy(position)</tt>:  get the chaotic energy at the given relative position.
 *                            The returned value is between 1.0 (death) and 0.0 (healing)
 */
class StemElement {

private:
  /** Absolute position of the element.
   * This is p1 when drawing Bezier curve.
   */
  double pos_[3];

  /** Control vector for the spline starting
   * at this point. p1 + ctrl_ = p2 (second Bezier control
   * point)
   */
  double ctrl_[3];

  /** RGBA colors for the stem.
   */
  double color_[4];

  /** The next element. This lets us construct points
   * p3 and p4:
   *   p3 = next_.pos - next_.ctrl_
   *   p4 = next_.pos
   */
  StemElement *next_;
};

/** This class holds information on the environment in which the stems grow. It is
 * basically an array of doubles with 0 meaning "no danger" and 1.0 "kill".
 * When the stems grow, they are also drawn on this grid so that we can avoid collisions.
 */
struct StemGrid {

  /** Grid width.
   */
  size_t width_;

  /** Grid height.
   */
  size_t height_;

  /** The actual grid.
   */
  double *grid_;
};

class Stem : public LuaScript {
public:
  // inlet 1
  void draw(const Value &val) {
    if (!is_opengl_thread()) {
      fprintf(stderr, "%s: cannot call draw (not an OpenGL thread)\n", name_.c_str());
      return;
    }

    call_lua("draw", val);
  }

  /** Open all standard libraries and openGL libraries (called by LuaScript on init).
   */
  virtual void open_lua_libs() {
    LuaScript::open_lua_libs();
    open_lua_lib("opengl", luaopen_opengl);
    open_lua_lib("glut", luaopen_glut);
  }

private:

};

extern "C" void init(Planet &planet) {
  CLASS (Stem, "Stem script.", "script: [script content] or file: [path to file]")
  // {1}
  INLET (Stem, draw, Value(Json("[0,0]")).push_back("Receives width/height from an OpenGL thread."))

  ADD_SUPER_METHOD(Stem, Script, file, StringIO("path", "Set path to script content."))
  ADD_SUPER_METHOD(Stem, Script, script, StringIO("lua code", "Script content."))
  ADD_SUPER_METHOD(Stem, Script, reload, RealIO("seconds", "How often shoudl we check file for relaod."))
}
