#include "opencv/cv.h"

#include "lua_dub_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Mat::Mat()
 * include/opencv/cxcore.hpp:746
 */
static int Mat_Mat1(lua_State *L) {
  Mat * retval__       = new Mat();
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(int _rows, int _cols, int _type)
 * include/opencv/cxcore.hpp:754
 */
static int Mat_Mat2(lua_State *L) {
  int _rows            = luaL_checkint   (L, 1);
  int _cols            = luaL_checkint   (L, 2);
  int _type            = luaL_checkint   (L, 3);
  Mat * retval__       = new Mat(_rows, _cols, _type);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(Size _size, int _type)
 * include/opencv/cxcore.hpp:755
 */
static int Mat_Mat3(lua_State *L) {
  Size *_size          = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int _type            = luaL_checkint   (L, 2);
  Mat * retval__       = new Mat(*_size, _type);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(int _rows, int _cols, int _type, const Scalar &_s)
 * include/opencv/cxcore.hpp:757
 */
static int Mat_Mat4(lua_State *L) {
  int _rows            = luaL_checkint   (L, 1);
  int _cols            = luaL_checkint   (L, 2);
  int _type            = luaL_checkint   (L, 3);
  const Scalar *_s     = *((const Scalar **)luaL_checkudata(L, 4, "cv.Scalar"));
  Mat * retval__       = new Mat(_rows, _cols, _type, *_s);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(Size _size, int _type, const Scalar &_s)
 * include/opencv/cxcore.hpp:758
 */
static int Mat_Mat5(lua_State *L) {
  Size *_size          = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int _type            = luaL_checkint   (L, 2);
  const Scalar *_s     = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
  Mat * retval__       = new Mat(*_size, _type, *_s);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const Mat &m)
 * include/opencv/cxcore.hpp:760
 */
static int Mat_Mat6(lua_State *L) {
  const Mat *m         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  Mat * retval__       = new Mat(*m);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(int _rows, int _cols, int _type, void *_data, size_t _step=AUTO_STEP)
 * include/opencv/cxcore.hpp:774
 */
static int Mat_Mat7(lua_State *L) {
  int top__            = lua_gettop(L);
  int _rows            = luaL_checkint   (L, 1);
  int _cols            = luaL_checkint   (L, 2);
  int _type            = luaL_checkint   (L, 3);
  void *_data          = *((void **)luaL_checkudata(L, 4, "cv.void"));
  size_t _step         = top__ < 5 ? cv::Mat::AUTO_STEP : luaL_checknumber(L, 5);
  Mat * retval__       = new Mat(_rows, _cols, _type, _data, _step);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(Size _size, int _type, void *_data, size_t _step=AUTO_STEP)
 * include/opencv/cxcore.hpp:775
 */
static int Mat_Mat8(lua_State *L) {
  int top__            = lua_gettop(L);
  Size *_size          = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int _type            = luaL_checkint   (L, 2);
  void *_data          = *((void **)luaL_checkudata(L, 3, "cv.void"));
  size_t _step         = top__ < 4 ? cv::Mat::AUTO_STEP : luaL_checknumber(L, 4);
  Mat * retval__       = new Mat(*_size, _type, _data, _step);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const Mat &m, const Range &rowRange, const Range &colRange)
 * include/opencv/cxcore.hpp:777
 */
static int Mat_Mat9(lua_State *L) {
  const Mat *m         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  const Range *rowRange = *((const Range **)luaL_checkudata(L, 2, "cv.Range"));
  const Range *colRange = *((const Range **)luaL_checkudata(L, 3, "cv.Range"));
  Mat * retval__       = new Mat(*m, *rowRange, *colRange);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const Mat &m, const Rect &roi)
 * include/opencv/cxcore.hpp:784
 */
static int Mat_Mat10(lua_State *L) {
  const Mat *m         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  const Rect *roi      = *((const Rect **)luaL_checkudata(L, 2, "cv.Rect"));
  Mat * retval__       = new Mat(*m, *roi);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const CvMat *m, bool copyData=false)
 * include/opencv/cxcore.hpp:786
 */
static int Mat_Mat11(lua_State *L) {
  int top__            = lua_gettop(L);
  const CvMat *m       = *((const CvMat **)luaL_checkudata(L, 1, "cv.CvMat"));
  bool copyData        = top__ < 2 ? false : luaL_checkint(L, 2);
  Mat * retval__       = new Mat(m, copyData);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const IplImage *img, bool copyData=false)
 * include/opencv/cxcore.hpp:788
 */
static int Mat_Mat12(lua_State *L) {
  int top__            = lua_gettop(L);
  const IplImage *img  = *((const IplImage **)luaL_checkudata(L, 1, "cv.IplImage"));
  bool copyData        = top__ < 2 ? false : luaL_checkint(L, 2);
  Mat * retval__       = new Mat(img, copyData);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}


/** cv::Mat::Mat(const MatExpr_Base &expr)
 * include/opencv/cxcore.hpp:792
 */
static int Mat_Mat13(lua_State *L) {
  const MatExpr_Base *expr = *((const MatExpr_Base **)luaL_checkudata(L, 1, "cv.MatExpr_Base"));
  Mat * retval__       = new Mat(*expr);
  lua_pushclass<Mat>(L, retval__, "cv.Mat");
  return 1;
}



/** Overloaded function chooser for Mat(...) */
static int Mat_Mat(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatExpr_Base")) {
    return Mat_Mat13(L);
  } else if (type__ == LUA_TNONE) {
    return Mat_Mat1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvMat")) {
    return Mat_Mat11(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Rect")) {
      return Mat_Mat10(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Range")) {
      return Mat_Mat9(L);
    } else if (type__ == LUA_TNONE) {
      return Mat_Mat6(L);
    } else {
      // use any to raise errors
      return Mat_Mat6(L);
    }
  } else if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        type__ = lua_type(L, 4);
        if (type__ == LUA_TNONE) {
          return Mat_Mat2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.void")) {
          return Mat_Mat7(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Scalar")) {
          return Mat_Mat4(L);
        } else {
          // use any to raise errors
          return Mat_Mat4(L);
        }
      } else {
        // use any to raise errors
        return Mat_Mat4(L);
      }
    } else {
      // use any to raise errors
      return Mat_Mat4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.IplImage")) {
    return Mat_Mat12(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNONE) {
        return Mat_Mat3(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.void")) {
        return Mat_Mat8(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Scalar")) {
        return Mat_Mat5(L);
      } else {
        // use any to raise errors
        return Mat_Mat5(L);
      }
    } else {
      // use any to raise errors
      return Mat_Mat5(L);
    }
  } else {
    // use any to raise errors
    return Mat_Mat5(L);
  }
}

/* ============================ Destructor       ====================== */

static int Mat_destructor(lua_State *L) {
  Mat **userdata = (Mat**)luaL_checkudata(L, 1, "cv.Mat");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Mat__tostring(lua_State *L) {
  Mat **userdata = (Mat**)luaL_checkudata(L, 1, "cv.Mat");
  lua_pushfstring(L, "cv.Mat: %p", *userdata);
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void cv::Mat::addref()
 * include/opencv/cxcore.hpp:891
 */
static int Mat_addref(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  self__->addref();
  return 0;
}


/** Mat& cv::Mat::adjustROI(int dtop, int dbottom, int dleft, int dright)
 * include/opencv/cxcore.hpp:909
 */
static int Mat_adjustROI(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int dtop             = luaL_checkint   (L, 1);
  int dbottom          = luaL_checkint   (L, 2);
  int dleft            = luaL_checkint   (L, 3);
  int dright           = luaL_checkint   (L, 4);
  Mat  retval__        = self__->adjustROI(dtop, dbottom, dleft, dright);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** void cv::Mat::assignTo(Mat &m, int type=-1) const 
 * include/opencv/cxcore.hpp:845
 */
static int Mat_assignTo(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int top__            = lua_gettop(L);
  Mat *m               = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  int type             = top__ < 2 ? -1 : luaL_checkint(L, 2);
  self__->assignTo(*m, type);
  return 0;
}


/** int cv::Mat::channels() const 
 * include/opencv/cxcore.hpp:967
 */
static int Mat_channels(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int  retval__        = self__->channels();
  lua_pushnumber(L, retval__);
  return 1;
}


/** Mat cv::Mat::clone() const 
 * include/opencv/cxcore.hpp:824
 */
static int Mat_clone(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Mat  retval__        = self__->clone();
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** Mat cv::Mat::col(int x) const 
 * include/opencv/cxcore.hpp:804
 */
static int Mat_col(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int x                = luaL_checkint   (L, 1);
  Mat  retval__        = self__->col(x);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}



/** Mat cv::Mat::colRange(int startcol, int endcol) const 
 * include/opencv/cxcore.hpp:809
 */
static int Mat_colRange1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int startcol         = luaL_checkint   (L, 1);
  int endcol           = luaL_checkint   (L, 2);
  Mat  retval__        = self__->colRange(startcol, endcol);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** Mat cv::Mat::colRange(const Range &r) const 
 * include/opencv/cxcore.hpp:810
 */
static int Mat_colRange2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  const Range *r       = *((const Range **)luaL_checkudata(L, 1, "cv.Range"));
  Mat  retval__        = self__->colRange(*r);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}



/** Overloaded function chooser for colRange(...) */
static int Mat_colRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Range")) {
    return Mat_colRange2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Mat_colRange1(L);
  } else {
    // use any to raise errors
    return Mat_colRange1(L);
  }
}


/** void cv::Mat::convertTo(Mat &m, int rtype, double alpha=1, double beta=0) const 
 * include/opencv/cxcore.hpp:843
 */
static int Mat_convertTo(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int top__            = lua_gettop(L);
  Mat *m               = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  int rtype            = luaL_checkint   (L, 2);
  double alpha         = top__ < 3 ? 1 : luaL_checknumber(L, 3);
  double beta          = top__ < 4 ? 0 : luaL_checknumber(L, 4);
  self__->convertTo(*m, rtype, alpha, beta);
  return 0;
}



/** void cv::Mat::copyTo(Mat &m) const 
 * include/opencv/cxcore.hpp:829
 */
static int Mat_copyTo1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Mat *m               = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  self__->copyTo(*m);
  return 0;
}


/** void cv::Mat::copyTo(Mat &m, const Mat &mask) const 
 * include/opencv/cxcore.hpp:831
 */
static int Mat_copyTo2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Mat *m               = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  const Mat *mask      = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
  self__->copyTo(*m, *mask);
  return 0;
}



/** Overloaded function chooser for copyTo(...) */
static int Mat_copyTo(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNONE) {
      return Mat_copyTo1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return Mat_copyTo2(L);
    } else {
      // use any to raise errors
      return Mat_copyTo2(L);
    }
  } else {
    // use any to raise errors
    return Mat_copyTo2(L);
  }
}



/** void cv::Mat::create(int _rows, int _cols, int _type)
 * include/opencv/cxcore.hpp:888
 */
static int Mat_create1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int _rows            = luaL_checkint   (L, 1);
  int _cols            = luaL_checkint   (L, 2);
  int _type            = luaL_checkint   (L, 3);
  self__->create(_rows, _cols, _type);
  return 0;
}


/** void cv::Mat::create(Size _size, int _type)
 * include/opencv/cxcore.hpp:889
 */
static int Mat_create2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size *_size          = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int _type            = luaL_checkint   (L, 2);
  self__->create(*_size, _type);
  return 0;
}



/** Overloaded function chooser for create(...) */
static int Mat_create(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_create1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Mat_create2(L);
  } else {
    // use any to raise errors
    return Mat_create2(L);
  }
}


