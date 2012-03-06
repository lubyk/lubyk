/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#ifndef LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_
#define LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_

#include "lubyk.h"
using namespace lubyk;

#include "GL/glew.h"

#include "mimas/mimas.h"
#include "mimas/Widget.h"

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLContext>

#ifdef __macosx__
  //#include <OpenGL/gl.h>
  //#include <OpenGL/glu.h>
  void* mimasSelectModernOpenGLMac(GDHandle handle);
#else
  //#include <GL/gl.h>
  //#include <GL/glu.h>
#endif

#define CHECKERROR(msg) ErrorCheckValue = glGetError();\
if (ErrorCheckValue != GL_NO_ERROR) { \
  fprintf(stderr, "%s: %s \n", #msg, gluErrorString(ErrorCheckValue)); \
  exit(-1); \
}
namespace mimas {

  
/** GLSLWidget
 * This class lets you draw OpenGL elements with Widgets on top.
 * http://doc.trolltech.com/qq/qq26-openglcanvas.html
 *
 * @dub destructor: 'luaDestroy'
 *      super: 'QWidget'
 */
class GLSLWidget : public QGLWidget, public ThreadedLuaObject
{
  Q_OBJECT
  Q_PROPERTY(QString class READ cssClass)

#ifdef __macosx__
  struct GLSLContext : public QGLContext {
    GLSLContext(const QGLFormat& format, QPaintDevice* device) : QGLContext(format,device) {}
    GLSLContext(const QGLFormat& format) : QGLContext(format) {}

    virtual void* chooseMacVisual(GDHandle handle) {
      return mimasSelectModernOpenGLMac(handle);
    }
  };
#else
  struct GLSLFormat : public QGLFormat {
    GLSLFormat() {
      setVersion(3, 2);
      setProfile(QGLFormat::CoreProfile);
      setSampleBuffers(true);
    }
  };
#endif // __macosx__

public:
 
  GLSLWidget()
#ifdef __macosx__
    : QGLWidget(new GLSLContext(QGLFormat::defaultFormat()))
#else
    : QGLWidget(GLSLFormat())
#endif
  {
    setAttribute(Qt::WA_DeleteOnClose);
    // get focus on tab and click
    setFocusPolicy(Qt::StrongFocus);
    MIMAS_DEBUG_CC
  }   

  ~GLSLWidget() {
    destroyShaders();
    destroyVBO();
    MIMAS_DEBUG_GC
  }

  QString cssClass() const {
    return parent() ? QString("glwindow") : QString("glwidget");
  }

  QSize size_hint_;
  // =============================================================

  void updateGL() {
    update();
  }
  
  LuaStackSize openGLVersion(lua_State *L) {
    lua_pushstring(L, (char*)glGetString(GL_VERSION));
    lua_pushstring(L, (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    return 2;
  }

  bool compile(const char *vertex_shader, const char *fragment_shader) {
    if (!isVisible()) {
      // This should initialize OpenGL
      show();
    }
    if (!glGetString(GL_VERSION)) {
      printf("OpenGL not initialized. Show window before compiling.\n");
    } else {
      printf("COMPILE...\n");
      createShaders(vertex_shader, fragment_shader);
      createVBO();
    }
    updateGL();
    return true;
  }

protected:
  virtual void initializeGL() {
    printf("glewInit\n");     
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      fprintf(
        stderr,
        "glewInit error: %s\n",
        glewGetErrorString(err)
        );
      exit(EXIT_FAILURE);
    } 
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  /*
    lua_State *L = lua_;

    if (!pushLuaCallback("initializeGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'initializeGL' callback: %s\n", lua_tostring(L, -1));
    }
  }
  */

  virtual void resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
  }
/*

    lua_State *L = lua_;

    if (!pushLuaCallback("resizeGL")) return;
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    // <func> <self> <width> <height>
    int status = lua_pcall(L, 3, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'resizeGL' callback: %s\n", lua_tostring(L, -1));
    }
  }
  */

  virtual void paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  /*
    lua_State *L = lua_;

    if (!pushLuaCallback("paintGL")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'paintGL' callback: %s\n", lua_tostring(L, -1));
    }
  }
  */

  // --=============================================== Widget callbacks
  virtual void closeEvent(QCloseEvent *event) {
    Widget::closed(this, event);
  }

  virtual void mouseMoveEvent(QMouseEvent *event) {
    Widget::mouse(this, event);
  }

  virtual void mousePressEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MousePress))
      QWidget::mousePressEvent(event);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, DoubleClick))
      QWidget::mouseDoubleClickEvent(event);
  }

  virtual void mouseReleaseEvent(QMouseEvent *event) {
    if (!Widget::click(this, event, MouseRelease))
      QWidget::mouseReleaseEvent(event);
  }

  virtual void resizeEvent(QResizeEvent *event) {
    Widget::resized(this, width(), height());
  }

  virtual void moveEvent(QMoveEvent * event) {
    Widget::moved(this, event);
  }

  virtual void keyPressEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, true))
      QWidget::keyPressEvent(event);
  }

  virtual void keyReleaseEvent(QKeyEvent *event) {
    if (!Widget::keyboard(this, event, false))
      QWidget::keyReleaseEvent(event);
  }



  //virtual void paintEvent(QPaintEvent *event);
private:
  GLuint
    VertexShaderId,
    FragmentShaderId,
    ProgramId,
    VaoId,
    VboId,
    ColorBufferId;
   
  // FIXME: Write gl bindings so that we can do all this in Lua.
  void createVBO() {
    GLenum ErrorCheckValue;

    GLfloat Vertices[] = {
        -0.8f, -0.8f, 0.0f, 1.0f,
         0.0f,  0.8f, 0.0f, 1.0f,
         0.8f, -0.8f, 0.0f, 1.0f
    };
 
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
 
    CHECKERROR(createVBO);
 
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
 
    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
 
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
 
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create a VBO: %s \n",
            gluErrorString(ErrorCheckValue)
        );
 
        exit(-1);
    }
  }

  void destroyVBO() {

    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
      fprintf(
          stderr,
          "ERROR: Could not destroy the VBO: %s \n",
          gluErrorString(ErrorCheckValue)
          );

      exit(-1);
    }
  }


  void createShaders(const char *vertex_shader, const char *fragment_shader);

  void destroyShaders(void) {
    GLenum ErrorCheckValue = glGetError();
 
    glUseProgram(0);
 
    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
 
    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);
 
    glDeleteProgram(ProgramId);
 
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not destroy the shaders: %s \n",
            gluErrorString(ErrorCheckValue)
        );
 
        exit(-1);
    }
  }

};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_GLSLWIDGET_H_

