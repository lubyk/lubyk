#include "planet.h"

#import <Cocoa/Cocoa.h>

// ======================================== Planet
bool Planet::s_need_gui_ = false;
Semaphore Planet::s_need_gui_semaphore_;
Semaphore Planet::s_start_gui_semaphore_;


@interface DummyThread : NSThread {
  bool done_;
}
- (id)init;
- (void)main;
- (bool)done;
@end

@implementation DummyThread
- (id)init {
  if ( (self = [super init]) ) {
    done_ = false;
  }
  return self;
}

- (bool)done {
  return done_;
}

- (void)main {
  done_ = true;
}
@end


void Planet::wait_for_gui() {
  s_need_gui_ = false;
  s_need_gui_semaphore_.acquire();
  // wait until we need a GUI or we quit

  if (s_need_gui_) {
    // Make sure Cocoa runs in multi-threaded mode
    if (![NSThread isMultiThreaded]) {
      DummyThread *dummy = [[DummyThread alloc] init];
      [dummy start];
      while (![dummy done]) // spin wait
        ;
      [dummy release];
      assert([NSThread isMultiThreaded]);
    }

    // what does sharedApplication do ?
    [NSApplication sharedApplication];

    gui_started_ = true;
    // let gui_ready return
    s_start_gui_semaphore_.release();
    [NSApp run];
  }
  join();
}

void Planet::stop_gui() {
  if (gui_started_) {
    // can we stop without such a kill all ?
    [NSApp terminate:NULL];
  } else {
    s_need_gui_semaphore_.release();
  }
}

bool Planet::gui_ready() {
  printf("gui ready\n");
  if (s_need_gui_) return true;
  s_need_gui_ = true;
  // let main thread initialize NSApplication
  s_need_gui_semaphore_.release();
  printf("gui released\n");
  // wait until initialization is finished
  s_start_gui_semaphore_.acquire();
  printf("gui out\n");
  return true;
}