#include "rubyk.h"
#include "opengl.h"

typedef enum {
  XYPlot,
  TimePlot,
  DotsPlot,       // uses 'dot' vectors with color information (first 3 columns = RGB)
  LabelDotsPlot,  // first column = int label
} plot_type_t;

typedef enum {
  AutoColorMode,   // set from group index / label id
  BufferColorMode, // uses vectors with color information (first 3 columns = RGB)
  FixedColorMode, 
} plot_color_t;

class GLPlot : public Node
{
public:
  
  bool init ()
  {
    mLiveBuffer      = NULL;
    mLineCount       = 1;
    mGroupSize       = 1;
    mPointSize       = 4.0;
    mLineWidth       = 1.0;
    mMaxAmplitude    = 1.0;
    mMode            = TimePlot;
    
    mColorMode       = FixedColorMode;
    mAlpha           = 1.0;
    mFixedColorRed   = 1.0;
    mFixedColorGreen = 1.0;
    mFixedColorBlue  = 1.0;
    mWidth           = 600;
    mHeight          = 400;



    mDrawBase        = true;

    return true;
  }
  
  bool set (const Value& p)
  {
    std::string mode;
    p.get(&mLineCount,"line");
    p.get(&mGroupSize,"group");
    p.get(&mPointSize,"point");
    p.get(&mLineWidth,"thick");
    p.get(&mMaxAmplitude, "amplitude");
    p.get(&mAlpha, "alpha");
    p.get(&mFixedColorRed , "red");
    p.get(&mFixedColorGreen, "green");
    p.get(&mFixedColorBlue , "blue");
    
    p.get(&mDrawBase , "base");
    
    if (p.get(&mode, "mode"))
      set_mode_from_string(&mMode, mode);
      
    if (p.get(&mode, "color"))
      set_color_mode_from_string(&mColorMode, mode);
    
    return true;
  }
  
  // inlet 1
  void bang(const Value val)
  {
    mValue = &sig;
    val.get(&(mLiveBuffer));
    send(1, sig);
  }
  
  // inlet 2 (draw)
  void draw(const Value val)
  {
    const Matrix * mat;
    if (!is_opengl_thread()) return;
    if (mLiveBuffer == NULL) return;
    
    if (val.get(&mat) && mat->size() > 1) {
      mWidth  = mat->data[0];
      mHeight = mat->data[1];
    }
    
    // protected resource
    mServer->lock();
      switch(mMode) {
      case XYPlot:
        xy_plot(*mLiveBuffer);
        break;
      case DotsPlot:
      case LabelDotsPlot:
        dots_plot(*mLiveBuffer);
        break;
      default:
        time_plot(*mLiveBuffer);
      }
    mServer->unlock();
  }
  
