////// This file IS NOT the original avcodec.h from FFmpeg. It is a /////////
////// simplified and modified version containing just what is      /////////
////// needed to make elbg work within Worker.              Gaspard  /////////

/*
 * Copyright (C) 2007 Vitor Sessak <vitor1001@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <limits.h>  // INT_MAX

// redefine av_malloc, av_free without memory alignement. Some day it could be good to use memaligned for 
// the all lubyk...

/** mem.c **/
#include <cstdlib>
inline void *av_malloc(unsigned int size)
{
  return malloc(size);
}

inline void av_free(void *ptr)
{
  if (ptr) free(ptr);
}


/** common.h **/
/* assume b>0 */
#define ROUNDED_DIV(a,b) (((a)>0 ? (a) + ((b)>>1) : (a) - ((b)>>1))/(b))

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))