// build & run with
// g++ cocoa_gl_window.m -lobjc -Wno-import -framework Cocoa -framework OpenGL -o test && ./test
#import <Cocoa/Cocoa.h> // NSOpenGLView

#include <pthread.h>
#include <string.h>

#include <stdio.h>

/* ======================== OpenGL view ======================= */


@interface SimpleGLView : NSOpenGLView {
  NSTimer *render_timer_;
  bool draw_triangle_;
  bool need_resize_;
}
- (void)start_loop;
- (void)timerFired:(id)sender;
- (void)draw_triangle:(bool)should_draw;
@end

@implementation SimpleGLView

// Synchronize buffer swaps with vertical refresh rate
- (void)prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
}

-(void)draw_triangle:(bool)should_draw {
  draw_triangle_ = should_draw;
}

-(void)start_loop {
  printf("start_loop\n");
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
- (void)drawRect:(NSRect*)rect {
  if (need_resize_) {

    printf("reshape (%i)\n", draw_triangle_);
    NSRect sceneBounds;

    [ [ self openGLContext ] update ];
    sceneBounds = [ self bounds ];
    // Reset current viewport
    glViewport( 0, 0, sceneBounds.size.width, sceneBounds.size.height );
    glMatrixMode( GL_PROJECTION );   // Select the projection matrix
    glLoadIdentity();                // and reset it
    // Calculate the aspect ratio of the view
    gluPerspective( 45.0f, sceneBounds.size.width / sceneBounds.size.height,
      0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix
    glLoadIdentity();                // and reset it
    need_resize_ = false;
  }

  printf("draw\n");
   // Clear the screen and depth buffer
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();   // Reset the current modelview matrix

   glTranslatef( -1.5f, 0.0f, -6.0f );   // Left 1.5 units, into screen 6.0

   if (draw_triangle_) {
     glColor3f(1.0f,1.0f,1.0f);
     glBegin( GL_TRIANGLES );             // Draw a triangle
       glVertex3f(  0.0f,  1.0f, 0.0f );    // Top
       glVertex3f( -1.0f, -1.0f, 0.0f );    // Bottom left
       glColor3f(0.5f,0.2f,0.0f);
       glVertex3f(  1.0f, -1.0f, 0.0f );    // Bottom right
     glEnd();                             // Done with the triangle
   } else {
     glTranslatef( 3.0f, 0.0f, 0.0f );    // Move right 3 units

     glColor3f(0.2f,0.5f,0.0f);
     glBegin( GL_QUADS );                // Draw a quad
       glVertex3f( -1.0f,  1.0f, 0.0f );   // Top left
       glVertex3f(  1.0f,  1.0f, 0.0f );   // Top right
       glVertex3f(  1.0f, -1.0f, 0.0f );   // Bottom right
       glVertex3f( -1.0f, -1.0f, 0.0f );   // Bottom left
     glEnd();                            // Quad is complete
   }
   glFlush();
}

/*
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    // Add your drawing codes here

  return kCVReturnSuccess;
}

- (void)dealloc
{
    // Release the display link
  CVDisplayLinkRelease(displayLink);

  [super dealloc];
}
*/
@end // SimpleGLView

/* ======================== open window thread ======================= */

#define VIEW_WIDTH  400
#define VIEW_HEIGHT 400

void *open_window(void *data) {
  const char *title = (const char *)data;
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  SimpleGLView *gl_view = [[SimpleGLView alloc] initWithFrame:NSMakeRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT)];

  NSRect frame;
  if (strcmp(title, "Thread 1")) {
    frame = NSMakeRect(10,   50, VIEW_WIDTH, VIEW_HEIGHT);
    [gl_view draw_triangle:true];
  } else {
    frame = NSMakeRect(VIEW_WIDTH + 50, 50, VIEW_WIDTH, VIEW_HEIGHT);
    [gl_view draw_triangle:false];
  }



  int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;
  // NSTexturedBackgroundWindowMask

  NSWindow *win = [[NSWindow alloc] initWithContentRect:frame
    styleMask:style
    backing:NSBackingStoreBuffered
    defer:NO];

//  [win setAutoresizesSubviews:YES];
  [gl_view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
  [win setTitle:[NSString stringWithUTF8String:title]];
  [[win contentView] addSubview:gl_view];
  [win makeKeyAndOrderFront:win];
  [gl_view start_loop];
  [pool release];
  [gl_view release];
}

void *app_loop(void *data) {
  pthread_t thread1, thread2;
  char *title1 = "Thread 1";
  char *title2 = "Thread 2";

  pthread_create( &thread1, NULL, open_window, (void*) title1);
  pthread_create( &thread2, NULL, open_window, (void*) title2);

  pthread_join( thread1,    NULL);
  pthread_join( thread2,    NULL);
  return NULL;
}

/* ======================== main ======================= */

int main(int argc, char *argv[]) {
  pthread_t app_thread;
  [NSApplication sharedApplication]; // ?
  pthread_create( &app_thread, NULL, app_loop, NULL);
  [NSApp run];
  sleep(4);
  [NSApp terminate:NULL];

  pthread_join( app_thread, NULL);
  return 0;
}