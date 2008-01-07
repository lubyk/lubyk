#include "opengl.h"

#define PLOT_INLET_COUNT 3
typedef enum {
  XYPlot,
  TimePlot,
  DotsPlot, // uses 'dot' vectors with color information (first 3 columns = RGB)
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
    for (size_t i = 0; i < PLOT_INLET_COUNT; i++) {
      mLineCount[i] = 1;
      mGroupSize[i] = 1;
      mPointSize[i] = 4.0;
      mMaxAmplitude[i] = 1.0;
      mMode[i] = TimePlot;
    }
    return init_gl(p);
  }
  
  bool set (const Params& p)
  {
    std::string mode;
    p.get(&mLineCount[0],"line");
    p.get(&mGroupSize[0],"group");
    p.get(&mPointSize[0],"point");
    p.get(&mMaxAmplitude[0], "amplitude");
    
    p.get(&mLineCount[1],"line2");
    p.get(&mGroupSize[1],"group2");
    p.get(&mPointSize[1],"point2");
    p.get(&mMaxAmplitude[1], "amplitude2");
    
    p.get(&mLineCount[2],"line3");
    p.get(&mGroupSize[2],"group3");
    p.get(&mPointSize[2],"point3");
    p.get(&mMaxAmplitude[2], "amplitude3");
    
    if (p.get(&mode, "mode"))
      set_mode_from_string(&mMode[0], mode);
    
    if (p.get(&mode, "mode2"))
      set_mode_from_string(&mMode[1], mode);
    
    if (p.get(&mode, "mode3"))
      set_mode_from_string(&mMode[2], mode);
    
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
  
  // inlet 3
  void dots(const Signal& sig)
  {
    sig.get(&(mLiveBuffer[1]));
  }
  
  void draw()
  {
    // do not replace by size_t or i == -1 will never get caught !
    for(int i=PLOT_INLET_COUNT - 1; i>=0; i--) {
      // protected resource
      if (mLiveBuffer[i] == NULL) continue;
	    mServer->lock();
	      switch(mMode[i]) {
	      case XYPlot:
	        xy_plot(*mLiveBuffer[i], i == 0 ? 1.0 : 0.6, i, i == 0);
	        break;
	      case DotsPlot:
	        dots_plot(*mLiveBuffer[i], 1.0, i,  i == 0);
	        break;
	      default:
	        time_plot(*mLiveBuffer[i], i == 0 ? 1.0 : 0.5, i, i == 0);
	      }
	    mServer->unlock();
    }
  }
  
  virtual void spy()
  {  
    bprint(mSpy, mSpySize,"");    
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
        //set_color_from_int(g, pAlpha);
        
        glBegin(GL_LINE_STRIP);
          for(size_t i=0; i < value_count; i++) {
            
            glVertex2f(i * width_ratio, y_offset + mat.data[i * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
          }
          glVertex2f(mWindow.width, y_offset + mat.data[(value_count - 1) * mGroupSize[param_index] * mLineCount[param_index] + g + l_offset] * height_ratio);
        glEnd();
      }
    }
  }
  
  void xy_plot (const Matrix& mat, double pAlpha, size_t param_index, bool pDrawBase = true)
  {    
    size_t line_count   = mLineCount[param_index];
    size_t value_count  = mat.size() / (2 * line_count); 
    double width_ratio  = (double)mWindow.width  / (2.0 * mMaxAmplitude[param_index]);
    double height_ratio = (double)mWindow.height / (2.0 * mMaxAmplitude[param_index]); // values : [-1,1]
    double width_offset = (double)mWindow.width  / 2.0;
    double height_offset= (double)mWindow.height / 2.0;
    
    for(size_t l=0; l < line_count; l++) {
      // element in group
      // 0 1 0 : 0  1  0 0 1
      // 1 0 0 : 1  2  0 1 0
      // 0 0 1 : 2  3  0 1 1
      
      // build color from integer
      set_color_from_int(l, pAlpha);
      
      //glColor4f((int)((l+1)/2) % 4,(int)(l+1) % 2,(int)(l+1)/4,pAlpha);

      if (value_count > 0) {
        glBegin(GL_POINTS);
          glVertex2f(width_offset  + mat.data[l * 2    ] * width_ratio,
                    height_offset + mat.data[l * 2 + 1] * height_ratio);
        glEnd();
      }
      
      glBegin(GL_LINE_STRIP);
      for(size_t i=1; i < value_count; i++) {
        glVertex2f(width_offset  + mat.data[(line_count * i * 2) + l * 2   ] * width_ratio,
                   height_offset + mat.data[(line_count * i * 2) + l * 2 + 1] * height_ratio);
      }      
      glEnd(); 
    }
  }
  
  void dots_plot (const Matrix& mat, double pAlpha, size_t param_index, bool pDrawBase = true)
  {      
    size_t group_count  = mat.row_count();
    size_t value_count  = (mat.col_count() - 3) / 2;
    double width_ratio  = (double)mWindow.width  / (2.0 * mMaxAmplitude[param_index]);
    double height_ratio = (double)mWindow.height / (2.0 * mMaxAmplitude[param_index]); // values : [-1,1]
    double width_offset = (double)mWindow.width  / 2.0;
    double height_offset= (double)mWindow.height / 2.0;
    
    glPointSize(mPointSize[param_index]);
    
    for(size_t g=0; g < group_count; g++) {
      const double * row = mat[g];
      
      glColor4f(row[0],row[1],row[2],pAlpha);

      if (value_count > 0) {
        glBegin(GL_POINTS);
        for(size_t i=0; i < value_count; i++) {
          glVertex2f(width_offset  + row[3] * width_ratio,
                     height_offset + row[4] * height_ratio);
        }      
        glEnd();
      }
      
      if (value_count > 1) {
        glBegin(GL_LINE_STRIP);
        for(size_t i=0; i < value_count; i++) {
          glVertex2f(width_offset  + row[3 + i * 2   ] * width_ratio,
                     height_offset + row[3 + i * 2 + 1] * height_ratio);
        }      
        glEnd();
      }
    }
  }
  
  void set_mode_from_string(plot_type_t * pMode, const std::string& pStr)
  {
    if (pStr == "xy")
      *pMode = XYPlot;
    else if (pStr == "dots")
      *pMode = DotsPlot;
    else
      *pMode = TimePlot;
  }
  
  /*** Set RGB colors from an integer. */
  inline void set_color_from_int(int pId, double pAlpha)
  {
    uint col_id = hashId((uint)pId); // hashId defined in Hash template
    glColor4f(0.2 + 0.8 * (col_id % 100) / 100.0, // red color
              0.2 + 0.8 * (col_id % 60)  / 60.0,  // green color
              0.2 + 0.8 * (col_id % 20)  / 20.0,  // blue color
              pAlpha);
  }
  
  plot_type_t    mMode[PLOT_INLET_COUNT];       /**< Plot mode (XYPlot, TimePlot) each pair as an XY point instead of x(t). */
  const Matrix * mLiveBuffer[PLOT_INLET_COUNT]; /**< Pointer to the live stream (one matrix for each inlet). */
  size_t         mLineCount[PLOT_INLET_COUNT];
  size_t         mGroupSize[PLOT_INLET_COUNT];
  double         mPointSize[PLOT_INLET_COUNT];
  double         mMaxAmplitude[PLOT_INLET_COUNT];
};

extern "C" void init()
{
  CLASS(Plot)
  INLET(Plot,reference)
  INLET(Plot,dots)
}