/** Mat cv::Mat::cross(const Mat &m) const 
 * include/opencv/cxcore.hpp:870
 */
static int Mat_cross(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  const Mat *m         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  Mat  retval__        = self__->cross(*m);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** int cv::Mat::depth() const 
 * include/opencv/cxcore.hpp:961
 */
static int Mat_depth(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int  retval__        = self__->depth();
  lua_pushnumber(L, retval__);
  return 1;
}



/** Mat cv::Mat::diag(int d=0) const 
 * include/opencv/cxcore.hpp:815
 */
static int Mat_diag1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int top__            = lua_gettop(L);
  int d                = top__ < 1 ? 0 : luaL_checkint(L, 1);
  Mat  retval__        = self__->diag(d);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** static Mat cv::Mat::diag(const Mat &d)
 * include/opencv/cxcore.hpp:817
 */
static int Mat_diag2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  const Mat *d         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  Mat  retval__        = self__->diag(*d);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}



/** Overloaded function chooser for diag(...) */
static int Mat_diag(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return Mat_diag2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Mat_diag1(L);
  } else {
    // use any to raise errors
    return Mat_diag1(L);
  }
}


/** double cv::Mat::dot(const Mat &m) const 
 * include/opencv/cxcore.hpp:872
 */
static int Mat_dot(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  const Mat *m         = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  double  retval__     = self__->dot(*m);
  lua_pushnumber(L, retval__);
  return 1;
}


