// build with
// g++ cocoa_window.m -lobjc -Wno-import -framework Cocoa -o test
#import <Cocoa/Cocoa.h>

#include <pthread.h>
#include <string.h>

void *open_window(void *data) {
  const char *title = (const char *)data;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  NSRect frame;
  if (strcmp(title, "Thread 1")) {
    frame = NSMakeRect(0,   50, 200, 100);
  } else {
    frame = NSMakeRect(350, 50, 200, 100);
  }


  int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;
  // NSTexturedBackgroundWindowMask

  NSWindow *win = [[NSWindow alloc] initWithContentRect:frame
    styleMask:style
    backing:NSBackingStoreBuffered
    defer:NO];

  [win makeKeyAndOrderFront:win];
  sleep(2);

  [NSApp terminate:NULL];
  [pool release];
 //
 // NSRect frame = NSMakeRect(0, 0, 200, 200);
 // NSWindow* window  = [[NSWindow alloc] initWithContentRect:winRect
 //                                         styleMask:NSBorderlessWindowMask
 //                                         backing:NSBackingStoreBuffered
 //                                         defer:NO];
 // [window setBackgroundColor:[NSColor blueColor]];
 // [window makeKeyAndOrderFront:NSApp];
}

int main(int argc, char *argv[]) {
  pthread_t thread1, thread2;
  [NSApplication sharedApplication]; // ?

  char *title1 = "Thread 1";
  char *title2 = "Thread 2";
  /* Create independent threads each of which will execute function */

  pthread_create( &thread1, NULL, open_window, (void*) title1);
  pthread_create( &thread2, NULL, open_window, (void*) title2);

  [NSApp run];
   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);
  return 0;

}