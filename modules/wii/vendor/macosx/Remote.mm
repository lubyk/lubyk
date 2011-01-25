/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#include "wii/Remote.h"
#import "WiiRemoteDiscovery.h"

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

// Rubyk wii::Remote
#include "wii/Remote.h"

// WiiRemoteFramework WiiRemote
#import <Cocoa/Cocoa.h>
#import "WiiRemote.h"

/* ======================== LWiiRemoteDelegate ===================================== */

/*
@interface NSObject (WiiRemoteDelegate)

- (void) wiimoteWillSendData;
- (void) wiimoteDidSendData;

- (void) irPointMovedX:(float) px Y:(float) py;
- (void) rawIRData: (IRData[4]) irData;
- (void) joyStickChanged:(WiiJoyStickType) type tiltX:(unsigned short) tiltX tiltY:(unsigned short) tiltY;
- (void) analogButtonChanged:(WiiButtonType) type amount:(unsigned short) press;
- (void) pressureChanged:(WiiPressureSensorType) type pressureTR:(float) bPressureTR pressureBR:(float) bPressureBR
			  pressureTL:(float) bPressureTL pressureBL:(float) bPressureBL;
- (void) batteryLevelChanged:(double) level;
- (void) wiiRemoteDisconnected:(IOBluetoothDevice*) device;
- (void) gotMiiData: (Mii*) mii_data_buf at: (int) slot;
- (void) rawPressureChanged:(WiiBalanceBoardGrid) bbData;
- (void) allPressureChanged:(WiiPressureSensorType) type bbData:(WiiBalanceBoardGrid) bbData bbDataInKg:(WiiBalanceBoardGrid) bbDataInKg;
@end
*/
@interface LWiiRemoteDelegate : NSObject {
  wii::Remote *master_;
}

- (id) initWithRemote:(wii::Remote*)master;

//////// WiiRemoteDelegate ///////////
- (void) accelerationChanged:(WiiAccelerationSensorType) type accX:(unsigned short) accX accY:(unsigned short) accY accZ:(unsigned short) accZ;
- (void) buttonChanged:(WiiButtonType) type isPressed:(BOOL) isPressed;
@end

/* ======================== LWiiRemoteDelegate @implementation ===================== */

static const char *button_name_from_type(WiiButtonType type) {
  switch (type) {
  case WiiRemoteAButton:
    return "Remote.A";
  case WiiRemoteBButton:
    return "Remote.B";
  case WiiRemoteOneButton:
    return "Remote.1";
  case WiiRemoteTwoButton:
    return "Remote.2";
  case WiiRemoteMinusButton:
    return "Remote.-";
  case WiiRemoteHomeButton:
    return "Remote.H";
  case WiiRemotePlusButton:
    return "Remote.+";
  case WiiRemoteUpButton:
    return "Remote.U";
  case WiiRemoteDownButton:
    return "Remote.D";
  case WiiRemoteLeftButton:
    return "Remote.L";
  case WiiRemoteRightButton:
    return "Remote.R";

  case WiiNunchukZButton:
    return "Nunchuk.Z";
  case WiiNunchukCButton:
    return "Nunchuk.C";

  case WiiClassicControllerXButton:
    return "Classic.X";
  case WiiClassicControllerYButton:
    return "Classic.Y";
  case WiiClassicControllerAButton:
    return "Classic.A";
  case WiiClassicControllerBButton:
    return "Classic.B";
  case WiiClassicControllerLButton:
    return "Classic.L";
  case WiiClassicControllerRButton:
    return "Classic.R";
  case WiiClassicControllerZLButton:
    return "Classic.ZL";
  case WiiClassicControllerZRButton:
    return "Classic.ZR";
  case WiiClassicControllerUpButton:
    return "Classic.U";
  case WiiClassicControllerDownButton:
    return "Classic.D";
  case WiiClassicControllerLeftButton:
    return "Classic.L";
  case WiiClassicControllerRightButton:
    return "Classic.R";
  case WiiClassicControllerMinusButton:
    return "Classic.-";
  case WiiClassicControllerHomeButton:
    return "Classic.H";
  case WiiClassicControllerPlusButton:
    return "Classic.+";
  default:
    return "???";
  }
}

@implementation LWiiRemoteDelegate
- (id) initWithRemote:(wii::Remote*)master {
  self = [super init];
  if (self) {
    master_ = master;
  }
  return self;
}


- (void) accelerationChanged:(WiiAccelerationSensorType)type accX:(unsigned short)x accY:(unsigned short)y accZ:(unsigned short)z {
  master_->acceleration(
    type == WiiRemoteAccelerationSensor ? "Remote" : "Nunchuk",
    (float)x / 0xff,
    (float)y / 0xff,
    (float)z / 0xff
  );
}

- (void) buttonChanged:(WiiButtonType) type isPressed:(BOOL) isPressed {
  master_->button(
    button_name_from_type(type),
    isPressed
  );
}

@end // LWiiRemoteDelegate


/* ======================== wii::Remote::Implementation ======================= */

class wii::Remote::Implementation
{
  wii::Remote *master_;
  WiiRemote *wii_remote_;
  LWiiRemoteDelegate *wii_remote_delegate_;
	WiiRemoteDiscovery *discovery_;
public:
  Implementation(wii::Remote *master)
   : master_(master) {
    wii_remote_delegate_ = [[LWiiRemoteDelegate alloc] initWithRemote:master_];
    wii_remote_ = [[WiiRemote alloc] init];
  	[wii_remote_ setDelegate:wii_remote_delegate_];
  }

  ~Implementation() {
    [wii_remote_ release];
    [wii_remote_delegate_ release];
  	// [discovery_ release];
  }
};

/* ======================== wii::Remote ======================== */

namespace wii {

Remote::Remote(rubyk::Worker *worker, const char *remote_name)
 : acceleration_(worker),
   button_(worker) {
  if (remote_name) name_ = remote_name;
  impl_ = new Remote::Implementation(this);
}

Remote::~Remote() {
  delete impl_;
}

} // wii
