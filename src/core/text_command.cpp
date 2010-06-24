
#line 1 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
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

// include all
#include "rubyk.h"

namespace rk {

//#define DEBUG_PARSER

#ifdef DEBUG_PARSER
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


#line 47 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
static const char _text_command_actions[] = {
	0, 1, 0, 1, 1, 1, 3, 1, 
	4, 1, 5, 1, 11, 1, 13, 1, 
	14, 1, 15, 2, 0, 1, 2, 2, 
	6, 2, 2, 7, 2, 2, 8, 2, 
	2, 9, 2, 2, 10, 2, 5, 1, 
	2, 11, 13, 2, 12, 13, 3, 0, 
	5, 1, 3, 2, 9, 13, 3, 2, 
	10, 13, 3, 3, 12, 13, 3, 5, 
	0, 1
};

static const short _text_command_key_offsets[] = {
	0, 0, 8, 11, 12, 26, 30, 35, 
	39, 47, 61, 63, 67, 70, 81, 83, 
	83, 85, 92, 96, 99, 101, 107, 108, 
	109, 110, 111, 112, 113, 114, 115, 116, 
	116, 124, 135, 137, 144, 151, 159, 159, 
	161, 161, 163, 172, 181, 194, 195, 196, 
	197, 198, 199, 200, 201, 202, 203, 212, 
	221, 230, 239, 251, 260, 269, 278, 287, 
	296, 302, 314, 318, 329, 340, 352, 353, 
	359, 371, 375, 386, 397, 409, 423, 425, 
	429, 432, 443, 445, 445, 447, 454, 455, 
	457, 463, 464, 465, 466, 467, 468, 469, 
	470, 471, 472, 472, 480, 491, 493, 500, 
	507, 515, 515, 517, 517, 519, 528, 537, 
	550, 551, 552, 553, 554, 555, 556, 557, 
	558, 559, 568, 577, 586, 595, 607, 616, 
	625, 634, 643, 652, 656, 667, 671, 672, 
	683, 697, 698, 706
};

static const char _text_command_trans_keys[] = {
	9, 10, 32, 35, 46, 47, 97, 122, 
	9, 10, 32, 10, 9, 10, 32, 40, 
	61, 95, 124, 126, 47, 57, 65, 90, 
	97, 122, 9, 32, 61, 124, 9, 32, 
	62, 65, 90, 9, 32, 65, 90, 40, 
	95, 48, 57, 65, 90, 97, 122, 34, 
	39, 41, 43, 45, 46, 47, 102, 110, 
	116, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 39, 43, 45, 102, 110, 116, 48, 
	57, 39, 92, 48, 57, 9, 32, 41, 
	44, 46, 48, 57, 9, 10, 32, 35, 
	9, 32, 35, 48, 57, 9, 32, 41, 
	44, 48, 57, 97, 108, 115, 101, 117, 
	108, 108, 114, 117, 58, 95, 47, 57, 
	65, 90, 97, 122, 9, 32, 34, 39, 
	43, 45, 102, 110, 116, 48, 57, 34, 
	92, 9, 32, 41, 46, 47, 97, 122, 
	9, 32, 41, 46, 47, 97, 122, 58, 
	95, 46, 57, 65, 90, 97, 122, 39, 
	92, 48, 57, 9, 32, 41, 46, 47, 
	48, 57, 97, 122, 47, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 41, 
	46, 47, 58, 95, 48, 57, 65, 90, 
	97, 122, 97, 108, 115, 101, 117, 108, 
	108, 114, 117, 58, 95, 97, 47, 57, 
	65, 90, 98, 122, 58, 95, 108, 47, 
	57, 65, 90, 97, 122, 58, 95, 115, 
	47, 57, 65, 90, 97, 122, 58, 95, 
	101, 47, 57, 65, 90, 97, 122, 9, 
	32, 41, 44, 58, 95, 47, 57, 65, 
	90, 97, 122, 58, 95, 117, 47, 57, 
	65, 90, 97, 122, 58, 95, 108, 47, 
	57, 65, 90, 97, 122, 58, 95, 108, 
	47, 57, 65, 90, 97, 122, 58, 95, 
	114, 47, 57, 65, 90, 97, 122, 58, 
	95, 117, 47, 57, 65, 90, 97, 122, 
	9, 32, 46, 47, 97, 122, 9, 10, 
	32, 35, 95, 126, 47, 57, 65, 90, 
	97, 122, 46, 47, 97, 122, 9, 10, 
	32, 35, 95, 47, 57, 65, 90, 97, 
	122, 9, 10, 32, 35, 95, 46, 57, 
	65, 90, 97, 122, 9, 10, 32, 35, 
	95, 126, 46, 57, 65, 90, 97, 122, 
	124, 9, 32, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 126, 47, 57, 65, 
	90, 97, 122, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 9, 10, 32, 35, 95, 46, 
	57, 65, 90, 97, 122, 9, 10, 32, 
	35, 95, 126, 46, 57, 65, 90, 97, 
	122, 34, 39, 41, 43, 45, 46, 47, 
	102, 110, 116, 48, 57, 97, 122, 34, 
	92, 9, 32, 41, 44, 9, 32, 44, 
	9, 32, 34, 39, 43, 45, 102, 110, 
	116, 48, 57, 39, 92, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 10, 48, 
	57, 9, 32, 41, 44, 48, 57, 97, 
	108, 115, 101, 117, 108, 108, 114, 117, 
	58, 95, 47, 57, 65, 90, 97, 122, 
	9, 32, 34, 39, 43, 45, 102, 110, 
	116, 48, 57, 34, 92, 9, 32, 41, 
	46, 47, 97, 122, 9, 32, 41, 46, 
	47, 97, 122, 58, 95, 46, 57, 65, 
	90, 97, 122, 39, 92, 48, 57, 9, 
	32, 41, 46, 47, 48, 57, 97, 122, 
	47, 58, 95, 48, 57, 65, 90, 97, 
	122, 9, 32, 41, 46, 47, 58, 95, 
	48, 57, 65, 90, 97, 122, 97, 108, 
	115, 101, 117, 108, 108, 114, 117, 58, 
	95, 97, 47, 57, 65, 90, 98, 122, 
	58, 95, 108, 47, 57, 65, 90, 97, 
	122, 58, 95, 115, 47, 57, 65, 90, 
	97, 122, 58, 95, 101, 47, 57, 65, 
	90, 97, 122, 9, 32, 41, 44, 58, 
	95, 47, 57, 65, 90, 97, 122, 58, 
	95, 117, 47, 57, 65, 90, 97, 122, 
	58, 95, 108, 47, 57, 65, 90, 97, 
	122, 58, 95, 108, 47, 57, 65, 90, 
	97, 122, 58, 95, 114, 47, 57, 65, 
	90, 97, 122, 58, 95, 117, 47, 57, 
	65, 90, 97, 122, 46, 47, 97, 122, 
	9, 32, 61, 95, 124, 47, 57, 65, 
	90, 97, 122, 9, 32, 61, 124, 62, 
	9, 32, 61, 95, 124, 46, 57, 65, 
	90, 97, 122, 9, 10, 32, 40, 61, 
	95, 124, 126, 46, 57, 65, 90, 97, 
	122, 10, 9, 10, 32, 35, 46, 47, 
	97, 122, 0
};

static const char _text_command_single_lengths[] = {
	0, 6, 3, 1, 8, 4, 3, 2, 
	2, 10, 2, 4, 3, 9, 2, 0, 
	0, 5, 4, 3, 0, 4, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	2, 9, 2, 5, 5, 2, 0, 2, 
	0, 0, 5, 3, 7, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	3, 3, 6, 3, 3, 3, 3, 3, 
	4, 6, 2, 5, 5, 6, 1, 4, 
	6, 2, 5, 5, 6, 10, 2, 4, 
	3, 9, 2, 0, 0, 5, 1, 0, 
	4, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 0, 2, 9, 2, 5, 5, 
	2, 0, 2, 0, 0, 5, 3, 7, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 3, 3, 3, 3, 6, 3, 3, 
	3, 3, 3, 2, 5, 4, 1, 5, 
	8, 1, 6, 0
};

static const char _text_command_range_lengths[] = {
	0, 1, 0, 0, 3, 0, 1, 1, 
	3, 2, 0, 0, 0, 1, 0, 0, 
	1, 1, 0, 0, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 1, 0, 1, 1, 3, 0, 0, 
	0, 1, 2, 3, 3, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	1, 3, 1, 3, 3, 3, 0, 1, 
	3, 1, 3, 3, 3, 2, 0, 0, 
	0, 1, 0, 0, 1, 1, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 3, 1, 0, 1, 1, 
	3, 0, 0, 0, 1, 2, 3, 3, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 1, 3, 0, 0, 3, 
	3, 0, 1, 0
};

static const short _text_command_index_offsets[] = {
	0, 0, 8, 12, 14, 26, 31, 36, 
	40, 46, 59, 62, 67, 71, 82, 85, 
	86, 88, 95, 100, 104, 106, 112, 114, 
	116, 118, 120, 122, 124, 126, 128, 130, 
	131, 137, 148, 151, 158, 165, 171, 172, 
	175, 176, 178, 186, 193, 204, 206, 208, 
	210, 212, 214, 216, 218, 220, 222, 229, 
	236, 243, 250, 260, 267, 274, 281, 288, 
	295, 301, 311, 315, 324, 333, 343, 345, 
	351, 361, 365, 374, 383, 393, 406, 409, 
	414, 418, 429, 432, 433, 435, 442, 444, 
	446, 452, 454, 456, 458, 460, 462, 464, 
	466, 468, 470, 471, 477, 488, 491, 498, 
	505, 511, 512, 515, 516, 518, 526, 533, 
	544, 546, 548, 550, 552, 554, 556, 558, 
	560, 562, 569, 576, 583, 590, 600, 607, 
	614, 621, 628, 635, 639, 648, 653, 655, 
	664, 676, 678, 686
};

static const unsigned char _text_command_indicies[] = {
	1, 2, 1, 3, 4, 5, 4, 0, 
	1, 2, 1, 0, 2, 3, 6, 7, 
	6, 8, 9, 4, 10, 11, 4, 4, 
	4, 0, 12, 12, 13, 14, 0, 15, 
	15, 16, 17, 0, 15, 15, 17, 0, 
	18, 17, 17, 17, 17, 0, 19, 20, 
	21, 22, 22, 23, 24, 26, 27, 28, 
	25, 23, 0, 30, 31, 29, 32, 32, 
	33, 34, 0, 35, 35, 36, 0, 36, 
	36, 19, 20, 22, 22, 37, 38, 39, 
	25, 0, 30, 41, 40, 40, 25, 0, 
	32, 32, 33, 34, 42, 25, 0, 43, 
	44, 43, 45, 0, 46, 46, 3, 0, 
	47, 0, 32, 32, 33, 34, 47, 0, 
	48, 0, 49, 0, 50, 0, 51, 0, 
	52, 0, 53, 0, 51, 0, 54, 0, 
	50, 0, 29, 55, 23, 23, 23, 23, 
	0, 55, 55, 56, 57, 58, 58, 60, 
	61, 62, 59, 0, 64, 65, 63, 66, 
	66, 33, 67, 68, 67, 0, 69, 69, 
	21, 23, 24, 23, 0, 55, 23, 23, 
	23, 23, 0, 63, 64, 71, 70, 70, 
	59, 0, 66, 66, 33, 72, 68, 59, 
	67, 0, 23, 55, 23, 73, 23, 23, 
	0, 66, 66, 33, 67, 74, 55, 23, 
	73, 23, 75, 0, 76, 0, 77, 0, 
	78, 0, 79, 0, 80, 0, 81, 0, 
	79, 0, 82, 0, 78, 0, 55, 23, 
	83, 23, 23, 23, 0, 55, 23, 84, 
	23, 23, 23, 0, 55, 23, 85, 23, 
	23, 23, 0, 55, 23, 86, 23, 23, 
	23, 0, 32, 32, 33, 34, 55, 23, 
	23, 23, 23, 0, 55, 23, 87, 23, 
	23, 23, 0, 55, 23, 88, 23, 23, 
	23, 0, 55, 23, 86, 23, 23, 23, 
	0, 55, 23, 89, 23, 23, 23, 0, 
	55, 23, 85, 23, 23, 23, 0, 16, 
	16, 90, 91, 90, 0, 92, 93, 92, 
	94, 90, 95, 90, 90, 90, 0, 96, 
	97, 96, 0, 92, 93, 92, 94, 96, 
	96, 96, 96, 0, 92, 93, 92, 94, 
	96, 96, 96, 96, 0, 92, 93, 92, 
	94, 90, 95, 90, 90, 90, 0, 98, 
	0, 98, 98, 99, 100, 99, 0, 101, 
	102, 101, 103, 99, 104, 99, 99, 99, 
	0, 105, 106, 105, 0, 101, 102, 101, 
	103, 105, 105, 105, 105, 0, 101, 102, 
	101, 103, 105, 105, 105, 105, 0, 101, 
	102, 101, 103, 99, 104, 99, 99, 99, 
	0, 107, 108, 109, 110, 110, 111, 112, 
	114, 115, 116, 113, 111, 0, 118, 119, 
	117, 120, 120, 121, 122, 0, 123, 123, 
	124, 0, 124, 124, 107, 108, 110, 110, 
	125, 126, 127, 113, 0, 118, 129, 128, 
	128, 113, 0, 120, 120, 121, 122, 130, 
	113, 0, 131, 0, 132, 0, 120, 120, 
	121, 122, 132, 0, 133, 0, 134, 0, 
	135, 0, 136, 0, 137, 0, 138, 0, 
	136, 0, 139, 0, 135, 0, 117, 140, 
	111, 111, 111, 111, 0, 140, 140, 141, 
	142, 143, 143, 145, 146, 147, 144, 0, 
	149, 150, 148, 151, 151, 121, 152, 153, 
	152, 0, 154, 154, 109, 111, 112, 111, 
	0, 140, 111, 111, 111, 111, 0, 148, 
	149, 156, 155, 155, 144, 0, 151, 151, 
	121, 157, 153, 144, 152, 0, 111, 140, 
	111, 158, 111, 111, 0, 151, 151, 121, 
	152, 159, 140, 111, 158, 111, 160, 0, 
	161, 0, 162, 0, 163, 0, 164, 0, 
	165, 0, 166, 0, 164, 0, 167, 0, 
	163, 0, 140, 111, 168, 111, 111, 111, 
	0, 140, 111, 169, 111, 111, 111, 0, 
	140, 111, 170, 111, 111, 111, 0, 140, 
	111, 171, 111, 111, 111, 0, 120, 120, 
	121, 122, 140, 111, 111, 111, 111, 0, 
	140, 111, 172, 111, 111, 111, 0, 140, 
	111, 173, 111, 111, 111, 0, 140, 111, 
	171, 111, 111, 111, 0, 140, 111, 174, 
	111, 111, 111, 0, 140, 111, 170, 111, 
	111, 111, 0, 175, 176, 175, 0, 177, 
	177, 178, 175, 179, 175, 175, 175, 0, 
	180, 180, 181, 14, 0, 16, 0, 177, 
	177, 178, 175, 179, 175, 175, 175, 0, 
	6, 7, 6, 8, 9, 4, 10, 11, 
	4, 4, 4, 0, 183, 182, 1, 2, 
	1, 3, 4, 5, 4, 0, 184, 0
};

static const unsigned char _text_command_trans_targs[] = {
	0, 2, 138, 3, 4, 136, 5, 138, 
	77, 6, 70, 131, 5, 6, 70, 7, 
	64, 8, 9, 10, 14, 18, 16, 32, 
	37, 17, 54, 59, 62, 10, 11, 31, 
	12, 18, 13, 12, 13, 22, 26, 29, 
	14, 15, 20, 19, 138, 3, 19, 21, 
	23, 24, 25, 11, 27, 28, 30, 33, 
	34, 39, 41, 42, 45, 49, 52, 34, 
	35, 38, 36, 32, 37, 36, 39, 40, 
	43, 44, 37, 32, 46, 47, 48, 35, 
	50, 51, 53, 55, 56, 57, 58, 60, 
	61, 63, 65, 69, 19, 138, 3, 66, 
	67, 68, 71, 72, 76, 19, 138, 3, 
	73, 74, 75, 78, 82, 86, 84, 99, 
	104, 85, 121, 126, 129, 78, 79, 98, 
	80, 86, 81, 80, 81, 89, 93, 96, 
	82, 83, 87, 138, 88, 90, 91, 92, 
	79, 94, 95, 97, 100, 101, 106, 108, 
	109, 112, 116, 119, 101, 102, 105, 103, 
	99, 104, 103, 106, 107, 110, 111, 104, 
	99, 113, 114, 115, 102, 117, 118, 120, 
	122, 123, 124, 125, 127, 128, 130, 132, 
	135, 133, 134, 70, 133, 134, 137, 139, 
	0
};

static const char _text_command_trans_actions[] = {
	15, 0, 13, 0, 1, 1, 22, 58, 
	5, 22, 22, 22, 0, 0, 0, 0, 
	0, 1, 7, 3, 3, 0, 19, 19, 
	19, 19, 19, 19, 19, 19, 3, 19, 
	37, 9, 37, 3, 3, 19, 19, 19, 
	19, 19, 19, 11, 40, 11, 0, 19, 
	19, 19, 19, 19, 19, 19, 19, 3, 
	3, 3, 19, 19, 19, 19, 19, 19, 
	3, 19, 37, 62, 62, 3, 19, 19, 
	46, 19, 46, 46, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 1, 1, 31, 50, 31, 28, 
	1, 1, 0, 1, 1, 34, 54, 34, 
	28, 1, 1, 3, 3, 0, 19, 19, 
	19, 19, 19, 19, 19, 19, 3, 19, 
	37, 9, 37, 3, 3, 19, 19, 19, 
	19, 19, 19, 43, 19, 19, 19, 19, 
	19, 19, 19, 19, 3, 3, 3, 19, 
	19, 19, 19, 19, 19, 3, 19, 37, 
	62, 62, 3, 19, 19, 46, 19, 46, 
	46, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 1, 
	1, 25, 25, 25, 0, 0, 0, 17, 
	0
};

static const char _text_command_eof_actions[] = {
	0, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 0, 0, 0
};

static const int text_command_start = 1;
static const int text_command_first_final = 138;

static const int text_command_en_eat_line = 137;
static const int text_command_en_main = 1;


#line 46 "/Users/gaspard/git/rubyk/src/core/text_command.rl"


void TextCommand::initialize() {
  int cs;

  silent_     = false;
  clear();
  
#line 416 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
	{
	cs = text_command_start;
	}

#line 54 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
  current_state_ = cs;
}

void TextCommand::listen() {
  char buffer[1024];
  char *line = buffer; //  when not using readline, use 'buffer' for storage

  if (!silent_) *output_ << PLANET_WELCOME;

  clear();

  thread_ready();

  if (init_script_ != "") {
    parse(init_script_.c_str());
    parse("\n");
  }

  while(should_run() && getline(&line, 1023)) {
    parse(line);
    parse("\n");
    if (should_run()) saveline(line); // command was not a 'quit'
    freeline(line);
  }
}

void TextCommand::parse(const std::string &string) {
  ScopedLock lock(parse_lock_); // TODO: what happens to a lock on thread cancel ?

  const char *p  = string.data();     // data pointer
  const char *pe = p + string.size(); // past end
  const char *eof = NULL;             // FIXME: this should be set to 'pe' on the last string block...
  int cs = current_state_;            // restore machine state

  DEBUG(printf("parse:\"%s\"\n",string.c_str()));

  
#line 459 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _text_command_trans_keys + _text_command_key_offsets[cs];
	_trans = _text_command_index_offsets[cs];

	_klen = _text_command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _text_command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _text_command_indicies[_trans];
	cs = _text_command_trans_targs[_trans];

	if ( _text_command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _text_command_actions + _text_command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 91 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      DEBUG(printf("_%c_",(*p)));
      token_ += (*p); /* append */
    }
	break;
	case 1:
#line 96 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      // FIXME: this is a temporary hack until we sub parse with Value...
      parameter_string_ += (*p);
    }
	break;
	case 2:
#line 101 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(var_);}
	break;
	case 3:
#line 103 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(method_);}
	break;
	case 4:
#line 105 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(class_);}
	break;
	case 5:
#line 107 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ set_from_token(value_);}
	break;
	case 6:
#line 109 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ from_node_     = var_; }
	break;
	case 7:
#line 111 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ from_port_ = var_; }
	break;
	case 8:
#line 113 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ to_port_   = var_; }
	break;
	case 9:
#line 119 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      to_node_   = var_;
      create_link();
    }
	break;
	case 10:
