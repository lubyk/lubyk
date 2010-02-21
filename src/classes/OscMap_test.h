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

#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/log_object.h"
#include "ip/UdpSocket.h"


#define APP1_PORT 7016
#define APP2_PORT 7017

/** In these tests, we have the following setup:
 *  planet_ : contains /v/value  + /map     (OscMap)
 *  app2_   : contains /slider/1 + app2_sender_  (OscCommand)
 *  /map receives messages on port APP1_PORT sent by app2_
 *  app1_ notifies changes to app2_ on port APP2_PORT through /map command.
 */
class OscMapTest : public ParseHelper
{
 public:
  OscMapTest() : planet_end_point_("localhot", APP1_PORT) {
    app2_sender_  = app2_.adopt_command(new OscCommand(APP2_PORT));
    Object *o = app2_.adopt(new Object("slider"));
    app2_log_ = o->adopt(new LogObject("1"));  //   /slider/1
  }
  
  void test_send_map_reply( void ) {
    setup("v=Value(4.5)\nmap=OscMap(script:'/slider/1 [0,10] --> /v/value [50,100]' port:7001 reply_port:7002)");
    Value res = planet_->call("/map/port", Value(APP1_PORT));
    assert_equal((double)APP1_PORT, res.r);
    res = planet_->call("/map/reply_port", Value(APP2_PORT));
    assert_equal((double)APP2_PORT, res.r);
    
    Object *value_method = planet_->object_at("/v/value"); // to call without passing by root
    send("/slider/1", 5.0);
    res = value_method->trigger(gNilValue);
    assert_equal(75.0, res.r);
    assert_equal("", reply());
    value_method->trigger(Value(120.0));
    assert_equal("10", reply());
  }
  
  
 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }
  
  void send(const char *url, const Value &val) {
    app2_sender_->send(planet_end_point_, url, val);
    millisleep(50);
  }
  
  std::string reply() {
    millisleep(50);
    return app2_log_->str();
  }
  
  IpEndpointName planet_end_point_;
  Root app2_;
  OscCommand *app2_sender_;
  LogObject *app2_log_;
};
