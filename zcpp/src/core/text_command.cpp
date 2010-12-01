
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
	2, 9, 2, 2, 10, 2, 3, 1, 
	2, 5, 1, 2, 11, 13, 2, 12, 
	13, 3, 0, 5, 1, 3, 2, 9, 
	13, 3, 2, 10, 13, 3, 3, 12, 
	13, 3, 5, 0, 1
};

static const short _text_command_key_offsets[] = {
	0, 0, 8, 11, 12, 27, 31, 36, 
	40, 48, 62, 64, 68, 71, 82, 84, 
	84, 86, 93, 97, 100, 102, 108, 109, 
	110, 111, 112, 113, 114, 115, 116, 117, 
	117, 125, 136, 138, 145, 152, 160, 160, 
	162, 162, 164, 173, 182, 195, 196, 197, 
	198, 199, 200, 201, 202, 203, 204, 213, 
	222, 231, 240, 252, 261, 270, 279, 288, 
	297, 303, 315, 319, 330, 341, 353, 354, 
	360, 372, 376, 387, 398, 410, 411, 425, 
	427, 431, 434, 445, 447, 447, 449, 456, 
	458, 464, 465, 466, 467, 468, 469, 470, 
	471, 472, 473, 473, 481, 492, 494, 501, 
	508, 516, 516, 518, 518, 520, 529, 538, 
	551, 552, 553, 554, 555, 556, 557, 558, 
	559, 560, 569, 578, 587, 596, 608, 617, 
	626, 635, 644, 653, 657, 668, 672, 673, 
	684, 699, 700, 708
};

