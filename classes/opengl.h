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

class OpenGL : public Node
{
public:
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
    
    mWindow.height = p.val("height", 600);
    mWindow.width = p.val("width", 800);
    
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

    NEW_THREAD(OpenGL,start_loop);

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
  
  /** How could we get 'this' pointer back ? */
  static void cast_draw (void)
  {
    OpenGL * node = (OpenGL*)thread_this();
    if (!node->run_thread()) {
      pthread_exit(0);
      //glutDestroyWindow(node->mWindow.id);
      /* glutDestroyWindow makes rubyk die. Bad, bad, bad. */
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    node->draw();
    glFlush();
  }
  
  void start_loop()
  {
    // runs in new thread
    glutMainLoop();
  }
  
  
  pthread_t mThread;
  bool mQuitGL; // should go into class when draw is in there too...
};
