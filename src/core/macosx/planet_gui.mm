#include "rubyk/planet.h"

#import <Cocoa/Cocoa.h>

// ======================================== Planet
bool Planet::s_need_gui_ = false;
Semaphore Planet::s_need_gui_semaphore_;
Semaphore Planet::s_start_gui_semaphore_;

@interface PlanetHelper : NSThread { // < NSApplicationDelegate >
  bool done_;
  Planet *planet_;
}
- (id)initWithPlanet:(Planet*)planet;
/* ================ NSThread ===== */
/** This part is just used to put Cocoa into multi-threaded mode. */
- (void)main;
- (bool)done;
/* ================ NSApplicationDelegate ===== */
/** This part is used to properly quit when we had to install
 * an event loop.
 */
- (void)applicationWillTerminate:(NSNotification *)aNotification;
@end

@implementation PlanetHelper
- (id)initWithPlanet:(Planet*)planet {
  if ( (self = [super init]) ) {
    done_   = false;
    planet_ = planet;
  }
  return self;
}

- (bool)done {
  return done_;
}

- (void)main {
  done_ = true;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
  planet_->quit();
}

@end

static PlanetHelper *gAppHelper = NULL;

const Value Planet::quit(const Value &val) {
  if (gui_started_) {
    [NSApp terminate:NULL];
    // never reached
  } else {
    s_need_gui_semaphore_.release();
    worker_.kill();
    // clean exit
  }
  // We do not 'clear' here to avoid shooting in our own foot. Killing worker
  // ensures planet joins out in main loop and is normally deleted.
  return gNilValue;
}

// Called by NSApp on termination
void Planet::quit() {
  worker_.kill();
  // kill commands and destroy objects
  clear();
}

void Planet::wait_for_gui() {
  s_need_gui_ = false;
  s_need_gui_semaphore_.acquire();
  // wait until we need a GUI or we quit
  if (s_need_gui_ && !gAppHelper) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    gAppHelper = [[PlanetHelper alloc] initWithPlanet:this];

    // Make sure Cocoa runs in multi-threaded mode
    if (![NSThread isMultiThreaded]) {
      [gAppHelper start];
      while (![gAppHelper done]) // spin wait
        ;
      assert([NSThread isMultiThreaded]);
    }

    // Create the shared application
    [NSApplication sharedApplication];

    // Register PlanetHelper as delegate
    [NSApp setDelegate:gAppHelper];

    gui_started_ = true;
    // let gui_ready return
    s_start_gui_semaphore_.release();
    [pool drain];

    [NSApp run];
    // never reached
  }
  join();
}


bool Planet::gui_ready() {
  if (s_need_gui_) return true;
  s_need_gui_ = true;
  // let main thread initialize NSApplication
  s_need_gui_semaphore_.release();

  // wait until initialization is finished
  s_start_gui_semaphore_.acquire();

  return true;
}