#line 124 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      to_node_   = var_;
      remove_link();
    }
	break;
	case 11:
#line 129 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ create_instance(); }
	break;
	case 12:
#line 135 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ execute_command(); }
	break;
	case 13:
#line 139 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      clear();
    }
	break;
	case 14:
#line 143 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "# Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 137; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 15:
#line 152 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 618 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _text_command_actions + _text_command_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 14:
#line 143 "/Users/gaspard/git/rubyk/src/core/text_command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *output_ << "# Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 137; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 645 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
		}
	}
	}

	_out: {}
	}

#line 207 "/Users/gaspard/git/rubyk/src/core/text_command.rl"

//  printf("{%s}\n",p);
  current_state_ = cs;
}

void TextCommand::set_from_token(std::string &string) {
  DEBUG(if (&string == &value_) std::cout << "[val " << token_ << "]" << std::endl);
  DEBUG(if (&string == &var_)   std::cout << "[var " << token_ << "]" << std::endl);
  DEBUG(if (&string == &class_) std::cout << "[cla " << token_ << "]" << std::endl);

  string = token_;
  token_ = "";
}

void TextCommand::create_instance() {
  Value params(Json(parameter_string_.c_str()));
  if (params.is_nil()) {
    params.set_type(HASH_VALUE); // empty hash value
  }

  Value list;
  names_to_urls();

  DEBUG(std::cout << "NEW "<< var_ << " = " << class_ << "(" << params << ")");
  list.push_back(var_);
  list.push_back(params);

  Value res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/new"), list);

  Value links;
  if (res.is_string()) {
    links = root_->call(LINK_URL, gNilValue); // create pending links
  }

  if (res.is_string() && !silent_) {
    print_result(root_->call(INSPECT_URL, res));
    for (size_t i = 0; i < links.size(); ++i) {
      print_result(links[i]);
    }
  } else if (!silent_) {
    print_result(res);
  }
}

