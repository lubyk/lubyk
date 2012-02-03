#include "mimas/GLSLWidget.h"

using namespace mimas;

const GLchar* FragmentShader =
{
    "#version 150\n"\
 
    "in vec4 ex_Color;\n"\
    "out vec4 out_Color;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   out_Color = ex_Color;\n"\
    "}\n"
};
const GLchar* VertexShader =
{
    "#version 150\n"\
 
    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Color;\n"\
    "out vec4 ex_Color;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   gl_Position = in_Position;\n"\
    "   ex_Color = in_Color;\n"\
    "}\n"
};
 
static bool compileShader(GLuint shader_id, const char *source) {
  printf("Compile ==============\n%s\n===============\n", source);
  GLenum ErrorCheckValue = glGetError();
  //glShaderSource(VertexShaderId, 1, &source, NULL);
  glShaderSource(shader_id, 1, &source, NULL);
  CHECKERROR(glShaderSource);
  glCompileShader(shader_id);
  CHECKERROR(glCompileShader);
  GLint params;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &params);
  if (GL_FALSE == params) {
    GLint len;
    //glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
    char buf[2048];
    glGetShaderInfoLog(shader_id, 2084, &len, buf);
    printf("Compilation error: %s\n", buf);
    return false;
  }
  return true;
}

void GLSLWidget::createShaders(const char *vertex_shader, const char *fragment_shader) {
  GLenum ErrorCheckValue = glGetError();

  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  CHECKERROR(glCreateShader);
  if (!compileShader(VertexShaderId, vertex_shader)) return;


  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  CHECKERROR(glCreateShader);
  if (!compileShader(FragmentShaderId, fragment_shader)) return;

  ProgramId = glCreateProgram();
  CHECKERROR(glCreateProgram);
  glAttachShader(ProgramId, VertexShaderId);
  CHECKERROR(glAttachShader);
  glAttachShader(ProgramId, FragmentShaderId);
  CHECKERROR(glAttachShader2);
  glLinkProgram(ProgramId);
  CHECKERROR(glLinkProgram);
  glUseProgram(ProgramId);
  CHECKERROR(glUseProgram);
}