/** size_t cv::Mat::elemSize() const 
 * include/opencv/cxcore.hpp:942
 */
static int Mat_elemSize(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  size_t  retval__     = self__->elemSize();
  lua_pushnumber(L, retval__);
  return 1;
}


/** size_t cv::Mat::elemSize1() const 
 * include/opencv/cxcore.hpp:948
 */
static int Mat_elemSize1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  size_t  retval__     = self__->elemSize1();
  lua_pushnumber(L, retval__);
  return 1;
}


/** bool cv::Mat::empty() const 
 * include/opencv/cxcore.hpp:983
 */
static int Mat_empty(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  bool  retval__       = self__->empty();
  lua_pushnumber(L, retval__);
  return 1;
}



/** static MatExpr_Initializer cv::Mat::eye(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:879
 */
static int Mat_eye1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int rows             = luaL_checkint   (L, 1);
  int cols             = luaL_checkint   (L, 2);
  int type             = luaL_checkint   (L, 3);
  MatExpr_Initializer  retval__ = self__->eye(rows, cols, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}


/** static MatExpr_Initializer cv::Mat::eye(Size size, int type)
 * include/opencv/cxcore.hpp:880
 */
static int Mat_eye2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size *size           = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int type             = luaL_checkint   (L, 2);
  MatExpr_Initializer  retval__ = self__->eye(*size, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}



