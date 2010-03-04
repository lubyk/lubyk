// A simple demonstration of OpenGL lighting
// gcc gl_simple.c -framework OpenGL -framework GLUT -o test && ./test

// depth testing
#include <GLUT/glut.h> // #include <gl/glut.h>

void Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(
    45.0,
    w / h,
    1.0,
    100.0
    );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}
void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.8f,0,0);
  glTranslatef(0,0,-5);
  glutSolidTeapot(0.6);
  glColor3f(0.0,0.5f,0);

  // cube in teapot: not seen if depth test ok
  glutSolidCube(0.6);
  glutSwapBuffers();
}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Mi primer ventana");
  glutInitWindowSize(500,500);

  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutMainLoop();
  return 0;
}