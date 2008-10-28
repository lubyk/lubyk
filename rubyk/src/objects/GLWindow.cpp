#include "gl_node.h"
#include <stdlib.h>  // exit
#include <stdio.h>

/////////////// GLWINDOW HACK ///////////

typedef void (*plot_thread)(void * pEvent);
extern plot_thread gGLWindowStartThread;
extern void * gGLWindowNode;
extern bool   gQuitGl;  /**< Used to tell thread to exit. */
extern bool   gRunning; /**< Used to tell when server has stopped running. */

/////////////////////////////////////
/** Draw an openGL window. It's a pity we need multiple inheritance here... */
class GLWindow : public GLNode, public Node
{
public:
  GLWindow()
  {
    mMouseMatrix.set_sizes(1,2);
  }
  
  virtual ~GLWindow()
  {
    ///////////// GLWINDOW HACK ////////////////
    gQuitGl = true;
    glutPostRedisplay();
    ////////////////
  }
  
  bool init(const Params& p)
  { 
    mHeight     = 600;
    mWidth      = 800;
    mFullscreen = false;
    mClearGrey  = 0.2;
    mTitle      = "GLWindow";
    TRY(mSize, set_sizes(1, 2));
    
    ////////////// GLWINDOW HACK ////////////
    gGLWindowNode = (void*)this;
    gGLWindowStartThread = &GLWindow::start_opengl;
    gQuitGl = false;
    ////////////////////////////////////

    return true;
  }
  
  bool set (const Params& p)
  {
    mHeight     = p.val("height", mHeight);
    mWidth      = p.val("width", mWidth);
    mFullscreen = p.val("fullscreen", mFullscreen);
    mClearGrey  = p.val("clear", mClearGrey);
    mTitle      = p.val("title", mTitle);
    
    mSize.data[0] = mWidth;
    mSize.data[1] = mHeight;
    return true;
  }
  
  // inlet 1
  virtual void bang(const Signal& sig)
  {
    mNeedRedisplay = true;
    send(1, sig);
  }
  
  // never used. Dummy so that GLWindow compiles.
  virtual void draw(const Signal& sig)
  {}
  
  virtual void key_press(unsigned char pKey, int x, int y)
  {
    if (!handle_default_keys(pKey)) {
      mServer->lock();
        send(3, (int)pKey);
      mServer->unlock();
    }
  }
  
  virtual void mouse_move(int x, int y)
  {
    mMouseMatrix.data[0] = (double)x;
    mMouseMatrix.data[1] = (double)y;
    mServer->lock();
      send(4, mMouseMatrix);
    mServer->unlock();
  }
  
  virtual void spy()
  { 
    bprint(mSpy, mSpySize,"%s %ix%i", mTitle.c_str(), mWidth, mHeight);  
  }
  
protected:
  
  //////////// GLWINDOW HACK /////////////
  static void start_opengl (void * data)
  {
    GLWindow * node = (GLWindow*)data;
    pthread_setspecific(sThisKey, data);
    set_is_opengl_thread();
    node->start_loop();
    // never reaches this
  }
  ////////////////////////////////////
  
  bool handle_default_keys(unsigned char pKey)
  {
    switch(pKey) {
    case ' ':
      mFullscreen = !mFullscreen;
      resize_window();
      return true;
    case '\e':
      mServer->lock();
        mServer->quit();
      mServer->unlock();
      return true;
    default:
      return false;
    }
  }
  
  void resize_window()
  {
    if (mFullscreen) {
     glutFullScreen(); 
    } else {
     glutReshapeWindow(mWidth, mHeight);
    }
  }
  
  virtual void stop_my_threads()
  {
    std::list<pthread_t>::iterator it  = mThreadIds.begin();
    std::list<pthread_t>::iterator end = mThreadIds.end();

    int i = 0;
    while(it != end) {
      pthread_t id = *it;
      it = mThreadIds.erase(it); // tell thread to stop
      glutPostRedisplay();     // needed by opengl threads so they run one last time
      pthread_join(id, NULL);  // wait
      i++;
    }
  }

private:
  
  static void cast_draw (void)
  {
    GLWindow * node = (GLWindow*)thread_this();
    //////////////// GLWINDOW HACK //////////
    struct timespec sleeper;
    sleeper.tv_sec  = 0; 
    sleeper.tv_nsec = 10 * 1000000; // 1 ms
    
    if (gQuitGl) {
      while (gRunning) {
        nanosleep (&sleeper, NULL);
      }
      exit(0);
    /////////////////////////////////////
    //if (!node->run_thread()) {
      //pthread_exit(0);
    
      /* glutDestroyWindow(node->mId); */
      /* glutDestroyWindow makes rubyk die. Bad, bad, bad. */
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    node->send(2, node->mSize);
    glFlush();
  }
  
  static void cast_idle (void)
  {
    GLWindow * node = (GLWindow*)thread_this();
    if (node->mNeedRedisplay) glutPostRedisplay();
    node->mNeedRedisplay = false;
  }
  
  static void cast_key_press (unsigned char pKey, int x, int y)
  {
    GLWindow * node = (GLWindow*)thread_this();
    node->key_press(pKey, x, y);
  }
  
  static void cast_mouse_move (int x, int y)
  {
    GLWindow * node = (GLWindow*)thread_this();
    node->mouse_move(x,y);
  }

  /* Init and start openGL thread. */
  void start_loop()
  {  
    // runs in new thread
    
    int argc = 0;
    const char *argv[] = {mTitle.c_str()};
    
    glutInit(&argc, const_cast<char**>(argv));

    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(mWidth, mHeight);
    mId = glutCreateWindow(mTitle.c_str());
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);                                // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend function
    
    glClearColor(mClearGrey,mClearGrey,mClearGrey,1.0);
    
    gluOrtho2D(0,mWidth,0,mHeight);
    glutDisplayFunc(&GLWindow::cast_draw);
    glutIdleFunc(&GLWindow::cast_idle);
    glutKeyboardFunc(&GLWindow::cast_key_press);
    glutMotionFunc(&GLWindow::cast_mouse_move);

    resize_window();
    
    /* This thread runs with a lower priority. */
    mServer->normal_priority();
    glutMainLoop();
  }
  
  std::string mTitle;        /**< Window title. */
  Matrix      mSize;         /**< Window size. */
  int         mHeight;       /**< Window height (in pixels). */
  int         mWidth;        /**< Window width (in pixels). */
  int         mId;           /**< Window id. */
  bool        mFullscreen;   /**< True if fullscreen is enabled. */
  double      mClearGrey;   /**< Shades of grey to clear screen. */
  pthread_t   mThread;       /**< Thread running all openGL stuff. */
  Matrix      mMouseMatrix;  /**< Mouse position matrix. */
  
protected:
  
  bool mNeedRedisplay;
};


extern "C" void init()
{
  CLASS(GLWindow)
  OUTLET(GLWindow, out)
  OUTLET(GLWindow, draw)
  OUTLET(GLWindow, keys)
  OUTLET(GLWindow, mousexy)
}