  virtual const Value inspect(const Value val) 
  { 
    std::string str;
    
    if (mMode == TimePlot)
      str.append(bprint(mSpy, mSpySize, "time"));
    else if (mMode == DotsPlot)
      str.append(bprint(mSpy, mSpySize, "dots"));
    else if (mMode == LabelDotsPlot)
      str.append(bprint(mSpy, mSpySize, "label_dots"));
    else
      str.append(bprint(mSpy, mSpySize, "???"));
    bprint(mSpy, mSpySize,"%s", str.c_str());    
  }
  
private:
  /** Plot input matrix as a set of time based values. 
    * For example if we have m samples of n values, the values should be (all values for sample 1 then all values
    * for sample two, etc): (v_{1,1}, v_{1,2}, .., v_{1,n}, .., v_{m,n}).
    * The values can be grouped in 'line' groups of 'group' values. (4 lines of 3 values for 4 accelerometers 3D for example.)
    */
  void time_plot (const Matrix& mat)
  {
    size_t l_offset;
    size_t g_offset;

    size_t value_count  = mat.size() / (mLineCount * mGroupSize); 
    size_t sample_count = value_count;

    real_t width_ratio;
    real_t height_ratio = (real_t)mHeight / (2.0 * mLineCount * mMaxAmplitude); // values : [-1,1]
    real_t y_offset;

    size_t sample_offset = 0; // move all points to the left (shift view to the right)
    real_t x_offset      = 0;
    bool   draw_box      = false;

    ///// set x_offset, width_zoom with val.get_meta(...) /////
    mValue->get_meta(&sample_offset, H("sample_offset")); // shift display window right / left
    mValue->get_meta(&sample_count,  H("sample_count"));  // total number of samples per window when computing width_ratio
    mValue->get_meta(&draw_box,      H("draw_box"));      // draw a surrounding box

    real_t col_ratio = 0.8; //1.0 / mGroupSize;

    if (sample_count > 1)
      width_ratio = (real_t)mWidth  / (sample_count - 1);
    else
      width_ratio = (real_t)mWidth;

    x_offset = sample_offset * width_ratio;

    glPointSize(mPointSize);
    glLineWidth(mLineWidth);

    if (draw_box) {
      glColor4f(0.6,0.6,0.0,0.3);
      glRectd(x_offset, 0.0, x_offset + value_count * width_ratio, mHeight);
    }

    for(size_t l=0; l < mLineCount; l++) {
      l_offset = l * mGroupSize;
      y_offset = (l + 0.5) * mHeight / mLineCount;

      // draw base line
      if (mDrawBase) {
        glColor4f(0.3,0.3,0.3,0.9);
        glBegin(GL_LINE_STRIP);
        glVertex2d(0,y_offset);
        glVertex2d(mWidth,y_offset);
        glEnd();
      }


      for(size_t g=0; g < mGroupSize; g++) {
        g_offset = g * value_count;

        // element in group
        glColor4f(col_ratio * (g % mGroupSize),col_ratio * ((g+1) % mGroupSize),col_ratio * ((g+2) % mGroupSize),mAlpha);
        //set_color_from_int(g, pAlpha);

        glBegin(GL_LINE_STRIP);
        for(size_t i=0; i < value_count; i++) {

          glVertex2d(x_offset + i * width_ratio, y_offset + mat.data[i * mGroupSize * mLineCount + g + l_offset] * height_ratio);
        }
        glVertex2d(x_offset + value_count * width_ratio, y_offset + mat.data[(value_count - 1) * mGroupSize * mLineCount + g + l_offset] * height_ratio);
        glEnd();
      }
    }
  }
  /** Plot input matrix as a set of xy coordinates values. 
    * For example if we have two lines (A,B) of 3 points each the values would be: {Ax1,Ay1,Ax2,Ay2,Ax3,Ay3,Bx1,By1,Bx2,By2,Bx3,By3}
    */
  void xy_plot (const Matrix& mat)
  {    
    size_t line_count   = mLineCount;
    size_t value_count  = mat.size() / (2 * line_count); 
    real_t width_ratio  = (real_t)mWidth  / (2.0 * mMaxAmplitude);
    real_t height_ratio = (real_t)mHeight / (2.0 * mMaxAmplitude); // values : [-1,1]
    real_t width_offset = (real_t)mWidth  / 2.0;
    real_t height_offset= (real_t)mHeight / 2.0;

    glPointSize(mPointSize);
    glLineWidth(mLineWidth);

    for(size_t l=0; l < line_count; l++) {
      // element in group
      // 0 1 0 : 0  1  0 0 1
      // 1 0 0 : 1  2  0 1 0
      // 0 0 1 : 2  3  0 1 1

      // build color from integer
      if (mColorMode == AutoColorMode)
        set_color_from_int(l, mAlpha);
      else if (mColorMode == FixedColorMode)
        glColor4f(mFixedColorRed, mFixedColorGreen, mFixedColorBlue, mAlpha);
      else
        set_color_from_int(l, mAlpha); // FIXME: set color from buffer

      //glColor4f((int)((l+1)/2) % 4,(int)(l+1) % 2,(int)(l+1)/4,pAlpha);

      if (value_count > 0) {
        glBegin(GL_POINTS);
        glVertex2d(width_offset  + mat.data[l * 2    ] * width_ratio,
          height_offset + mat.data[l * 2 + 1] * height_ratio);
        glEnd();
      }

      glBegin(GL_LINE_STRIP);
      for(size_t i=0; i < value_count; i++) {
        glVertex2d(width_offset  + mat.data[(line_count * i * 2) + l * 2   ] * width_ratio,
          height_offset + mat.data[(line_count * i * 2) + l * 2 + 1] * height_ratio);
      }      
      glEnd(); 
    }
  }
  /** Plot input matrix as a set of xy coordinate values with color information.
    * The format is {color_info, data}. 'data' is the same as xy_plot. 'color_info' can be either an integer
    * used to create an arbitrary color (color:auto) or three values to define an RGB value.
    */
  void dots_plot (const Matrix& mat)
  {      
    size_t group_count  = mat.row_count();
    size_t value_count;
    real_t width_ratio  = (real_t)mWidth  / (2.0 * mMaxAmplitude);
    real_t height_ratio = (real_t)mHeight / (2.0 * mMaxAmplitude); // values : [-1,1]
    real_t width_offset = (real_t)mWidth  / 2.0;
    real_t height_offset= (real_t)mHeight / 2.0;

    glPointSize(mPointSize);
    glLineWidth(mLineWidth);

    for(size_t g=0; g < group_count; g++) {
      const real_t * row = mat[g];
      size_t start_index;

      if (mMode == DotsPlot) {
        glColor4f(row[0],row[1],row[2],mAlpha);
        start_index = 3;
      } else {
        set_color_from_int((int)row[0], mAlpha);
        start_index = 1;
      }

      value_count = (mat.col_count() - start_index) / 2;

      if (value_count > 0) {
        glBegin(GL_POINTS);
        for(size_t i=0; i < value_count; i++) {
          glVertex2d(width_offset  + row[start_index] * width_ratio,
            height_offset + row[start_index+1] * height_ratio);
        }      
        glEnd();
      }

      if (value_count > 1) {
        glBegin(GL_LINE_STRIP);
        for(size_t i=0; i < value_count; i++) {
          glVertex2d(width_offset  + row[start_index + i * 2   ] * width_ratio,
            height_offset + row[start_index + i * 2 + 1] * height_ratio);
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
    else if (pStr == "label_dots")
      *pMode = LabelDotsPlot;
    else
      *pMode = TimePlot;
  }
  
  void set_color_mode_from_string(plot_color_t * pMode, const std::string& pStr)
  {
    if (pStr == "fixed")
      *pMode = FixedColorMode;
    else if (pStr == "buffer")
      *pMode = BufferColorMode;
    else
      *pMode = AutoColorMode;
  }
  
  /*** Set RGB colors from an integer. */
  inline void set_color_from_int(int pId, real_t pAlpha)
  {
    uint col_id = hashId((uint)pId); // hashId defined in Hash template
    glColor4f(0.2 + 0.8 * (col_id % 100) / 100.0, // red color
              0.2 + 0.8 * (col_id % 60)  / 60.0,  // green color
              0.2 + 0.8 * (col_id % 20)  / 20.0,  // blue color
              pAlpha);
  }
  
  plot_type_t    mMode;       /**< Plot mode (XYPlot, TimePlot) each pair as an XY point instead of x(t). */
  const Matrix * mLiveBuffer; /**< Pointer to the live stream (one matrix for each inlet). */
  const Value * mValue;     /**< Live signal received on each inlet. */
  size_t         mLineCount;
  size_t         mGroupSize;
  real_t         mPointSize;
  real_t         mLineWidth;
  real_t         mMaxAmplitude;
  plot_color_t   mColorMode;
  real_t         mAlpha;
  real_t         mFixedColorRed;
  real_t         mFixedColorGreen;
  real_t         mFixedColorBlue;
  real_t         mHeight;
  real_t         mWidth;
  bool           mDrawBase;
};

extern "C" void init()
{
  CLASS(GLPlot)
  OUTLET(GLPlot,bang)
  INLET(GLPlot, draw)
}