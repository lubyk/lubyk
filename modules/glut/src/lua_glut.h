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

/* Adds some of the GLUT helpers (draw sphere, perspective) without adding the window
 and keyboard related code.

*/

#ifndef RUBYK_GL_GLUT_H_
#define RUBYK_GL_GLUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
 #ifdef BUILDING_LUAGL_DLL
   #define LUAGLUT_API __declspec(dllexport)
 #else
   #define LUAGLUT_API __declspec(dllimport)
 #endif
 #include <windows.h>
#else
 #ifdef BUILDING_LUAGL_DLL
   #define LUAGLUT_API
 #else
   #define LUAGLUT_API extern
 #endif
#endif

#include "lualib.h"
#include "lauxlib.h"


LUAGLUT_API int luaopen_glut(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif // RUBYK_GL_GLUT_H_