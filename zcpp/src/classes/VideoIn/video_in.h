/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_VIDEO_IN_H_
#define LUBYK_INCLUDE_LUBYK_VIDEO_IN_H_

#include "lubyk/oscit.h"

namespace rk {

/** This is a wrapper facade around a QTCaptureView window or equivalent.
 */
class VideoIn {
public:
  VideoIn();

  virtual ~VideoIn();

  /** Select a device from its unique device ID or name.
   * @return device description on success, an error on failure.
   */
  const Value set_device(const std::string &device_name);

  /** Stop capturing video frames.
   */
  void stop_capture();

  /** Start capturing video frames.
   */
  const Value start_capture();

  /** Open a preview window for the video signal. If the window is already open, this
   * method moves it around.
   */
  const Value open_preview(int x, int y);

  /** Close preview window.
   */
  const Value close_preview();

  /** This method can be implemented in sub-classes to do something on each frame
   * received.
   */
  virtual void frame_changed(const Value &matrix) {}

  /** List video sources.
   * You can then use the source key or value to open Video streams.
   * @return a hash describing the video in devices (key = source_id, value = description).
   */
  static const Value sources();

private:
  class Implementation;
  Implementation *impl_;
};

} // rk

#endif // LUBYK_INCLUDE_LUBYK_VIDEO_IN_H_