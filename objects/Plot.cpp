#include "opengl.h"

#define PLOT_INLET_COUNT 2
typedef enum {
  XYPlot,
  TimePlot,
} plot_type_t;

class Plot : public OpenGL
{
public:
  Plot()
  {
    for(size_t i=0; i< PLOT_INLET_COUNT; i++)
      mLiveBuffer[i] = NULL;
  }
  
  bool init (const Params& p)
  {
    std::string mode;
    mLineCount[0] = p.val("line", 1);
    mGroupSize[0] = p.val("group", 1);
    mMaxAmplitude[0] = p.val("amplitude", 1.0);
    
    mLineCount[1] = p.val("line2", 1);
    mGroupSize[1] = p.val("group2", 1);
    mMaxAmplitude[1] = p.val("amplitude2", 1.0);
    
    set_mode_from_string(&mMode[0], p.val("mode", std::string("time")));
    set_mode_from_string(&mMode[1], p.val("mode2", std::string("time")));
    
    return init_gl(p);
  }
  
  bool set (const Params& p)
  {
    std::string mode;
    p.get(&mLineCount[0],"line");
    p.get(&mGroupSize[0],"group");
    p.get(&mMaxAmplitude[0], "amplitude");
    
    p.get(&mLineCount[1],"line2");
    p.get(&mGroupSize[1],"group2");
    p.get(&mMaxAmplitude[1], "amplitude2");
    
    if (p.get(&mode, "mode"))
      set_mode_from_string(&mMode[0], mode);
    
    if (p.get(&mode, "mode2"))
      set_mode_from_string(&mMode[1], mode);
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    sig.get(&(mLiveBuffer[0]));
    glutPostRedisplay();
  }
  
  // inlet 2
  void reference(const Signal& sig)
  {
    sig.get(&(mLiveBuffer[1]));
  }
  
  void draw()
  { 
    // do not replace by size_t or i == -1 will never get caught !
    for(int i=PLOT_INLET_COUNT - 1; i>=0; i--) {
      if (mLiveBuffer[i] == NULL) continue;
      switch(mMode[i]) {
      case XYPlot:
        xy_plot(*mLiveBuffer[i], i == 0 ? 1.0 : 0.6, i, i == 0);
        break;
      default:
        time_plot(*mLiveBuffer[i], i == 0 ? 1.0 : 0.5, i, i == 0);
      }
    }
  }
  
private:
  
  void time_plot (const Matrix& mat, double pAlpha, size_t param_index, bool pDrawBase = true)
  {
    size_t l_offset;
    size_t g_offset;
    
    size_t value_count  = mat.size() / (mLineCount[param_index] * mGroupSize[param_index]); 
    double width_ratio;
    double height_ratio = (double)mWindow.height / (2.0 * mLineCount[param_index] * mMaxAmplitude[param_index]); // values : [-1,1]
    double y_offset;
    double col_ratio = 1.0 / mGroupSize[param_index];
    
    if (value_count > 1)
        width_ratio = (double)mWindow.width  / (value_count - 1);
    else
        width_ratio = (double)mWindow.width;
    
    for(size_t l=0; l < mLineCount[param_index]; l++) {
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
      
      
      for(size_t g=0; g < mGroupSize[param_index]; g++) {
        g_offset = g * value_count;
        
        // element in group
        glColor4f(col_ratio * (g % mGroupSize[param_index]),col_ratio * ((g+1) % mGroupSize[param_index]),col_ratio * ((g+2) % mGroupSize[param_index]),pAlpha);
        
        glBegin(GL_LINE_STRIP);
          for(size_t i=0; i < value_count; i++) {
            
            
            
            //////////////////////////////////////
            ////// FIXME: the data is         ////
            ////// changed during draw since  ////
            ////// drawing is done in another ////
            ////// thread !!                  ////
            ////// possible fix: copy data in ////
            ////// 'server' thread            ////
            //////////////////////////////////////
            
            
            
            
            glVertex2f(i * width_ratio, y_offset + mat.data[i * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
          }
          glVertex2f(mWindow.width, y_offset + mat.data[(value_count - 1) * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
        glEnd();
      }
    }
  }
  
  void xy_plot (const Matrix& mat, double pAlpha, size_t param_index, bool pDrawBase = true)
  {    
    // we use mLiveBufferSize as the main buffer size. Others are cropped.
    size_t value_count  = mat.size() / (2 * mLineCount[param_index]); 
    double width_ratio  = (double)mWindow.width  / (2.0 * mMaxAmplitude[param_index]);
    double height_ratio = (double)mWindow.height / (2.0 * mMaxAmplitude[param_index]); // values : [-1,1]
    double with_offset  = (double)mWindow.width / 2.0;
    double height_offset= (double)mWindow.height / 2.0;
    
    for(size_t l=0; l < mLineCount[param_index]; l++) {
      size_t offset_line = l * value_count;
      // element in group
      // 0 1 0 : 0  1  0 0 1
      // 1 0 0 : 1  2  0 1 0
      // 0 0 1 : 2  3  0 1 1
      
      glColor4f((int)((l+1)/2) % 4,(int)(l+1) % 2,(int)(l+1)/4,pAlpha);
      //glColor4f(0.0,1.0,0.0, pAlpha);

      glBegin(GL_LINE_STRIP);
      if (value_count > 0) {

        gl_square(with_offset   + mat.data[offset_line * 2]     * width_ratio,
                  height_offset + mat.data[offset_line * 2 + 1] * height_ratio, 4.0);
      }
      for(size_t i=1; i < value_count; i++) {
        glVertex2f(with_offset   + mat.data[(offset_line + i) * 2    ] * width_ratio,
                   height_offset + mat.data[(offset_line + i) * 2 + 1] * height_ratio);
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
  
  plot_type_t    mMode[PLOT_INLET_COUNT];       /**< Plot mode (XYPlot, TimePlot) each pair as an XY point instead of x(t). */
  const Matrix * mLiveBuffer[PLOT_INLET_COUNT]; /**< Pointer to the live stream (one matrix for each inlet). */
  size_t         mLineCount[PLOT_INLET_COUNT];
  size_t         mGroupSize[PLOT_INLET_COUNT];
  double         mMaxAmplitude[PLOT_INLET_COUNT];
};

extern "C" void init()
{
  CLASS(Plot)
  INLET(Plot,reference)
}