void TextCommand::change_link(char op) {
  names_to_urls();

  DEBUG(std::cout << op << " LINK " << from_node_ << "." << from_port_ << "=>" << to_port_ << "." << to_node_ << std::endl);
  ListValue list;

  if (from_port_ == "") {
    list.push_back(std::string(from_node_));
  } else {
    list.push_back(std::string(from_node_).append("/out/").append(from_port_));
  }

  if (op == 'c') {
    list.push_back("=>");
  } else {
    list.push_back("||");
  }

  if (to_port_ == "") {
    list.push_back(std::string(to_node_));
  } else {
    list.push_back(std::string(to_node_).append("/in/").append(to_port_));
  }

  print_result(root_->call(LINK_URL, list));
}

void TextCommand::execute_method() {
  Value res;
  // why doesn't this work ? Value params(Json(parameter_string_));
  Value params = Value(Json(parameter_string_));
  names_to_urls();

  DEBUG(std::cout << "METHOD " << var_ << "." << method_ << "(" << params << ")" << std::endl);

  if (method_ == "set") {
    // TODO: should 'set' live in normal tree space ?
    ObjectHandle target;
    if (root_->get_object_at(var_, &target)) {
      // FIXME: this is not correct: we should make ALL objects thread-safe.
      target->lock();
        res = target->set(params);
      target->unlock();
    } else {
      res = ErrorValue(NOT_FOUND_ERROR, var_);
    }
  } else {
    if (method_ == "b") method_ = "bang";
    var_.append("/").append(method_);
    res = root_->call(var_, params);
  }
  print_result(res);
}

void TextCommand::execute_class_method() {
  Value res;
  Value params = Value(Json(parameter_string_));

  DEBUG(std::cout << "CLASS_METHOD " << std::string(CLASS_URL).append("/").append(class_).append("/").append(method_) << "(" << params << ")" << std::endl);
  res = root_->call(std::string(CLASS_URL).append("/").append(class_).append("/").append(method_), params);
  print_result(res);
}

void TextCommand::execute_command() {
  Value res;
  Value params = Value(Json(parameter_string_));

  DEBUG(std::cout << "CMD " << method_ << "(" << params << ")" << std::endl);
  if (method_ == "lib") {
    res = root_->call(LIB_URL, params);
  } else if (method_ == "quit" || method_ == "q") {
    quit();  // Readline won't quit with a SIGTERM (see doc/prototypes/term_readline.cpp) so
             // we have to use quit() instead of kill().

    res = root_->call(QUIT_URL, gNilValue);
  } else {
    names_to_urls();
    res = root_->call(method_, params);
  }
  print_result(res);
}

void TextCommand::clear() {
  token_      = "";
  var_        = "";
  class_      = "";
  parameter_string_ = "";
  from_port_  = "";
  to_port_    = "";
}

} // rk
