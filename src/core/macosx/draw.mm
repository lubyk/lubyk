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
#include "rubyk/support/draw.h"

#include "oscit/matrix.h"


#include "rubyk/cocoa.h"

using namespace rk;

cv::Mat cv::LoadImage(const char *path) {
  ScopedPool pool;
  NSURL *url  = [NSURL fileURLWithPath:[[NSString stringWithUTF8String:path]
                                         stringByExpandingTildeInPath]];
  NSImage *image = [[NSImage alloc] initByReferencingURL:url];

  if (!image) {
    std::cerr << "Could not create image from path " << path << "\n";
    // empty cv::Mat.
    return cv::Mat();
  }

  [image autorelease];

  NSSize size = [image size];
  [image lockFocus];
    NSBitmapImageRep *image_rep = [[NSBitmapImageRep alloc] initWithFocusedViewRect:
                                    NSMakeRect(0, 0, size.width, size.height)];
  [image unlockFocus];

  if (!image_rep) {
    std::cerr << "Could not get image data from path " << path << "\n";
    // empty cv::Mat.
    return cv::Mat();
  }

  [image_rep autorelease];

  // bit depth
  if ([image_rep bitsPerSample] != 8) {
    std::cerr << "Unspupported image depth (" << [image_rep bitsPerSample] << ")\n";
    return cv::Mat();
  }

  cv::Mat matrix(size.height, size.width, CV_MAKETYPE(CV_8U, [image_rep samplesPerPixel]), [image_rep bitmapData]);

  // make sure Mat makes a copy of the data and starts managing it !
  return matrix.clone();
}
