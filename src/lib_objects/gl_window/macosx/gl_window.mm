#include "gl_window.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#import <Cocoa/Cocoa.h> // NSOpenGLView

#include "planet.h"

/* ======================== OpenGL view ======================= */

@interface OpenGLView : NSOpenGLView {
  NSTimer *render_timer_;
  bool draw_triangle_;
  bool need_resize_;
  bool gl_thread_setup_;
  GLWindow *gl_window_;
}
- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)gl_window;
- (void)set_timer;
- (void)timerFired:(id)sender;
- (void)draw_triangle:(bool)should_draw;
@end

@implementation OpenGLView

- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)gl_window {
  gl_window_ = gl_window;
  gl_thread_setup_ = false;
  return [self initWithFrame:frame];
}

// Synchronize buffer swaps with vertical refresh rate
- (void)prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
}

-(void)draw_triangle:(bool)should_draw {
  draw_triangle_ = should_draw;
}

-(void)set_timer {
  // very small interval (1ms). Maximal frame rate = vertical refresh rate.
  render_timer_ = [[NSTimer timerWithTimeInterval:0.001
    target:self
    selector:@selector(timerFired:)
    userInfo:nil
    repeats:true] retain];

  [[NSRunLoop currentRunLoop] addTimer:render_timer_
    forMode:NSDefaultRunLoopMode];
  [[NSRunLoop currentRunLoop] addTimer:render_timer_
    forMode:NSEventTrackingRunLoopMode]; //Ensure timer fires during resize
}

// Timer callback method
- (void)timerFired:(id)sender {
  // It is good practice in a Cocoa application to allow the system to send the -drawRect:
  // message when it needs to draw, and not to invoke it directly from the timer.
  // All we do here is tell the display it needs a refresh
  [self setNeedsDisplay:YES];
}

/** Initial OpenGL setup
 */

- (BOOL) initGL {
  glShadeModel( GL_SMOOTH );                // Enable smooth shading
  glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );   // Black background
  glClearDepth( 1.0f );                     // Depth buffer setup
  glEnable( GL_DEPTH_TEST );                // Enable depth testing
  glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
   // Really nice perspective calculations
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  return true;
}

/*
 * Resize ourself
*/
- (void) reshape {
  need_resize_ = true;
}
/*
 * Called between system frame swapping.
 */
- (void)drawRect:(NSRect)rect {
  if (need_resize_) {
    // Declare this thread as being an OpenGL thread to allow OpenGL calls.
    if (!gl_thread_setup_) Node::set_is_opengl_thread();

    NSRect sceneBounds;

    // We need to call this method whenever the size or location changes
    [[self openGLContext] update];
    sceneBounds = [self bounds];
    gl_window_->resized(sceneBounds.size.width, sceneBounds.size.height);
    need_resize_ = false;
  }

  gl_window_->draw();
  glFlush();
}

@end // OpenGLView

/* ======================== open window thread ======================= */

class GLWindow::Implementation {
public:
  Implementation(GLWindow *master, int x, int y, int width, int height) : should_run_(true) {
    if (Planet::gui_ready()) {
      NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
      view_ = [[OpenGLView alloc] initWithFrame:NSMakeRect(0, 0, width, height) glWindow:master];

      int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;

      window_ = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
        styleMask:style
        backing:NSBackingStoreBuffered
        defer:NO];

      [view_ setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [[window_ contentView] addSubview:view_];
      [window_ makeKeyAndOrderFront:window_];
      gl_thread_.start_thread<Implementation, &Implementation::run>(this, NULL);
      ok_ = true;
      [pool drain];
    } else {
      ok_ = false;
    }
  }

  ~Implementation() {
    should_run_ = false;
    gl_thread_.join();
    [view_ release];
    [window_ release];
  }

  void run(Thread *runner) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    runner->thread_ready();

    [view_ set_timer];

    NSRunLoop *run_loop = [NSRunLoop currentRunLoop];
    NSDate *date = [[NSDate alloc] initWithTimeIntervalSinceNow:0.1];

    while (should_run_ && [run_loop runMode:NSDefaultRunLoopMode beforeDate:date]) {
      [date release];
      date = [[NSDate alloc] initWithTimeIntervalSinceNow:0.1];
    }
    [pool drain];
  }

  void set_title(const char *title) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [window_ setTitle:[NSString stringWithUTF8String:title]];
    [pool drain];
  }

  bool ok() {
    return ok_;
  }

  void redraw() {
    [view_ setNeedsDisplay:YES];
  }

private:
  Thread gl_thread_;
  bool should_run_;
  bool ok_;
  NSWindow *window_;
  OpenGLView *view_;
};

GLWindow::GLWindow() : impl_(NULL) {}

bool GLWindow::open_window(int x, int y, int width, int height) {
  if (!impl_) {
    impl_ = new Implementation(this, x, y, width, height);
  }
  return impl_->ok();
}

void GLWindow::close_window() {
  if (impl_) delete impl_;
  impl_ = NULL;
}

void GLWindow::redraw() {
  if (impl_) {
    impl_->redraw();
  }
}



