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

/* The code below is modeled after the code from Fabio Guerra and Cleyde Marlyse (LuaGL)
 */

#include "lua_glut.h"

#include <string.h>

#ifdef __macosx__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/gl.h>
#include <malloc.h>
#endif

typedef struct glut_str_value {
  const char *str;
  void  *value;
} glut_str_value;

static const glut_str_value glut_str[] = {
   { "STROKE_ROMAN"        , GLUT_STROKE_ROMAN },
   { "STROKE_MONO_ROMAN"   , GLUT_STROKE_MONO_ROMAN },
   { 0, 0}
};

#define ENUM_ERROR (void *)-2

/** SolidCone (base, height, slices, stacks) -> none
 */
static int glut_solid_cone(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) && lua_isnumber(L, 4)) {

     /* call GLUT functions */
     glutSolidCone((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                   (GLint)lua_tonumber(L, 3), (GLint)lua_tonumber(L, 4));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.SolidCone'");
  }

  return 0;
}

/** WireCone (base, height, slices, stacks) -> none
 */
static int glut_wire_cone(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) && lua_isnumber(L, 4)) {

     /* call GLUT functions */
     glutWireCone((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                   (GLint)lua_tonumber(L, 3), (GLint)lua_tonumber(L, 4));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.WireCone'");
  }

  return 0;
}

/** SolidCube (size) -> none
 */
static int glut_solid_cube(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1)) {

     /* call GLUT functions */
     glutSolidCube((GLdouble)lua_tonumber(L, 1));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.SolidCube'");
  }

  return 0;
}

/** WireCube (size) -> none
 */
static int glut_wire_cube(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1)) {

     /* call GLUT functions */
     glutWireCube((GLdouble)lua_tonumber(L, 1));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.WireCube'");
  }

  return 0;
}

/** SolidDodecahedron () -> None
 */
static int glut_solid_dodecahedron(lua_State *L) {
  glutSolidDodecahedron();
}

/** WireDodecahedron () -> None
 */
static int glut_wire_dodecahedron(lua_State *L) {
  glutWireDodecahedron();
}

/** SolidIcosahedron () -> None
 */
static int glut_solid_icosahedron(lua_State *L) {
  glutSolidIcosahedron();
}

/** WireIcosahedron () -> None
 */
static int glut_wire_icosahedron(lua_State *L) {
  glutWireIcosahedron();
}

/** SolidOctahedron () -> None
 */
static int glut_solid_octahedron(lua_State *L) {
  glutSolidOctahedron();
}

/** WireOctahedron () -> None
 */
static int glut_wire_octahedron(lua_State *L) {
  glutWireOctahedron();
}

/** SolidSphere	(radius, slices, stacks) -> none
 */
static int glut_solid_sphere(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {

     /* call GLUT functions */
     glutSolidSphere((GLdouble)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.SolidSphere'");
  }

  return 0;
}

/** WireSphere (radius, slices, stacks) -> none
 */
 static int glut_wire_sphere(lua_State *L) {
   /* test argument type */
 if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {

    /* call GLUT functions */
    glutWireSphere((GLdouble)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3));
 } else {
   luaL_error(L, "incorrect argument to function 'glut.WireSphere'");
 }

 return 0;
}

/** SolidTeapot (size) -> none
 */
static int glut_solid_teapot(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1)) {

     /* call GLUT functions */
     glutSolidTeapot((GLdouble)lua_tonumber(L, 1));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.SolidTeapot'");
  }

  return 0;
}

/** WireTeapot (size) -> none
 */
static int glut_wire_teapot(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1)) {

     /* call GLUT functions */
     glutWireTeapot((GLdouble)lua_tonumber(L, 1));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.WireTeapot'");
  }

  return 0;
}

/** SolidTetrahedron () -> None
 */
static int glut_solid_tetrahedron(lua_State *L) {
  glutSolidTetrahedron();
}

/** WireTetrahedron () -> None
 */
static int glut_wire_tetrahedron(lua_State *L) {
  glutWireTetrahedron();
}

/** SolidTorus (innerRadius, outerRadius, nsides, rings) -> None
 */
