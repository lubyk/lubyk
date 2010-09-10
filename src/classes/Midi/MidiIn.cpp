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
#include "rubyk.h"
using namespace rk;

#include "MidiOut/RtMidi.h"

/** This class lets you receive midi events through virtual ports (where possible)
 * or connect to midi senders and receive midi data from them.
 */
class MidiIn : public Node {
 public:
  MidiIn() : port_id_(-1), midi_in_(NULL) {
    set_is_ok(false);  // port not opened

    try {
      midi_in_ = new RtMidiIn;

      // set callback
      midi_in_->setCallback(MidiIn::s_callback, (void*)this);
    } catch (RtError &error) {
      midi_in_ = NULL;
      error_.set(UNKNOWN_ERROR, error.getMessageString());
    }
  }

  virtual ~MidiIn() {
    if (midi_in_) {
      delete midi_in_;
    }
  }

  const Value init () {
    Node::init();

    if (error_.is_error()) {
      return error_;
    } else if (!is_ok()) {
      return open_port(port_id_);
    } else {
      return gNilValue;
    }
  }

  // Send midi data out
  void receive(const Value &val) {
    if (val.is_midi()) {
      send(val);
    }
  }

  // [1] Get/set midi out port
  const Value port(const Value &val) {
    if (val.is_real()) {
      return open_port(val.r);
    } else if (val.is_string()) {
      if (midi_in_ == NULL) return error_;
      // 1. find port
      size_t port_count = midi_in_->getPortCount();
      std::string name;

      for (size_t i = 0; i < port_count; ++i) {
        try {
          name = midi_in_->getPortName(i);
          if (val.str() == name) {
            return open_port(i);
          }
        } catch (RtError &error) {
          error_.set(UNKNOWN_ERROR, error.getMessageString());
          return error_;
        }
      }
    }
    return port_id_ == -1 ? Value(name_) : Value(port_id_);
  }

  virtual void inspect(Value *hash) const {
    if (is_ok() && port_id_ >= 0) {
      std::string name;
      try {
        name = midi_in_->getPortName(port_id_);
        hash->set("port", name);
      } catch (RtError &error) {
        hash->set("port", error.getMessageString());
      }
    } else if (is_ok()) {
      hash->set("port", name_);
    } else {
      hash->set("port", "--");
    }
  }
private:

  /** Static callback to trigger when new messages arrive.
   */
  static void s_callback(double timestamp, std::vector<unsigned char> *message, void *data) {
    if (message->size() == 3) {
      MidiValue m;
      m.midi_message_->set(*message);
      ((MidiIn *)data)->receive(m);
    }
  }

  const Value open_port(int port) {
    if (midi_in_ == NULL) return error_;
    midi_in_->closePort();
    set_is_ok(false);

    if (port == -1) {
      // create a virtual port
      try {
        midi_in_->openVirtualPort(name_);
      } catch (RtError &error) {
        return error_.set(UNKNOWN_ERROR, error.getMessageString());
      }
    } else {
      // try to connect to the given port
      try {
        midi_in_->openPort( port );
      } catch (RtError &error) {
        return error_.set(UNKNOWN_ERROR, error.getMessageString());
      }
    }
    port_id_ = port;
    set_is_ok(true);
    return Value(port_id_);
  }

  // static bool input_list(std::ostream * pOutput, std::vector<std::string>& ports)
  // {
  //   RtMidiIn *midiout = 0;
  //   unsigned int i,nPorts;
  //   std::string portName;
  //
  //   try {
  //     midiout = new RtMidiIn();
  //   }
  //   catch (RtError &error) {
  //     *pOutput << error.getMessageString() << std::endl;
  //     return false;
  //   }
  //
  //   ports.clear();
  //
  //   nPorts = midiout->getPortCount();
  //
  //   for ( i=0; i<nPorts; i++ ) {
  //     try {
  //       portName = midiout->getPortName(i);
  //       ports.push_back(portName);
  //     } catch (RtError &error) {
  //       *pOutput << error.getMessageString() << std::endl;
  //       return false;
  //     }
  //   }
  //   return true;
  // }

  /** Midi port id to which the element is connected.
   *  If the value is -1 this means it has opened its own virtual port.
   */
  int port_id_;

  /** Pointer to our RtMidiIn instance (MidiIn is just a wrapper around
   *  RtMidiIn).
   */
  RtMidiIn *midi_in_;

  /** ErrorValue to store the error message that can occur during
   *  object construction.
   */
  Value error_;

  Mutex mutex_;
};

extern "C" void init(Planet &planet) {
  CLASS (MidiIn, "Receives midi values.", "port: [port number/name]")
  METHOD(MidiIn, port, Oscit::any_io("Port number or string."))
  OUTLET(MidiIn, midi, Oscit::midi_io("Midi events."))
}

