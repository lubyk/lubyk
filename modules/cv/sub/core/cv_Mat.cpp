#include "cv/cv.h"

#include "lua_cpp_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Mat::Mat()
 * include/opencv/cxcore.hpp:746
 */
static int Mat_Mat1(lua_State *L) {
  try {
    Mat * retval__ = new Mat();
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(int _rows, int _cols, int _type)
 * include/opencv/cxcore.hpp:754
 */
static int Mat_Mat2(lua_State *L) {
  try {
    int _rows = dubL_checkint(L, 1);
    int _cols = dubL_checkint(L, 2);
    int _type = dubL_checkint(L, 3);
    Mat * retval__ = new Mat(_rows, _cols, _type);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(Size _size, int _type)
 * include/opencv/cxcore.hpp:755
 */
static int Mat_Mat3(lua_State *L) {
  try {
    Size *_size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int _type = dubL_checkint(L, 2);
    Mat * retval__ = new Mat(*_size, _type);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(int _rows, int _cols, int _type, const Scalar &_s)
 * include/opencv/cxcore.hpp:757
 */
static int Mat_Mat4(lua_State *L) {
  try {
    int _rows = dubL_checkint(L, 1);
    int _cols = dubL_checkint(L, 2);
    int _type = dubL_checkint(L, 3);
    const Scalar *_s = *((const Scalar **)dubL_checkudata(L, 4, "cv.Scalar"));
    Mat * retval__ = new Mat(_rows, _cols, _type, *_s);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(Size _size, int _type, const Scalar &_s)
 * include/opencv/cxcore.hpp:758
 */
static int Mat_Mat5(lua_State *L) {
  try {
    Size *_size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int _type = dubL_checkint(L, 2);
    const Scalar *_s = *((const Scalar **)dubL_checkudata(L, 3, "cv.Scalar"));
    Mat * retval__ = new Mat(*_size, _type, *_s);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const Mat &m)
 * include/opencv/cxcore.hpp:760
 */
static int Mat_Mat6(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    Mat * retval__ = new Mat(*m);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const Mat &m, const Range &rowRange, const Range &colRange)
 * include/opencv/cxcore.hpp:777
 */
static int Mat_Mat9(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    const Range *rowRange = *((const Range **)dubL_checkudata(L, 2, "cv.Range"));
    const Range *colRange = *((const Range **)dubL_checkudata(L, 3, "cv.Range"));
    Mat * retval__ = new Mat(*m, *rowRange, *colRange);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const Mat &m, const Rect &roi)
 * include/opencv/cxcore.hpp:784
 */
static int Mat_Mat10(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    const Rect *roi = *((const Rect **)dubL_checkudata(L, 2, "cv.Rect"));
    Mat * retval__ = new Mat(*m, *roi);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const CvMat *m, bool copyData=false)
 * include/opencv/cxcore.hpp:786
 */
static int Mat_Mat11(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat * retval__;
    const CvMat *m = *((const CvMat **)dubL_checkudata(L, 1, "cv.CvMat"));
    if (top__ < 2) {
      retval__ = new Mat(m);
    } else {
      bool copyData = lua_toboolean(L, 2);
      retval__ = new Mat(m, copyData);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const IplImage *img, bool copyData=false)
 * include/opencv/cxcore.hpp:788
 */
static int Mat_Mat12(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat * retval__;
    const IplImage *img = *((const IplImage **)dubL_checkudata(L, 1, "cv.IplImage"));
    if (top__ < 2) {
      retval__ = new Mat(img);
    } else {
      bool copyData = lua_toboolean(L, 2);
      retval__ = new Mat(img, copyData);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Mat::Mat(const MatExpr_Base &expr)
 * include/opencv/cxcore.hpp:792
 */
static int Mat_Mat13(lua_State *L) {
  try {
    const MatExpr_Base *expr = *((const MatExpr_Base **)dubL_checkudata(L, 1, "cv.MatExpr_Base"));
    Mat * retval__ = new Mat(*expr);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.Mat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.Mat: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for Mat(...) */
static int Mat_Mat(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatExpr_Base")) {
    return Mat_Mat13(L);
  } else if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return Mat_Mat2(L);
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
  } else if (top__ < 1) {
    return Mat_Mat1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvMat")) {
    return Mat_Mat11(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Rect")) {
      return Mat_Mat10(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Range")) {
      return Mat_Mat9(L);
    } else if (top__ < 2) {
      return Mat_Mat6(L);
    } else {
      // use any to raise errors
      return Mat_Mat6(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.IplImage")) {
    return Mat_Mat12(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (top__ < 3) {
        return Mat_Mat3(L);
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
  Mat **userdata = (Mat**)dubL_checkudata(L, 1, "cv.Mat");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Mat__tostring(lua_State *L) {
  Mat **userdata = (Mat**)dubL_checkudata(L, 1, "cv.Mat");

  lua_pushfstring(L, "<cv.Mat: %p %dx%d>", *userdata, (*userdata)->rows, (*userdata)->cols);

  return 1;
}

/* ============================ Member Methods   ====================== */


/** void cv::Mat::addref()
 * include/opencv/cxcore.hpp:892
 */
static int Mat_addref(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    self__->addref();
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.addref: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.addref: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.addref: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat& cv::Mat::adjustROI(int dtop, int dbottom, int dleft, int dright)
 * include/opencv/cxcore.hpp:910
 */
static int Mat_adjustROI(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int dtop = dubL_checkint(L, 1);
    int dbottom = dubL_checkint(L, 2);
    int dleft = dubL_checkint(L, 3);
    int dright = dubL_checkint(L, 4);
    Mat  retval__ = self__->adjustROI(dtop, dbottom, dleft, dright);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.adjustROI: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.adjustROI: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.adjustROI: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void cv::Mat::assignTo(Mat &m, int type=-1) const
 * include/opencv/cxcore.hpp:846
 */
static int Mat_assignTo(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Mat *m = *((Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      self__->assignTo(*m);
    } else {
      int type = dubL_checkint(L, 2);
      self__->assignTo(*m, type);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.assignTo: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.assignTo: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.assignTo: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int cv::Mat::channels() const
 * include/opencv/cxcore.hpp:968
 */
static int Mat_channels(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int  retval__ = self__->channels();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.channels: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.channels: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.channels: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::clone() const
 * include/opencv/cxcore.hpp:824
 */
static int Mat_clone(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Mat  retval__ = self__->clone();
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.clone: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.clone: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.clone: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::col(int x) const
 * include/opencv/cxcore.hpp:804
 */
static int Mat_col(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int x = dubL_checkint(L, 1);
    Mat  retval__ = self__->col(x);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.col: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.col: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.col: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Mat cv::Mat::colRange(int startcol, int endcol) const
 * include/opencv/cxcore.hpp:809
 */
static int Mat_colRange1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int startcol = dubL_checkint(L, 1);
    int endcol = dubL_checkint(L, 2);
    Mat  retval__ = self__->colRange(startcol, endcol);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.colRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.colRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.colRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::colRange(const Range &r) const
 * include/opencv/cxcore.hpp:810
 */
static int Mat_colRange2(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    const Range *r = *((const Range **)dubL_checkudata(L, 1, "cv.Range"));
    Mat  retval__ = self__->colRange(*r);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.colRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.colRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.colRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for colRange(...) */
static int Mat_colRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_colRange1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Range")) {
    return Mat_colRange2(L);
  } else {
    // use any to raise errors
    return Mat_colRange2(L);
  }
}


/** void cv::Mat::convertTo(Mat &m, int rtype, double alpha=1, double beta=0) const
 * include/opencv/cxcore.hpp:844
 */
static int Mat_convertTo(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Mat *m = *((Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    int rtype = dubL_checkint(L, 2);
    if (top__ < 3) {
      self__->convertTo(*m, rtype);
    } else {
      double alpha = dubL_checknumber(L, 3);
      if (top__ < 4) {
        self__->convertTo(*m, rtype, alpha);
      } else {
        double beta = dubL_checknumber(L, 4);
        self__->convertTo(*m, rtype, alpha, beta);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.convertTo: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.convertTo: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.convertTo: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** void cv::Mat::copyTo(Mat &m) const
 * include/opencv/cxcore.hpp:829
 */
static int Mat_copyTo1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Mat *m = *((Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    self__->copyTo(*m);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.copyTo: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.copyTo: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.copyTo: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void cv::Mat::copyTo(Mat &m, const Mat &mask) const
 * include/opencv/cxcore.hpp:831
 */
static int Mat_copyTo2(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Mat *m = *((Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    const Mat *mask = *((const Mat **)dubL_checkudata(L, 2, "cv.Mat"));
    self__->copyTo(*m, *mask);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.copyTo: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.copyTo: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.copyTo: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for copyTo(...) */
static int Mat_copyTo(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
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
 * include/opencv/cxcore.hpp:889
 */
static int Mat_create1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int _rows = dubL_checkint(L, 1);
    int _cols = dubL_checkint(L, 2);
    int _type = dubL_checkint(L, 3);
    self__->create(_rows, _cols, _type);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.create: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.create: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.create: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void cv::Mat::create(Size _size, int _type)
 * include/opencv/cxcore.hpp:890
 */
static int Mat_create2(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Size *_size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int _type = dubL_checkint(L, 2);
    self__->create(*_size, _type);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.create: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.create: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.create: Unknown exception");
    lua_error(L);
    return 0;
  }
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
 * include/opencv/cxcore.hpp:871
 */
static int Mat_cross(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    const Mat *m = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    Mat  retval__ = self__->cross(*m);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.cross: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.cross: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.cross: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int cv::Mat::depth() const
 * include/opencv/cxcore.hpp:962
 */
static int Mat_depth(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int  retval__ = self__->depth();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.depth: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.depth: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.depth: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Mat cv::Mat::diag(int d=0) const
 * include/opencv/cxcore.hpp:815
 */
static int Mat_diag1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Mat  retval__;
    if (top__ < 1) {
      retval__ = self__->diag();
    } else {
      int d = dubL_checkint(L, 1);
      retval__ = self__->diag(d);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.diag: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.diag: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.diag: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** static Mat cv::Mat::diag(const Mat &d)
 * include/opencv/cxcore.hpp:817
 */
static int Mat_diag2(lua_State *L) {
  try {
    const Mat *d = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    Mat  retval__ = Mat::diag(*d);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.diag: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.diag: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.diag: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for diag(...) */
static int Mat_diag(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_diag1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return Mat_diag2(L);
  } else {
    // use any to raise errors
    return Mat_diag2(L);
  }
}


/** double cv::Mat::dot(const Mat &m) const
 * include/opencv/cxcore.hpp:873
 */
static int Mat_dot(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    const Mat *m = *((const Mat **)dubL_checkudata(L, 1, "cv.Mat"));
    double  retval__ = self__->dot(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.dot: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.dot: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.dot: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** size_t cv::Mat::elemSize() const
 * include/opencv/cxcore.hpp:943
 */
static int Mat_elemSize(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    size_t  retval__ = self__->elemSize();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.elemSize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.elemSize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.elemSize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** size_t cv::Mat::elemSize1() const
 * include/opencv/cxcore.hpp:949
 */
static int Mat_elemSize1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    size_t  retval__ = self__->elemSize1();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.elemSize1: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.elemSize1: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.elemSize1: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** bool cv::Mat::empty() const
 * include/opencv/cxcore.hpp:984
 */
static int Mat_empty(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    bool  retval__ = self__->empty();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.empty: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.empty: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.empty: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static MatExpr_Initializer cv::Mat::eye(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:880
 */
static int Mat_eye1(lua_State *L) {
  try {
    int rows = dubL_checkint(L, 1);
    int cols = dubL_checkint(L, 2);
    int type = dubL_checkint(L, 3);
    MatExpr_Initializer  retval__ = Mat::eye(rows, cols, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.eye: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.eye: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.eye: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** static MatExpr_Initializer cv::Mat::eye(Size size, int type)
 * include/opencv/cxcore.hpp:881
 */
static int Mat_eye2(lua_State *L) {
  try {
    Size *size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int type = dubL_checkint(L, 2);
    MatExpr_Initializer  retval__ = Mat::eye(*size, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.eye: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.eye: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.eye: Unknown exception");
    lua_error(L);
    return 0;
  }
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
 * include/opencv/cxcore.hpp:937
 */
static int Mat_isContinuous(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    bool  retval__ = self__->isContinuous();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.isContinuous: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.isContinuous: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.isContinuous: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void cv::Mat::locateROI(Size &wholeSize, Point &ofs) const
 * include/opencv/cxcore.hpp:898
 */
static int Mat_locateROI(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Size *wholeSize = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    Point *ofs = *((Point **)dubL_checkudata(L, 2, "cv.Point"));
    self__->locateROI(*wholeSize, *ofs);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.locateROI: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.locateROI: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.locateROI: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static MatExpr_Initializer cv::Mat::ones(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:878
 */
static int Mat_ones1(lua_State *L) {
  try {
    int rows = dubL_checkint(L, 1);
    int cols = dubL_checkint(L, 2);
    int type = dubL_checkint(L, 3);
    MatExpr_Initializer  retval__ = Mat::ones(rows, cols, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.ones: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.ones: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.ones: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** static MatExpr_Initializer cv::Mat::ones(Size size, int type)
 * include/opencv/cxcore.hpp:879
 */
static int Mat_ones2(lua_State *L) {
  try {
    Size *size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int type = dubL_checkint(L, 2);
    MatExpr_Initializer  retval__ = Mat::ones(*size, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.ones: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.ones: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.ones: Unknown exception");
    lua_error(L);
    return 0;
  }
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
 * include/opencv/cxcore.hpp:895
 */
static int Mat_release(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    self__->release();
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.release: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.release: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.release: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::reshape(int _cn, int _rows=0) const
 * include/opencv/cxcore.hpp:854
 */
static int Mat_reshape(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Mat  retval__;
    int _cn = dubL_checkint(L, 1);
    if (top__ < 2) {
      retval__ = self__->reshape(_cn);
    } else {
      int _rows = dubL_checkint(L, 2);
      retval__ = self__->reshape(_cn, _rows);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.reshape: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.reshape: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.reshape: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::row(int y) const
 * include/opencv/cxcore.hpp:802
 */
static int Mat_row(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int y = dubL_checkint(L, 1);
    Mat  retval__ = self__->row(y);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.row: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.row: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.row: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Mat cv::Mat::rowRange(int startrow, int endrow) const
 * include/opencv/cxcore.hpp:806
 */
static int Mat_rowRange1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int startrow = dubL_checkint(L, 1);
    int endrow = dubL_checkint(L, 2);
    Mat  retval__ = self__->rowRange(startrow, endrow);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.rowRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.rowRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.rowRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Mat cv::Mat::rowRange(const Range &r) const
 * include/opencv/cxcore.hpp:807
 */
static int Mat_rowRange2(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    const Range *r = *((const Range **)dubL_checkudata(L, 1, "cv.Range"));
    Mat  retval__ = self__->rowRange(*r);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.rowRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.rowRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.rowRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for rowRange(...) */
static int Mat_rowRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    return Mat_rowRange1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Range")) {
    return Mat_rowRange2(L);
  } else {
    // use any to raise errors
    return Mat_rowRange2(L);
  }
}


/** Mat& cv::Mat::setTo(const Scalar &s, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:851
 */
static int Mat_setTo(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Mat  retval__;
    const Scalar *s = *((const Scalar **)dubL_checkudata(L, 1, "cv.Scalar"));
    if (top__ < 2) {
      retval__ = self__->setTo(*s);
    } else {
      const Mat *mask = *((const Mat **)dubL_checkudata(L, 2, "cv.Mat"));
      retval__ = self__->setTo(*s, *mask);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.setTo: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.setTo: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.setTo: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Size cv::Mat::size() const
 * include/opencv/cxcore.hpp:982
 */
static int Mat_size(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    Size  retval__ = self__->size();
    lua_pushclass<Size>(L, retval__, "cv.Size");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.size: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.size: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.size: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** size_t cv::Mat::step1() const
 * include/opencv/cxcore.hpp:975
 */
static int Mat_step1(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    size_t  retval__ = self__->step1();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.step1: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.step1: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.step1: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int cv::Mat::type() const
 * include/opencv/cxcore.hpp:956
 */
static int Mat_type(lua_State *L) {
  try {
    Mat *self__ = *((Mat**)dubL_checkudata(L, 1, "cv.Mat"));
    lua_remove(L, 1);
    int  retval__ = self__->type();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.type: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.type: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.type: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static MatExpr_Initializer cv::Mat::zeros(int rows, int cols, int type)
 * include/opencv/cxcore.hpp:876
 */
static int Mat_zeros1(lua_State *L) {
  try {
    int rows = dubL_checkint(L, 1);
    int cols = dubL_checkint(L, 2);
    int type = dubL_checkint(L, 3);
    MatExpr_Initializer  retval__ = Mat::zeros(rows, cols, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.zeros: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.zeros: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.zeros: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** static MatExpr_Initializer cv::Mat::zeros(Size size, int type)
 * include/opencv/cxcore.hpp:877
 */
static int Mat_zeros2(lua_State *L) {
  try {
    Size *size = *((Size **)dubL_checkudata(L, 1, "cv.Size"));
    int type = dubL_checkint(L, 2);
    MatExpr_Initializer  retval__ = Mat::zeros(*size, type);
    lua_pushclass<MatExpr_Initializer>(L, retval__, "cv.MatExpr_Initializer");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mat.zeros: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.zeros: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.zeros: Unknown exception");
    lua_error(L);
    return 0;
  }
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
  {"isContinuous"      , Mat_isContinuous},
  {"locateROI"         , Mat_locateROI},
  {"release"           , Mat_release},
  {"reshape"           , Mat_reshape},
  {"row"               , Mat_row},
  {"rowRange"          , Mat_rowRange},
  {"setTo"             , Mat_setTo},
  {"size"              , Mat_size},
  {"step1"             , Mat_step1},
  {"type"              , Mat_type},
  {"__tostring"        , Mat__tostring},
  {"__gc"              , Mat_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Mat_namespace_methods[] = {
  {"Mat"               , Mat_Mat},
  {"Mat_eye"           , Mat_eye},
  {"Mat_ones"          , Mat_ones},
  {"Mat_zeros"         , Mat_zeros},
  {NULL, NULL},
};


static const struct lua_constants_Reg Mat_namespace_constants[] = {
  {"MAGIC_VAL"         , cv::Mat::MAGIC_VAL},
  {"AUTO_STEP"         , cv::Mat::AUTO_STEP},
  {"CONTINUOUS_FLAG"   , cv::Mat::CONTINUOUS_FLAG},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_cv_Mat(lua_State *L) {
#else
extern "C" int luaopen_cv_Mat(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Mat");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Mat_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "cv", Mat_namespace_methods);


  // register class enums
  register_constants(L, "cv.Mat_const", Mat_namespace_constants);

}
