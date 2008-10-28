#ifndef _GL_NODE_
#define _GL_NODE_
#include "class.h"

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif

/** Abstract class to wrap OpenGL related objects in a single inheritance branch. */
class GLNode
{
public:
  GLNode ()
  {
    if (sGLThreadKey) pthread_key_create(&sGLThreadKey, NULL);
  }
  
  virtual ~GLNode ()
  {}
  
  inline bool is_opengl_thread ()
  {
    return pthread_getspecific(sGLThreadKey) != NULL;
  }
  
  void static set_is_opengl_thread ()
  {
    pthread_setspecific(sGLThreadKey, (void*)true);
  }
  
  virtual void draw (const Signal& sig) = 0;
  
  void draw_inlet (const Signal& sig)
  {
    if (!is_opengl_thread()) {
      printf("not an opengl thread !\n");
      return;
    }
    glPushMatrix();
    draw(sig);
    glPopMatrix();
  }
  
public:
  static pthread_key_t sGLThreadKey;   /**< Key to test if the running thread is an OpenGL thread. */  
};

#define GL_CLASS(klass)  CLASS(klass) \
                         SUPER_INLET(klass,GLNode,draw_inlet)
#endif // _GL_NODE_