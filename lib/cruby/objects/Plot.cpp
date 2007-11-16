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
    mRefBuffer = NULL;
    mBuffer    = NULL;
    return init_gl(p);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    if (sig.get(&mBuffer))
      mBufferSize = sig.array.size;
    glutPostRedisplay();
  }
  
  // inlet 2
  void reference(const Signal& sig)
  {  
    if (sig.get(&mRefBuffer)) {
      mRefBufferSize = sig.array.size;
    }
  }
  
  void draw()
  {
    if (mRefBuffer) draw_buffer(mRefBuffer, mRefBufferSize, 0.4, false); // 0.6 = alpha, false = do not draw base line
    if (mBuffer)    draw_buffer(mBuffer, mBufferSize, 1.0);
  }
  
private:
  
  void draw_buffer (double * pBuffer, int pBufferSize, double pAlpha, bool pDrawBase = true)
  {
    int l, l_offset;
    int g, g_offset;
    
    // we use mBufferSize as the main buffer size. Others are cropped.
    int value_offset = (mBufferSize - pBufferSize) / (mLineCount * mGroupSize);
    int value_count  = mBufferSize / (mLineCount * mGroupSize); 
    
    double width_ratio  = (double)mWindow.width  / value_count;
    double height_ratio = (double)mWindow.height / (2.0 * mLineCount * mMaxAmplitude); // values : [-1,1]
    double y_offset;
    double col_ratio = 1.0 / mGroupSize;
    
    for(int l=0; l < mLineCount; l++) {
      l_offset = l * mGroupSize;
      y_offset = (l + 0.5) * mWindow.height / mLineCount;
      
      // draw base line
      if (pDrawBase) {
        glColor4f(0.3,0.3,0.3,0.9);
        glBegin(GL_LINE_STRIP);
          glVertex2f(0,y_offset);
          glVertex2f(mWindow.width,y_offset);
        glEnd();
      }
      
      
      for(int g=0; g < mGroupSize; g++) {
        g_offset = g * value_count;
        
        // element in group
        glColor4f(col_ratio * (g % mGroupSize),col_ratio * ((g+1) % mGroupSize),col_ratio * ((g+2) % mGroupSize),pAlpha);
        
        glBegin(GL_LINE_STRIP);
          for(int i=0; i < (value_count - value_offset); i++) {
            glVertex2f((i + value_offset) * width_ratio, y_offset + pBuffer[i * mGroupSize * mLineCount + g + l_offset] * height_ratio);
          }
        glEnd();
      }
    }
  }
  
  
  
  double * mBuffer;
  int      mBufferSize;
  double * mRefBuffer;
  int      mRefBufferSize;
  int      mLineCount;
  int      mGroupSize;
  double   mMaxAmplitude;
};

extern "C" void init()
{
  CLASS(Plot)
  INLET(Plot,reference)
}