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
  GLWindow *gl_window_;
}
- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)gl_window;
- (void)run;
- (void)timerFired:(id)sender;
- (void)draw_triangle:(bool)should_draw;
@end

@implementation OpenGLView

- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)gl_window {
  gl_window_ = gl_window;
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

-(void)run {
  printf("run\n");
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

  [[NSRunLoop currentRunLoop] run];
}

// Timer callback method
- (void)timerFired:(id)sender {
  // It is good practice in a Cocoa application to allow the system to send the -drawRect:
  // message when it needs to draw, and not to invoke it directly from the timer.
  // All we do here is tell the display it needs a refresh
  [self setNeedsDisplay:YES];
}

/*
 * Initial OpenGL setup
 */
- (BOOL) initGL
{
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
    NSRect sceneBounds;

    // FIXME: what does this do ?
    [ [ self openGLContext ] update ];
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
  Implementation(GLWindow *master, int x, int y, int width, int height) {
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
    [pool release];
  }

  ~Implementation() {
    [view_ release];
    [window_ release];
  }

  void run(Thread *runner) {
    runner->thread_ready();
    [view_ run];
  }

  void set_title(const char *title) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [window_ setTitle:[NSString stringWithUTF8String:title]];
    [pool release];
  }

  bool ok() {
    return ok_;
  }

private:
  Thread gl_thread_;
  bool ok_;
  NSWindow *window_;
  OpenGLView *view_;
};

GLWindow::GLWindow() : impl_(NULL) {}

bool GLWindow::open_window(Planet *planet, int x, int y, int width, int height) {
  if (!impl_) {
    if (!planet->gui_ready()) return false;
    impl_ = new Implementation(this, x, y, width, height);
  }
  return impl_->ok();
}

void GLWindow::close_window() {
  if (impl_) delete impl_;
  impl_ = NULL;
}





