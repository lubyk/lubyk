#include "class.h"

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include <stdlib.h>  // exit
#include <stdio.h>

/////////////// PLOT HACK ///////////

typedef void (*plot_thread)(void * pEvent);
extern plot_thread gPlotThread;
extern void * gPlotThreadData;
extern bool gRunning;
bool gQuitGl;

/////////////////////////////////////

struct OglWindow
{
  int height;
  int width;
  int id;
  bool fullscreen;
};

class OpenGL;

class OpenGL : public Node
{
public:
  OpenGL()
  {
    mMouseMatrix.set_sizes(1,2);
  }
  
  ///////////// PLOT HACK ////////////////
  virtual ~OpenGL()
  {
    gQuitGl = true;
    glutPostRedisplay();
  }
  ////////////////
  virtual void draw()
  {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
      glVertex2i(200,125);
      glVertex2i(100,375);
      glVertex2i(300,375);
    glEnd();
  }
  
  virtual void key_press(unsigned char pKey, int x, int y)
  {
    if (!handle_default_keys(pKey)) {
      mServer->lock();
        send(1, (int)pKey);
      mServer->unlock();
    }
  }
  
  virtual void mouse_move(int x, int y)
  {
    mMouseMatrix.data[0] = (double)x;
    mMouseMatrix.data[1] = (double)y;
    mServer->lock();
      send(2, mMouseMatrix);
    mServer->unlock();
  }
  
  bool init(const Params& p)
  {
    return init_gl(p);
  }
  
  
protected:
  
  bool init_gl(const Params& p)
  { 
    mWindow.height = p.val("height", 600);
    mWindow.width = p.val("width", 800);
    mWindow.fullscreen = p.val("fullscreen", false);
    mClearColor = p.val("clear", 0.2);
    
    ////////////// PLOT HACK ////////////
    gPlotThreadData = (void*)this;
    gPlotThread = &OpenGL::start_hack;
    gQuitGl = false;
    ////////////////////////////////////
    // NEW_THREAD(OpenGL,start_loop);

    return true;
  }
  
  
  //////////// PLOT HACK /////////////
  static void start_hack (void * data)
  {
    OpenGL * node = (OpenGL*)data;
    pthread_setspecific(sThisKey, data);
    node->start_loop();
    // never reaches this
  }
  ////////////////////////////////////
  
  bool handle_default_keys(unsigned char pKey)
  {
    switch(pKey) {
    case ' ':
      mWindow.fullscreen = !mWindow.fullscreen;
      resize_window();
      return true;
    default:
      return false;
    }
  }
  
  void resize_window()
  {
    if (mWindow.fullscreen) {
     glutFullScreen(); 
    } else {
     glutReshapeWindow(mWindow.width, mWindow.height);
    }
  }
  
  void gl_square(double x, double y, double sz)
  {
    glVertex2f(x - sz/2, y - sz/2);
    glVertex2f(x + sz/2, y - sz/2);
    glVertex2f(x + sz/2, y + sz/2);
    glVertex2f(x - sz/2, y + sz/2);
    glVertex2f(x - sz/2, y - sz/2);
    glVertex2f(x, y);
  }
  
  
  OglWindow  mWindow;
  
  
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
    OpenGL * node = (OpenGL*)thread_this();
    //////////////// PLOT HACK //////////
    struct timespec sleeper;
    sleeper.tv_sec  = 0; 
    sleeper.tv_nsec = 100 * 10000;
    
    if (gQuitGl) {
      while (gRunning) {
        nanosleep (&sleeper, NULL);
      }
      exit(0);
    /////////////////////////////////////
    //if (!node->run_thread()) {
      //pthread_exit(0);
    
      /* glutDestroyWindow(node->mWindow.id); */
      /* glutDestroyWindow makes rubyk die. Bad, bad, bad. */
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    node->draw();
    glFlush();
  }
  
  static void cast_idle (void)
  {
    OpenGL * node = (OpenGL*)thread_this();
    if (node->mNeedRedisplay) glutPostRedisplay();
    node->mNeedRedisplay = false;
  }
  
  static void cast_key_press (unsigned char pKey, int x, int y)
  {
    OpenGL * node = (OpenGL*)thread_this();
    node->key_press(pKey, x, y);
  }
  
  static void cast_mouse_move (int x, int y)
  {
    OpenGL * node = (OpenGL*)thread_this();
    node->mouse_move(x,y);
  }

  
  void start_loop()
  {  
    // runs in new thread
    
    int argc = 0;
    char *argv[] = {"hello"};
    
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(mWindow.width, mWindow.height);
    mWindow.id = glutCreateWindow("Plot");
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND); //Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
    
    // current window is mWin
    glClearColor(mClearColor,mClearColor,mClearColor,1.0);
    gluOrtho2D(0,mWindow.width,0,mWindow.height);
    glutDisplayFunc(&OpenGL::cast_draw);
    glutIdleFunc(&OpenGL::cast_idle);
    glutKeyboardFunc(&OpenGL::cast_key_press);
    glutMotionFunc(&OpenGL::cast_mouse_move);

    resize_window();
    mServer->normal_priority();
    glutMainLoop();
  }
  
  double mClearColor;
  pthread_t mThread;
  Matrix mMouseMatrix;
protected:
  
  bool mNeedRedisplay;
};
