/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
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


#ifndef RUBYK_INCLUDE_RUBYK_CONSTANTS_H_
#define RUBYK_INCLUDE_RUBYK_CONSTANTS_H_

/** This class is used to hold Rubyk constants.
 */
class Rubyk {
public:
  /** Root attribute key for object's class url.
   */
  static const char * const CLASS;

  /** Key for the list of nodes in the patch.
   */
  static const char * const NODES_KEY;

  /** Key for the Outlet's container.
   */
  static const char * const NODE_OUT_KEY;

  static const float NODE_DEFAULT_HUE;
  static const float SCRIPT_DEFAULT_HUE;

  static const float NODE_DEFAULT_WIDTH;
  static const float NODE_DEFAULT_HEIGHT;

  /** Key for script extension.
   */
  static const char * const SCRIPT_EXT;
};

#endif RUBYK_INCLUDE_RUBYK_CONSTANTS_H_