static const char _text_command_trans_keys[] = {
	9, 10, 32, 35, 46, 47, 97, 122, 
	9, 10, 32, 10, 9, 10, 32, 33, 
	40, 61, 95, 124, 126, 47, 57, 65, 
	90, 97, 122, 9, 32, 61, 124, 9, 
	32, 62, 65, 90, 9, 32, 65, 90, 
	40, 95, 48, 57, 65, 90, 97, 122, 
	34, 39, 41, 43, 45, 46, 47, 102, 
	110, 116, 48, 57, 97, 122, 34, 92, 
	9, 32, 41, 44, 9, 32, 44, 9, 
	32, 34, 39, 43, 45, 102, 110, 116, 
	48, 57, 39, 92, 48, 57, 9, 32, 
	41, 44, 46, 48, 57, 9, 10, 32, 
	35, 9, 32, 35, 48, 57, 9, 32, 
	41, 44, 48, 57, 97, 108, 115, 101, 
	117, 108, 108, 114, 117, 58, 95, 47, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	39, 43, 45, 102, 110, 116, 48, 57, 
	34, 92, 9, 32, 41, 46, 47, 97, 
	122, 9, 32, 41, 46, 47, 97, 122, 
	58, 95, 46, 57, 65, 90, 97, 122, 
	39, 92, 48, 57, 9, 32, 41, 46, 
	47, 48, 57, 97, 122, 47, 58, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	41, 46, 47, 58, 95, 48, 57, 65, 
	90, 97, 122, 97, 108, 115, 101, 117, 
	108, 108, 114, 117, 58, 95, 97, 47, 
	57, 65, 90, 98, 122, 58, 95, 108, 
	47, 57, 65, 90, 97, 122, 58, 95, 
	115, 47, 57, 65, 90, 97, 122, 58, 
	95, 101, 47, 57, 65, 90, 97, 122, 
	9, 32, 41, 44, 58, 95, 47, 57, 
	65, 90, 97, 122, 58, 95, 117, 47, 
	57, 65, 90, 97, 122, 58, 95, 108, 
	47, 57, 65, 90, 97, 122, 58, 95, 
	108, 47, 57, 65, 90, 97, 122, 58, 
	95, 114, 47, 57, 65, 90, 97, 122, 
	58, 95, 117, 47, 57, 65, 90, 97, 
	122, 9, 32, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 126, 47, 57, 65, 
	90, 97, 122, 46, 47, 97, 122, 9, 
	10, 32, 35, 95, 47, 57, 65, 90, 
	97, 122, 9, 10, 32, 35, 95, 46, 
	57, 65, 90, 97, 122, 9, 10, 32, 
	35, 95, 126, 46, 57, 65, 90, 97, 
	122, 124, 9, 32, 46, 47, 97, 122, 
	9, 10, 32, 35, 95, 126, 47, 57, 
	65, 90, 97, 122, 46, 47, 97, 122, 
	9, 10, 32, 35, 95, 47, 57, 65, 
	90, 97, 122, 9, 10, 32, 35, 95, 
	46, 57, 65, 90, 97, 122, 9, 10, 
	32, 35, 95, 126, 46, 57, 65, 90, 
	97, 122, 10, 34, 39, 41, 43, 45, 
	46, 47, 102, 110, 116, 48, 57, 97, 
	122, 34, 92, 9, 32, 41, 44, 9, 
	32, 44, 9, 32, 34, 39, 43, 45, 
	102, 110, 116, 48, 57, 39, 92, 48, 
	57, 9, 32, 41, 44, 46, 48, 57, 
	48, 57, 9, 32, 41, 44, 48, 57, 
	97, 108, 115, 101, 117, 108, 108, 114, 
	117, 58, 95, 47, 57, 65, 90, 97, 
	122, 9, 32, 34, 39, 43, 45, 102, 
	110, 116, 48, 57, 34, 92, 9, 32, 
	41, 46, 47, 97, 122, 9, 32, 41, 
	46, 47, 97, 122, 58, 95, 46, 57, 
	65, 90, 97, 122, 39, 92, 48, 57, 
	9, 32, 41, 46, 47, 48, 57, 97, 
	122, 47, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 41, 46, 47, 58, 
	95, 48, 57, 65, 90, 97, 122, 97, 
	108, 115, 101, 117, 108, 108, 114, 117, 
	58, 95, 97, 47, 57, 65, 90, 98, 
	122, 58, 95, 108, 47, 57, 65, 90, 
	97, 122, 58, 95, 115, 47, 57, 65, 
	90, 97, 122, 58, 95, 101, 47, 57, 
	65, 90, 97, 122, 9, 32, 41, 44, 
	58, 95, 47, 57, 65, 90, 97, 122, 
	58, 95, 117, 47, 57, 65, 90, 97, 
	122, 58, 95, 108, 47, 57, 65, 90, 
	97, 122, 58, 95, 108, 47, 57, 65, 
	90, 97, 122, 58, 95, 114, 47, 57, 
	65, 90, 97, 122, 58, 95, 117, 47, 
	57, 65, 90, 97, 122, 46, 47, 97, 
	122, 9, 32, 61, 95, 124, 47, 57, 
	65, 90, 97, 122, 9, 32, 61, 124, 
	62, 9, 32, 61, 95, 124, 46, 57, 
	65, 90, 97, 122, 9, 10, 32, 33, 
	40, 61, 95, 124, 126, 46, 57, 65, 
	90, 97, 122, 10, 9, 10, 32, 35, 
	46, 47, 97, 122, 0
};

static const char _text_command_single_lengths[] = {
	0, 6, 3, 1, 9, 4, 3, 2, 
	2, 10, 2, 4, 3, 9, 2, 0, 
	0, 5, 4, 3, 0, 4, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	2, 9, 2, 5, 5, 2, 0, 2, 
	0, 0, 5, 3, 7, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	3, 3, 6, 3, 3, 3, 3, 3, 
	4, 6, 2, 5, 5, 6, 1, 4, 
	6, 2, 5, 5, 6, 1, 10, 2, 
	4, 3, 9, 2, 0, 0, 5, 0, 
	4, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 0, 2, 9, 2, 5, 5, 
	2, 0, 2, 0, 0, 5, 3, 7, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 3, 3, 3, 3, 6, 3, 3, 
	3, 3, 3, 2, 5, 4, 1, 5, 
	9, 1, 6, 0
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
	3, 1, 3, 3, 3, 0, 2, 0, 
	0, 0, 1, 0, 0, 1, 1, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 3, 1, 0, 1, 1, 
	3, 0, 0, 0, 1, 2, 3, 3, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 1, 3, 0, 0, 3, 
	3, 0, 1, 0
};

