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

#include "oscit/matrix.h"
#include "lua_rubyk.h"

#include <iostream>

#ifdef __macosx__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/gl.h>
#include <malloc.h>
#endif

using namespace oscit;

/** Draw a matrix in the rectangle defined by the start and end coordinates.
 */
void draw_matrix(const Matrix &mat, float start_x, float start_y, float end_x, float end_y) {
  if (mat.cols && mat.rows) {
    if (mat.type() != CV_8UC3) {
      std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
      return;
    }

    size_t rows = mat.rows;
    size_t cols = mat.cols;
    size_t row_step = mat.step1();

    float x1, y1;
    float pix_width  = (end_x - start_x) / cols;
    float pix_height = (end_y - start_y) / rows;
    unsigned char *color;
    unsigned char *row_start;
    unsigned char *data = (unsigned char *)mat.data;


    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    // glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data);

    for (size_t j = 0; j < rows; ++j) {
      row_start = data + j * row_step;
      y1 = start_y + j * pix_height;
      for (size_t i = 0; i < cols; ++i) {
        color = row_start + 3 * i;
        x1    = start_x + pix_width * i;
        glColor3f(
          color[0] / 255.0,
          color[1] / 255.0,
          color[2] / 255.0
        );
        glRectf(
          x1,
          y1,
          x1 + pix_width,
          y1 + pix_height
        );
      }
    }
  }
}


static int lua_draw_matrix(lua_State *L) {
  Matrix *mat             = *((Matrix**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);

  float start_x        = luaL_checknumber(L, 1);
  float start_y        = luaL_checknumber(L, 2);
  float end_x          = luaL_checknumber(L, 3);
  float end_y          = luaL_checknumber(L, 4);
  draw_matrix(*mat, start_x, start_y, end_x, end_y);
  return 0;
}

// void draw_spline3D(p1, p2, p3, p4, c1, c4, resolution) {
//   local resolution = rk.resolution
//   local ta, tb, tc, td
//
//   gl.LineWidth(3.0)
//   gl.Begin("LINE_STRIP")
//     for t = 0,1.0,(1.0/resolution) do
//       ta = (1-t)^3
//       tb = 3 * t * (1-t)^2
//       tc = 3 * (1-t) * t^2
//       td = t^3
//
//       gl.Color(
//         c1[1] * (1-t) + c4[1] * t,
//         c1[2] * (1-t) + c4[2] * t,
//         c1[3] * (1-t) + c4[3] * t
//       )
//       gl.Vertex(
//         ta * p1[1] + tb * p2[1] + tc * p3[1] + td * p4[1],
//         ta * p1[2] + tb * p2[2] + tc * p3[2] + td * p4[2],
//         ta * p1[3] + tb * p2[3] + tc * p3[3] + td * p4[3]
//       )
//     end
//
//     gl.Vertex(p4)
//   gl.End()
// }


static int lua_draw_spline2D(lua_State *L) {
  float p11, p12, p21, p22, p31, p32, p41, p42, c11, c12, c13, c41, c42, c43, res;
  // parameters should be (p1, p2, p3, p4, c1, c4, res)

  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TTABLE);
  luaL_checktype(L, 3, LUA_TTABLE);
  luaL_checktype(L, 4, LUA_TTABLE);
  luaL_checktype(L, 5, LUA_TTABLE);
  luaL_checktype(L, 6, LUA_TTABLE);
  luaL_checktype(L, 7, LUA_TNUMBER);

  res = lua_tonumber(L, 7);

  lua_rawgeti(L, 1, 1); // -14
  lua_rawgeti(L, 1, 2); // -13

  lua_rawgeti(L, 2, 1); // -12
  lua_rawgeti(L, 2, 2); // -11

  lua_rawgeti(L, 3, 1); // -10
  lua_rawgeti(L, 3, 2); // -9

  lua_rawgeti(L, 4, 1); // -8
  lua_rawgeti(L, 4, 2); // -7

  lua_rawgeti(L, 5, 1); // -6
  lua_rawgeti(L, 5, 2); // -5
  lua_rawgeti(L, 5, 3); // -4

  lua_rawgeti(L, 6, 1); // -3
  lua_rawgeti(L, 6, 2); // -2
  lua_rawgeti(L, 6, 3); // -1

  p11 = lua_tonumber(L, -14);
  p12 = lua_tonumber(L, -13);
  p21 = lua_tonumber(L, -12);
  p22 = lua_tonumber(L, -11);
  p31 = lua_tonumber(L, -10);
  p32 = lua_tonumber(L, -9);
  p41 = lua_tonumber(L, -8);
  p42 = lua_tonumber(L, -7);
  c11 = lua_tonumber(L, -6);
  c12 = lua_tonumber(L, -5);
  c13 = lua_tonumber(L, -4);
  c41 = lua_tonumber(L, -3);
  c42 = lua_tonumber(L, -2);
  c43 = lua_tonumber(L, -1);
  lua_pop(L, 14);

  float ta, tb, tc, td;
  if (res == 0) return 0;

  float step = 1.0 / res;

  glBegin(GL_LINE_STRIP);
  for (float t = 0; t < 1.0; t += step) {
    ta = (1-t) * (1-t) * (1-t);
    tb = 3 * t * (1-t) * (1-t);
    tc = 3 * (1-t) * t * t;
    td = t * t * t;

    glColor3f(
      c11 * (1-t) + c41 * t,
      c12 * (1-t) + c42 * t,
      c13 * (1-t) + c43 * t
    );
    glVertex2f(
      ta * p11 + tb * p21 + tc * p31 + td * p41,
      ta * p12 + tb * p22 + tc * p32 + td * p42
    );
  }
  glVertex2f(p41, p42);
  glEnd();

  return 0;
}


static const luaL_reg rklib[] = {
  {"draw_matrix",   lua_draw_matrix},
  {"draw_spline2D", lua_draw_spline2D},
  {NULL, NULL}
};

int luaopen_rubyk(lua_State *L) {
  luaL_openlib(L, "rk", rklib, 0);
  return 1;
}