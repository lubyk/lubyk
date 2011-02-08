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

#include "lubyk.h"
using namespace lk;

#include "VideoIn/video_in.h"

class VideoInNode : public Node {
public:

  VideoInNode()
      : video_in_(this),
        preview_open_(false) {}

  const Value start() {
    Value res = video_in_.start_capture();
    return res;
  }

  /** Set device id method.
   */
  const Value device(const Value &val) {
    if (val.is_string()) {
      Value res = video_in_.set_device(val.str());
      if (res.is_error()) {
        return res;
      }
      device_name_ = val.str();
    }
    return Value(device_name_);
  }

  /** Display a preview window (input value is a boolean 1/0).
   */
  const Value preview(const Value &val) {
    if (val.is_real()) {
      if (val.r == 0.0) {
        Value res = video_in_.close_preview();
        if (res.is_error()) {
          return res;
        } else {
          preview_open_ = false;
        }
      } else {
        Value res = video_in_.open_preview(10,10);
        if (res.is_error()) {
          return res;
        } else {
          preview_open_ = true;
        }
      }
    }

    return Value(preview_open_);
  }

  static const Value sources(Root *planet, const Value &val) {
    return VideoIn::sources();
  }

private:
  struct VideoInProxy : public VideoIn {
    VideoInProxy(VideoInNode *master) : master_(master) {}

    virtual void frame_changed(const Value &frame) {
      //printf("%u\n", *(frame.matrix_->data));
      master_->send(frame);
    }

    VideoInNode *master_;
  };

  VideoInProxy video_in_;
  std::string device_name_;
  bool preview_open_;
};

extern "C" void init(Planet &planet) {
  CLASS_NAMED( VideoInNode, "VideoIn", "Video input.", "device: [input device] preview: [0/1]")
  METHOD(VideoInNode, device,  Oscit::string_io("Input device name or id."))
  METHOD(VideoInNode, preview, Oscit::range_io("Open/close a preview window.", 0, 1))
  OUTLET(VideoInNode, frames,  Oscit::matrix_io("Video frames as matrix."))
  CLASS_METHOD(VideoInNode, sources, Oscit::bang_io("Return a dictionary of video in devices"))
}