static const short _text_command_index_offsets[] = {
	0, 0, 8, 12, 14, 27, 32, 37, 
	41, 47, 60, 63, 68, 72, 83, 86, 
	87, 89, 96, 101, 105, 107, 113, 115, 
	117, 119, 121, 123, 125, 127, 129, 131, 
	132, 138, 149, 152, 159, 166, 172, 173, 
	176, 177, 179, 187, 194, 205, 207, 209, 
	211, 213, 215, 217, 219, 221, 223, 230, 
	237, 244, 251, 261, 268, 275, 282, 289, 
	296, 302, 312, 316, 325, 334, 344, 346, 
	352, 362, 366, 375, 384, 394, 396, 409, 
	412, 417, 421, 432, 435, 436, 438, 445, 
	447, 453, 455, 457, 459, 461, 463, 465, 
	467, 469, 471, 472, 478, 489, 492, 499, 
	506, 512, 513, 516, 517, 519, 527, 534, 
	545, 547, 549, 551, 553, 555, 557, 559, 
	561, 563, 570, 577, 584, 591, 601, 608, 
	615, 622, 629, 636, 640, 649, 654, 656, 
	665, 678, 680, 688
};

static const unsigned char _text_command_indicies[] = {
	1, 2, 1, 3, 4, 5, 4, 0, 
	1, 2, 1, 0, 2, 3, 6, 7, 
	6, 8, 9, 10, 4, 11, 12, 4, 
	4, 4, 0, 13, 13, 14, 15, 0, 
	16, 16, 17, 18, 0, 16, 16, 18, 
	0, 19, 18, 18, 18, 18, 0, 20, 
	21, 22, 23, 23, 24, 25, 27, 28, 
	29, 26, 24, 0, 31, 32, 30, 33, 
	33, 34, 35, 0, 36, 36, 37, 0, 
	37, 37, 20, 21, 23, 23, 38, 39, 
	40, 26, 0, 31, 42, 41, 41, 26, 
	0, 33, 33, 34, 35, 43, 26, 0, 
	44, 45, 44, 46, 0, 47, 47, 3, 
	0, 48, 0, 33, 33, 34, 35, 48, 
	0, 49, 0, 50, 0, 51, 0, 52, 
	0, 53, 0, 54, 0, 52, 0, 55, 
	0, 51, 0, 30, 56, 24, 24, 24, 
	24, 0, 56, 56, 57, 58, 59, 59, 
	61, 62, 63, 60, 0, 65, 66, 64, 
	67, 67, 34, 68, 69, 68, 0, 70, 
	70, 22, 24, 25, 24, 0, 56, 24, 
	24, 24, 24, 0, 64, 65, 72, 71, 
	71, 60, 0, 67, 67, 34, 73, 69, 
	60, 68, 0, 24, 56, 24, 74, 24, 
	24, 0, 67, 67, 34, 68, 75, 56, 
	24, 74, 24, 76, 0, 77, 0, 78, 
	0, 79, 0, 80, 0, 81, 0, 82, 
	0, 80, 0, 83, 0, 79, 0, 56, 
	24, 84, 24, 24, 24, 0, 56, 24, 
	85, 24, 24, 24, 0, 56, 24, 86, 
	24, 24, 24, 0, 56, 24, 87, 24, 
	24, 24, 0, 33, 33, 34, 35, 56, 
	24, 24, 24, 24, 0, 56, 24, 88, 
	24, 24, 24, 0, 56, 24, 89, 24, 
	24, 24, 0, 56, 24, 87, 24, 24, 
	24, 0, 56, 24, 90, 24, 24, 24, 
	0, 56, 24, 86, 24, 24, 24, 0, 
	17, 17, 91, 92, 91, 0, 93, 94, 
	93, 95, 91, 96, 91, 91, 91, 0, 
	97, 98, 97, 0, 93, 94, 93, 95, 
	97, 97, 97, 97, 0, 93, 94, 93, 
	95, 97, 97, 97, 97, 0, 93, 94, 
	93, 95, 91, 96, 91, 91, 91, 0, 
	99, 0, 99, 99, 100, 101, 100, 0, 
	102, 103, 102, 104, 100, 105, 100, 100, 
	100, 0, 106, 107, 106, 0, 102, 103, 
	102, 104, 106, 106, 106, 106, 0, 102, 
	103, 102, 104, 106, 106, 106, 106, 0, 
	102, 103, 102, 104, 100, 105, 100, 100, 
	100, 0, 108, 0, 109, 110, 111, 112, 
	112, 113, 114, 116, 117, 118, 115, 113, 
	0, 120, 121, 119, 122, 122, 123, 124, 
	0, 125, 125, 126, 0, 126, 126, 109, 
	110, 112, 112, 127, 128, 129, 115, 0, 
	120, 131, 130, 130, 115, 0, 122, 122, 
	123, 124, 132, 115, 0, 133, 0, 122, 
	122, 123, 124, 133, 0, 134, 0, 135, 
	0, 136, 0, 137, 0, 138, 0, 139, 
	0, 137, 0, 140, 0, 136, 0, 119, 
	141, 113, 113, 113, 113, 0, 141, 141, 
	142, 143, 144, 144, 146, 147, 148, 145, 
	0, 150, 151, 149, 152, 152, 123, 153, 
	154, 153, 0, 155, 155, 111, 113, 114, 
	113, 0, 141, 113, 113, 113, 113, 0, 
	149, 150, 157, 156, 156, 145, 0, 152, 
	152, 123, 158, 154, 145, 153, 0, 113, 
	141, 113, 159, 113, 113, 0, 152, 152, 
	123, 153, 160, 141, 113, 159, 113, 161, 
	0, 162, 0, 163, 0, 164, 0, 165, 
	0, 166, 0, 167, 0, 165, 0, 168, 
	0, 164, 0, 141, 113, 169, 113, 113, 
	113, 0, 141, 113, 170, 113, 113, 113, 
	0, 141, 113, 171, 113, 113, 113, 0, 
	141, 113, 172, 113, 113, 113, 0, 122, 
	122, 123, 124, 141, 113, 113, 113, 113, 
	0, 141, 113, 173, 113, 113, 113, 0, 
	141, 113, 174, 113, 113, 113, 0, 141, 
	113, 172, 113, 113, 113, 0, 141, 113, 
	175, 113, 113, 113, 0, 141, 113, 171, 
	113, 113, 113, 0, 176, 177, 176, 0, 
	178, 178, 179, 176, 180, 176, 176, 176, 
	0, 181, 181, 182, 15, 0, 17, 0, 
	178, 178, 179, 176, 180, 176, 176, 176, 
	0, 6, 7, 6, 8, 9, 10, 4, 
	11, 12, 4, 4, 4, 0, 184, 183, 
	1, 2, 1, 3, 4, 5, 4, 0, 
	185, 0
};