/** Overloaded function chooser for eye(...) */
static int Mat_eye(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_eye1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Mat_eye2(L);
  } else {
    // use any to raise errors
    return Mat_eye2(L);
  }
}


/** bool cv::Mat::isContinuous() const 
 * include/opencv/cxcore.hpp:936
 */
static int Mat_isContinuous(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  bool  retval__       = self__->isContinuous();
  lua_pushnumber(L, retval__);
  return 1;
}


/** void cv::Mat::locateROI(Size &wholeSize, Point &ofs) const 
 * include/opencv/cxcore.hpp:897
 */
static int Mat_locateROI(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size *wholeSize      = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  Point *ofs           = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
  self__->locateROI(*wholeSize, *ofs);
  return 0;
}



/** static MatExpr_Initializer cv::Mat::ones(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:877
 */
static int Mat_ones1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int rows             = luaL_checkint   (L, 1);
  int cols             = luaL_checkint   (L, 2);
  int type             = luaL_checkint   (L, 3);
  MatExpr_Initializer  retval__ = self__->ones(rows, cols, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}


/** static MatExpr_Initializer cv::Mat::ones(Size size, int type)
 * include/opencv/cxcore.hpp:878
 */
static int Mat_ones2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size *size           = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int type             = luaL_checkint   (L, 2);
  MatExpr_Initializer  retval__ = self__->ones(*size, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}



/** Overloaded function chooser for ones(...) */
static int Mat_ones(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_ones1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Mat_ones2(L);
  } else {
    // use any to raise errors
    return Mat_ones2(L);
  }
}


/** void cv::Mat::release()
 * include/opencv/cxcore.hpp:894
 */
static int Mat_release(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  self__->release();
  return 0;
}


/** Mat cv::Mat::reshape(int _cn, int _rows=0) const 
 * include/opencv/cxcore.hpp:853
 */
static int Mat_reshape(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int top__            = lua_gettop(L);
  int _cn              = luaL_checkint   (L, 1);
  int _rows            = top__ < 2 ? 0 : luaL_checkint(L, 2);
  Mat  retval__        = self__->reshape(_cn, _rows);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** Mat cv::Mat::row(int y) const 
 * include/opencv/cxcore.hpp:802
 */
static int Mat_row(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int y                = luaL_checkint   (L, 1);
  Mat  retval__        = self__->row(y);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}



/** Mat cv::Mat::rowRange(int startrow, int endrow) const 
 * include/opencv/cxcore.hpp:806
 */
static int Mat_rowRange1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int startrow         = luaL_checkint   (L, 1);
  int endrow           = luaL_checkint   (L, 2);
  Mat  retval__        = self__->rowRange(startrow, endrow);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** Mat cv::Mat::rowRange(const Range &r) const 
 * include/opencv/cxcore.hpp:807
 */
static int Mat_rowRange2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  const Range *r       = *((const Range **)luaL_checkudata(L, 1, "cv.Range"));
  Mat  retval__        = self__->rowRange(*r);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}



/** Overloaded function chooser for rowRange(...) */
static int Mat_rowRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Range")) {
    return Mat_rowRange2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Mat_rowRange1(L);
  } else {
    // use any to raise errors
    return Mat_rowRange1(L);
  }
}


