#include "class.h"

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include <stdlib.h>  // exit
#include <stdio.h>

struct OglWindow
{
  int height;
  int width;
  int id;
};

class OpenGL;

/** Until we know how to make multiple windows and keep them into each C++ object, we use globals... */
OpenGL * gOp;

class OpenGL : public Node
{
public:  
  virtual ~OpenGL () {
    if (mThread) {
      mQuitGL = true;
      glutPostRedisplay(); // so 'cast_draw' gets called in the other thread
      pthread_join( mThread, NULL);
    }
  }
  
  virtual void draw()
  {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
      glVertex2i(200,125);
      glVertex2i(100,375);
      glVertex2i(300,375);
    glEnd();
  }
  
  bool init(const Params& p)
  {
    return init_gl(p);
  }
  
  
protected:
  
  bool init_gl(const Params& p)
  {
    int argc = 0;
    char *argv[] = {"hello"};
    
    // this is really bad
    gOp = this;
    
    
    mWindow.height = p.val("height", 600);
    mWindow.width = p.val("width", 800);
    
    
    mThread = NULL;
    mQuitGL = false;
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(mWindow.width, mWindow.height);
    mWindow.id = glutCreateWindow("Plot");
    
    glEnable(GL_BLEND); //Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
    
    // current window is mWin
    glClearColor(0.2,0.2,0.2,1.0);
    gluOrtho2D(0,mWindow.width,0,mWindow.height);
    glutDisplayFunc(&OpenGL::cast_draw); // use templates...

    pthread_create( &mThread, NULL, &OpenGL::start_loop, this);

    return true;
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

private:
  
  /** How could we get 'this' pointer back ? */
  static void cast_draw (void)
  {
    if (gOp->mQuitGL)  {
      //glutDestroyWindow(gOp->mWindow.id);
      exit(0);
      /* glutDestroyWindow makes rubyk die. Bad, bad, bad. */
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    gOp->draw();
    glFlush();
  }
  
  static void * start_loop(void * data)
  {
    // runs in new thread
    glutMainLoop();
    return NULL;
  }
  
  
  pthread_t mThread;
  bool mQuitGL; // should go into class when draw is in there too...
};