static const unsigned char _text_command_trans_targs[] = {
	0, 2, 138, 3, 4, 136, 5, 138, 
	77, 78, 6, 70, 131, 5, 6, 70, 
	7, 64, 8, 9, 10, 14, 18, 16, 
	32, 37, 17, 54, 59, 62, 10, 11, 
	31, 12, 18, 13, 12, 13, 22, 26, 
	29, 14, 15, 20, 19, 138, 3, 19, 
	21, 23, 24, 25, 11, 27, 28, 30, 
	33, 34, 39, 41, 42, 45, 49, 52, 
	34, 35, 38, 36, 32, 37, 36, 39, 
	40, 43, 44, 37, 32, 46, 47, 48, 
	35, 50, 51, 53, 55, 56, 57, 58, 
	60, 61, 63, 65, 69, 19, 138, 3, 
	66, 67, 68, 71, 72, 76, 19, 138, 
	3, 73, 74, 75, 138, 79, 83, 77, 
	85, 99, 104, 86, 121, 126, 129, 79, 
	80, 98, 81, 77, 82, 81, 82, 89, 
	93, 96, 83, 84, 87, 88, 90, 91, 
	92, 80, 94, 95, 97, 100, 101, 106, 
	108, 109, 112, 116, 119, 101, 102, 105, 
	103, 99, 104, 103, 106, 107, 110, 111, 
	104, 99, 113, 114, 115, 102, 117, 118, 
	120, 122, 123, 124, 125, 127, 128, 130, 
	132, 135, 133, 134, 70, 133, 134, 137, 
	139, 0
};

