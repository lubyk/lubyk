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

#include "GLWindow/gl_window.h"
using namespace rk;

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "rubyk/planet.h"
#include "rubyk/cocoa.h"

/* ======================== OpenGLWindow =================================== */

@interface OpenGLWindow : NSWindow {
  GLWindow *gl_window_;
}
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)windowStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation glWindow:(GLWindow*)gl_window;
- (void)keyDown:(NSEvent *)event;
- (bool)canBecomeKeyWindow;
- (bool)canBecomeMainWindow;
@end

/* ======================== OpenGLWindow @implementation =================== */
@implementation OpenGLWindow
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)windowStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation glWindow:(GLWindow*)gl_window {
  gl_window_ = gl_window;
  return [self initWithContentRect:contentRect styleMask:windowStyle backing:bufferingType defer:deferCreation];
}

- (void)keyDown:(NSEvent *)event {
  std::cout << "keyDown.\n";
  if ([event keyCode] == 53) {
    // should leave fullscreen
    gl_window_->fullscreen(Value(false));
  }
}

- (void)mouseDown:(NSEvent *)event {
  // should leave fullscreen
  gl_window_->fullscreen(Value(false));
}

- (bool)canBecomeKeyWindow {
  return true;
}

- (bool)canBecomeMainWindow {
  return true;
}
@end

/* ======================== OpenGLView ===================================== */

@interface OpenGLView : NSOpenGLView {
  NSTimer *render_timer_;
  bool draw_triangle_;
  bool need_resize_;
  bool gl_thread_setup_;
  GLWindow *gl_window_;
}
- (id)initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat*)format  glWindow:(GLWindow*)gl_window;
- (void)set_timer;
- (void)timerFired:(id)sender;
- (void)draw_triangle:(bool)should_draw;
- (bool)canBecomeKeyView;
- (void)keyDown:(NSEvent *)event;
@end

/* ======================== OpenGLView @implementation ===================== */

@implementation OpenGLView

- (id)initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat*)format  glWindow:(GLWindow*)gl_window {
  gl_window_ = gl_window;
  gl_thread_setup_ = false;
  if (format == NULL) {
    // use default
    return [self initWithFrame:frame];
  } else {
    return [self initWithFrame:frame pixelFormat:format];
  }
}

// Synchronize buffer swaps with vertical refresh rate
- (void)prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
}

-(void)draw_triangle:(bool)should_draw {
  draw_triangle_ = should_draw;
}

- (bool)canBecomeKeyView {
  return true;
}

- (void)keyDown:(NSEvent *)event {
  std::cout << "key down\n";
}

-(void)set_timer {
  // this is a minimal timer in case no bangs are received (1.5s).
  render_timer_ = [[NSTimer timerWithTimeInterval:1.5
    target:self
    selector:@selector(timerFired:)
    userInfo:nil
    repeats:true] retain];

  // This code was used to trigger refresh as fast as possible without any knowledge on
  // drawing needs.

  // [[NSRunLoop currentRunLoop] addTimer:render_timer_
  //   forMode:NSDefaultRunLoopMode];
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


/* ======================== GLWindow::Implementation ======================= */

class GLWindow::Implementation {
public:
  Implementation(GLWindow *master, int x, int y, int width, int height, bool fullscreen) : fullscreen_(fullscreen), should_run_(true) {
    if (Planet::gui_ready()) {
      ScopedPool pool;
      NSOpenGLPixelFormatAttribute attrs[] = {
          // NSOpenGLPFAFullScreen,
          // NSOpenGLPFADoubleBuffer,
          NSOpenGLPFADepthSize, 32,
          0
      };
      // create pixel format
      NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

      if (format == NULL) {
        std::cerr << "Could not create pixel format (32bit depth)\n";
      }

      if (fullscreen_) {
        std::cout << "FULL\n";
        // start fullscreen mode
        NSRect screen_rect = [[NSScreen mainScreen] frame];

        // create window the same size as the screen
        window_ = [[OpenGLWindow alloc] initWithContentRect:screen_rect
          styleMask:NSBorderlessWindowMask
          backing:NSBackingStoreBuffered
          defer:YES
          glWindow:master];

        [window_ setLevel:NSMainMenuWindowLevel + 1]; // live above menu bar
        [window_ setOpaque:YES];
        [window_ setHidesOnDeactivate:NO]; // not sure this is good

        // view fits inside window
        NSRect view_rect = NSMakeRect(0.0, 0.0, screen_rect.size.width, screen_rect.size.height);
        view_ = [[OpenGLView alloc] initWithFrame:view_rect pixelFormat:format glWindow:master];
      } else {
        // windowed mode
        int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;

        window_ = [[OpenGLWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
          styleMask:style
          backing:NSBackingStoreBuffered
          defer:NO
          glWindow:master];

        view_ = [[OpenGLView alloc] initWithFrame:NSMakeRect(0, 0, width, height) pixelFormat:format glWindow:master];

        [view_ setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      }

      // the window_ now owns view_
      [window_ setContentView:view_];

      gl_thread_.start_thread<Implementation, &Implementation::run>(this, NULL);

      // display
      [window_ makeKeyAndOrderFront:nil]; // no sender

      ok_ = true;
    } else {
      ok_ = false;
    }
  }

  ~Implementation() {
    should_run_ = false;
    gl_thread_.join();
    // view_ is owned by window, do not release
    [window_ release];
  }

  void run(Thread *runner) {
    ScopedPool pool;
    runner->thread_ready();

    [view_ set_timer];

    NSRunLoop *run_loop = [NSRunLoop currentRunLoop];
    NSDate *date = [[NSDate alloc] initWithTimeIntervalSinceNow:0.1];

    while (should_run_ && [run_loop runMode:NSDefaultRunLoopMode beforeDate:date]) {
      [date release];
      date = [[NSDate alloc] initWithTimeIntervalSinceNow:0.1];
    }
  }

  void set_title(const char *title) {
    ScopedPool pool;
    [window_ setTitle:[NSString stringWithUTF8String:title]];
  }

  bool ok() {
    return ok_;
  }

  void redraw() {
    [view_ setNeedsDisplay:YES];
  }

  Thread gl_thread_;
  NSWindow *window_;
  OpenGLView *view_;
  bool fullscreen_;
  bool should_run_;
  bool ok_;
};

/* ======================== GLWindow implementation ======================== */

GLWindow::GLWindow() : impl_(NULL), should_be_fullscreen_(false) {}

bool GLWindow::create_window() {

  if (!impl_) {
    impl_ = new GLWindow::Implementation(this, x_, y_, width_, height_, should_be_fullscreen_);
  }
  return impl_->ok();
}

bool GLWindow::open_window(int x, int y, int width, int height) {
  x_ = x;
  y_ = y;
  width_  = width;
  height_ = height;
  return create_window();
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

const Value GLWindow::fullscreen(const Value &val) {
  ScopedPool pool;
  if (val.is_real()) {
    if (val.r == 1.0) {
      should_be_fullscreen_ = true;
    } else {
      should_be_fullscreen_ = false;
    }

    if (impl_ && impl_->fullscreen_ != should_be_fullscreen_) {
      // must change mode
      // TODO: it might be better to change the window behavior instead of delete and recreate
      delete impl_;
      impl_ = NULL;
      create_window();
    }

    // consider success
    return val;
  } else {
    return Value(should_be_fullscreen_);
  }
}
