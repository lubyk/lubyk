#include "opengl.h"

class Plot : public OpenGL
{
public:
  virtual ~Plot () {}
  
  bool init (const Params& p)
  {
    mLineCount = p.val("line", 1);
    mGroupSize = p.val("group", 1);
    mMaxAmplitude = p.val("amplitude", 1.0);
    
    return init_gl(p);
  }
  
  void bang(const Signal& sig)
  {
    if (sig.type == ArraySignal) {
      mBuffer = sig.array.value;
      mBufferSize = sig.array.size;
    }
    glutPostRedisplay();
  }
  
  void draw()
  {
    int l, l_offset;
    int g, g_offset;
    int value_count = mBufferSize / (mLineCount * mGroupSize);
    
    double width_ratio  = mWindow.width  / value_count;
    double height_ratio = mWindow.height / (2.0 * mLineCount * mMaxAmplitude); // values : [-1,1]
    double y_offset;
    double col_ratio = 1.0 / mGroupSize;
    
    for(int l=0; l < mLineCount; l++) {
      l_offset = l * mGroupSize;
      y_offset = (l + 0.5) * mWindow.height / mLineCount;
      
      // draw base line once
      glColor4f(0.3,0.3,0.0,0.9);
      glBegin(GL_LINE_STRIP);
        glVertex2f(0,y_offset);
        glVertex2f(mWindow.width,y_offset);
      glEnd();
      
      
      for(int g=0; g < mGroupSize; g++) {
        g_offset = g * value_count;
        
        // element in group
        glColor4f(col_ratio * (g % mGroupSize),col_ratio * ((g+1) % mGroupSize),col_ratio * ((g+2) % mGroupSize),1.0);
        
        glBegin(GL_LINE_STRIP);
          for(int i=0; i < value_count; i++) {
            glVertex2f(i * width_ratio, y_offset + mBuffer[i * mGroupSize * mLineCount + g + l_offset] * height_ratio);
          }
        glEnd();
      }
    }
  }
  
private:
  double * mBuffer;
  int      mBufferSize;
  int      mLineCount;
  int      mGroupSize;
  double   mMaxAmplitude;
};

extern "C" void init()
{
  CLASS(Plot)
}