static const char _text_command_trans_actions[] = {
	15, 0, 13, 0, 1, 1, 22, 61, 
	37, 5, 22, 22, 22, 0, 0, 0, 
	0, 0, 1, 7, 3, 3, 0, 19, 
	19, 19, 19, 19, 19, 19, 19, 3, 
	19, 40, 9, 40, 3, 3, 19, 19, 
	19, 19, 19, 19, 11, 43, 11, 0, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	3, 3, 3, 19, 19, 19, 19, 19, 
	19, 3, 19, 40, 65, 65, 3, 19, 
	19, 49, 19, 49, 49, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 1, 1, 31, 53, 31, 
	28, 1, 1, 0, 1, 1, 34, 57, 
	34, 28, 1, 1, 46, 3, 3, 0, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	3, 19, 40, 9, 40, 3, 3, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 3, 3, 3, 
	19, 19, 19, 19, 19, 19, 3, 19, 
	40, 65, 65, 3, 19, 19, 49, 19, 
	49, 49, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	1, 1, 25, 25, 25, 0, 0, 0, 
	17, 0
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
  
#line 417 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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

  
#line 460 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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
#line 619 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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
#line 646 "/Users/gaspard/git/rubyk/src/core/text_command.cpp"
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
    links = root_->call(LINK_URL, gBangValue); // create pending links
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
    list.push_back(std::string(to_node_).append("/").append(to_port_));
  }

  print_result(root_->call(LINK_URL, list));
}

// FIXME: remove
void TextCommand::execute_method() {
  Value res;
  JsonValue params(parameter_string_);

  names_to_urls();

  DEBUG(std::cout << "METHOD " << var_ << "." << method_ << "(" << params << ")" << std::endl);

  if (method_ == "set") {
    // TODO: should 'set' live in normal tree space ?
    ObjectHandle target;
    if (root_->get_object_at(var_, &target)) {
      res = target->set(params);
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
  Value params;

  if (parameter_string_ == "!") {
    params = gBangValue;
  } else {
    params = JsonValue(parameter_string_);
  }

  DEBUG(std::cout << "CMD " << method_ << "(" << params << ")" << std::endl);
  if (method_ == "lib") {
    res = root_->call(LIB_URL, params);
  } else if (method_ == "quit" || method_ == "q") {
    quit();  // Readline won't quit with a SIGTERM (see doc/prototypes/term_readline.cpp) so
             // we have to use quit() instead of kill().

    res = root_->call(QUIT_URL, gBangValue);
  } else {
    names_to_urls();

    ObjectHandle target;
    if (root_->get_object_at(method_, &target)) {
      if (target.type_cast<Node>()) {
        // nodes do not have a trigger method
        if (params.is_hash()) {
          // set
          res = target->set(params);
        } else if (params.is_nil() || params.is_empty()) {
          res = target->trigger(gNilValue); // = inspect
        } else if (!target->first_child(&target)) {
          // use first child
          res = ErrorValue(NOT_FOUND_ERROR, method_).append(" no first child found.");
        } else {
          res = root_->call(target, params, NULL);
        }
      } else {
        // not a Node
        res = root_->call(target, params, NULL);
      }
    } else {
      // object not found, let root deal with errors
      res = root_->call(method_, params);
    }
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
