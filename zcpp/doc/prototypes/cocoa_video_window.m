// build & run with
// g++ cocoa_gl_window.m -lobjc -Wno-import -framework Cocoa -framework OpenGL -framework CoreVideo -o test && ./test
#import <Cocoa/Cocoa.h> // NSOpenGLView
#import <QuartzCore/QuartzCore.h> // CVDisplayLinkRef

#include <pthread.h>
#include <string.h>

#include <stdio.h>

/* ======================== OpenGL view ======================= */

@interface SimpleGLView : NSOpenGLView {
  /** display link for managing rendering thread.
   */
  CVDisplayLinkRef display_link_;
}
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime;
@end

/** This is the renderer output callback function.
 */
static CVReturn simple_gl_view_callback(
    CVDisplayLinkRef display_link,
    const CVTimeStamp *now,
    const CVTimeStamp *outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags* flagsOut,
    void *displayLinkContext) {

  CVReturn result = [(SimpleGLView*)displayLinkContext getFrameForTime:outputTime];
  return result;
}

@implementation SimpleGLView
- (void)prepareOpenGL
{
  // Synchronize buffer swaps with vertical refresh rate
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

  // Create a display link capable of being used with all active displays
  CVDisplayLinkCreateWithActiveCGDisplays(&display_link_);

  // Set the renderer output callback function
  CVDisplayLinkSetOutputCallback(display_link_, &simple_gl_view_callback, self);

  // Set the display link for the current renderer
  CGLContextObj cgl_context = [[self openGLContext] CGLContextObj];
  CGLPixelFormatObj cgl_pixel_format = [[self pixelFormat] CGLPixelFormatObj];
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(display_link_, cgl_context, cgl_pixel_format);

  // Activate the display link
  CVDisplayLinkStart(display_link_);
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
 * Called between system frame swapping.
 */
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime {
  printf("draw\n");
   // Clear the screen and depth buffer
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();   // Reset the current modelview matrix

   glTranslatef( -1.5f, 0.0f, -6.0f );   // Left 1.5 units, into screen 6.0

   glBegin( GL_TRIANGLES );             // Draw a triangle
   glVertex3f(  0.0f,  1.0f, 0.0f );    // Top
   glVertex3f( -1.0f, -1.0f, 0.0f );    // Bottom left
   glVertex3f(  1.0f, -1.0f, 0.0f );    // Bottom right
   glEnd();                             // Done with the triangle

   glTranslatef( 3.0f, 0.0f, 0.0f );    // Move right 3 units

   glBegin( GL_QUADS );                // Draw a quad
   glVertex3f( -1.0f,  1.0f, 0.0f );   // Top left
   glVertex3f(  1.0f,  1.0f, 0.0f );   // Top right
   glVertex3f(  1.0f, -1.0f, 0.0f );   // Bottom right
   glVertex3f( -1.0f, -1.0f, 0.0f );   // Bottom left
   glEnd();                            // Quad is complete

   //[ [ self openGLContext ] flushBuffer ];

   return kCVReturnSuccess;
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

  NSRect frame;
  if (strcmp(title, "Thread 1")) {
    frame = NSMakeRect(10,   50, VIEW_WIDTH, VIEW_HEIGHT);
  } else {
    frame = NSMakeRect(VIEW_WIDTH + 50, 50, VIEW_WIDTH, VIEW_HEIGHT);
  }

  SimpleGLView *gl_view = [[SimpleGLView alloc] initWithFrame:NSMakeRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT)];

  int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;
  // NSTexturedBackgroundWindowMask

  NSWindow *win = [[NSWindow alloc] initWithContentRect:frame
    styleMask:style
    backing:NSBackingStoreBuffered
    defer:NO];

  [win setTitle:[NSString stringWithUTF8String:title]];
  [[win contentView] addSubview:gl_view];
  [win makeKeyAndOrderFront:win];
  sleep(2);

  [NSApp terminate:NULL];
  [pool release];
}

void *app_loop(void *data) {
  pthread_t thread1, thread2;
  char *title1 = "Thread 1";
  char *title2 = "Thread 2";
  /* Create independent threads each of which will execute function */

  pthread_create( &thread1, NULL, open_window, (void*) title1);
 // pthread_create( &thread2, NULL, open_window, (void*) title2);

  pthread_join( thread1,    NULL);
  //pthread_join( thread2,    NULL);
  return NULL;
}

/* ======================== main ======================= */

int main(int argc, char *argv[]) {
  pthread_t app_thread;
  [NSApplication sharedApplication]; // ?
  pthread_create( &app_thread, NULL, app_loop, NULL);
  [NSApp run];

  pthread_join( app_thread, NULL);
  return 0;
}