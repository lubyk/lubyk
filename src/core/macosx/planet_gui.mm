#include "planet.h"

#import <Cocoa/Cocoa.h>

// bool Planet::s_need_gui_ = false;
// Semaphore Planet::s_need_gui_semaphore_;
// Semaphore Planet::s_start_gui_semaphore_;

void Planet::wait_for_gui() {
  s_need_gui_ = false;
  printf("wait_for_gui\n");
  s_need_gui_semaphore_.acquire();
  printf("wait_for_gui done.\n");
  if (s_need_gui_) {
    printf("needs gui\n");
    // what does sharedApplication do ?
    [NSApplication sharedApplication];
    // let gui_ready return
    gui_started_ = true;
    s_start_gui_semaphore_.release();
    printf("starting\n");
    [NSApp run];
  }
    printf("gui done\n");
  join();
}

void Planet::stop_gui() {
  if (gui_started_) {
    [NSApp terminate:NULL];
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