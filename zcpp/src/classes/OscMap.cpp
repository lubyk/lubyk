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

#include <list>
#include <stdexcept> // runtime_error

#include "lubyk.h"
using namespace rk;

/** Encapsulate OscMapCommand into a Node script.
 */
class OscMap : public Node {
public:
  OscMap() : cmd_(NULL) {}

  enum Defaults {
    DefaultHue = 59,
  };

  virtual const Value init() {
    // call super
    Node::init();

    // set Script Widget
    attributes_.set(Oscit::VIEW, Oscit::WIDGET, "Script");
    attributes_.set(Oscit::VIEW, Oscit::HUE, (float)OscMap::DefaultHue);

    // start listening to incomming messages. We need to start now so that the socket is created if we need to set port number.
    cmd_ = root_->adopt_command(new OscMapCommand());
    return gNilValue;
  }

  /** Serialize object as hash: do not send script content if file path
   * information is present.
   */
  virtual const Value to_hash() {
   Value result = Node::to_hash();

   if (result.has_key("file")) {
     result.remove("script");
   }
   return result;
  }

  const Value file(const Value &val) {
    return cmd_->file(val);
  }

  const Value script(const Value &val) {
    return cmd_->script(val);
  }

  const Value reload(const Value &val) {
    return cmd_->reload(val);
  }

  const Value port(const Value &val) {
    return cmd_->port(val);
  }

  const Value reply_port(const Value &val) {
    return cmd_->reply_port(val);
  }

private:
  OscMapCommand *cmd_;
};

extern "C" void init(Planet &planet) {
  CLASS(OscMap, "Open udp ports and map calls from these ports.", "script: [mapping definitions] or file: [path to mapping file]")
  // [1]
  METHOD(OscMap, file, Oscit::string_io("File path to mappings definitions."))
  // [2]
  METHOD(OscMap, script, Oscit::string_io("Mappings definitions."))
  // [3]
  METHOD(OscMap, reload, Oscit::real_io("How often to stat file for reload [s]."))
  // [4]
  METHOD(OscMap, port, Oscit::real_io("Incomming port number."))
  // [5]
  METHOD(OscMap, reply_port, Oscit::real_io("Outgoing reply port number."))
}
