#include <QApplication>

#include <QGLWidget>
#include <QGLContext>

#include <iostream>

void* select_3_2_mac_visual(GDHandle handle);


struct Core3_2_context : public QGLContext
{
  Core3_2_context(const QGLFormat& format, QPaintDevice* device) : QGLContext(format,device) {}
  Core3_2_context(const QGLFormat& format) : QGLContext(format) {}

  virtual void* chooseMacVisual(GDHandle handle)
  {
    return select_3_2_mac_visual(handle);
  }
};

struct OglWidget : public QGLWidget
{
  OglWidget() : QGLWidget(new Core3_2_context(QGLFormat::defaultFormat()))
  {
  }
  virtual void initializeGL()
  {
    std::cout<<glGetString(GL_VERSION)<<std::endl;
  }
  virtual void paintGL()
  {
    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};

int main(int argc, char *argv[])
{
  QApplication app(argc,argv);

  OglWidget w;
  w.show();

  return app.exec();
}
