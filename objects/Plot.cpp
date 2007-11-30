#include "opengl.h"

#define PLOT_INLET_COUNT 2
typedef enum {
  XYPlot,
  TimePlot,
} plot_type_t;

class Plot : public OpenGL
{
public:
  virtual ~Plot () {}
  
  bool init (const Params& p)
  {
    std::string mode;
    mLineCount[0] = p.val("line", 1);
    mGroupSize[0] = p.val("group", 1);
    mMaxAmplitude[0] = p.val("amplitude", 1.0);
    
    mLineCount[1] = p.val("line2", mLineCount[0]);
    mGroupSize[1] = p.val("group2", mGroupSize[0]);
    mMaxAmplitude[1] = p.val("amplitude2", mMaxAmplitude[0]);
    
    set_mode_from_string(&mMode[0], p.val("mode", std::string("time")));
    set_mode_from_string(&mMode[1], p.val("mode2", std::string("time")));
    for(int i=0; i< PLOT_INLET_COUNT; i++) {
      mLiveBuffer[i]     = NULL;
      mLiveBufferSize[i] = 0;
    }
    return init_gl(p);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    if (sig.get(&mLiveBuffer[0]))
      mLiveBufferSize[0] = sig.array.size;
    glutPostRedisplay();
  }
  
  // inlet 2
  void reference(const Signal& sig)
  { 
    if (sig.get(&mLiveBuffer[1])) {
      mLiveBufferSize[1] = sig.array.size;
    }
  }
  
  void draw()
  {
    for(int i=PLOT_INLET_COUNT - 1; i>=0; i--) {
      if (!mLiveBuffer[i]) continue;
      switch(mMode[i]) {
      case XYPlot:
        xy_plot(mLiveBuffer[i], mLiveBufferSize[i], i == 0 ? 1.0 : 0.6, i, i == 0);
        break;
      default:
        time_plot(mLiveBuffer[i], mLiveBufferSize[i], i == 0 ? 1.0 : 0.5, i, i == 0);
      }
    }
  }
  
private:
  
  void time_plot (double * pBuffer, int pBufferSize, double pAlpha, int param_index, bool pDrawBase = true)
  {
    int l, l_offset;
    int g, g_offset;
    
    int value_count  = mLiveBufferSize[param_index] / (mLineCount[param_index] * mGroupSize[param_index]); 
    double width_ratio;
    double height_ratio = (double)mWindow.height / (2.0 * mLineCount[param_index] * mMaxAmplitude[param_index]); // values : [-1,1]
    double y_offset;
    double col_ratio = 1.0 / mGroupSize[param_index];
    
    if (value_count > 1)
        width_ratio = (double)mWindow.width  / (value_count - 1);
    else
        width_ratio = (double)mWindow.width;
    
    for(int l=0; l < mLineCount[param_index]; l++) {
      l_offset = l * mGroupSize[param_index];
      y_offset = (l + 0.5) * mWindow.height / mLineCount[param_index];
      
      // draw base line
      if (pDrawBase) {
        glColor4f(0.3,0.3,0.3,0.9);
        glBegin(GL_LINE_STRIP);
          glVertex2f(0,y_offset);
          glVertex2f(mWindow.width,y_offset);
        glEnd();
      }
      
      
      for(int g=0; g < mGroupSize[param_index]; g++) {
        g_offset = g * value_count;
        
        // element in group
        glColor4f(col_ratio * (g % mGroupSize[param_index]),col_ratio * ((g+1) % mGroupSize[param_index]),col_ratio * ((g+2) % mGroupSize[param_index]),pAlpha);
        
        glBegin(GL_LINE_STRIP);
          for(int i=0; i < value_count; i++) {
            glVertex2f(i * width_ratio, y_offset + pBuffer[i * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
          }
          glVertex2f(mWindow.width, y_offset + pBuffer[(value_count - 1) * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
        glEnd();
      }
    }
  }
  
  void xy_plot (double * pBuffer, int pBufferSize, double pAlpha, int param_index, bool pDrawBase = true)
  {
    int l, l_offset;
    int g, g_offset;
    
    // we use mLiveBufferSize as the main buffer size. Others are cropped.
    int value_count  = pBufferSize / (2 * mLineCount[param_index]); 
    double width_ratio  = (double)mWindow.width  / (2.0 * mMaxAmplitude[param_index]);
    double height_ratio = (double)mWindow.height / (2.0 * mMaxAmplitude[param_index]); // values : [-1,1]
    double with_offset  = (double)mWindow.width / 2.0;
    double height_offset= (double)mWindow.height / 2.0;
    double col_ratio = 1.0 / mLineCount[param_index];
    
    for(int l=0; l < mLineCount[param_index]; l++) {
      int offset_line = l * value_count;
      // element in group
      // 0 1 0 : 0  1  0 0 1
      // 1 0 0 : 1  2  0 1 0
      // 0 0 1 : 2  3  0 1 1
      
      glColor4f((int)((l+1)/2) % 4,(int)(l+1) % 2,(int)(l+1)/4,pAlpha);
      //glColor4f(0.0,1.0,0.0, pAlpha);

      glBegin(GL_LINE_STRIP);
      if (value_count > 0) {

        gl_square(with_offset   + pBuffer[offset_line * 2]     * width_ratio,
                  height_offset + pBuffer[offset_line * 2 + 1] * height_ratio, 4.0);
      }
      for(int i=1; i < value_count; i++) {
        glVertex2f(with_offset   + pBuffer[(offset_line + i) * 2    ] * width_ratio,
                   height_offset + pBuffer[(offset_line + i) * 2 + 1] * height_ratio);
      }      
      glEnd(); 
    }
  }
  
  void set_mode_from_string(plot_type_t * pMode, const std::string& pStr)
  {
    if (pStr == "xy")
      *pMode = XYPlot;
    else
      *pMode = TimePlot;
  }
  
  plot_type_t mMode[PLOT_INLET_COUNT]; /**< Plot mode (XYPlot, TimePlot) each pair as an XY point instead of x(t). */
  double * mLiveBuffer[PLOT_INLET_COUNT];
  int      mLiveBufferSize[PLOT_INLET_COUNT];
  int      mLineCount[PLOT_INLET_COUNT];
  int      mGroupSize[PLOT_INLET_COUNT];
  double   mMaxAmplitude[PLOT_INLET_COUNT];
};

extern "C" void init()
{
  CLASS(Plot)
  INLET(Plot,reference)
}