static int glut_solid_torus(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) && lua_isnumber(L, 4)) {

     /* call GLUT functions */
     glutSolidTorus((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                   (GLint)lua_tonumber(L, 3), (GLint)lua_tonumber(L, 4));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.SolidTorus'");
  }

  return 0;
}

/** WireTorus	(innerRadius, outerRadius, nsides, rings) -> None
 */
static int glut_wire_torus(lua_State *L) {
  /* test argument type */
  if (lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
     lua_isnumber(L, 3) && lua_isnumber(L, 4)) {

     /* call GLUT functions */
     glutWireTorus((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                   (GLint)lua_tonumber(L, 3), (GLint)lua_tonumber(L, 4));
  } else {
    luaL_error(L, "incorrect argument to function 'glut.WireTorus'");
  }

  return 0;
}

void *get_glut_enum(lua_State *L, int index) {
  const char *str = lua_tostring(L, index);
  int i = 0;

  while(glut_str[i].str != 0)
  {
     if(strcmp(str, glut_str[i].str) == 0) return glut_str[i].value;

     ++i;
  }
  return ENUM_ERROR;
}

/** StrokeCharacter(font, string) -> None
 */
static int glut_stroke_character(lua_State *L) {
  void *e;

   /* test argument */
  if(!lua_isstring(L, 1) || !lua_isstring(L, 2))
    luaL_error(L, "incorrect argument to function 'gl.StrokeCharacter'");

  e = get_glut_enum(L, 1);

   /* test argument */
  if(e == ENUM_ERROR)
    luaL_error(L, "incorrect string argument to function 'gl.StrokeCharacter'");


  const char *p = lua_tostring(L, 2);
  int c;

  glPushMatrix();
    while ( (c = *p) ) {
      if (c < 32 || c > 127) {
        glPopMatrix();
        luaL_error(L, "incorrect character in function 'gl.StrokeCharacter'");
      }
      glutStrokeCharacter((void*)e, c);
      ++p;
    }
  glPopMatrix();
  return 0;
}

/** StrokeLength (font, string) -> number
 *   StrokeWidth (font, string) -> number
 */
static int glut_stroke_length(lua_State *L) {
  void *e;

   /* test argument */
  if(!lua_isstring(L, 1) || !lua_isstring(L, 2))
    luaL_error(L, "incorrect argument to function 'gl.StrokeLength'");

  e = get_glut_enum(L, 1);

   /* test argument */
  if(e == ENUM_ERROR)
    luaL_error(L, "incorrect string argument to function 'gl.StrokeLength'");


  const char *p = lua_tostring(L, 2);

  /* call opengl function and push the return value on the stack */
  lua_pushnumber(L, glutStrokeLength(e, p));

  return 1;
}

static const luaL_reg glutlib[] = {
  {"SolidCone", glut_solid_cone},
  {"WireCone", glut_wire_cone},
  {"SolidCube", glut_solid_cube},
  {"WireCube", glut_wire_cube},
  {"SolidDodecahedron", glut_solid_dodecahedron},
  {"WireDodecahedron", glut_wire_dodecahedron},
  {"SolidIcosahedron", glut_solid_icosahedron},
  {"WireIcosahedron", glut_wire_icosahedron},
  {"SolidOctahedron", glut_solid_octahedron},
  {"WireOctahedron", glut_wire_octahedron},
  {"SolidSphere", glut_solid_sphere},
  {"WireSphere", glut_wire_sphere},
  {"SolidTeapot", glut_solid_teapot},
  {"WireTeapot", glut_wire_teapot},
  {"SolidTetrahedron", glut_solid_tetrahedron},
  {"WireTetrahedron", glut_wire_tetrahedron},
  {"SolidTorus", glut_solid_torus},
  {"WireTorus", glut_wire_torus},
  {"StrokeCharacter", glut_stroke_character},
  {"StrokeWidth", glut_stroke_length},
  {"StrokeLength", glut_stroke_length},
  {NULL, NULL}
};

LUAGLUT_API int luaopen_glut(lua_State *L){
  luaL_openlib(L, "glut", glutlib, 0);
  return 1;
}