/** Mat& cv::Mat::setTo(const Scalar &s, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:850
 */
static int Mat_setTo(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int top__            = lua_gettop(L);
  const Scalar *s      = *((const Scalar **)luaL_checkudata(L, 1, "cv.Scalar"));
  const Mat *mask      = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
  Mat  retval__        = self__->setTo(*s, *mask);
  lua_pushclass<Mat>(L, retval__, "Mat.Mat");
  return 1;
}


/** Size cv::Mat::size() const 
 * include/opencv/cxcore.hpp:981
 */
static int Mat_size(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size  retval__       = self__->size();
  lua_pushclass<Size>(L, retval__, "Mat.Size");
  return 1;
}


/** size_t cv::Mat::step1() const 
 * include/opencv/cxcore.hpp:974
 */
static int Mat_step1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  size_t  retval__     = self__->step1();
  lua_pushnumber(L, retval__);
  return 1;
}


/** int cv::Mat::type() const 
 * include/opencv/cxcore.hpp:955
 */
static int Mat_type(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int  retval__        = self__->type();
  lua_pushnumber(L, retval__);
  return 1;
}



/** static MatExpr_Initializer cv::Mat::zeros(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:875
 */
static int Mat_zeros1(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  int rows             = luaL_checkint   (L, 1);
  int cols             = luaL_checkint   (L, 2);
  int type             = luaL_checkint   (L, 3);
  MatExpr_Initializer  retval__ = self__->zeros(rows, cols, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}


/** static MatExpr_Initializer cv::Mat::zeros(Size size, int type)
 * include/opencv/cxcore.hpp:876
 */
static int Mat_zeros2(lua_State *L) {
  Mat *self__          = *((Mat**)luaL_checkudata(L, 1, "cv.Mat"));
  lua_remove(L, 1);
  Size *size           = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
  int type             = luaL_checkint   (L, 2);
  MatExpr_Initializer  retval__ = self__->zeros(*size, type);
  lua_pushclass<MatExpr_Initializer>(L, retval__, "Mat.MatExpr_Initializer");
  return 1;
}



/** Overloaded function chooser for zeros(...) */
static int Mat_zeros(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_zeros1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Mat_zeros2(L);
  } else {
    // use any to raise errors
    return Mat_zeros2(L);
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Mat_member_methods[] = {
  {"addref"            , Mat_addref},
  {"adjustROI"         , Mat_adjustROI},
  {"assignTo"          , Mat_assignTo},
  {"channels"          , Mat_channels},
  {"clone"             , Mat_clone},
  {"col"               , Mat_col},
  {"colRange"          , Mat_colRange},
  {"convertTo"         , Mat_convertTo},
  {"copyTo"            , Mat_copyTo},
  {"create"            , Mat_create},
  {"cross"             , Mat_cross},
  {"depth"             , Mat_depth},
  {"diag"              , Mat_diag},
  {"dot"               , Mat_dot},
  {"elemSize"          , Mat_elemSize},
  {"elemSize1"         , Mat_elemSize1},
  {"empty"             , Mat_empty},
  {"eye"               , Mat_eye},
  {"isContinuous"      , Mat_isContinuous},
  {"locateROI"         , Mat_locateROI},
  {"ones"              , Mat_ones},
  {"release"           , Mat_release},
  {"reshape"           , Mat_reshape},
  {"row"               , Mat_row},
  {"rowRange"          , Mat_rowRange},
  {"setTo"             , Mat_setTo},
  {"size"              , Mat_size},
  {"step1"             , Mat_step1},
  {"type"              , Mat_type},
  {"zeros"             , Mat_zeros},
  {"__tostring"        , Mat__tostring},
  {"__gc"              , Mat_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Mat_namespace_methods[] = {
  {"Mat"               , Mat_Mat},
  {NULL, NULL},
};


static const struct lua_constants_Reg Mat_namespace_constants[] = {
  {"MAGIC_VAL"         , cv::Mat::MAGIC_VAL},
  {"AUTO_STEP"         , cv::Mat::AUTO_STEP},
  {"CONTINUOUS_FLAG"   , cv::Mat::CONTINUOUS_FLAG},
  {NULL, NULL},
};


void luaopen_cv_Mat(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Mat"); // "dub.Matrix"

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Mat_member_methods);

  // register class methods in a global table like "dub"
  luaL_register(L, "cv", Mat_namespace_methods);


  // register class enums
  register_constants(L, "cv.Mat_const", Mat_namespace_constants);

}
