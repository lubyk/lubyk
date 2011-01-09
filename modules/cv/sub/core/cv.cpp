#include "cv/cv.h"

#include "lua_cpp_helper.h"

using namespace cv;



/** CV_EXPORTS RotatedRect cv::CamShift(const Mat &probImage, Rect &window, TermCriteria criteria)
 * include/opencv/cv.hpp:602
 */
static int cv_CamShift(lua_State *L) {
  try {
    const Mat *probImage = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Rect *window = *((Rect **)luaL_checkudata(L, 2, "cv.Rect"));
    TermCriteria *criteria = *((TermCriteria **)luaL_checkudata(L, 3, "cv.TermCriteria"));
    RotatedRect  retval__ = CamShift(*probImage, *window, *criteria);
    lua_pushclass<RotatedRect>(L, retval__, "cv.RotatedRect");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.CamShift: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.CamShift: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.CamShift: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::Canny(const Mat &image, Mat &edges, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false)
 * include/opencv/cv.hpp:271
 */
static int cv_Canny(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *image = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *edges = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double threshold1 = luaL_checknumber(L, 3);
    double threshold2 = luaL_checknumber(L, 4);
    if (top__ < 5) {
      Canny(*image, *edges, threshold1, threshold2);
    } else {
      int apertureSize = luaL_checkint(L, 5);
      if (top__ < 6) {
        Canny(*image, *edges, threshold1, threshold2, apertureSize);
      } else {
        bool L2gradient = lua_toboolean(L, 6);
        Canny(*image, *edges, threshold1, threshold2, apertureSize, L2gradient);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Canny: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Canny: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Canny: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::GaussianBlur(const Mat &src, Mat &dst, Size ksize, double sigma1, double sigma2=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:232
 */
static int cv_GaussianBlur(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Size *ksize = *((Size **)luaL_checkudata(L, 3, "cv.Size"));
    double sigma1 = luaL_checknumber(L, 4);
    if (top__ < 5) {
      GaussianBlur(*src, *dst, *ksize, sigma1);
    } else {
      double sigma2 = luaL_checknumber(L, 5);
      if (top__ < 6) {
        GaussianBlur(*src, *dst, *ksize, sigma1, sigma2);
      } else {
        int borderType = luaL_checkint(L, 6);
        GaussianBlur(*src, *dst, *ksize, sigma1, sigma2, borderType);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.GaussianBlur: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.GaussianBlur: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.GaussianBlur: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::LUT(const Mat &a, const Mat &lut, Mat &b)
 * include/opencv/cxcore.hpp:1107
 */
static int cv_LUT(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *lut = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    LUT(*a, *lut, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.LUT: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.LUT: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.LUT: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::Laplacian(const Mat &src, Mat &dst, int ddepth, int ksize=1, double scale=1, double delta=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:267
 */
static int cv_Laplacian(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    if (top__ < 4) {
      Laplacian(*src, *dst, ddepth);
    } else {
      int ksize = luaL_checkint(L, 4);
      if (top__ < 5) {
        Laplacian(*src, *dst, ddepth, ksize);
      } else {
        double scale = luaL_checknumber(L, 5);
        if (top__ < 6) {
          Laplacian(*src, *dst, ddepth, ksize, scale);
        } else {
          double delta = luaL_checknumber(L, 6);
          if (top__ < 7) {
            Laplacian(*src, *dst, ddepth, ksize, scale, delta);
          } else {
            int borderType = luaL_checkint(L, 7);
            Laplacian(*src, *dst, ddepth, ksize, scale, delta, borderType);
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Laplacian: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Laplacian: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Laplacian: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::Mahalanobis(const Mat &v1, const Mat &v2, const Mat &icovar)
 * include/opencv/cxcore.hpp:1247
 */
static int cv_Mahalanobis(lua_State *L) {
  try {
    const Mat *v1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *v2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *icovar = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double  retval__ = Mahalanobis(*v1, *v2, *icovar);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mahalanobis: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mahalanobis: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mahalanobis: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static double cv::Mahalonobis(const Mat &v1, const Mat &v2, const Mat &icovar)
 * include/opencv/cxcore.hpp:1249
 */
static int cv_Mahalonobis(lua_State *L) {
  try {
    const Mat *v1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *v2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *icovar = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double  retval__ = Mahalonobis(*v1, *v2, *icovar);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Mahalonobis: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mahalonobis: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mahalonobis: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::RQDecomp3x3(const Mat &M, Mat &R, Mat &Q)
 * include/opencv/cv.hpp:760
 */
static int cv_RQDecomp3x31(lua_State *L) {
  try {
    const Mat *M = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *R = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *Q = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    RQDecomp3x3(*M, *R, *Q);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.RQDecomp3x3: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.RQDecomp3x3: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.RQDecomp3x3: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS Vec3d cv::RQDecomp3x3(const Mat &M, Mat &R, Mat &Q, Mat &Qx, Mat &Qy, Mat &Qz)
 * include/opencv/cv.hpp:762
 */
static int cv_RQDecomp3x32(lua_State *L) {
  try {
    const Mat *M = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *R = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *Q = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *Qx = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *Qy = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *Qz = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    Vec3d  retval__ = RQDecomp3x3(*M, *R, *Q, *Qx, *Qy, *Qz);
    lua_pushclass<Vec3d>(L, retval__, "cv.Vec3d");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.RQDecomp3x3: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.RQDecomp3x3: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.RQDecomp3x3: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for RQDecomp3x3(...) */
static int cv_RQDecomp3x3(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_RQDecomp3x31(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_RQDecomp3x32(L);
        } else {
          // use any to raise errors
          return cv_RQDecomp3x32(L);
        }
      } else {
        // use any to raise errors
        return cv_RQDecomp3x32(L);
      }
    } else {
      // use any to raise errors
      return cv_RQDecomp3x32(L);
    }
  } else {
    // use any to raise errors
    return cv_RQDecomp3x32(L);
  }
}




/** CV_EXPORTS void cv::Rodrigues(const Mat &src, Mat &dst)
 * include/opencv/cv.hpp:740
 */
static int cv_Rodrigues1(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Rodrigues(*src, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Rodrigues: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Rodrigues: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Rodrigues: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::Rodrigues(const Mat &src, Mat &dst, Mat &jacobian)
 * include/opencv/cv.hpp:741
 */
static int cv_Rodrigues2(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *jacobian = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Rodrigues(*src, *dst, *jacobian);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Rodrigues: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Rodrigues: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Rodrigues: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for Rodrigues(...) */
static int cv_Rodrigues(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (top__ < 3) {
        return cv_Rodrigues1(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        return cv_Rodrigues2(L);
      } else {
        // use any to raise errors
        return cv_Rodrigues2(L);
      }
    } else {
      // use any to raise errors
      return cv_Rodrigues2(L);
    }
  } else {
    // use any to raise errors
    return cv_Rodrigues2(L);
  }
}



/** CV_EXPORTS void cv::Scharr(const Mat &src, Mat &dst, int ddepth, int dx, int dy, double scale=1, double delta=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:263
 */
static int cv_Scharr(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    int dx = luaL_checkint(L, 4);
    int dy = luaL_checkint(L, 5);
    if (top__ < 6) {
      Scharr(*src, *dst, ddepth, dx, dy);
    } else {
      double scale = luaL_checknumber(L, 6);
      if (top__ < 7) {
        Scharr(*src, *dst, ddepth, dx, dy, scale);
      } else {
        double delta = luaL_checknumber(L, 7);
        if (top__ < 8) {
          Scharr(*src, *dst, ddepth, dx, dy, scale, delta);
        } else {
          int borderType = luaL_checkint(L, 8);
          Scharr(*src, *dst, ddepth, dx, dy, scale, delta, borderType);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scharr: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scharr: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scharr: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::Sobel(const Mat &src, Mat &dst, int ddepth, int dx, int dy, int ksize=3, double scale=1, double delta=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:259
 */
static int cv_Sobel(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    int dx = luaL_checkint(L, 4);
    int dy = luaL_checkint(L, 5);
    if (top__ < 6) {
      Sobel(*src, *dst, ddepth, dx, dy);
    } else {
      int ksize = luaL_checkint(L, 6);
      if (top__ < 7) {
        Sobel(*src, *dst, ddepth, dx, dy, ksize);
      } else {
        double scale = luaL_checknumber(L, 7);
        if (top__ < 8) {
          Sobel(*src, *dst, ddepth, dx, dy, ksize, scale);
        } else {
          double delta = luaL_checknumber(L, 8);
          if (top__ < 9) {
            Sobel(*src, *dst, ddepth, dx, dy, ksize, scale, delta);
          } else {
            int borderType = luaL_checkint(L, 9);
            Sobel(*src, *dst, ddepth, dx, dy, ksize, scale, delta, borderType);
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Sobel: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Sobel: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Sobel: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::absdiff(const Mat &a, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1157
 */
static int cv_absdiff1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    absdiff(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.absdiff: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.absdiff: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.absdiff: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::absdiff(const Mat &a, const Scalar &s, Mat &c)
 * include/opencv/cxcore.hpp:1158
 */
static int cv_absdiff2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    absdiff(*a, *s, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.absdiff: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.absdiff: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.absdiff: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::absdiff(const MatND &a, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1791
 */
static int cv_absdiff3(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    absdiff(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.absdiff: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.absdiff: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.absdiff: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::absdiff(const MatND &a, const Scalar &s, MatND &c)
 * include/opencv/cxcore.hpp:1792
 */
static int cv_absdiff4(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    absdiff(*a, *s, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.absdiff: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.absdiff: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.absdiff: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for absdiff(...) */
static int cv_absdiff(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_absdiff3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_absdiff4(L);
    } else {
      // use any to raise errors
      return cv_absdiff4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_absdiff1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_absdiff2(L);
    } else {
      // use any to raise errors
      return cv_absdiff2(L);
    }
  } else {
    // use any to raise errors
    return cv_absdiff2(L);
  }
}



/** CV_EXPORTS void cv::accumulate(const Mat &src, Mat &dst, const Mat &mask=Mat())
 * include/opencv/cv.hpp:375
 */
static int cv_accumulate(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      accumulate(*src, *dst);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
      accumulate(*src, *dst, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.accumulate: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.accumulate: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.accumulate: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::accumulateProduct(const Mat &src1, const Mat &src2, Mat &dst, const Mat &mask=Mat())
 * include/opencv/cv.hpp:378
 */
static int cv_accumulateProduct(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *src2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      accumulateProduct(*src1, *src2, *dst);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      accumulateProduct(*src1, *src2, *dst, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.accumulateProduct: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.accumulateProduct: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.accumulateProduct: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::accumulateSquare(const Mat &src, Mat &dst, const Mat &mask=Mat())
 * include/opencv/cv.hpp:376
 */
static int cv_accumulateSquare(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      accumulateSquare(*src, *dst);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
      accumulateSquare(*src, *dst, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.accumulateSquare: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.accumulateSquare: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.accumulateSquare: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::accumulateWeighted(const Mat &src, Mat &dst, double alpha, const Mat &mask=Mat())
 * include/opencv/cv.hpp:380
 */
static int cv_accumulateWeighted(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double alpha = luaL_checknumber(L, 3);
    if (top__ < 4) {
      accumulateWeighted(*src, *dst, alpha);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      accumulateWeighted(*src, *dst, alpha, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.accumulateWeighted: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.accumulateWeighted: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.accumulateWeighted: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::adaptiveThreshold(const Mat &src, Mat &dst, double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C)
 * include/opencv/cv.hpp:391
 */
static int cv_adaptiveThreshold(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double maxValue = luaL_checknumber(L, 3);
    int adaptiveMethod = luaL_checkint(L, 4);
    int thresholdType = luaL_checkint(L, 5);
    int blockSize = luaL_checkint(L, 6);
    double C = luaL_checknumber(L, 7);
    adaptiveThreshold(*src, *dst, maxValue, adaptiveMethod, thresholdType, blockSize, C);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.adaptiveThreshold: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.adaptiveThreshold: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.adaptiveThreshold: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::add(const Mat &a, const Mat &b, Mat &c, const Mat &mask)
 * include/opencv/cxcore.hpp:1079
 */
static int cv_add1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    add(*a, *b, *c, *mask);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::add(const Mat &a, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1087
 */
static int cv_add2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    add(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::add(const Mat &a, const Scalar &s, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1095
 */
static int cv_add3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      add(*a, *s, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      add(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::add(const MatND &a, const MatND &b, MatND &c, const MatND &mask)
 * include/opencv/cxcore.hpp:1748
 */
static int cv_add4(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
    add(*a, *b, *c, *mask);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::add(const MatND &a, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1750
 */
static int cv_add5(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    add(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::add(const MatND &a, const Scalar &s, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1752
 */
static int cv_add6(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      add(*a, *s, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      add(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.add: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.add: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.add: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for add(...) */
static int cv_add(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.MatND")) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_add5(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.MatND")) {
          return cv_add4(L);
        } else {
          // use any to raise errors
          return cv_add4(L);
        }
      } else {
        // use any to raise errors
        return cv_add4(L);
      }
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_add6(L);
    } else {
      // use any to raise errors
      return cv_add6(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_add2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_add1(L);
        } else {
          // use any to raise errors
          return cv_add1(L);
        }
      } else {
        // use any to raise errors
        return cv_add1(L);
      }
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_add3(L);
    } else {
      // use any to raise errors
      return cv_add3(L);
    }
  } else {
    // use any to raise errors
    return cv_add3(L);
  }
}




/** CV_EXPORTS void cv::addWeighted(const Mat &a, double alpha, const Mat &b, double beta, double gamma, Mat &c)
 * include/opencv/cxcore.hpp:1105
 */
static int cv_addWeighted1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double alpha = luaL_checknumber(L, 2);
    const Mat *b = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double beta = luaL_checknumber(L, 4);
    double gamma = luaL_checknumber(L, 5);
    Mat *c = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    addWeighted(*a, alpha, *b, beta, gamma, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.addWeighted: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.addWeighted: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.addWeighted: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::addWeighted(const MatND &a, double alpha, const MatND &b, double beta, double gamma, MatND &c)
 * include/opencv/cxcore.hpp:1761
 */
static int cv_addWeighted2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double alpha = luaL_checknumber(L, 2);
    const MatND *b = *((const MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    double beta = luaL_checknumber(L, 4);
    double gamma = luaL_checknumber(L, 5);
    MatND *c = *((MatND **)luaL_checkudata(L, 6, "cv.MatND"));
    addWeighted(*a, alpha, *b, beta, gamma, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.addWeighted: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.addWeighted: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.addWeighted: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for addWeighted(...) */
static int cv_addWeighted(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_addWeighted2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_addWeighted1(L);
  } else {
    // use any to raise errors
    return cv_addWeighted1(L);
  }
}



/** static size_t cv::alignSize(size_t sz, int n)
 * include/opencv/cxcore.hpp:159
 */
static int cv_alignSize(lua_State *L) {
  try {
    size_t sz = luaL_checkint(L, 1);
    int n = luaL_checkint(L, 2);
    size_t  retval__ = alignSize(sz, n);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.alignSize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.alignSize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.alignSize: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::arcLength(const Mat &curve, bool closed)
 * include/opencv/cv.hpp:559
 */
static int cv_arcLength(lua_State *L) {
  try {
    const Mat *curve = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    bool closed = lua_toboolean(L, 2);
    double  retval__ = arcLength(*curve, closed);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.arcLength: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.arcLength: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.arcLength: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::bilateralFilter(const Mat &src, Mat &dst, int d, double sigmaColor, double sigmaSpace, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:235
 */
static int cv_bilateralFilter(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int d = luaL_checkint(L, 3);
    double sigmaColor = luaL_checknumber(L, 4);
    double sigmaSpace = luaL_checknumber(L, 5);
    if (top__ < 6) {
      bilateralFilter(*src, *dst, d, sigmaColor, sigmaSpace);
    } else {
      int borderType = luaL_checkint(L, 6);
      bilateralFilter(*src, *dst, d, sigmaColor, sigmaSpace, borderType);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bilateralFilter: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bilateralFilter: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bilateralFilter: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::bitwise_and(const Mat &a, const Mat &b, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1150
 */
static int cv_bitwise_and1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_and(*a, *b, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_and(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_and: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_and: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_and: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_and(const Mat &a, const Scalar &s, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1153
 */
static int cv_bitwise_and2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_and(*a, *s, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_and(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_and: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_and: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_and: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_and(const MatND &a, const MatND &b, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1784
 */
static int cv_bitwise_and3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_and(*a, *b, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_and(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_and: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_and: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_and: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_and(const MatND &a, const Scalar &s, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1787
 */
static int cv_bitwise_and4(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_and(*a, *s, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_and(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_and: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_and: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_and: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for bitwise_and(...) */
static int cv_bitwise_and(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_bitwise_and3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_and4(L);
    } else {
      // use any to raise errors
      return cv_bitwise_and4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_bitwise_and1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_and2(L);
    } else {
      // use any to raise errors
      return cv_bitwise_and2(L);
    }
  } else {
    // use any to raise errors
    return cv_bitwise_and2(L);
  }
}




/** CV_EXPORTS void cv::bitwise_not(const Mat &a, Mat &c)
 * include/opencv/cxcore.hpp:1156
 */
static int cv_bitwise_not1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    bitwise_not(*a, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_not: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_not: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_not: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_not(const MatND &a, MatND &c)
 * include/opencv/cxcore.hpp:1790
 */
static int cv_bitwise_not2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    bitwise_not(*a, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_not: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_not: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_not: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for bitwise_not(...) */
static int cv_bitwise_not(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_bitwise_not2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_bitwise_not1(L);
  } else {
    // use any to raise errors
    return cv_bitwise_not1(L);
  }
}




/** CV_EXPORTS void cv::bitwise_or(const Mat &a, const Mat &b, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1151
 */
static int cv_bitwise_or1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_or(*a, *b, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_or(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_or: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_or: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_or: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_or(const Mat &a, const Scalar &s, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1154
 */
static int cv_bitwise_or2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_or(*a, *s, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_or(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_or: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_or: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_or: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_or(const MatND &a, const MatND &b, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1785
 */
static int cv_bitwise_or3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_or(*a, *b, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_or(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_or: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_or: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_or: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_or(const MatND &a, const Scalar &s, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1788
 */
static int cv_bitwise_or4(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_or(*a, *s, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_or(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_or: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_or: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_or: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for bitwise_or(...) */
static int cv_bitwise_or(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_bitwise_or3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_or4(L);
    } else {
      // use any to raise errors
      return cv_bitwise_or4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_bitwise_or1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_or2(L);
    } else {
      // use any to raise errors
      return cv_bitwise_or2(L);
    }
  } else {
    // use any to raise errors
    return cv_bitwise_or2(L);
  }
}




/** CV_EXPORTS void cv::bitwise_xor(const Mat &a, const Mat &b, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1152
 */
static int cv_bitwise_xor1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_xor(*a, *b, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_xor(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_xor: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_xor(const Mat &a, const Scalar &s, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1155
 */
static int cv_bitwise_xor2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      bitwise_xor(*a, *s, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      bitwise_xor(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_xor: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_xor(const MatND &a, const MatND &b, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1786
 */
static int cv_bitwise_xor3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_xor(*a, *b, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_xor(*a, *b, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_xor: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::bitwise_xor(const MatND &a, const Scalar &s, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1789
 */
static int cv_bitwise_xor4(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      bitwise_xor(*a, *s, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      bitwise_xor(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.bitwise_xor: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.bitwise_xor: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for bitwise_xor(...) */
static int cv_bitwise_xor(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_bitwise_xor3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_xor4(L);
    } else {
      // use any to raise errors
      return cv_bitwise_xor4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_bitwise_xor1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_bitwise_xor2(L);
    } else {
      // use any to raise errors
      return cv_bitwise_xor2(L);
    }
  } else {
    // use any to raise errors
    return cv_bitwise_xor2(L);
  }
}



/** static void cv::blur(const Mat &src, Mat &dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:243
 */
static int cv_blur(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Size *ksize = *((Size **)luaL_checkudata(L, 3, "cv.Size"));
    if (top__ < 4) {
      blur(*src, *dst, *ksize);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 4, "cv.Point"));
      if (top__ < 5) {
        blur(*src, *dst, *ksize, *anchor);
      } else {
        int borderType = luaL_checkint(L, 5);
        blur(*src, *dst, *ksize, *anchor, borderType);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.blur: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.blur: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.blur: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::borderInterpolate(int p, int len, int borderType)
 * include/opencv/cv.hpp:58
 */
static int cv_borderInterpolate(lua_State *L) {
  try {
    int p = luaL_checkint(L, 1);
    int len = luaL_checkint(L, 2);
    int borderType = luaL_checkint(L, 3);
    int  retval__ = borderInterpolate(p, len, borderType);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.borderInterpolate: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.borderInterpolate: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.borderInterpolate: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Rect cv::boundingRect(const Mat &points)
 * include/opencv/cv.hpp:560
 */
static int cv_boundingRect(lua_State *L) {
  try {
    const Mat *points = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Rect  retval__ = boundingRect(*points);
    lua_pushclass<Rect>(L, retval__, "cv.Rect");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.boundingRect: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.boundingRect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.boundingRect: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::boxFilter(const Mat &src, Mat &dst, int ddepth, Size ksize, Point anchor=Point(-1,-1), bool normalize=true, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:239
 */
static int cv_boxFilter(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    Size *ksize = *((Size **)luaL_checkudata(L, 4, "cv.Size"));
    if (top__ < 5) {
      boxFilter(*src, *dst, ddepth, *ksize);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 5, "cv.Point"));
      if (top__ < 6) {
        boxFilter(*src, *dst, ddepth, *ksize, *anchor);
      } else {
        bool normalize = lua_toboolean(L, 6);
        if (top__ < 7) {
          boxFilter(*src, *dst, ddepth, *ksize, *anchor, normalize);
        } else {
          int borderType = luaL_checkint(L, 7);
          boxFilter(*src, *dst, ddepth, *ksize, *anchor, normalize, borderType);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.boxFilter: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.boxFilter: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.boxFilter: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::calcCovarMatrix(const Mat &samples, Mat &covar, Mat &mean, int flags, int ctype=CV_64F)
 * include/opencv/cxcore.hpp:1215
 */
static int cv_calcCovarMatrix2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *samples = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *covar = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *mean = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int flags = luaL_checkint(L, 4);
    if (top__ < 5) {
      calcCovarMatrix(*samples, *covar, *mean, flags);
    } else {
      int ctype = luaL_checkint(L, 5);
      calcCovarMatrix(*samples, *covar, *mean, flags, ctype);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.calcCovarMatrix: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.calcCovarMatrix: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.calcCovarMatrix: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for calcCovarMatrix(...) */
static int cv_calcCovarMatrix(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_calcCovarMatrix2(L);
  } else {
    // use any to raise errors
    return cv_calcCovarMatrix2(L);
  }
}



/** CV_EXPORTS double cv::calcGlobalOrientation(const Mat &orientation, const Mat &mask, const Mat &mhi, double timestamp, double duration)
 * include/opencv/cv.hpp:598
 */
static int cv_calcGlobalOrientation(lua_State *L) {
  try {
    const Mat *orientation = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *mhi = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double timestamp = luaL_checknumber(L, 4);
    double duration = luaL_checknumber(L, 5);
    double  retval__ = calcGlobalOrientation(*orientation, *mask, *mhi, timestamp, duration);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.calcGlobalOrientation: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.calcGlobalOrientation: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.calcGlobalOrientation: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::calcMotionGradient(const Mat &mhi, Mat &mask, Mat &orientation, double delta1, double delta2, int apertureSize=3)
 * include/opencv/cv.hpp:594
 */
static int cv_calcMotionGradient(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *mhi = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *mask = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *orientation = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double delta1 = luaL_checknumber(L, 4);
    double delta2 = luaL_checknumber(L, 5);
    if (top__ < 6) {
      calcMotionGradient(*mhi, *mask, *orientation, delta1, delta2);
    } else {
      int apertureSize = luaL_checkint(L, 6);
      calcMotionGradient(*mhi, *mask, *orientation, delta1, delta2, apertureSize);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.calcMotionGradient: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.calcMotionGradient: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.calcMotionGradient: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::calcOpticalFlowFarneback(const Mat &prev0, const Mat &next0, Mat &flow0, double pyr_scale, int levels, int winsize, int iterations, int poly_n, double poly_sigma, int flags)
 * include/opencv/cv.hpp:423
 */
static int cv_calcOpticalFlowFarneback(lua_State *L) {
  try {
    const Mat *prev0 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *next0 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *flow0 = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double pyr_scale = luaL_checknumber(L, 4);
    int levels = luaL_checkint(L, 5);
    int winsize = luaL_checkint(L, 6);
    int iterations = luaL_checkint(L, 7);
    int poly_n = luaL_checkint(L, 8);
    double poly_sigma = luaL_checknumber(L, 9);
    int flags = luaL_checkint(L, 10);
    calcOpticalFlowFarneback(*prev0, *next0, *flow0, pyr_scale, levels, winsize, iterations, poly_n, poly_sigma, flags);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.calcOpticalFlowFarneback: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.calcOpticalFlowFarneback: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.calcOpticalFlowFarneback: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::calibrationMatrixValues(const Mat &cameraMatrix, Size imageSize, double apertureWidth, double apertureHeight, double &fovx, double &fovy, double &focalLength, Point2d &principalPoint, double &aspectRatio)
 * include/opencv/cv.hpp:856
 */
static int cv_calibrationMatrixValues(lua_State *L) {
  try {
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Size *imageSize = *((Size **)luaL_checkudata(L, 2, "cv.Size"));
    double apertureWidth = luaL_checknumber(L, 3);
    double apertureHeight = luaL_checknumber(L, 4);
    double fovx = luaL_checknumber(L, 5);
    double fovy = luaL_checknumber(L, 6);
    double focalLength = luaL_checknumber(L, 7);
    Point2d *principalPoint = *((Point2d **)luaL_checkudata(L, 8, "cv.Point2d"));
    double aspectRatio = luaL_checknumber(L, 9);
    calibrationMatrixValues(*cameraMatrix, *imageSize, apertureWidth, apertureHeight, fovx, fovy, focalLength, *principalPoint, aspectRatio);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.calibrationMatrixValues: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.calibrationMatrixValues: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.calibrationMatrixValues: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::cartToPolar(const Mat &x, const Mat &y, Mat &magnitude, Mat &angle, bool angleInDegrees=false)
 * include/opencv/cxcore.hpp:1180
 */
static int cv_cartToPolar(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *x = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *y = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *magnitude = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *angle = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      cartToPolar(*x, *y, *magnitude, *angle);
    } else {
      bool angleInDegrees = lua_toboolean(L, 5);
      cartToPolar(*x, *y, *magnitude, *angle, angleInDegrees);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cartToPolar: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cartToPolar: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cartToPolar: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS bool cv::checkRange(const MatND &a, bool quiet=true, int *idx=0, double minVal=-DBL_MAX, double maxVal=DBL_MAX)
 * include/opencv/cxcore.hpp:1809
 */
static int cv_checkRange2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    bool  retval__;
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    if (top__ < 2) {
      retval__ = checkRange(*a);
    } else {
      bool quiet = lua_toboolean(L, 2);
      if (top__ < 3) {
        retval__ = checkRange(*a, quiet);
      } else {

        DubArgPointer<int> ptr_idx;
        int *idx = ptr_idx(L, 3);
        if (top__ < 4) {
          retval__ = checkRange(*a, quiet, idx);
        } else {
          double minVal = luaL_checknumber(L, 4);
          if (top__ < 5) {
            retval__ = checkRange(*a, quiet, idx, minVal);
          } else {
            double maxVal = luaL_checknumber(L, 5);
            retval__ = checkRange(*a, quiet, idx, minVal, maxVal);
          }
        }
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.checkRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.checkRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.checkRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for checkRange(...) */
static int cv_checkRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_checkRange2(L);
  } else {
    // use any to raise errors
    return cv_checkRange2(L);
  }
}



/** CV_EXPORTS void cv::circle(Mat &img, Point center, int radius, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
 * include/opencv/cxcore.hpp:1287
 */
static int cv_circle(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point *center = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    int radius = luaL_checkint(L, 3);
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 4, "cv.Scalar"));
    if (top__ < 5) {
      circle(*img, *center, radius, *color);
    } else {
      int thickness = luaL_checkint(L, 5);
      if (top__ < 6) {
        circle(*img, *center, radius, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 6);
        if (top__ < 7) {
          circle(*img, *center, radius, *color, thickness, lineType);
        } else {
          int shift = luaL_checkint(L, 7);
          circle(*img, *center, radius, *color, thickness, lineType, shift);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.circle: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.circle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.circle: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS bool cv::clipLine(Size imgSize, Point &pt1, Point &pt2)
 * include/opencv/cxcore.hpp:1308
 */
static int cv_clipLine1(lua_State *L) {
  try {
    Size *imgSize = *((Size **)luaL_checkudata(L, 1, "cv.Size"));
    Point *pt1 = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    Point *pt2 = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
    bool  retval__ = clipLine(*imgSize, *pt1, *pt2);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.clipLine: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.clipLine: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.clipLine: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS bool cv::clipLine(Rect img_rect, Point &pt1, Point &pt2)
 * include/opencv/cxcore.hpp:1309
 */
static int cv_clipLine2(lua_State *L) {
  try {
    Rect *img_rect = *((Rect **)luaL_checkudata(L, 1, "cv.Rect"));
    Point *pt1 = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    Point *pt2 = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
    bool  retval__ = clipLine(*img_rect, *pt1, *pt2);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.clipLine: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.clipLine: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.clipLine: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for clipLine(...) */
static int cv_clipLine(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Rect")) {
    return cv_clipLine2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return cv_clipLine1(L);
  } else {
    // use any to raise errors
    return cv_clipLine1(L);
  }
}




/** CV_EXPORTS void cv::compare(const Mat &a, const Mat &b, Mat &c, int cmpop)
 * include/opencv/cxcore.hpp:1163
 */
static int cv_compare1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int cmpop = luaL_checkint(L, 4);
    compare(*a, *b, *c, cmpop);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compare: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compare: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compare: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::compare(const Mat &a, double s, Mat &c, int cmpop)
 * include/opencv/cxcore.hpp:1164
 */
static int cv_compare2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double s = luaL_checknumber(L, 2);
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int cmpop = luaL_checkint(L, 4);
    compare(*a, s, *c, cmpop);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compare: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compare: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compare: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::compare(const MatND &a, const MatND &b, MatND &c, int cmpop)
 * include/opencv/cxcore.hpp:1797
 */
static int cv_compare3(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    int cmpop = luaL_checkint(L, 4);
    compare(*a, *b, *c, cmpop);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compare: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compare: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compare: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::compare(const MatND &a, double s, MatND &c, int cmpop)
 * include/opencv/cxcore.hpp:1798
 */
static int cv_compare4(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double s = luaL_checknumber(L, 2);
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    int cmpop = luaL_checkint(L, 4);
    compare(*a, s, *c, cmpop);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compare: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compare: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compare: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for compare(...) */
static int cv_compare(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_compare4(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_compare3(L);
    } else {
      // use any to raise errors
      return cv_compare3(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_compare2(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_compare1(L);
    } else {
      // use any to raise errors
      return cv_compare1(L);
    }
  } else {
    // use any to raise errors
    return cv_compare1(L);
  }
}




/** CV_EXPORTS double cv::compareHist(const MatND &H1, const MatND &H2, int method)
 * include/opencv/cv.hpp:451
 */
static int cv_compareHist1(lua_State *L) {
  try {
    const MatND *H1 = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *H2 = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    int method = luaL_checkint(L, 3);
    double  retval__ = compareHist(*H1, *H2, method);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compareHist: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compareHist: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compareHist: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::compareHist(const SparseMat &H1, const SparseMat &H2, int method)
 * include/opencv/cv.hpp:453
 */
static int cv_compareHist2(lua_State *L) {
  try {
    const SparseMat *H1 = *((const SparseMat **)luaL_checkudata(L, 1, "cv.SparseMat"));
    const SparseMat *H2 = *((const SparseMat **)luaL_checkudata(L, 2, "cv.SparseMat"));
    int method = luaL_checkint(L, 3);
    double  retval__ = compareHist(*H1, *H2, method);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.compareHist: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.compareHist: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.compareHist: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for compareHist(...) */
static int cv_compareHist(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.SparseMat")) {
    return cv_compareHist2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_compareHist1(L);
  } else {
    // use any to raise errors
    return cv_compareHist1(L);
  }
}



/** CV_EXPORTS void cv::completeSymm(Mat &a, bool lowerToUpper=false)
 * include/opencv/cxcore.hpp:1196
 */
static int cv_completeSymm(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *a = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      completeSymm(*a);
    } else {
      bool lowerToUpper = lua_toboolean(L, 2);
      completeSymm(*a, lowerToUpper);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.completeSymm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.completeSymm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.completeSymm: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::composeRT(const Mat &rvec1, const Mat &tvec1, const Mat &rvec2, const Mat &tvec2, Mat &rvec3, Mat &tvec3)
 * include/opencv/cv.hpp:775
 */
static int cv_composeRT1(lua_State *L) {
  try {
    const Mat *rvec1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *tvec1 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *rvec2 = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *tvec2 = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *rvec3 = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *tvec3 = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    composeRT(*rvec1, *tvec1, *rvec2, *tvec2, *rvec3, *tvec3);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.composeRT: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.composeRT: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.composeRT: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::composeRT(const Mat &rvec1, const Mat &tvec1, const Mat &rvec2, const Mat &tvec2, Mat &rvec3, Mat &tvec3, Mat &dr3dr1, Mat &dr3dt1, Mat &dr3dr2, Mat &dr3dt2, Mat &dt3dr1, Mat &dt3dt1, Mat &dt3dr2, Mat &dt3dt2)
 * include/opencv/cv.hpp:783
 */
static int cv_composeRT2(lua_State *L) {
  try {
    const Mat *rvec1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *tvec1 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *rvec2 = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *tvec2 = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *rvec3 = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *tvec3 = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    Mat *dr3dr1 = *((Mat **)luaL_checkudata(L, 7, "cv.Mat"));
    Mat *dr3dt1 = *((Mat **)luaL_checkudata(L, 8, "cv.Mat"));
    Mat *dr3dr2 = *((Mat **)luaL_checkudata(L, 9, "cv.Mat"));
    Mat *dr3dt2 = *((Mat **)luaL_checkudata(L, 10, "cv.Mat"));
    Mat *dt3dr1 = *((Mat **)luaL_checkudata(L, 11, "cv.Mat"));
    Mat *dt3dt1 = *((Mat **)luaL_checkudata(L, 12, "cv.Mat"));
    Mat *dt3dr2 = *((Mat **)luaL_checkudata(L, 13, "cv.Mat"));
    Mat *dt3dt2 = *((Mat **)luaL_checkudata(L, 14, "cv.Mat"));
    composeRT(*rvec1, *tvec1, *rvec2, *tvec2, *rvec3, *tvec3, *dr3dr1, *dr3dt1, *dr3dr2, *dr3dt2, *dt3dr1, *dt3dt1, *dt3dr2, *dt3dt2);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.composeRT: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.composeRT: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.composeRT: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for composeRT(...) */
static int cv_composeRT(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          type__ = lua_type(L, 5);
          if (type__ == LUA_TUSERDATA && is_userdata(L, 5, "cv.Mat")) {
            type__ = lua_type(L, 6);
            if (type__ == LUA_TUSERDATA && is_userdata(L, 6, "cv.Mat")) {
              type__ = lua_type(L, 7);
              if (top__ < 7) {
                return cv_composeRT1(L);
              } else if (type__ == LUA_TUSERDATA && is_userdata(L, 7, "cv.Mat")) {
                return cv_composeRT2(L);
              } else {
                // use any to raise errors
                return cv_composeRT2(L);
              }
            } else {
              // use any to raise errors
              return cv_composeRT2(L);
            }
          } else {
            // use any to raise errors
            return cv_composeRT2(L);
          }
        } else {
          // use any to raise errors
          return cv_composeRT2(L);
        }
      } else {
        // use any to raise errors
        return cv_composeRT2(L);
      }
    } else {
      // use any to raise errors
      return cv_composeRT2(L);
    }
  } else {
    // use any to raise errors
    return cv_composeRT2(L);
  }
}



/** CV_EXPORTS double cv::contourArea(const Mat &contour)
 * include/opencv/cv.hpp:561
 */
static int cv_contourArea(lua_State *L) {
  try {
    const Mat *contour = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double  retval__ = contourArea(*contour);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.contourArea: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.contourArea: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.contourArea: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::convertMaps(const Mat &map1, const Mat &map2, Mat &dstmap1, Mat &dstmap2, int dstmap1type, bool nninterpolation=false)
 * include/opencv/cv.hpp:361
 */
static int cv_convertMaps(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *map1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *map2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *dstmap1 = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *dstmap2 = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    int dstmap1type = luaL_checkint(L, 5);
    if (top__ < 6) {
      convertMaps(*map1, *map2, *dstmap1, *dstmap2, dstmap1type);
    } else {
      bool nninterpolation = lua_toboolean(L, 6);
      convertMaps(*map1, *map2, *dstmap1, *dstmap2, dstmap1type, nninterpolation);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.convertMaps: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.convertMaps: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.convertMaps: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::convertScaleAbs(const Mat &a, Mat &c, double alpha=1, double beta=0)
 * include/opencv/cxcore.hpp:1106
 */
static int cv_convertScaleAbs(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      convertScaleAbs(*a, *c);
    } else {
      double alpha = luaL_checknumber(L, 3);
      if (top__ < 4) {
        convertScaleAbs(*a, *c, alpha);
      } else {
        double beta = luaL_checknumber(L, 4);
        convertScaleAbs(*a, *c, alpha, beta);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.convertScaleAbs: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.convertScaleAbs: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.convertScaleAbs: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::copyMakeBorder(const Mat &src, Mat &dst, int top, int bottom, int left, int right, int borderType, const Scalar &value=Scalar())
 * include/opencv/cv.hpp:227
 */
static int cv_copyMakeBorder(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int top = luaL_checkint(L, 3);
    int bottom = luaL_checkint(L, 4);
    int left = luaL_checkint(L, 5);
    int right = luaL_checkint(L, 6);
    int borderType = luaL_checkint(L, 7);
    if (top__ < 8) {
      copyMakeBorder(*src, *dst, top, bottom, left, right, borderType);
    } else {
      const Scalar *value = *((const Scalar **)luaL_checkudata(L, 8, "cv.Scalar"));
      copyMakeBorder(*src, *dst, top, bottom, left, right, borderType, *value);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.copyMakeBorder: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.copyMakeBorder: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.copyMakeBorder: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::cornerEigenValsAndVecs(const Mat &src, Mat &dst, int blockSize, int ksize, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:283
 */
static int cv_cornerEigenValsAndVecs(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int blockSize = luaL_checkint(L, 3);
    int ksize = luaL_checkint(L, 4);
    if (top__ < 5) {
      cornerEigenValsAndVecs(*src, *dst, blockSize, ksize);
    } else {
      int borderType = luaL_checkint(L, 5);
      cornerEigenValsAndVecs(*src, *dst, blockSize, ksize, borderType);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cornerEigenValsAndVecs: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cornerEigenValsAndVecs: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cornerEigenValsAndVecs: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::cornerHarris(const Mat &src, Mat &dst, int blockSize, int ksize, double k, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:279
 */
static int cv_cornerHarris(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int blockSize = luaL_checkint(L, 3);
    int ksize = luaL_checkint(L, 4);
    double k = luaL_checknumber(L, 5);
    if (top__ < 6) {
      cornerHarris(*src, *dst, blockSize, ksize, k);
    } else {
      int borderType = luaL_checkint(L, 6);
      cornerHarris(*src, *dst, blockSize, ksize, k, borderType);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cornerHarris: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cornerHarris: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cornerHarris: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::cornerMinEigenVal(const Mat &src, Mat &dst, int blockSize, int ksize=3, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:275
 */
static int cv_cornerMinEigenVal(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int blockSize = luaL_checkint(L, 3);
    if (top__ < 4) {
      cornerMinEigenVal(*src, *dst, blockSize);
    } else {
      int ksize = luaL_checkint(L, 4);
      if (top__ < 5) {
        cornerMinEigenVal(*src, *dst, blockSize, ksize);
      } else {
        int borderType = luaL_checkint(L, 5);
        cornerMinEigenVal(*src, *dst, blockSize, ksize, borderType);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cornerMinEigenVal: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cornerMinEigenVal: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cornerMinEigenVal: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS int cv::countNonZero(const Mat &m)
 * include/opencv/cxcore.hpp:1110
 */
static int cv_countNonZero1(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    int  retval__ = countNonZero(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.countNonZero: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.countNonZero: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.countNonZero: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS int cv::countNonZero(const MatND &m)
 * include/opencv/cxcore.hpp:1764
 */
static int cv_countNonZero2(lua_State *L) {
  try {
    const MatND *m = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    int  retval__ = countNonZero(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.countNonZero: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.countNonZero: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.countNonZero: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for countNonZero(...) */
static int cv_countNonZero(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_countNonZero2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_countNonZero1(L);
  } else {
    // use any to raise errors
    return cv_countNonZero1(L);
  }
}



/** CV_EXPORTS float cv::cubeRoot(float val)
 * include/opencv/cxcore.hpp:1174
 */
static int cv_cubeRoot(lua_State *L) {
  try {
    float val = luaL_checknumber(L, 1);
    float  retval__ = cubeRoot(val);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cubeRoot: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cubeRoot: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cubeRoot: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::cvtColor(const Mat &src, Mat &dst, int code, int dstCn=0)
 * include/opencv/cv.hpp:505
 */
static int cv_cvtColor(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int code = luaL_checkint(L, 3);
    if (top__ < 4) {
      cvtColor(*src, *dst, code);
    } else {
      int dstCn = luaL_checkint(L, 4);
      cvtColor(*src, *dst, code, dstCn);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.cvtColor: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.cvtColor: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.cvtColor: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::dct(const Mat &src, Mat &dst, int flags=0)
 * include/opencv/cxcore.hpp:1253
 */
static int cv_dct(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      dct(*src, *dst);
    } else {
      int flags = luaL_checkint(L, 3);
      dct(*src, *dst, flags);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.dct: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.dct: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.dct: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::decomposeProjectionMatrix(const Mat &projMatrix, Mat &cameraMatrix, Mat &rotMatrix, Mat &transVect)
 * include/opencv/cv.hpp:765
 */
static int cv_decomposeProjectionMatrix1(lua_State *L) {
  try {
    const Mat *projMatrix = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *cameraMatrix = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *rotMatrix = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *transVect = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    decomposeProjectionMatrix(*projMatrix, *cameraMatrix, *rotMatrix, *transVect);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.decomposeProjectionMatrix: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.decomposeProjectionMatrix: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.decomposeProjectionMatrix: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::decomposeProjectionMatrix(const Mat &projMatrix, Mat &cameraMatrix, Mat &rotMatrix, Mat &transVect, Mat &rotMatrixX, Mat &rotMatrixY, Mat &rotMatrixZ, Vec3d &eulerAngles)
 * include/opencv/cv.hpp:769
 */
static int cv_decomposeProjectionMatrix2(lua_State *L) {
  try {
    const Mat *projMatrix = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *cameraMatrix = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *rotMatrix = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *transVect = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *rotMatrixX = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *rotMatrixY = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    Mat *rotMatrixZ = *((Mat **)luaL_checkudata(L, 7, "cv.Mat"));
    Vec3d *eulerAngles = *((Vec3d **)luaL_checkudata(L, 8, "cv.Vec3d"));
    decomposeProjectionMatrix(*projMatrix, *cameraMatrix, *rotMatrix, *transVect, *rotMatrixX, *rotMatrixY, *rotMatrixZ, *eulerAngles);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.decomposeProjectionMatrix: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.decomposeProjectionMatrix: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.decomposeProjectionMatrix: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for decomposeProjectionMatrix(...) */
static int cv_decomposeProjectionMatrix(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          type__ = lua_type(L, 5);
          if (top__ < 5) {
            return cv_decomposeProjectionMatrix1(L);
          } else if (type__ == LUA_TUSERDATA && is_userdata(L, 5, "cv.Mat")) {
            return cv_decomposeProjectionMatrix2(L);
          } else {
            // use any to raise errors
            return cv_decomposeProjectionMatrix2(L);
          }
        } else {
          // use any to raise errors
          return cv_decomposeProjectionMatrix2(L);
        }
      } else {
        // use any to raise errors
        return cv_decomposeProjectionMatrix2(L);
      }
    } else {
      // use any to raise errors
      return cv_decomposeProjectionMatrix2(L);
    }
  } else {
    // use any to raise errors
    return cv_decomposeProjectionMatrix2(L);
  }
}



/** CV_EXPORTS double cv::determinant(const Mat &m)
 * include/opencv/cxcore.hpp:1198
 */
static int cv_determinant(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double  retval__ = determinant(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.determinant: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.determinant: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.determinant: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::dft(const Mat &src, Mat &dst, int flags=0, int nonzeroRows=0)
 * include/opencv/cxcore.hpp:1251
 */
static int cv_dft(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      dft(*src, *dst);
    } else {
      int flags = luaL_checkint(L, 3);
      if (top__ < 4) {
        dft(*src, *dst, flags);
      } else {
        int nonzeroRows = luaL_checkint(L, 4);
        dft(*src, *dst, flags, nonzeroRows);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.dft: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.dft: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.dft: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::dilate(const Mat &src, Mat &dst, const Mat &kernel, Point anchor=Point(-1,-1), int iterations=1, int borderType=BORDER_CONSTANT, const Scalar &borderValue=morphologyDefaultBorderValue())
 * include/opencv/cv.hpp:317
 */
static int cv_dilate(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *kernel = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      dilate(*src, *dst, *kernel);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 4, "cv.Point"));
      if (top__ < 5) {
        dilate(*src, *dst, *kernel, *anchor);
      } else {
        int iterations = luaL_checkint(L, 5);
        if (top__ < 6) {
          dilate(*src, *dst, *kernel, *anchor, iterations);
        } else {
          int borderType = luaL_checkint(L, 6);
          if (top__ < 7) {
            dilate(*src, *dst, *kernel, *anchor, iterations, borderType);
          } else {
            const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
            dilate(*src, *dst, *kernel, *anchor, iterations, borderType, *borderValue);
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.dilate: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.dilate: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.dilate: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::distanceTransform(const Mat &src, Mat &dst, Mat &labels, int distanceType, int maskSize)
 * include/opencv/cv.hpp:480
 */
static int cv_distanceTransform1(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *labels = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int distanceType = luaL_checkint(L, 4);
    int maskSize = luaL_checkint(L, 5);
    distanceTransform(*src, *dst, *labels, distanceType, maskSize);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.distanceTransform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.distanceTransform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.distanceTransform: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::distanceTransform(const Mat &src, Mat &dst, int distanceType, int maskSize)
 * include/opencv/cv.hpp:483
 */
static int cv_distanceTransform2(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int distanceType = luaL_checkint(L, 3);
    int maskSize = luaL_checkint(L, 4);
    distanceTransform(*src, *dst, distanceType, maskSize);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.distanceTransform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.distanceTransform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.distanceTransform: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for distanceTransform(...) */
static int cv_distanceTransform(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        return cv_distanceTransform2(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        return cv_distanceTransform1(L);
      } else {
        // use any to raise errors
        return cv_distanceTransform1(L);
      }
    } else {
      // use any to raise errors
      return cv_distanceTransform1(L);
    }
  } else {
    // use any to raise errors
    return cv_distanceTransform1(L);
  }
}




/** CV_EXPORTS void cv::divide(const Mat &a, const Mat &b, Mat &c, double scale=1)
 * include/opencv/cxcore.hpp:1099
 */
static int cv_divide1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      divide(*a, *b, *c);
    } else {
      double scale = luaL_checknumber(L, 4);
      divide(*a, *b, *c, scale);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.divide: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.divide: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.divide: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::divide(double scale, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1100
 */
static int cv_divide2(lua_State *L) {
  try {
    double scale = luaL_checknumber(L, 1);
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    divide(scale, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.divide: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.divide: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.divide: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::divide(const MatND &a, const MatND &b, MatND &c, double scale=1)
 * include/opencv/cxcore.hpp:1755
 */
static int cv_divide3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      divide(*a, *b, *c);
    } else {
      double scale = luaL_checknumber(L, 4);
      divide(*a, *b, *c, scale);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.divide: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.divide: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.divide: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::divide(double scale, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1756
 */
static int cv_divide4(lua_State *L) {
  try {
    double scale = luaL_checknumber(L, 1);
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    divide(scale, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.divide: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.divide: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.divide: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for divide(...) */
static int cv_divide(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_divide4(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_divide2(L);
    } else {
      // use any to raise errors
      return cv_divide2(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_divide3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_divide1(L);
  } else {
    // use any to raise errors
    return cv_divide1(L);
  }
}



/** CV_EXPORTS void cv::drawChessboardCorners(Mat &image, Size patternSize, const Mat &corners, bool patternWasFound)
 * include/opencv/cv.hpp:822
 */
static int cv_drawChessboardCorners(lua_State *L) {
  try {
    Mat *image = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Size *patternSize = *((Size **)luaL_checkudata(L, 2, "cv.Size"));
    const Mat *corners = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    bool patternWasFound = lua_toboolean(L, 4);
    drawChessboardCorners(*image, *patternSize, *corners, patternWasFound);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.drawChessboardCorners: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.drawChessboardCorners: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.drawChessboardCorners: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS bool cv::eigen(const Mat &a, Mat &eigenvalues, int lowindex=-1, int highindex=-1)
 * include/opencv/cxcore.hpp:1207
 */
static int cv_eigen1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    bool  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *eigenvalues = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      retval__ = eigen(*a, *eigenvalues);
    } else {
      int lowindex = luaL_checkint(L, 3);
      if (top__ < 4) {
        retval__ = eigen(*a, *eigenvalues, lowindex);
      } else {
        int highindex = luaL_checkint(L, 4);
        retval__ = eigen(*a, *eigenvalues, lowindex, highindex);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.eigen: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.eigen: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.eigen: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS bool cv::eigen(const Mat &a, Mat &eigenvalues, Mat &eigenvectors, int lowindex=-1, int highindex=-1)
 * include/opencv/cxcore.hpp:1209
 */
static int cv_eigen2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    bool  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *eigenvalues = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *eigenvectors = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      retval__ = eigen(*a, *eigenvalues, *eigenvectors);
    } else {
      int lowindex = luaL_checkint(L, 4);
      if (top__ < 5) {
        retval__ = eigen(*a, *eigenvalues, *eigenvectors, lowindex);
      } else {
        int highindex = luaL_checkint(L, 5);
        retval__ = eigen(*a, *eigenvalues, *eigenvectors, lowindex, highindex);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.eigen: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.eigen: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.eigen: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for eigen(...) */
static int cv_eigen(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        return cv_eigen1(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        return cv_eigen2(L);
      } else {
        // use any to raise errors
        return cv_eigen2(L);
      }
    } else {
      // use any to raise errors
      return cv_eigen2(L);
    }
  } else {
    // use any to raise errors
    return cv_eigen2(L);
  }
}




/** CV_EXPORTS void cv::ellipse(Mat &img, Point center, Size axes, double angle, double startAngle, double endAngle, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
 * include/opencv/cxcore.hpp:1292
 */
static int cv_ellipse1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point *center = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    Size *axes = *((Size **)luaL_checkudata(L, 3, "cv.Size"));
    double angle = luaL_checknumber(L, 4);
    double startAngle = luaL_checknumber(L, 5);
    double endAngle = luaL_checknumber(L, 6);
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
    if (top__ < 8) {
      ellipse(*img, *center, *axes, angle, startAngle, endAngle, *color);
    } else {
      int thickness = luaL_checkint(L, 8);
      if (top__ < 9) {
        ellipse(*img, *center, *axes, angle, startAngle, endAngle, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 9);
        if (top__ < 10) {
          ellipse(*img, *center, *axes, angle, startAngle, endAngle, *color, thickness, lineType);
        } else {
          int shift = luaL_checkint(L, 10);
          ellipse(*img, *center, *axes, angle, startAngle, endAngle, *color, thickness, lineType, shift);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.ellipse: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.ellipse: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.ellipse: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::ellipse(Mat &img, const RotatedRect &box, const Scalar &color, int thickness=1, int lineType=8)
 * include/opencv/cxcore.hpp:1295
 */
static int cv_ellipse2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const RotatedRect *box = *((const RotatedRect **)luaL_checkudata(L, 2, "cv.RotatedRect"));
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    if (top__ < 4) {
      ellipse(*img, *box, *color);
    } else {
      int thickness = luaL_checkint(L, 4);
      if (top__ < 5) {
        ellipse(*img, *box, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 5);
        ellipse(*img, *box, *color, thickness, lineType);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.ellipse: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.ellipse: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.ellipse: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for ellipse(...) */
static int cv_ellipse(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Point")) {
      return cv_ellipse1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.RotatedRect")) {
      return cv_ellipse2(L);
    } else {
      // use any to raise errors
      return cv_ellipse2(L);
    }
  } else {
    // use any to raise errors
    return cv_ellipse2(L);
  }
}



/** CV_EXPORTS void cv::equalizeHist(const Mat &src, Mat &dst)
 * include/opencv/cv.hpp:455
 */
static int cv_equalizeHist(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    equalizeHist(*src, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.equalizeHist: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.equalizeHist: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.equalizeHist: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::erode(const Mat &src, Mat &dst, const Mat &kernel, Point anchor=Point(-1,-1), int iterations=1, int borderType=BORDER_CONSTANT, const Scalar &borderValue=morphologyDefaultBorderValue())
 * include/opencv/cv.hpp:313
 */
static int cv_erode(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *kernel = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      erode(*src, *dst, *kernel);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 4, "cv.Point"));
      if (top__ < 5) {
        erode(*src, *dst, *kernel, *anchor);
      } else {
        int iterations = luaL_checkint(L, 5);
        if (top__ < 6) {
          erode(*src, *dst, *kernel, *anchor, iterations);
        } else {
          int borderType = luaL_checkint(L, 6);
          if (top__ < 7) {
            erode(*src, *dst, *kernel, *anchor, iterations, borderType);
          } else {
            const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
            erode(*src, *dst, *kernel, *anchor, iterations, borderType, *borderValue);
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.erode: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.erode: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.erode: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::error(const Exception &exc)
 * include/opencv/cxcore.hpp:111
 */
static int cv_error(lua_State *L) {
  try {
    const Exception *exc = *((const Exception **)luaL_checkudata(L, 1, "cv.Exception"));
    error(*exc);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.error: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.error: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.error: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Mat cv::estimateRigidTransform(const Mat &A, const Mat &B, bool fullAffine)
 * include/opencv/cv.hpp:586
 */
static int cv_estimateRigidTransform(lua_State *L) {
  try {
    const Mat *A = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *B = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    bool fullAffine = lua_toboolean(L, 3);
    Mat  retval__ = estimateRigidTransform(*A, *B, fullAffine);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.estimateRigidTransform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.estimateRigidTransform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.estimateRigidTransform: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::exp(const Mat &a, Mat &b)
 * include/opencv/cxcore.hpp:1172
 */
static int cv_exp1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    exp(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.exp: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.exp: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.exp: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::exp(const MatND &a, MatND &b)
 * include/opencv/cxcore.hpp:1806
 */
static int cv_exp2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    MatND *b = *((MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    exp(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.exp: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.exp: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.exp: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for exp(...) */
static int cv_exp(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_exp2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_exp1(L);
  } else {
    // use any to raise errors
    return cv_exp1(L);
  }
}



/** CV_EXPORTS float cv::fastAtan2(float y, float x)
 * include/opencv/cxcore.hpp:1175
 */
static int cv_fastAtan2(lua_State *L) {
  try {
    float y = luaL_checknumber(L, 1);
    float x = luaL_checknumber(L, 2);
    float  retval__ = fastAtan2(y, x);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.fastAtan2: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.fastAtan2: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.fastAtan2: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::filter2D(const Mat &src, Mat &dst, int ddepth, const Mat &kernel, Point anchor=Point(-1,-1), double delta=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:249
 */
static int cv_filter2D(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    const Mat *kernel = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      filter2D(*src, *dst, ddepth, *kernel);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 5, "cv.Point"));
      if (top__ < 6) {
        filter2D(*src, *dst, ddepth, *kernel, *anchor);
      } else {
        double delta = luaL_checknumber(L, 6);
        if (top__ < 7) {
          filter2D(*src, *dst, ddepth, *kernel, *anchor, delta);
        } else {
          int borderType = luaL_checkint(L, 7);
          filter2D(*src, *dst, ddepth, *kernel, *anchor, delta, borderType);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.filter2D: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.filter2D: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.filter2D: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::filterSpeckles(Mat &img, double newVal, int maxSpeckleSize, double maxDiff, Mat &buf)
 * include/opencv/cv.hpp:963
 */
static int cv_filterSpeckles(lua_State *L) {
  try {
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double newVal = luaL_checknumber(L, 2);
    int maxSpeckleSize = luaL_checkint(L, 3);
    double maxDiff = luaL_checknumber(L, 4);
    Mat *buf = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    filterSpeckles(*img, newVal, maxSpeckleSize, maxDiff, *buf);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.filterSpeckles: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.filterSpeckles: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.filterSpeckles: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS Mat cv::findFundamentalMat(const Mat &points1, const Mat &points2, int method=FM_RANSAC, double param1=3., double param2=0.99)
 * include/opencv/cv.hpp:906
 */
static int cv_findFundamentalMat2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat  retval__;
    const Mat *points1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *points2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      retval__ = findFundamentalMat(*points1, *points2);
    } else {
      int method = luaL_checkint(L, 3);
      if (top__ < 4) {
        retval__ = findFundamentalMat(*points1, *points2, method);
      } else {
        double param1 = luaL_checknumber(L, 4);
        if (top__ < 5) {
          retval__ = findFundamentalMat(*points1, *points2, method, param1);
        } else {
          double param2 = luaL_checknumber(L, 5);
          retval__ = findFundamentalMat(*points1, *points2, method, param1, param2);
        }
      }
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.findFundamentalMat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.findFundamentalMat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.findFundamentalMat: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for findFundamentalMat(...) */
static int cv_findFundamentalMat(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_findFundamentalMat2(L);
  } else {
    // use any to raise errors
    return cv_findFundamentalMat2(L);
  }
}



/** CV_EXPORTS RotatedRect cv::fitEllipse(const Mat &points)
 * include/opencv/cv.hpp:575
 */
static int cv_fitEllipse(lua_State *L) {
  try {
    const Mat *points = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    RotatedRect  retval__ = fitEllipse(*points);
    lua_pushclass<RotatedRect>(L, retval__, "cv.RotatedRect");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.fitEllipse: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.fitEllipse: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.fitEllipse: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::fitLine(const Mat &points, Vec4f &line, int distType, double param, double reps, double aeps)
 * include/opencv/cv.hpp:578
 */
static int cv_fitLine(lua_State *L) {
  try {
    const Mat *points = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Vec4f *line = *((Vec4f **)luaL_checkudata(L, 2, "cv.Vec4f"));
    int distType = luaL_checkint(L, 3);
    double param = luaL_checknumber(L, 4);
    double reps = luaL_checknumber(L, 5);
    double aeps = luaL_checknumber(L, 6);
    fitLine(*points, *line, distType, param, reps, aeps);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.fitLine: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.fitLine: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.fitLine: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::flip(const Mat &a, Mat &b, int flipCode)
 * include/opencv/cxcore.hpp:1141
 */
static int cv_flip(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int flipCode = luaL_checkint(L, 3);
    flip(*a, *b, flipCode);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.flip: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.flip: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.flip: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS string cv::fromUtf16(const WString &str)
 * include/opencv/cxcore.hpp:77
 */
static int cv_fromUtf16(lua_State *L) {
  try {
    const WString *str = *((const WString **)luaL_checkudata(L, 1, "cv.WString"));
    string  retval__ = fromUtf16(*str);
    lua_pushclass<string>(L, retval__, "cv.string");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.fromUtf16: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.fromUtf16: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.fromUtf16: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::gemm(const Mat &a, const Mat &b, double alpha, const Mat &c, double gamma, Mat &d, int flags=0)
 * include/opencv/cxcore.hpp:1188
 */
static int cv_gemm(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double alpha = luaL_checknumber(L, 3);
    const Mat *c = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    double gamma = luaL_checknumber(L, 5);
    Mat *d = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    if (top__ < 7) {
      gemm(*a, *b, alpha, *c, gamma, *d);
    } else {
      int flags = luaL_checkint(L, 7);
      gemm(*a, *b, alpha, *c, gamma, *d, flags);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.gemm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.gemm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.gemm: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int64 cv::getCPUTickCount()
 * include/opencv/cxcore.hpp:138
 */
static int cv_getCPUTickCount(lua_State *L) {
  try {
    int64  retval__ = getCPUTickCount();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getCPUTickCount: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getCPUTickCount: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getCPUTickCount: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Mat cv::getDefaultNewCameraMatrix(const Mat &cameraMatrix, Size imgsize=Size(), bool centerPrincipalPoint=false)
 * include/opencv/cv.hpp:407
 */
static int cv_getDefaultNewCameraMatrix(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat  retval__;
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      retval__ = getDefaultNewCameraMatrix(*cameraMatrix);
    } else {
      Size *imgsize = *((Size **)luaL_checkudata(L, 2, "cv.Size"));
      if (top__ < 3) {
        retval__ = getDefaultNewCameraMatrix(*cameraMatrix, *imgsize);
      } else {
        bool centerPrincipalPoint = lua_toboolean(L, 3);
        retval__ = getDefaultNewCameraMatrix(*cameraMatrix, *imgsize, centerPrincipalPoint);
      }
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getDefaultNewCameraMatrix: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getDefaultNewCameraMatrix: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getDefaultNewCameraMatrix: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::getDerivKernels(Mat &kx, Mat &ky, int dx, int dy, int ksize, bool normalize=false, int ktype=CV_32F)
 * include/opencv/cv.hpp:187
 */
static int cv_getDerivKernels(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *kx = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *ky = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int dx = luaL_checkint(L, 3);
    int dy = luaL_checkint(L, 4);
    int ksize = luaL_checkint(L, 5);
    if (top__ < 6) {
      getDerivKernels(*kx, *ky, dx, dy, ksize);
    } else {
      bool normalize = lua_toboolean(L, 6);
      if (top__ < 7) {
        getDerivKernels(*kx, *ky, dx, dy, ksize, normalize);
      } else {
        int ktype = luaL_checkint(L, 7);
        getDerivKernels(*kx, *ky, dx, dy, ksize, normalize, ktype);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getDerivKernels: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getDerivKernels: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getDerivKernels: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static size_t cv::getElemSize(int type)
 * include/opencv/cxcore.hpp:732
 */
static int cv_getElemSize(lua_State *L) {
  try {
    int type = luaL_checkint(L, 1);
    size_t  retval__ = getElemSize(type);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getElemSize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getElemSize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getElemSize: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Mat cv::getGaussianKernel(int ksize, double sigma, int ktype=CV_64F)
 * include/opencv/cv.hpp:180
 */
static int cv_getGaussianKernel(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat  retval__;
    int ksize = luaL_checkint(L, 1);
    double sigma = luaL_checknumber(L, 2);
    if (top__ < 3) {
      retval__ = getGaussianKernel(ksize, sigma);
    } else {
      int ktype = luaL_checkint(L, 3);
      retval__ = getGaussianKernel(ksize, sigma, ktype);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getGaussianKernel: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getGaussianKernel: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getGaussianKernel: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::getKernelType(const Mat &kernel, Point anchor)
 * include/opencv/cv.hpp:152
 */
static int cv_getKernelType(lua_State *L) {
  try {
    const Mat *kernel = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point *anchor = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    int  retval__ = getKernelType(*kernel, *anchor);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getKernelType: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getKernelType: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getKernelType: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::getNumThreads()
 * include/opencv/cxcore.hpp:133
 */
static int cv_getNumThreads(lua_State *L) {
  try {
    int  retval__ = getNumThreads();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getNumThreads: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getNumThreads: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getNumThreads: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::getOptimalDFTSize(int vecsize)
 * include/opencv/cxcore.hpp:1257
 */
static int cv_getOptimalDFTSize(lua_State *L) {
  try {
    int vecsize = luaL_checkint(L, 1);
    int  retval__ = getOptimalDFTSize(vecsize);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getOptimalDFTSize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getOptimalDFTSize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getOptimalDFTSize: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::getRectSubPix(const Mat &image, Size patchSize, Point2f center, Mat &patch, int patchType=-1)
 * include/opencv/cv.hpp:369
 */
static int cv_getRectSubPix(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *image = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Size *patchSize = *((Size **)luaL_checkudata(L, 2, "cv.Size"));
    Point2f *center = *((Point2f **)luaL_checkudata(L, 3, "cv.Point2f"));
    Mat *patch = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      getRectSubPix(*image, *patchSize, *center, *patch);
    } else {
      int patchType = luaL_checkint(L, 5);
      getRectSubPix(*image, *patchSize, *center, *patch, patchType);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getRectSubPix: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getRectSubPix: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getRectSubPix: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Mat cv::getRotationMatrix2D(Point2f center, double angle, double scale)
 * include/opencv/cv.hpp:363
 */
static int cv_getRotationMatrix2D(lua_State *L) {
  try {
    Point2f *center = *((Point2f **)luaL_checkudata(L, 1, "cv.Point2f"));
    double angle = luaL_checknumber(L, 2);
    double scale = luaL_checknumber(L, 3);
    Mat  retval__ = getRotationMatrix2D(*center, angle, scale);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getRotationMatrix2D: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getRotationMatrix2D: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getRotationMatrix2D: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Mat cv::getStructuringElement(int shape, Size ksize, Point anchor=Point(-1,-1))
 * include/opencv/cv.hpp:219
 */
static int cv_getStructuringElement(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat  retval__;
    int shape = luaL_checkint(L, 1);
    Size *ksize = *((Size **)luaL_checkudata(L, 2, "cv.Size"));
    if (top__ < 3) {
      retval__ = getStructuringElement(shape, *ksize);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
      retval__ = getStructuringElement(shape, *ksize, *anchor);
    }
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getStructuringElement: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getStructuringElement: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getStructuringElement: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Size cv::getTextSize(const string &text, int fontFace, double fontScale, int thickness, int *baseLine)
 * include/opencv/cxcore.hpp:1349
 */
static int cv_getTextSize(lua_State *L) {
  try {
    const string *text = *((const string **)luaL_checkudata(L, 1, "cv.string"));
    int fontFace = luaL_checkint(L, 2);
    double fontScale = luaL_checknumber(L, 3);
    int thickness = luaL_checkint(L, 4);

    DubArgPointer<int> ptr_baseLine;
    int *baseLine = ptr_baseLine(L, 5);
    Size  retval__ = getTextSize(*text, fontFace, fontScale, thickness, baseLine);
    lua_pushclass<Size>(L, retval__, "cv.Size");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getTextSize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getTextSize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getTextSize: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::getThreadNum()
 * include/opencv/cxcore.hpp:134
 */
static int cv_getThreadNum(lua_State *L) {
  try {
    int  retval__ = getThreadNum();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getThreadNum: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getThreadNum: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getThreadNum: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int64 cv::getTickCount()
 * include/opencv/cxcore.hpp:136
 */
static int cv_getTickCount(lua_State *L) {
  try {
    int64  retval__ = getTickCount();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getTickCount: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getTickCount: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getTickCount: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::getTickFrequency()
 * include/opencv/cxcore.hpp:137
 */
static int cv_getTickFrequency(lua_State *L) {
  try {
    double  retval__ = getTickFrequency();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.getTickFrequency: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.getTickFrequency: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.getTickFrequency: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::grabCut(const Mat &img, Mat &mask, Rect rect, Mat &bgdModel, Mat &fgdModel, int iterCount, int mode=GC_EVAL)
 * include/opencv/cv.hpp:472
 */
static int cv_grabCut(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *img = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *mask = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Rect *rect = *((Rect **)luaL_checkudata(L, 3, "cv.Rect"));
    Mat *bgdModel = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *fgdModel = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    int iterCount = luaL_checkint(L, 6);
    if (top__ < 7) {
      grabCut(*img, *mask, *rect, *bgdModel, *fgdModel, iterCount);
    } else {
      int mode = luaL_checkint(L, 7);
      grabCut(*img, *mask, *rect, *bgdModel, *fgdModel, iterCount, mode);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.grabCut: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.grabCut: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.grabCut: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::idct(const Mat &src, Mat &dst, int flags=0)
 * include/opencv/cxcore.hpp:1254
 */
static int cv_idct(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      idct(*src, *dst);
    } else {
      int flags = luaL_checkint(L, 3);
      idct(*src, *dst, flags);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.idct: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.idct: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.idct: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::idft(const Mat &src, Mat &dst, int flags=0, int nonzeroRows=0)
 * include/opencv/cxcore.hpp:1252
 */
static int cv_idft(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      idft(*src, *dst);
    } else {
      int flags = luaL_checkint(L, 3);
      if (top__ < 4) {
        idft(*src, *dst, flags);
      } else {
        int nonzeroRows = luaL_checkint(L, 4);
        idft(*src, *dst, flags, nonzeroRows);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.idft: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.idft: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.idft: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::inRange(const Mat &src, const Mat &lowerb, const Mat &upperb, Mat &dst)
 * include/opencv/cxcore.hpp:1160
 */
static int cv_inRange1(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *lowerb = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *upperb = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    inRange(*src, *lowerb, *upperb, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.inRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.inRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.inRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::inRange(const Mat &src, const Scalar &lowerb, const Scalar &upperb, Mat &dst)
 * include/opencv/cxcore.hpp:1162
 */
static int cv_inRange2(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *lowerb = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    const Scalar *upperb = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    inRange(*src, *lowerb, *upperb, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.inRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.inRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.inRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::inRange(const MatND &src, const MatND &lowerb, const MatND &upperb, MatND &dst)
 * include/opencv/cxcore.hpp:1794
 */
static int cv_inRange3(lua_State *L) {
  try {
    const MatND *src = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *lowerb = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    const MatND *upperb = *((const MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    MatND *dst = *((MatND **)luaL_checkudata(L, 4, "cv.MatND"));
    inRange(*src, *lowerb, *upperb, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.inRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.inRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.inRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::inRange(const MatND &src, const Scalar &lowerb, const Scalar &upperb, MatND &dst)
 * include/opencv/cxcore.hpp:1796
 */
static int cv_inRange4(lua_State *L) {
  try {
    const MatND *src = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const Scalar *lowerb = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    const Scalar *upperb = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    MatND *dst = *((MatND **)luaL_checkudata(L, 4, "cv.MatND"));
    inRange(*src, *lowerb, *upperb, *dst);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.inRange: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.inRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.inRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for inRange(...) */
static int cv_inRange(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_inRange3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_inRange4(L);
    } else {
      // use any to raise errors
      return cv_inRange4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_inRange1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_inRange2(L);
    } else {
      // use any to raise errors
      return cv_inRange2(L);
    }
  } else {
    // use any to raise errors
    return cv_inRange2(L);
  }
}



/** CV_EXPORTS void cv::initUndistortRectifyMap(const Mat &cameraMatrix, const Mat &distCoeffs, const Mat &R, const Mat &newCameraMatrix, Size size, int m1type, Mat &map1, Mat &map2)
 * include/opencv/cv.hpp:402
 */
static int cv_initUndistortRectifyMap(lua_State *L) {
  try {
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *distCoeffs = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *R = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *newCameraMatrix = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Size *size = *((Size **)luaL_checkudata(L, 5, "cv.Size"));
    int m1type = luaL_checkint(L, 6);
    Mat *map1 = *((Mat **)luaL_checkudata(L, 7, "cv.Mat"));
    Mat *map2 = *((Mat **)luaL_checkudata(L, 8, "cv.Mat"));
    initUndistortRectifyMap(*cameraMatrix, *distCoeffs, *R, *newCameraMatrix, *size, m1type, *map1, *map2);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.initUndistortRectifyMap: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.initUndistortRectifyMap: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.initUndistortRectifyMap: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::inpaint(const Mat &src, const Mat &inpaintMask, Mat &dst, double inpaintRange, int flags)
 * include/opencv/cv.hpp:477
 */
static int cv_inpaint(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *inpaintMask = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double inpaintRange = luaL_checknumber(L, 4);
    int flags = luaL_checkint(L, 5);
    inpaint(*src, *inpaintMask, *dst, inpaintRange, flags);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.inpaint: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.inpaint: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.inpaint: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::integral(const Mat &src, Mat &sum, int sdepth=-1)
 * include/opencv/cv.hpp:371
 */
static int cv_integral1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *sum = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      integral(*src, *sum);
    } else {
      int sdepth = luaL_checkint(L, 3);
      integral(*src, *sum, sdepth);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.integral: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.integral: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.integral: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::integral(const Mat &src, Mat &sum, Mat &sqsum, int sdepth=-1)
 * include/opencv/cv.hpp:372
 */
static int cv_integral2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *sum = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *sqsum = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      integral(*src, *sum, *sqsum);
    } else {
      int sdepth = luaL_checkint(L, 4);
      integral(*src, *sum, *sqsum, sdepth);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.integral: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.integral: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.integral: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::integral(const Mat &src, Mat &sum, Mat &sqsum, Mat &tilted, int sdepth=-1)
 * include/opencv/cv.hpp:373
 */
static int cv_integral3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *sum = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *sqsum = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *tilted = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      integral(*src, *sum, *sqsum, *tilted);
    } else {
      int sdepth = luaL_checkint(L, 5);
      integral(*src, *sum, *sqsum, *tilted, sdepth);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.integral: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.integral: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.integral: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for integral(...) */
static int cv_integral(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        return cv_integral1(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (type__ == LUA_TNUMBER) {
          return cv_integral2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_integral3(L);
        } else {
          // use any to raise errors
          return cv_integral3(L);
        }
      } else {
        // use any to raise errors
        return cv_integral3(L);
      }
    } else {
      // use any to raise errors
      return cv_integral3(L);
    }
  } else {
    // use any to raise errors
    return cv_integral3(L);
  }
}



/** CV_EXPORTS double cv::invert(const Mat &a, Mat &c, int flags=DECOMP_LU)
 * include/opencv/cxcore.hpp:1200
 */
static int cv_invert(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      retval__ = invert(*a, *c);
    } else {
      int flags = luaL_checkint(L, 3);
      retval__ = invert(*a, *c, flags);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.invert: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.invert: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.invert: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::invertAffineTransform(const Mat &M, Mat &iM)
 * include/opencv/cv.hpp:366
 */
static int cv_invertAffineTransform(lua_State *L) {
  try {
    const Mat *M = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *iM = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    invertAffineTransform(*M, *iM);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.invertAffineTransform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.invertAffineTransform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.invertAffineTransform: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS bool cv::isContourConvex(const Mat &contour)
 * include/opencv/cv.hpp:573
 */
static int cv_isContourConvex(lua_State *L) {
  try {
    const Mat *contour = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    bool  retval__ = isContourConvex(*contour);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.isContourConvex: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.isContourConvex: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.isContourConvex: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::line(Mat &img, Point pt1, Point pt2, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
 * include/opencv/cxcore.hpp:1275
 */
static int cv_line(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point *pt1 = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    Point *pt2 = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 4, "cv.Scalar"));
    if (top__ < 5) {
      line(*img, *pt1, *pt2, *color);
    } else {
      int thickness = luaL_checkint(L, 5);
      if (top__ < 6) {
        line(*img, *pt1, *pt2, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 6);
        if (top__ < 7) {
          line(*img, *pt1, *pt2, *color, thickness, lineType);
        } else {
          int shift = luaL_checkint(L, 7);
          line(*img, *pt1, *pt2, *color, thickness, lineType, shift);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.line: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.line: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.line: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::log(const Mat &a, Mat &b)
 * include/opencv/cxcore.hpp:1173
 */
static int cv_log1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    log(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.log: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.log: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.log: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::log(const MatND &a, MatND &b)
 * include/opencv/cxcore.hpp:1807
 */
static int cv_log2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    MatND *b = *((MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    log(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.log: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.log: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.log: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for log(...) */
static int cv_log(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_log2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_log1(L);
  } else {
    // use any to raise errors
    return cv_log1(L);
  }
}



/** CV_EXPORTS void cv::magnitude(const Mat &x, const Mat &y, Mat &magnitude)
 * include/opencv/cxcore.hpp:1183
 */
static int cv_magnitude(lua_State *L) {
  try {
    const Mat *x = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *y = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *arg_magnitude = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    magnitude(*x, *y, *arg_magnitude);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.magnitude: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.magnitude: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.magnitude: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::matMulDeriv(const Mat &A, const Mat &B, Mat &dABdA, Mat &dABdB)
 * include/opencv/cv.hpp:771
 */
static int cv_matMulDeriv(lua_State *L) {
  try {
    const Mat *A = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *B = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *dABdA = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *dABdB = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    matMulDeriv(*A, *B, *dABdA, *dABdB);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.matMulDeriv: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.matMulDeriv: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.matMulDeriv: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::matchShapes(const Mat &contour1, const Mat &contour2, int method, double parameter)
 * include/opencv/cv.hpp:567
 */
static int cv_matchShapes(lua_State *L) {
  try {
    const Mat *contour1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *contour2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int method = luaL_checkint(L, 3);
    double parameter = luaL_checknumber(L, 4);
    double  retval__ = matchShapes(*contour1, *contour2, method, parameter);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.matchShapes: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.matchShapes: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.matchShapes: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::matchTemplate(const Mat &image, const Mat &templ, Mat &result, int method)
 * include/opencv/cv.hpp:529
 */
static int cv_matchTemplate(lua_State *L) {
  try {
    const Mat *image = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *templ = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *result = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int method = luaL_checkint(L, 4);
    matchTemplate(*image, *templ, *result, method);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.matchTemplate: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.matchTemplate: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.matchTemplate: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::max(const Mat &a, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1167
 */
static int cv_max1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    max(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.max: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.max: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.max: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::max(const Mat &a, double alpha, Mat &c)
 * include/opencv/cxcore.hpp:1168
 */
static int cv_max2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double alpha = luaL_checknumber(L, 2);
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    max(*a, alpha, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.max: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.max: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.max: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::max(const MatND &a, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1801
 */
static int cv_max3(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    max(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.max: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.max: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.max: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::max(const MatND &a, double alpha, MatND &c)
 * include/opencv/cxcore.hpp:1802
 */
static int cv_max4(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double alpha = luaL_checknumber(L, 2);
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    max(*a, alpha, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.max: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.max: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.max: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for max(...) */
static int cv_max(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_max4(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_max3(L);
    } else {
      // use any to raise errors
      return cv_max3(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_max2(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_max1(L);
    } else {
      // use any to raise errors
      return cv_max1(L);
    }
  } else {
    // use any to raise errors
    return cv_max1(L);
  }
}




/** CV_EXPORTS Scalar cv::mean(const Mat &m)
 * include/opencv/cxcore.hpp:1112
 */
static int cv_mean1(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Scalar  retval__ = mean(*m);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mean: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mean: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mean: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS Scalar cv::mean(const Mat &m, const Mat &mask)
 * include/opencv/cxcore.hpp:1113
 */
static int cv_mean2(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Scalar  retval__ = mean(*m, *mask);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mean: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mean: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mean: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS Scalar cv::mean(const MatND &m)
 * include/opencv/cxcore.hpp:1766
 */
static int cv_mean3(lua_State *L) {
  try {
    const MatND *m = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    Scalar  retval__ = mean(*m);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mean: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mean: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mean: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS Scalar cv::mean(const MatND &m, const MatND &mask)
 * include/opencv/cxcore.hpp:1767
 */
static int cv_mean4(lua_State *L) {
  try {
    const MatND *m = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *mask = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    Scalar  retval__ = mean(*m, *mask);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mean: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mean: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mean: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for mean(...) */
static int cv_mean(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return cv_mean3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_mean4(L);
    } else {
      // use any to raise errors
      return cv_mean4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return cv_mean1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_mean2(L);
    } else {
      // use any to raise errors
      return cv_mean2(L);
    }
  } else {
    // use any to raise errors
    return cv_mean2(L);
  }
}



/** CV_EXPORTS int cv::meanShift(const Mat &probImage, Rect &window, TermCriteria criteria)
 * include/opencv/cv.hpp:605
 */
static int cv_meanShift(lua_State *L) {
  try {
    const Mat *probImage = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Rect *window = *((Rect **)luaL_checkudata(L, 2, "cv.Rect"));
    TermCriteria *criteria = *((TermCriteria **)luaL_checkudata(L, 3, "cv.TermCriteria"));
    int  retval__ = meanShift(*probImage, *window, *criteria);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.meanShift: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.meanShift: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.meanShift: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::meanStdDev(const Mat &m, Scalar &mean, Scalar &stddev, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1114
 */
static int cv_meanStdDev1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Scalar *mean = *((Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Scalar *stddev = *((Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    if (top__ < 4) {
      meanStdDev(*m, *mean, *stddev);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      meanStdDev(*m, *mean, *stddev, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.meanStdDev: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.meanStdDev: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.meanStdDev: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::meanStdDev(const MatND &m, Scalar &mean, Scalar &stddev, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1768
 */
static int cv_meanStdDev2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *m = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    Scalar *mean = *((Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Scalar *stddev = *((Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    if (top__ < 4) {
      meanStdDev(*m, *mean, *stddev);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      meanStdDev(*m, *mean, *stddev, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.meanStdDev: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.meanStdDev: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.meanStdDev: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for meanStdDev(...) */
static int cv_meanStdDev(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_meanStdDev2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_meanStdDev1(L);
  } else {
    // use any to raise errors
    return cv_meanStdDev1(L);
  }
}



/** CV_EXPORTS void cv::medianBlur(const Mat &src, Mat &dst, int ksize)
 * include/opencv/cv.hpp:229
 */
static int cv_medianBlur(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ksize = luaL_checkint(L, 3);
    medianBlur(*src, *dst, ksize);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.medianBlur: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.medianBlur: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.medianBlur: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::min(const Mat &a, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1165
 */
static int cv_min1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    min(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.min: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.min: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.min: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::min(const Mat &a, double alpha, Mat &c)
 * include/opencv/cxcore.hpp:1166
 */
static int cv_min2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double alpha = luaL_checknumber(L, 2);
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    min(*a, alpha, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.min: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.min: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.min: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::min(const MatND &a, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1799
 */
static int cv_min3(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    min(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.min: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.min: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.min: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::min(const MatND &a, double alpha, MatND &c)
 * include/opencv/cxcore.hpp:1800
 */
static int cv_min4(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double alpha = luaL_checknumber(L, 2);
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    min(*a, alpha, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.min: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.min: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.min: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for min(...) */
static int cv_min(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_min4(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_min3(L);
    } else {
      // use any to raise errors
      return cv_min3(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_min2(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_min1(L);
    } else {
      // use any to raise errors
      return cv_min1(L);
    }
  } else {
    // use any to raise errors
    return cv_min1(L);
  }
}



/** CV_EXPORTS RotatedRect cv::minAreaRect(const Mat &points)
 * include/opencv/cv.hpp:562
 */
static int cv_minAreaRect(lua_State *L) {
  try {
    const Mat *points = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    RotatedRect  retval__ = minAreaRect(*points);
    lua_pushclass<RotatedRect>(L, retval__, "cv.RotatedRect");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.minAreaRect: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.minAreaRect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.minAreaRect: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::minEnclosingCircle(const Mat &points, Point2f &center, float &radius)
 * include/opencv/cv.hpp:564
 */
static int cv_minEnclosingCircle(lua_State *L) {
  try {
    const Mat *points = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point2f *center = *((Point2f **)luaL_checkudata(L, 2, "cv.Point2f"));
    float radius = luaL_checknumber(L, 3);
    minEnclosingCircle(*points, *center, radius);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.minEnclosingCircle: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.minEnclosingCircle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.minEnclosingCircle: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::minMaxLoc(const MatND &a, double *minVal, double *maxVal, int *minIdx=0, int *maxIdx=0, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1777
 */
static int cv_minMaxLoc2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));

    DubArgPointer<double> ptr_minVal;
    double *minVal = ptr_minVal(L, 2);

    DubArgPointer<double> ptr_maxVal;
    double *maxVal = ptr_maxVal(L, 3);
    if (top__ < 4) {
      minMaxLoc(*a, minVal, maxVal);
    } else {

      DubArgPointer<int> ptr_minIdx;
      int *minIdx = ptr_minIdx(L, 4);
      if (top__ < 5) {
        minMaxLoc(*a, minVal, maxVal, minIdx);
      } else {

        DubArgPointer<int> ptr_maxIdx;
        int *maxIdx = ptr_maxIdx(L, 5);
        if (top__ < 6) {
          minMaxLoc(*a, minVal, maxVal, minIdx, maxIdx);
        } else {
          const MatND *mask = *((const MatND **)luaL_checkudata(L, 6, "cv.MatND"));
          minMaxLoc(*a, minVal, maxVal, minIdx, maxIdx, *mask);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.minMaxLoc: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.minMaxLoc: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.minMaxLoc: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::minMaxLoc(const SparseMat &a, double *minVal, double *maxVal, int *minIdx=0, int *maxIdx=0)
 * include/opencv/cxcore.hpp:2082
 */
static int cv_minMaxLoc3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const SparseMat *a = *((const SparseMat **)luaL_checkudata(L, 1, "cv.SparseMat"));

    DubArgPointer<double> ptr_minVal;
    double *minVal = ptr_minVal(L, 2);

    DubArgPointer<double> ptr_maxVal;
    double *maxVal = ptr_maxVal(L, 3);
    if (top__ < 4) {
      minMaxLoc(*a, minVal, maxVal);
    } else {

      DubArgPointer<int> ptr_minIdx;
      int *minIdx = ptr_minIdx(L, 4);
      if (top__ < 5) {
        minMaxLoc(*a, minVal, maxVal, minIdx);
      } else {

        DubArgPointer<int> ptr_maxIdx;
        int *maxIdx = ptr_maxIdx(L, 5);
        minMaxLoc(*a, minVal, maxVal, minIdx, maxIdx);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.minMaxLoc: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.minMaxLoc: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.minMaxLoc: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for minMaxLoc(...) */
static int cv_minMaxLoc(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.SparseMat")) {
    return cv_minMaxLoc3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_minMaxLoc2(L);
  } else {
    // use any to raise errors
    return cv_minMaxLoc2(L);
  }
}



/** CV_EXPORTS Moments cv::moments(const Mat &array, bool binaryImage=false)
 * include/opencv/cv.hpp:521
 */
static int cv_moments(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Moments  retval__;
    const Mat *array = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      retval__ = moments(*array);
    } else {
      bool binaryImage = lua_toboolean(L, 2);
      retval__ = moments(*array, binaryImage);
    }
    lua_pushclass<Moments>(L, retval__, "cv.Moments");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.moments: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.moments: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.moments: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static Scalar cv::morphologyDefaultBorderValue()
 * include/opencv/cv.hpp:211
 */
static int cv_morphologyDefaultBorderValue(lua_State *L) {
  try {
    Scalar  retval__ = morphologyDefaultBorderValue();
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.morphologyDefaultBorderValue: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.morphologyDefaultBorderValue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.morphologyDefaultBorderValue: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::morphologyEx(const Mat &src, Mat &dst, int op, const Mat &kernel, Point anchor=Point(-1,-1), int iterations=1, int borderType=BORDER_CONSTANT, const Scalar &borderValue=morphologyDefaultBorderValue())
 * include/opencv/cv.hpp:321
 */
static int cv_morphologyEx(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int op = luaL_checkint(L, 3);
    const Mat *kernel = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      morphologyEx(*src, *dst, op, *kernel);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 5, "cv.Point"));
      if (top__ < 6) {
        morphologyEx(*src, *dst, op, *kernel, *anchor);
      } else {
        int iterations = luaL_checkint(L, 6);
        if (top__ < 7) {
          morphologyEx(*src, *dst, op, *kernel, *anchor, iterations);
        } else {
          int borderType = luaL_checkint(L, 7);
          if (top__ < 8) {
            morphologyEx(*src, *dst, op, *kernel, *anchor, iterations, borderType);
          } else {
            const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 8, "cv.Scalar"));
            morphologyEx(*src, *dst, op, *kernel, *anchor, iterations, borderType, *borderValue);
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.morphologyEx: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.morphologyEx: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.morphologyEx: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::mulSpectrums(const Mat &a, const Mat &b, Mat &c, int flags, bool conjB=false)
 * include/opencv/cxcore.hpp:1256
 */
static int cv_mulSpectrums(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    int flags = luaL_checkint(L, 4);
    if (top__ < 5) {
      mulSpectrums(*a, *b, *c, flags);
    } else {
      bool conjB = lua_toboolean(L, 5);
      mulSpectrums(*a, *b, *c, flags, conjB);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mulSpectrums: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mulSpectrums: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mulSpectrums: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::mulTransposed(const Mat &a, Mat &c, bool aTa, const Mat &delta=Mat(), double scale=1, int rtype=-1)
 * include/opencv/cxcore.hpp:1191
 */
static int cv_mulTransposed(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    bool aTa = lua_toboolean(L, 3);
    if (top__ < 4) {
      mulTransposed(*a, *c, aTa);
    } else {
      const Mat *delta = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      if (top__ < 5) {
        mulTransposed(*a, *c, aTa, *delta);
      } else {
        double scale = luaL_checknumber(L, 5);
        if (top__ < 6) {
          mulTransposed(*a, *c, aTa, *delta, scale);
        } else {
          int rtype = luaL_checkint(L, 6);
          mulTransposed(*a, *c, aTa, *delta, scale, rtype);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.mulTransposed: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.mulTransposed: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.mulTransposed: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::multiply(const Mat &a, const Mat &b, Mat &c, double scale=1)
 * include/opencv/cxcore.hpp:1098
 */
static int cv_multiply1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      multiply(*a, *b, *c);
    } else {
      double scale = luaL_checknumber(L, 4);
      multiply(*a, *b, *c, scale);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.multiply: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.multiply: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.multiply: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::multiply(const MatND &a, const MatND &b, MatND &c, double scale=1)
 * include/opencv/cxcore.hpp:1754
 */
static int cv_multiply2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      multiply(*a, *b, *c);
    } else {
      double scale = luaL_checknumber(L, 4);
      multiply(*a, *b, *c, scale);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.multiply: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.multiply: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.multiply: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for multiply(...) */
static int cv_multiply(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_multiply2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_multiply1(L);
  } else {
    // use any to raise errors
    return cv_multiply1(L);
  }
}




/** CV_EXPORTS double cv::norm(const Mat &a, int normType=NORM_L2)
 * include/opencv/cxcore.hpp:1115
 */
static int cv_norm1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      retval__ = norm(*a);
    } else {
      int normType = luaL_checkint(L, 2);
      retval__ = norm(*a, normType);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const Mat &a, const Mat &b, int normType=NORM_L2)
 * include/opencv/cxcore.hpp:1116
 */
static int cv_norm2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      retval__ = norm(*a, *b);
    } else {
      int normType = luaL_checkint(L, 3);
      retval__ = norm(*a, *b, normType);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const Mat &a, int normType, const Mat &mask)
 * include/opencv/cxcore.hpp:1117
 */
static int cv_norm3(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    int normType = luaL_checkint(L, 2);
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    double  retval__ = norm(*a, normType, *mask);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const Mat &a, const Mat &b, int normType, const Mat &mask)
 * include/opencv/cxcore.hpp:1119
 */
static int cv_norm4(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int normType = luaL_checkint(L, 3);
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    double  retval__ = norm(*a, *b, normType, *mask);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const MatND &a, int normType=NORM_L2, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1769
 */
static int cv_norm5(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    if (top__ < 2) {
      retval__ = norm(*a);
    } else {
      int normType = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = norm(*a, normType);
      } else {
        const MatND *mask = *((const MatND **)luaL_checkudata(L, 3, "cv.MatND"));
        retval__ = norm(*a, normType, *mask);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const MatND &a, const MatND &b, int normType=NORM_L2, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1771
 */
static int cv_norm6(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    if (top__ < 3) {
      retval__ = norm(*a, *b);
    } else {
      int normType = luaL_checkint(L, 3);
      if (top__ < 4) {
        retval__ = norm(*a, *b, normType);
      } else {
        const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
        retval__ = norm(*a, *b, normType, *mask);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS double cv::norm(const SparseMat &src, int normType)
 * include/opencv/cxcore.hpp:2083
 */
static int cv_norm7(lua_State *L) {
  try {
    const SparseMat *src = *((const SparseMat **)luaL_checkudata(L, 1, "cv.SparseMat"));
    int normType = luaL_checkint(L, 2);
    double  retval__ = norm(*src, normType);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.norm: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.norm: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.norm: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for norm(...) */
static int cv_norm(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.SparseMat")) {
    return cv_norm7(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return cv_norm5(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_norm6(L);
    } else {
      // use any to raise errors
      return cv_norm6(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (top__ < 3) {
        return cv_norm1(L);
      } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        return cv_norm3(L);
      } else {
        // use any to raise errors
        return cv_norm3(L);
      }
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_norm2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_norm4(L);
        } else {
          // use any to raise errors
          return cv_norm4(L);
        }
      } else {
        // use any to raise errors
        return cv_norm4(L);
      }
    } else {
      // use any to raise errors
      return cv_norm4(L);
    }
  } else {
    // use any to raise errors
    return cv_norm4(L);
  }
}




/** CV_EXPORTS void cv::normalize(const Mat &a, Mat &b, double alpha=1, double beta=0, int norm_type=NORM_L2, int rtype=-1, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1121
 */
static int cv_normalize1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      normalize(*a, *b);
    } else {
      double alpha = luaL_checknumber(L, 3);
      if (top__ < 4) {
        normalize(*a, *b, alpha);
      } else {
        double beta = luaL_checknumber(L, 4);
        if (top__ < 5) {
          normalize(*a, *b, alpha, beta);
        } else {
          int norm_type = luaL_checkint(L, 5);
          if (top__ < 6) {
            normalize(*a, *b, alpha, beta, norm_type);
          } else {
            int rtype = luaL_checkint(L, 6);
            if (top__ < 7) {
              normalize(*a, *b, alpha, beta, norm_type, rtype);
            } else {
              const Mat *mask = *((const Mat **)luaL_checkudata(L, 7, "cv.Mat"));
              normalize(*a, *b, alpha, beta, norm_type, rtype, *mask);
            }
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.normalize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.normalize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.normalize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::normalize(const MatND &a, MatND &b, double alpha=1, double beta=0, int norm_type=NORM_L2, int rtype=-1, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1773
 */
static int cv_normalize2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    MatND *b = *((MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    if (top__ < 3) {
      normalize(*a, *b);
    } else {
      double alpha = luaL_checknumber(L, 3);
      if (top__ < 4) {
        normalize(*a, *b, alpha);
      } else {
        double beta = luaL_checknumber(L, 4);
        if (top__ < 5) {
          normalize(*a, *b, alpha, beta);
        } else {
          int norm_type = luaL_checkint(L, 5);
          if (top__ < 6) {
            normalize(*a, *b, alpha, beta, norm_type);
          } else {
            int rtype = luaL_checkint(L, 6);
            if (top__ < 7) {
              normalize(*a, *b, alpha, beta, norm_type, rtype);
            } else {
              const MatND *mask = *((const MatND **)luaL_checkudata(L, 7, "cv.MatND"));
              normalize(*a, *b, alpha, beta, norm_type, rtype, *mask);
            }
          }
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.normalize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.normalize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.normalize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::normalize(const SparseMat &src, SparseMat &dst, double alpha, int normType)
 * include/opencv/cxcore.hpp:2084
 */
static int cv_normalize3(lua_State *L) {
  try {
    const SparseMat *src = *((const SparseMat **)luaL_checkudata(L, 1, "cv.SparseMat"));
    SparseMat *dst = *((SparseMat **)luaL_checkudata(L, 2, "cv.SparseMat"));
    double alpha = luaL_checknumber(L, 3);
    int normType = luaL_checkint(L, 4);
    normalize(*src, *dst, alpha, normType);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.normalize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.normalize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.normalize: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for normalize(...) */
static int cv_normalize(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.SparseMat")) {
    return cv_normalize3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_normalize2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_normalize1(L);
  } else {
    // use any to raise errors
    return cv_normalize1(L);
  }
}



/** CV_EXPORTS void cv::perspectiveTransform(const Mat &src, Mat &dst, const Mat &m)
 * include/opencv/cxcore.hpp:1194
 */
static int cv_perspectiveTransform(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *m = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    perspectiveTransform(*src, *dst, *m);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.perspectiveTransform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.perspectiveTransform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.perspectiveTransform: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::phase(const Mat &x, const Mat &y, Mat &angle, bool angleInDegrees=false)
 * include/opencv/cxcore.hpp:1182
 */
static int cv_phase(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *x = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *y = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *angle = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      phase(*x, *y, *angle);
    } else {
      bool angleInDegrees = lua_toboolean(L, 4);
      phase(*x, *y, *angle, angleInDegrees);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.phase: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.phase: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.phase: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::pointPolygonTest(const Mat &contour, Point2f pt, bool measureDist)
 * include/opencv/cv.hpp:583
 */
static int cv_pointPolygonTest(lua_State *L) {
  try {
    const Mat *contour = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point2f *pt = *((Point2f **)luaL_checkudata(L, 2, "cv.Point2f"));
    bool measureDist = lua_toboolean(L, 3);
    double  retval__ = pointPolygonTest(*contour, *pt, measureDist);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.pointPolygonTest: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.pointPolygonTest: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.pointPolygonTest: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::polarToCart(const Mat &magnitude, const Mat &angle, Mat &x, Mat &y, bool angleInDegrees=false)
 * include/opencv/cxcore.hpp:1177
 */
static int cv_polarToCart(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *magnitude = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *angle = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *x = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *y = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      polarToCart(*magnitude, *angle, *x, *y);
    } else {
      bool angleInDegrees = lua_toboolean(L, 5);
      polarToCart(*magnitude, *angle, *x, *y, angleInDegrees);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.polarToCart: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.polarToCart: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.polarToCart: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::pow(const Mat &a, double power, Mat &b)
 * include/opencv/cxcore.hpp:1171
 */
static int cv_pow1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double power = luaL_checknumber(L, 2);
    Mat *b = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    pow(*a, power, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.pow: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.pow: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.pow: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::pow(const MatND &a, double power, MatND &b)
 * include/opencv/cxcore.hpp:1805
 */
static int cv_pow2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double power = luaL_checknumber(L, 2);
    MatND *b = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    pow(*a, power, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.pow: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.pow: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.pow: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for pow(...) */
static int cv_pow(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_pow2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_pow1(L);
  } else {
    // use any to raise errors
    return cv_pow1(L);
  }
}



/** CV_EXPORTS void cv::preCornerDetect(const Mat &src, Mat &dst, int ksize, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:286
 */
static int cv_preCornerDetect(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ksize = luaL_checkint(L, 3);
    if (top__ < 4) {
      preCornerDetect(*src, *dst, ksize);
    } else {
      int borderType = luaL_checkint(L, 4);
      preCornerDetect(*src, *dst, ksize, borderType);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.preCornerDetect: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.preCornerDetect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.preCornerDetect: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::putText(Mat &img, const string &text, Point org, int fontFace, double fontScale, Scalar color, int thickness=1, int linetype=8, bool bottomLeftOrigin=false)
 * include/opencv/cxcore.hpp:1345
 */
static int cv_putText(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const string *text = *((const string **)luaL_checkudata(L, 2, "cv.string"));
    Point *org = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
    int fontFace = luaL_checkint(L, 4);
    double fontScale = luaL_checknumber(L, 5);
    Scalar *color = *((Scalar **)luaL_checkudata(L, 6, "cv.Scalar"));
    if (top__ < 7) {
      putText(*img, *text, *org, fontFace, fontScale, *color);
    } else {
      int thickness = luaL_checkint(L, 7);
      if (top__ < 8) {
        putText(*img, *text, *org, fontFace, fontScale, *color, thickness);
      } else {
        int linetype = luaL_checkint(L, 8);
        if (top__ < 9) {
          putText(*img, *text, *org, fontFace, fontScale, *color, thickness, linetype);
        } else {
          bool bottomLeftOrigin = lua_toboolean(L, 9);
          putText(*img, *text, *org, fontFace, fontScale, *color, thickness, linetype, bottomLeftOrigin);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.putText: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.putText: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.putText: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::pyrDown(const Mat &src, Mat &dst, const Size &dstsize=Size())
 * include/opencv/cv.hpp:393
 */
static int cv_pyrDown(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      pyrDown(*src, *dst);
    } else {
      const Size *dstsize = *((const Size **)luaL_checkudata(L, 3, "cv.Size"));
      pyrDown(*src, *dst, *dstsize);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.pyrDown: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.pyrDown: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.pyrDown: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::pyrUp(const Mat &src, Mat &dst, const Size &dstsize=Size())
 * include/opencv/cv.hpp:394
 */
static int cv_pyrUp(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      pyrUp(*src, *dst);
    } else {
      const Size *dstsize = *((const Size **)luaL_checkudata(L, 3, "cv.Size"));
      pyrUp(*src, *dst, *dstsize);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.pyrUp: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.pyrUp: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.pyrUp: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static void cv::randn(Mat &dst, const Scalar &mean, const Scalar &stddev)
 * include/opencv/cxcore.hpp:1270
 */
static int cv_randn(lua_State *L) {
  try {
    Mat *dst = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *mean = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    const Scalar *stddev = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    randn(*dst, *mean, *stddev);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.randn: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.randn: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.randn: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** static void cv::randu(Mat &dst, const Scalar &low, const Scalar &high)
 * include/opencv/cxcore.hpp:1268
 */
static int cv_randu(lua_State *L) {
  try {
    Mat *dst = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *low = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    const Scalar *high = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    randu(*dst, *low, *high);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.randu: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.randu: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.randu: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::rectangle(Mat &img, Point pt1, Point pt2, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
 * include/opencv/cxcore.hpp:1279
 */
static int cv_rectangle1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Point *pt1 = *((Point **)luaL_checkudata(L, 2, "cv.Point"));
    Point *pt2 = *((Point **)luaL_checkudata(L, 3, "cv.Point"));
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 4, "cv.Scalar"));
    if (top__ < 5) {
      rectangle(*img, *pt1, *pt2, *color);
    } else {
      int thickness = luaL_checkint(L, 5);
      if (top__ < 6) {
        rectangle(*img, *pt1, *pt2, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 6);
        if (top__ < 7) {
          rectangle(*img, *pt1, *pt2, *color, thickness, lineType);
        } else {
          int shift = luaL_checkint(L, 7);
          rectangle(*img, *pt1, *pt2, *color, thickness, lineType, shift);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.rectangle: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.rectangle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.rectangle: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::rectangle(Mat &img, Rect rec, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
 * include/opencv/cxcore.hpp:1283
 */
static int cv_rectangle2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *img = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Rect *rec = *((Rect **)luaL_checkudata(L, 2, "cv.Rect"));
    const Scalar *color = *((const Scalar **)luaL_checkudata(L, 3, "cv.Scalar"));
    if (top__ < 4) {
      rectangle(*img, *rec, *color);
    } else {
      int thickness = luaL_checkint(L, 4);
      if (top__ < 5) {
        rectangle(*img, *rec, *color, thickness);
      } else {
        int lineType = luaL_checkint(L, 5);
        if (top__ < 6) {
          rectangle(*img, *rec, *color, thickness, lineType);
        } else {
          int shift = luaL_checkint(L, 6);
          rectangle(*img, *rec, *color, thickness, lineType, shift);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.rectangle: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.rectangle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.rectangle: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for rectangle(...) */
static int cv_rectangle(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Rect")) {
      return cv_rectangle2(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Point")) {
      return cv_rectangle1(L);
    } else {
      // use any to raise errors
      return cv_rectangle1(L);
    }
  } else {
    // use any to raise errors
    return cv_rectangle1(L);
  }
}



/** CV_EXPORTS void cv::reduce(const Mat &m, Mat &dst, int dim, int rtype, int dtype=-1)
 * include/opencv/cxcore.hpp:1126
 */
static int cv_reduce(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int dim = luaL_checkint(L, 3);
    int rtype = luaL_checkint(L, 4);
    if (top__ < 5) {
      reduce(*m, *dst, dim, rtype);
    } else {
      int dtype = luaL_checkint(L, 5);
      reduce(*m, *dst, dim, rtype, dtype);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.reduce: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.reduce: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.reduce: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::remap(const Mat &src, Mat &dst, const Mat &map1, const Mat &map2, int interpolation, int borderMode=BORDER_CONSTANT, const Scalar &borderValue=Scalar())
 * include/opencv/cv.hpp:358
 */
static int cv_remap(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *map1 = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *map2 = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    int interpolation = luaL_checkint(L, 5);
    if (top__ < 6) {
      remap(*src, *dst, *map1, *map2, interpolation);
    } else {
      int borderMode = luaL_checkint(L, 6);
      if (top__ < 7) {
        remap(*src, *dst, *map1, *map2, interpolation, borderMode);
      } else {
        const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
        remap(*src, *dst, *map1, *map2, interpolation, borderMode, *borderValue);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.remap: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.remap: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.remap: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::repeat(const Mat &a, int ny, int nx, Mat &b)
 * include/opencv/cxcore.hpp:1143
 */
static int cv_repeat1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    int ny = luaL_checkint(L, 2);
    int nx = luaL_checkint(L, 3);
    Mat *b = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    repeat(*a, ny, nx, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.repeat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.repeat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.repeat: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** static Mat cv::repeat(const Mat &src, int ny, int nx)
 * include/opencv/cxcore.hpp:1145
 */
static int cv_repeat2(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    int ny = luaL_checkint(L, 2);
    int nx = luaL_checkint(L, 3);
    Mat  retval__ = repeat(*src, ny, nx);
    lua_pushclass<Mat>(L, retval__, "cv.Mat");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.repeat: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.repeat: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.repeat: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for repeat(...) */
static int cv_repeat(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TNUMBER) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_repeat2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_repeat1(L);
        } else {
          // use any to raise errors
          return cv_repeat1(L);
        }
      } else {
        // use any to raise errors
        return cv_repeat1(L);
      }
    } else {
      // use any to raise errors
      return cv_repeat1(L);
    }
  } else {
    // use any to raise errors
    return cv_repeat1(L);
  }
}



/** CV_EXPORTS void cv::reprojectImageTo3D(const Mat &disparity, Mat &_3dImage, const Mat &Q, bool handleMissingValues=false)
 * include/opencv/cv.hpp:967
 */
static int cv_reprojectImageTo3D(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *disparity = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *_3dImage = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *Q = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      reprojectImageTo3D(*disparity, *_3dImage, *Q);
    } else {
      bool handleMissingValues = lua_toboolean(L, 4);
      reprojectImageTo3D(*disparity, *_3dImage, *Q, handleMissingValues);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.reprojectImageTo3D: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.reprojectImageTo3D: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.reprojectImageTo3D: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::resize(const Mat &src, Mat &dst, Size dsize, double fx=0, double fy=0, int interpolation=INTER_LINEAR)
 * include/opencv/cv.hpp:343
 */
static int cv_resize(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Size *dsize = *((Size **)luaL_checkudata(L, 3, "cv.Size"));
    if (top__ < 4) {
      resize(*src, *dst, *dsize);
    } else {
      double fx = luaL_checknumber(L, 4);
      if (top__ < 5) {
        resize(*src, *dst, *dsize, fx);
      } else {
        double fy = luaL_checknumber(L, 5);
        if (top__ < 6) {
          resize(*src, *dst, *dsize, fx, fy);
        } else {
          int interpolation = luaL_checkint(L, 6);
          resize(*src, *dst, *dsize, fx, fy, interpolation);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.resize: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.resize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.resize: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::scaleAdd(const Mat &a, double alpha, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1103
 */
static int cv_scaleAdd1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    double alpha = luaL_checknumber(L, 2);
    const Mat *b = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    scaleAdd(*a, alpha, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.scaleAdd: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.scaleAdd: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.scaleAdd: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::scaleAdd(const MatND &a, double alpha, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1759
 */
static int cv_scaleAdd2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    double alpha = luaL_checknumber(L, 2);
    const MatND *b = *((const MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 4, "cv.MatND"));
    scaleAdd(*a, alpha, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.scaleAdd: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.scaleAdd: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.scaleAdd: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for scaleAdd(...) */
static int cv_scaleAdd(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_scaleAdd2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_scaleAdd1(L);
  } else {
    // use any to raise errors
    return cv_scaleAdd1(L);
  }
}



/** CV_EXPORTS void cv::sepFilter2D(const Mat &src, Mat &dst, int ddepth, const Mat &kernelX, const Mat &kernelY, Point anchor=Point(-1,-1), double delta=0, int borderType=BORDER_DEFAULT)
 * include/opencv/cv.hpp:254
 */
static int cv_sepFilter2D(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int ddepth = luaL_checkint(L, 3);
    const Mat *kernelX = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    const Mat *kernelY = *((const Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    if (top__ < 6) {
      sepFilter2D(*src, *dst, ddepth, *kernelX, *kernelY);
    } else {
      Point *anchor = *((Point **)luaL_checkudata(L, 6, "cv.Point"));
      if (top__ < 7) {
        sepFilter2D(*src, *dst, ddepth, *kernelX, *kernelY, *anchor);
      } else {
        double delta = luaL_checknumber(L, 7);
        if (top__ < 8) {
          sepFilter2D(*src, *dst, ddepth, *kernelX, *kernelY, *anchor, delta);
        } else {
          int borderType = luaL_checkint(L, 8);
          sepFilter2D(*src, *dst, ddepth, *kernelX, *kernelY, *anchor, delta, borderType);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sepFilter2D: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sepFilter2D: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sepFilter2D: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS bool cv::setBreakOnError(bool value)
 * include/opencv/cxcore.hpp:112
 */
static int cv_setBreakOnError(lua_State *L) {
  try {
    bool value = lua_toboolean(L, 1);
    bool  retval__ = setBreakOnError(value);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.setBreakOnError: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.setBreakOnError: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.setBreakOnError: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::setIdentity(Mat &c, const Scalar &s=Scalar(1))
 * include/opencv/cxcore.hpp:1197
 */
static int cv_setIdentity(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Mat *c = *((Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    if (top__ < 2) {
      setIdentity(*c);
    } else {
      const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
      setIdentity(*c, *s);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.setIdentity: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.setIdentity: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.setIdentity: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::setNumThreads(int)
 * include/opencv/cxcore.hpp:132
 */
static int cv_setNumThreads(lua_State *L) {
  try {
    int arg1 = luaL_checkint(L, 1);
    setNumThreads(arg1);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.setNumThreads: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.setNumThreads: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.setNumThreads: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::setUseOptimized(bool)
 * include/opencv/cxcore.hpp:163
 */
static int cv_setUseOptimized(lua_State *L) {
  try {
    bool arg1 = lua_toboolean(L, 1);
    setUseOptimized(arg1);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.setUseOptimized: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.setUseOptimized: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.setUseOptimized: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS bool cv::solve(const Mat &a, const Mat &b, Mat &x, int flags=DECOMP_LU)
 * include/opencv/cxcore.hpp:1201
 */
static int cv_solve(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    bool  retval__;
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *x = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      retval__ = solve(*a, *b, *x);
    } else {
      int flags = luaL_checkint(L, 4);
      retval__ = solve(*a, *b, *x, flags);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.solve: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.solve: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.solve: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS int cv::solveCubic(const Mat &coeffs, Mat &roots)
 * include/opencv/cxcore.hpp:1204
 */
static int cv_solveCubic(lua_State *L) {
  try {
    const Mat *coeffs = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *roots = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int  retval__ = solveCubic(*coeffs, *roots);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.solveCubic: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.solveCubic: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.solveCubic: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::solvePnP(const Mat &objectPoints, const Mat &imagePoints, const Mat &cameraMatrix, const Mat &distCoeffs, Mat &rvec, Mat &tvec, bool useExtrinsicGuess=false)
 * include/opencv/cv.hpp:805
 */
static int cv_solvePnP(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *objectPoints = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *imagePoints = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *distCoeffs = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Mat *rvec = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *tvec = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    if (top__ < 7) {
      solvePnP(*objectPoints, *imagePoints, *cameraMatrix, *distCoeffs, *rvec, *tvec);
    } else {
      bool useExtrinsicGuess = lua_toboolean(L, 7);
      solvePnP(*objectPoints, *imagePoints, *cameraMatrix, *distCoeffs, *rvec, *tvec, useExtrinsicGuess);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.solvePnP: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.solvePnP: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.solvePnP: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::solvePoly(const Mat &coeffs, Mat &roots, int maxIters=300)
 * include/opencv/cxcore.hpp:1205
 */
static int cv_solvePoly(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    double  retval__;
    const Mat *coeffs = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *roots = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    if (top__ < 3) {
      retval__ = solvePoly(*coeffs, *roots);
    } else {
      int maxIters = luaL_checkint(L, 3);
      retval__ = solvePoly(*coeffs, *roots, maxIters);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.solvePoly: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.solvePoly: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.solvePoly: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::sort(const Mat &a, Mat &b, int flags)
 * include/opencv/cxcore.hpp:1202
 */
static int cv_sort(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int flags = luaL_checkint(L, 3);
    sort(*a, *b, flags);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sort: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sort: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sort: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::sortIdx(const Mat &a, Mat &b, int flags)
 * include/opencv/cxcore.hpp:1203
 */
static int cv_sortIdx(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    int flags = luaL_checkint(L, 3);
    sortIdx(*a, *b, flags);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sortIdx: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sortIdx: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sortIdx: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::sqrt(const Mat &a, Mat &b)
 * include/opencv/cxcore.hpp:1170
 */
static int cv_sqrt1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    sqrt(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sqrt: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sqrt: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sqrt: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::sqrt(const MatND &a, MatND &b)
 * include/opencv/cxcore.hpp:1804
 */
static int cv_sqrt2(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    MatND *b = *((MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    sqrt(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sqrt: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sqrt: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sqrt: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for sqrt(...) */
static int cv_sqrt(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_sqrt2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_sqrt1(L);
  } else {
    // use any to raise errors
    return cv_sqrt1(L);
  }
}




/** CV_EXPORTS void cv::stereoRectify(const Mat &cameraMatrix1, const Mat &distCoeffs1, const Mat &cameraMatrix2, const Mat &distCoeffs2, Size imageSize, const Mat &R, const Mat &T, Mat &R1, Mat &R2, Mat &P1, Mat &P2, Mat &Q, int flags=CALIB_ZERO_DISPARITY)
 * include/opencv/cv.hpp:873
 */
static int cv_stereoRectify1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *cameraMatrix1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *distCoeffs1 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *cameraMatrix2 = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *distCoeffs2 = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    Size *imageSize = *((Size **)luaL_checkudata(L, 5, "cv.Size"));
    const Mat *R = *((const Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    const Mat *T = *((const Mat **)luaL_checkudata(L, 7, "cv.Mat"));
    Mat *R1 = *((Mat **)luaL_checkudata(L, 8, "cv.Mat"));
    Mat *R2 = *((Mat **)luaL_checkudata(L, 9, "cv.Mat"));
    Mat *P1 = *((Mat **)luaL_checkudata(L, 10, "cv.Mat"));
    Mat *P2 = *((Mat **)luaL_checkudata(L, 11, "cv.Mat"));
    Mat *Q = *((Mat **)luaL_checkudata(L, 12, "cv.Mat"));
    if (top__ < 13) {
      stereoRectify(*cameraMatrix1, *distCoeffs1, *cameraMatrix2, *distCoeffs2, *imageSize, *R, *T, *R1, *R2, *P1, *P2, *Q);
    } else {
      int flags = luaL_checkint(L, 13);
      stereoRectify(*cameraMatrix1, *distCoeffs1, *cameraMatrix2, *distCoeffs2, *imageSize, *R, *T, *R1, *R2, *P1, *P2, *Q, flags);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.stereoRectify: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.stereoRectify: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.stereoRectify: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for stereoRectify(...) */
static int cv_stereoRectify(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_stereoRectify1(L);
  } else {
    // use any to raise errors
    return cv_stereoRectify1(L);
  }
}



/** CV_EXPORTS bool cv::stereoRectifyUncalibrated(const Mat &points1, const Mat &points2, const Mat &F, Size imgSize, Mat &H1, Mat &H2, double threshold=5)
 * include/opencv/cv.hpp:887
 */
static int cv_stereoRectifyUncalibrated(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    bool  retval__;
    const Mat *points1 = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *points2 = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *F = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Size *imgSize = *((Size **)luaL_checkudata(L, 4, "cv.Size"));
    Mat *H1 = *((Mat **)luaL_checkudata(L, 5, "cv.Mat"));
    Mat *H2 = *((Mat **)luaL_checkudata(L, 6, "cv.Mat"));
    if (top__ < 7) {
      retval__ = stereoRectifyUncalibrated(*points1, *points2, *F, *imgSize, *H1, *H2);
    } else {
      double threshold = luaL_checknumber(L, 7);
      retval__ = stereoRectifyUncalibrated(*points1, *points2, *F, *imgSize, *H1, *H2, threshold);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.stereoRectifyUncalibrated: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.stereoRectifyUncalibrated: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.stereoRectifyUncalibrated: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::subtract(const Mat &a, const Mat &b, Mat &c, const Mat &mask)
 * include/opencv/cxcore.hpp:1086
 */
static int cv_subtract1(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    subtract(*a, *b, *c, *mask);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const Mat &a, const Mat &b, Mat &c)
 * include/opencv/cxcore.hpp:1094
 */
static int cv_subtract2(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Mat *b = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    subtract(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const Mat &a, const Scalar &s, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1096
 */
static int cv_subtract3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 2, "cv.Scalar"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      subtract(*a, *s, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      subtract(*a, *s, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const Scalar &s, const Mat &a, Mat &c, const Mat &mask=Mat())
 * include/opencv/cxcore.hpp:1102
 */
static int cv_subtract4(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 1, "cv.Scalar"));
    const Mat *a = *((const Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    Mat *c = *((Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    if (top__ < 4) {
      subtract(*s, *a, *c);
    } else {
      const Mat *mask = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
      subtract(*s, *a, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const MatND &a, const MatND &b, MatND &c, const MatND &mask)
 * include/opencv/cxcore.hpp:1749
 */
static int cv_subtract5(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
    subtract(*a, *b, *c, *mask);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const MatND &a, const MatND &b, MatND &c)
 * include/opencv/cxcore.hpp:1751
 */
static int cv_subtract6(lua_State *L) {
  try {
    const MatND *a = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    const MatND *b = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    subtract(*a, *b, *c);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS void cv::subtract(const Scalar &s, const MatND &a, MatND &c, const MatND &mask=MatND())
 * include/opencv/cxcore.hpp:1758
 */
static int cv_subtract7(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Scalar *s = *((const Scalar **)luaL_checkudata(L, 1, "cv.Scalar"));
    const MatND *a = *((const MatND **)luaL_checkudata(L, 2, "cv.MatND"));
    MatND *c = *((MatND **)luaL_checkudata(L, 3, "cv.MatND"));
    if (top__ < 4) {
      subtract(*s, *a, *c);
    } else {
      const MatND *mask = *((const MatND **)luaL_checkudata(L, 4, "cv.MatND"));
      subtract(*s, *a, *c, *mask);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.subtract: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.subtract: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.subtract: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for subtract(...) */
static int cv_subtract(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.MatND")) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_subtract6(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.MatND")) {
          return cv_subtract5(L);
        } else {
          // use any to raise errors
          return cv_subtract5(L);
        }
      } else {
        // use any to raise errors
        return cv_subtract5(L);
      }
    } else {
      // use any to raise errors
      return cv_subtract5(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      type__ = lua_type(L, 3);
      if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "cv.Mat")) {
        type__ = lua_type(L, 4);
        if (top__ < 4) {
          return cv_subtract2(L);
        } else if (type__ == LUA_TUSERDATA && is_userdata(L, 4, "cv.Mat")) {
          return cv_subtract1(L);
        } else {
          // use any to raise errors
          return cv_subtract1(L);
        }
      } else {
        // use any to raise errors
        return cv_subtract1(L);
      }
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Scalar")) {
      return cv_subtract3(L);
    } else {
      // use any to raise errors
      return cv_subtract3(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Scalar")) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.MatND")) {
      return cv_subtract7(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "cv.Mat")) {
      return cv_subtract4(L);
    } else {
      // use any to raise errors
      return cv_subtract4(L);
    }
  } else {
    // use any to raise errors
    return cv_subtract4(L);
  }
}




/** CV_EXPORTS Scalar cv::sum(const Mat &m)
 * include/opencv/cxcore.hpp:1109
 */
static int cv_sum1(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Scalar  retval__ = sum(*m);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sum: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sum: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sum: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** CV_EXPORTS Scalar cv::sum(const MatND &m)
 * include/opencv/cxcore.hpp:1763
 */
static int cv_sum2(lua_State *L) {
  try {
    const MatND *m = *((const MatND **)luaL_checkudata(L, 1, "cv.MatND"));
    Scalar  retval__ = sum(*m);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.sum: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.sum: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.sum: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for sum(...) */
static int cv_sum(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.MatND")) {
    return cv_sum2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_sum1(L);
  } else {
    // use any to raise errors
    return cv_sum1(L);
  }
}



/** CV_EXPORTS RNG& cv::theRNG()
 * include/opencv/cxcore.hpp:1264
 */
static int cv_theRNG(lua_State *L) {
  try {
    RNG  retval__ = theRNG();
    lua_pushclass<RNG>(L, retval__, "cv.RNG");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.theRNG: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.theRNG: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.theRNG: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS double cv::threshold(const Mat &src, Mat &dst, double thresh, double maxval, int type)
 * include/opencv/cv.hpp:385
 */
static int cv_threshold(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double thresh = luaL_checknumber(L, 3);
    double maxval = luaL_checknumber(L, 4);
    int type = luaL_checkint(L, 5);
    double  retval__ = threshold(*src, *dst, thresh, maxval, type);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.threshold: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.threshold: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.threshold: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS WString cv::toUtf16(const string &str)
 * include/opencv/cxcore.hpp:78
 */
static int cv_toUtf16(lua_State *L) {
  try {
    const string *str = *((const string **)luaL_checkudata(L, 1, "cv.string"));
    WString  retval__ = toUtf16(*str);
    lua_pushclass<WString>(L, retval__, "cv.WString");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.toUtf16: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.toUtf16: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.toUtf16: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS Scalar cv::trace(const Mat &m)
 * include/opencv/cxcore.hpp:1199
 */
static int cv_trace(lua_State *L) {
  try {
    const Mat *m = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Scalar  retval__ = trace(*m);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.trace: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.trace: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.trace: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::transform(const Mat &src, Mat &dst, const Mat &m)
 * include/opencv/cxcore.hpp:1193
 */
static int cv_transform(lua_State *L) {
  try {
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *m = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    transform(*src, *dst, *m);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.transform: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.transform: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.transform: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::transpose(const Mat &a, Mat &b)
 * include/opencv/cxcore.hpp:1192
 */
static int cv_transpose(lua_State *L) {
  try {
    const Mat *a = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *b = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    transpose(*a, *b);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.transpose: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.transpose: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.transpose: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::undistort(const Mat &src, Mat &dst, const Mat &cameraMatrix, const Mat &distCoeffs, const Mat &newCameraMatrix=Mat())
 * include/opencv/cv.hpp:399
 */
static int cv_undistort(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *distCoeffs = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      undistort(*src, *dst, *cameraMatrix, *distCoeffs);
    } else {
      const Mat *newCameraMatrix = *((const Mat **)luaL_checkudata(L, 5, "cv.Mat"));
      undistort(*src, *dst, *cameraMatrix, *distCoeffs, *newCameraMatrix);
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.undistort: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.undistort: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.undistort: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/** CV_EXPORTS void cv::undistortPoints(const Mat &src, Mat &dst, const Mat &cameraMatrix, const Mat &distCoeffs, const Mat &R=Mat(), const Mat &P=Mat())
 * include/opencv/cv.hpp:738
 */
static int cv_undistortPoints2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *cameraMatrix = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    const Mat *distCoeffs = *((const Mat **)luaL_checkudata(L, 4, "cv.Mat"));
    if (top__ < 5) {
      undistortPoints(*src, *dst, *cameraMatrix, *distCoeffs);
    } else {
      const Mat *R = *((const Mat **)luaL_checkudata(L, 5, "cv.Mat"));
      if (top__ < 6) {
        undistortPoints(*src, *dst, *cameraMatrix, *distCoeffs, *R);
      } else {
        const Mat *P = *((const Mat **)luaL_checkudata(L, 6, "cv.Mat"));
        undistortPoints(*src, *dst, *cameraMatrix, *distCoeffs, *R, *P);
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.undistortPoints: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.undistortPoints: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.undistortPoints: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for undistortPoints(...) */
static int cv_undistortPoints(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Mat")) {
    return cv_undistortPoints2(L);
  } else {
    // use any to raise errors
    return cv_undistortPoints2(L);
  }
}



/** CV_EXPORTS void cv::updateMotionHistory(const Mat &silhouette, Mat &mhi, double timestamp, double duration)
 * include/opencv/cv.hpp:589
 */
static int cv_updateMotionHistory(lua_State *L) {
  try {
    const Mat *silhouette = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *mhi = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    double timestamp = luaL_checknumber(L, 3);
    double duration = luaL_checknumber(L, 4);
    updateMotionHistory(*silhouette, *mhi, timestamp, duration);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.updateMotionHistory: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.updateMotionHistory: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.updateMotionHistory: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS bool cv::useOptimized()
 * include/opencv/cxcore.hpp:164
 */
static int cv_useOptimized(lua_State *L) {
  try {
    bool  retval__ = useOptimized();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.useOptimized: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.useOptimized: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.useOptimized: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::warpAffine(const Mat &src, Mat &dst, const Mat &M, Size dsize, int flags=INTER_LINEAR, int borderMode=BORDER_CONSTANT, const Scalar &borderValue=Scalar())
 * include/opencv/cv.hpp:349
 */
static int cv_warpAffine(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *M = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Size *dsize = *((Size **)luaL_checkudata(L, 4, "cv.Size"));
    if (top__ < 5) {
      warpAffine(*src, *dst, *M, *dsize);
    } else {
      int flags = luaL_checkint(L, 5);
      if (top__ < 6) {
        warpAffine(*src, *dst, *M, *dsize, flags);
      } else {
        int borderMode = luaL_checkint(L, 6);
        if (top__ < 7) {
          warpAffine(*src, *dst, *M, *dsize, flags, borderMode);
        } else {
          const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
          warpAffine(*src, *dst, *M, *dsize, flags, borderMode, *borderValue);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.warpAffine: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.warpAffine: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.warpAffine: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::warpPerspective(const Mat &src, Mat &dst, const Mat &M, Size dsize, int flags=INTER_LINEAR, int borderMode=BORDER_CONSTANT, const Scalar &borderValue=Scalar())
 * include/opencv/cv.hpp:354
 */
static int cv_warpPerspective(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    const Mat *src = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *dst = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    const Mat *M = *((const Mat **)luaL_checkudata(L, 3, "cv.Mat"));
    Size *dsize = *((Size **)luaL_checkudata(L, 4, "cv.Size"));
    if (top__ < 5) {
      warpPerspective(*src, *dst, *M, *dsize);
    } else {
      int flags = luaL_checkint(L, 5);
      if (top__ < 6) {
        warpPerspective(*src, *dst, *M, *dsize, flags);
      } else {
        int borderMode = luaL_checkint(L, 6);
        if (top__ < 7) {
          warpPerspective(*src, *dst, *M, *dsize, flags, borderMode);
        } else {
          const Scalar *borderValue = *((const Scalar **)luaL_checkudata(L, 7, "cv.Scalar"));
          warpPerspective(*src, *dst, *M, *dsize, flags, borderMode, *borderValue);
        }
      }
    }
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.warpPerspective: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.warpPerspective: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.warpPerspective: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** CV_EXPORTS void cv::watershed(const Mat &image, Mat &markers)
 * include/opencv/cv.hpp:457
 */
static int cv_watershed(lua_State *L) {
  try {
    const Mat *image = *((const Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Mat *markers = *((Mat **)luaL_checkudata(L, 2, "cv.Mat"));
    watershed(*image, *markers);
    return 0;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.watershed: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.watershed: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.watershed: Unknown exception");
    lua_error(L);
    return 0;
  }
}

// #include "draw.h"
// /** cv::Mat cv::LoadImage(const char *path)
//  * rubyk/support/draw.h:141
//  */
// static int cv_LoadImage(lua_State *L) {
//   try {
//     const char *path = luaL_checkstring(L, 1);
//     cv::Mat  retval__ = LoadImage(path);
//     lua_pushclass<cv::Mat>(L, retval__, "cv.Mat");
//     return 1;
//   } catch (std::exception &e) {
//     std::string *s = new std::string("cv.LoadImage: ");
//     s->append(e.what());
//     lua_pushstring(L, s->c_str());
//     delete s;
//     lua_error(L);
//     // never reached
//     return 0;
//   } catch (...) {
//     lua_pushstring(L, "cv.LoadImage: Unknown exception");
//     lua_error(L);
//     return 0;
//   }
// }
// 
// Register namespace


static const struct luaL_Reg cv_functions[] = {
  {"CamShift"                      , cv_CamShift},
  {"Canny"                         , cv_Canny},
  {"GaussianBlur"                  , cv_GaussianBlur},
  {"LUT"                           , cv_LUT},
//  {"LoadImage"                     , cv_LoadImage},
  {"Laplacian"                     , cv_Laplacian},
  {"Mahalanobis"                   , cv_Mahalanobis},
  {"Mahalonobis"                   , cv_Mahalonobis},
  {"RQDecomp3x3"                   , cv_RQDecomp3x3},
  {"Rodrigues"                     , cv_Rodrigues},
  {"Scharr"                        , cv_Scharr},
  {"Sobel"                         , cv_Sobel},
  {"absdiff"                       , cv_absdiff},
  {"accumulate"                    , cv_accumulate},
  {"accumulateProduct"             , cv_accumulateProduct},
  {"accumulateSquare"              , cv_accumulateSquare},
  {"accumulateWeighted"            , cv_accumulateWeighted},
  {"adaptiveThreshold"             , cv_adaptiveThreshold},
  {"add"                           , cv_add},
  {"addWeighted"                   , cv_addWeighted},
  {"alignSize"                     , cv_alignSize},
  {"arcLength"                     , cv_arcLength},
  {"bilateralFilter"               , cv_bilateralFilter},
  {"bitwise_and"                   , cv_bitwise_and},
  {"bitwise_not"                   , cv_bitwise_not},
  {"bitwise_or"                    , cv_bitwise_or},
  {"bitwise_xor"                   , cv_bitwise_xor},
  {"blur"                          , cv_blur},
  {"borderInterpolate"             , cv_borderInterpolate},
  {"boundingRect"                  , cv_boundingRect},
  {"boxFilter"                     , cv_boxFilter},
  {"calcCovarMatrix"               , cv_calcCovarMatrix},
  {"calcGlobalOrientation"         , cv_calcGlobalOrientation},
  {"calcMotionGradient"            , cv_calcMotionGradient},
  {"calcOpticalFlowFarneback"      , cv_calcOpticalFlowFarneback},
  {"calibrationMatrixValues"       , cv_calibrationMatrixValues},
  {"cartToPolar"                   , cv_cartToPolar},
  {"checkRange"                    , cv_checkRange},
  {"circle"                        , cv_circle},
  {"clipLine"                      , cv_clipLine},
  {"compare"                       , cv_compare},
  {"compareHist"                   , cv_compareHist},
  {"completeSymm"                  , cv_completeSymm},
  {"composeRT"                     , cv_composeRT},
  {"contourArea"                   , cv_contourArea},
  {"convertMaps"                   , cv_convertMaps},
  {"convertScaleAbs"               , cv_convertScaleAbs},
  {"copyMakeBorder"                , cv_copyMakeBorder},
  {"cornerEigenValsAndVecs"        , cv_cornerEigenValsAndVecs},
  {"cornerHarris"                  , cv_cornerHarris},
  {"cornerMinEigenVal"             , cv_cornerMinEigenVal},
  {"countNonZero"                  , cv_countNonZero},
  {"cubeRoot"                      , cv_cubeRoot},
  {"cvtColor"                      , cv_cvtColor},
  {"dct"                           , cv_dct},
  {"decomposeProjectionMatrix"     , cv_decomposeProjectionMatrix},
  {"determinant"                   , cv_determinant},
  {"dft"                           , cv_dft},
  {"dilate"                        , cv_dilate},
  {"distanceTransform"             , cv_distanceTransform},
  {"divide"                        , cv_divide},
  {"drawChessboardCorners"         , cv_drawChessboardCorners},
  {"eigen"                         , cv_eigen},
  {"ellipse"                       , cv_ellipse},
  {"equalizeHist"                  , cv_equalizeHist},
  {"erode"                         , cv_erode},
  {"error"                         , cv_error},
  {"estimateRigidTransform"        , cv_estimateRigidTransform},
  {"exp"                           , cv_exp},
  {"fastAtan2"                     , cv_fastAtan2},
  {"filter2D"                      , cv_filter2D},
  {"filterSpeckles"                , cv_filterSpeckles},
  {"findFundamentalMat"            , cv_findFundamentalMat},
  {"fitEllipse"                    , cv_fitEllipse},
  {"fitLine"                       , cv_fitLine},
  {"flip"                          , cv_flip},
  {"fromUtf16"                     , cv_fromUtf16},
  {"gemm"                          , cv_gemm},
  {"getCPUTickCount"               , cv_getCPUTickCount},
  {"getDefaultNewCameraMatrix"     , cv_getDefaultNewCameraMatrix},
  {"getDerivKernels"               , cv_getDerivKernels},
  {"getElemSize"                   , cv_getElemSize},
  {"getGaussianKernel"             , cv_getGaussianKernel},
  {"getKernelType"                 , cv_getKernelType},
  {"getNumThreads"                 , cv_getNumThreads},
  {"getOptimalDFTSize"             , cv_getOptimalDFTSize},
  {"getRectSubPix"                 , cv_getRectSubPix},
  {"getRotationMatrix2D"           , cv_getRotationMatrix2D},
  {"getStructuringElement"         , cv_getStructuringElement},
  {"getTextSize"                   , cv_getTextSize},
  {"getThreadNum"                  , cv_getThreadNum},
  {"getTickCount"                  , cv_getTickCount},
  {"getTickFrequency"              , cv_getTickFrequency},
  {"grabCut"                       , cv_grabCut},
  {"idct"                          , cv_idct},
  {"idft"                          , cv_idft},
  {"inRange"                       , cv_inRange},
  {"initUndistortRectifyMap"       , cv_initUndistortRectifyMap},
  {"inpaint"                       , cv_inpaint},
  {"integral"                      , cv_integral},
  {"invert"                        , cv_invert},
  {"invertAffineTransform"         , cv_invertAffineTransform},
  {"isContourConvex"               , cv_isContourConvex},
  {"line"                          , cv_line},
  {"log"                           , cv_log},
  {"magnitude"                     , cv_magnitude},
  {"matMulDeriv"                   , cv_matMulDeriv},
  {"matchShapes"                   , cv_matchShapes},
  {"matchTemplate"                 , cv_matchTemplate},
  {"max"                           , cv_max},
  {"mean"                          , cv_mean},
  {"meanShift"                     , cv_meanShift},
  {"meanStdDev"                    , cv_meanStdDev},
  {"medianBlur"                    , cv_medianBlur},
  {"min"                           , cv_min},
  {"minAreaRect"                   , cv_minAreaRect},
  {"minEnclosingCircle"            , cv_minEnclosingCircle},
  {"minMaxLoc"                     , cv_minMaxLoc},
  {"moments"                       , cv_moments},
  {"morphologyDefaultBorderValue"  , cv_morphologyDefaultBorderValue},
  {"morphologyEx"                  , cv_morphologyEx},
  {"mulSpectrums"                  , cv_mulSpectrums},
  {"mulTransposed"                 , cv_mulTransposed},
  {"multiply"                      , cv_multiply},
  {"norm"                          , cv_norm},
  {"normalize"                     , cv_normalize},
  {"perspectiveTransform"          , cv_perspectiveTransform},
  {"phase"                         , cv_phase},
  {"pointPolygonTest"              , cv_pointPolygonTest},
  {"polarToCart"                   , cv_polarToCart},
  {"pow"                           , cv_pow},
  {"preCornerDetect"               , cv_preCornerDetect},
  {"putText"                       , cv_putText},
  {"pyrDown"                       , cv_pyrDown},
  {"pyrUp"                         , cv_pyrUp},
  {"randn"                         , cv_randn},
  {"randu"                         , cv_randu},
  {"rectangle"                     , cv_rectangle},
  {"reduce"                        , cv_reduce},
  {"remap"                         , cv_remap},
  {"repeat"                        , cv_repeat},
  {"reprojectImageTo3D"            , cv_reprojectImageTo3D},
  {"resize"                        , cv_resize},
  {"scaleAdd"                      , cv_scaleAdd},
  {"sepFilter2D"                   , cv_sepFilter2D},
  {"setBreakOnError"               , cv_setBreakOnError},
  {"setIdentity"                   , cv_setIdentity},
  {"setNumThreads"                 , cv_setNumThreads},
  {"setUseOptimized"               , cv_setUseOptimized},
  {"solve"                         , cv_solve},
  {"solveCubic"                    , cv_solveCubic},
  {"solvePnP"                      , cv_solvePnP},
  {"solvePoly"                     , cv_solvePoly},
  {"sort"                          , cv_sort},
  {"sortIdx"                       , cv_sortIdx},
  {"sqrt"                          , cv_sqrt},
  {"stereoRectify"                 , cv_stereoRectify},
  {"stereoRectifyUncalibrated"     , cv_stereoRectifyUncalibrated},
  {"subtract"                      , cv_subtract},
  {"sum"                           , cv_sum},
  {"theRNG"                        , cv_theRNG},
  {"threshold"                     , cv_threshold},
  {"toUtf16"                       , cv_toUtf16},
  {"trace"                         , cv_trace},
  {"transform"                     , cv_transform},
  {"transpose"                     , cv_transpose},
  {"undistort"                     , cv_undistort},
  {"undistortPoints"               , cv_undistortPoints},
  {"updateMotionHistory"           , cv_updateMotionHistory},
  {"useOptimized"                  , cv_useOptimized},
  {"warpAffine"                    , cv_warpAffine},
  {"warpPerspective"               , cv_warpPerspective},
  {"watershed"                     , cv_watershed},
  {NULL, NULL},
};


static const struct lua_constants_Reg cv_namespace_constants[] = {
  {"BORDER_REPLICATE"              , cv::BORDER_REPLICATE},
  {"BORDER_CONSTANT"               , cv::BORDER_CONSTANT},
  {"BORDER_REFLECT"                , cv::BORDER_REFLECT},
  {"BORDER_REFLECT_101"            , cv::BORDER_REFLECT_101},
  {"BORDER_REFLECT101"             , cv::BORDER_REFLECT101},
  {"BORDER_WRAP"                   , cv::BORDER_WRAP},
  {"BORDER_TRANSPARENT"            , cv::BORDER_TRANSPARENT},
  {"BORDER_DEFAULT"                , cv::BORDER_DEFAULT},
  {"BORDER_ISOLATED"               , cv::BORDER_ISOLATED},
  {"KERNEL_GENERAL"                , cv::KERNEL_GENERAL},
  {"KERNEL_SYMMETRICAL"            , cv::KERNEL_SYMMETRICAL},
  {"KERNEL_ASYMMETRICAL"           , cv::KERNEL_ASYMMETRICAL},
  {"KERNEL_SMOOTH"                 , cv::KERNEL_SMOOTH},
  {"KERNEL_INTEGER"                , cv::KERNEL_INTEGER},
  {"MORPH_ERODE"                   , cv::MORPH_ERODE},
  {"MORPH_DILATE"                  , cv::MORPH_DILATE},
  {"MORPH_OPEN"                    , cv::MORPH_OPEN},
  {"MORPH_CLOSE"                   , cv::MORPH_CLOSE},
  {"MORPH_GRADIENT"                , cv::MORPH_GRADIENT},
  {"MORPH_TOPHAT"                  , cv::MORPH_TOPHAT},
  {"MORPH_BLACKHAT"                , cv::MORPH_BLACKHAT},
  {"MORPH_RECT"                    , cv::MORPH_RECT},
  {"MORPH_CROSS"                   , cv::MORPH_CROSS},
  {"MORPH_ELLIPSE"                 , cv::MORPH_ELLIPSE},
  {"INTER_NEAREST"                 , cv::INTER_NEAREST},
  {"INTER_LINEAR"                  , cv::INTER_LINEAR},
  {"INTER_CUBIC"                   , cv::INTER_CUBIC},
  {"INTER_AREA"                    , cv::INTER_AREA},
  {"INTER_LANCZOS4"                , cv::INTER_LANCZOS4},
  {"INTER_MAX"                     , cv::INTER_MAX},
  {"WARP_INVERSE_MAP"              , cv::WARP_INVERSE_MAP},
  {"THRESH_BINARY"                 , cv::THRESH_BINARY},
  {"THRESH_BINARY_INV"             , cv::THRESH_BINARY_INV},
  {"THRESH_TRUNC"                  , cv::THRESH_TRUNC},
  {"THRESH_TOZERO"                 , cv::THRESH_TOZERO},
  {"THRESH_TOZERO_INV"             , cv::THRESH_TOZERO_INV},
  {"THRESH_MASK"                   , cv::THRESH_MASK},
  {"THRESH_OTSU"                   , cv::THRESH_OTSU},
  {"ADAPTIVE_THRESH_MEAN_C"        , cv::ADAPTIVE_THRESH_MEAN_C},
  {"ADAPTIVE_THRESH_GAUSSIAN_C"    , cv::ADAPTIVE_THRESH_GAUSSIAN_C},
  {"OPTFLOW_USE_INITIAL_FLOW"      , cv::OPTFLOW_USE_INITIAL_FLOW},
  {"OPTFLOW_FARNEBACK_GAUSSIAN"    , cv::OPTFLOW_FARNEBACK_GAUSSIAN},
  {"GC_BGD"                        , cv::GC_BGD},
  {"GC_FGD"                        , cv::GC_FGD},
  {"GC_PR_BGD"                     , cv::GC_PR_BGD},
  {"GC_PR_FGD"                     , cv::GC_PR_FGD},
  {"GC_INIT_WITH_RECT"             , cv::GC_INIT_WITH_RECT},
  {"GC_INIT_WITH_MASK"             , cv::GC_INIT_WITH_MASK},
  {"GC_EVAL"                       , cv::GC_EVAL},
  {"INPAINT_NS"                    , cv::INPAINT_NS},
  {"INPAINT_TELEA"                 , cv::INPAINT_TELEA},
  {"FLOODFILL_FIXED_RANGE"         , cv::FLOODFILL_FIXED_RANGE},
  {"FLOODFILL_MASK_ONLY"           , cv::FLOODFILL_MASK_ONLY},
  {"TM_SQDIFF"                     , cv::TM_SQDIFF},
  {"TM_SQDIFF_NORMED"              , cv::TM_SQDIFF_NORMED},
  {"TM_CCORR"                      , cv::TM_CCORR},
  {"TM_CCORR_NORMED"               , cv::TM_CCORR_NORMED},
  {"TM_CCOEFF"                     , cv::TM_CCOEFF},
  {"TM_CCOEFF_NORMED"              , cv::TM_CCOEFF_NORMED},
  {"RETR_EXTERNAL"                 , cv::RETR_EXTERNAL},
  {"RETR_LIST"                     , cv::RETR_LIST},
  {"RETR_CCOMP"                    , cv::RETR_CCOMP},
  {"RETR_TREE"                     , cv::RETR_TREE},
  {"CHAIN_APPROX_NONE"             , cv::CHAIN_APPROX_NONE},
  {"CHAIN_APPROX_SIMPLE"           , cv::CHAIN_APPROX_SIMPLE},
  {"CHAIN_APPROX_TC89_L1"          , cv::CHAIN_APPROX_TC89_L1},
  {"CHAIN_APPROX_TC89_KCOS"        , cv::CHAIN_APPROX_TC89_KCOS},
  {"LMEDS"                         , cv::LMEDS},
  {"RANSAC"                        , cv::RANSAC},
  {"CALIB_CB_ADAPTIVE_THRESH"      , cv::CALIB_CB_ADAPTIVE_THRESH},
  {"CALIB_CB_NORMALIZE_IMAGE"      , cv::CALIB_CB_NORMALIZE_IMAGE},
  {"CALIB_CB_FILTER_QUADS"         , cv::CALIB_CB_FILTER_QUADS},
  {"CALIB_USE_INTRINSIC_GUESS"     , cv::CALIB_USE_INTRINSIC_GUESS},
  {"CALIB_FIX_ASPECT_RATIO"        , cv::CALIB_FIX_ASPECT_RATIO},
  {"CALIB_FIX_PRINCIPAL_POINT"     , cv::CALIB_FIX_PRINCIPAL_POINT},
  {"CALIB_ZERO_TANGENT_DIST"       , cv::CALIB_ZERO_TANGENT_DIST},
  {"CALIB_FIX_FOCAL_LENGTH"        , cv::CALIB_FIX_FOCAL_LENGTH},
  {"CALIB_FIX_K1"                  , cv::CALIB_FIX_K1},
  {"CALIB_FIX_K2"                  , cv::CALIB_FIX_K2},
  {"CALIB_FIX_K3"                  , cv::CALIB_FIX_K3},
  {"CALIB_FIX_INTRINSIC"           , cv::CALIB_FIX_INTRINSIC},
  {"CALIB_SAME_FOCAL_LENGTH"       , cv::CALIB_SAME_FOCAL_LENGTH},
  {"CALIB_ZERO_DISPARITY"          , cv::CALIB_ZERO_DISPARITY},
  {"FM_7POINT"                     , cv::FM_7POINT},
  {"FM_8POINT"                     , cv::FM_8POINT},
  {"FM_LMEDS"                      , cv::FM_LMEDS},
  {"FM_RANSAC"                     , cv::FM_RANSAC},
  {"MAGIC_MASK"                    , cv::MAGIC_MASK},
  {"TYPE_MASK"                     , cv::TYPE_MASK},
  {"DEPTH_MASK"                    , cv::DEPTH_MASK},
  {"DECOMP_LU"                     , cv::DECOMP_LU},
  {"DECOMP_SVD"                    , cv::DECOMP_SVD},
  {"DECOMP_EIG"                    , cv::DECOMP_EIG},
  {"DECOMP_CHOLESKY"               , cv::DECOMP_CHOLESKY},
  {"DECOMP_QR"                     , cv::DECOMP_QR},
  {"DECOMP_NORMAL"                 , cv::DECOMP_NORMAL},
  {"NORM_INF"                      , cv::NORM_INF},
  {"NORM_L1"                       , cv::NORM_L1},
  {"NORM_L2"                       , cv::NORM_L2},
  {"NORM_TYPE_MASK"                , cv::NORM_TYPE_MASK},
  {"NORM_RELATIVE"                 , cv::NORM_RELATIVE},
  {"NORM_MINMAX"                   , cv::NORM_MINMAX},
  {"CMP_EQ"                        , cv::CMP_EQ},
  {"CMP_GT"                        , cv::CMP_GT},
  {"CMP_GE"                        , cv::CMP_GE},
  {"CMP_LT"                        , cv::CMP_LT},
  {"CMP_LE"                        , cv::CMP_LE},
  {"CMP_NE"                        , cv::CMP_NE},
  {"GEMM_1_T"                      , cv::GEMM_1_T},
  {"GEMM_2_T"                      , cv::GEMM_2_T},
  {"GEMM_3_T"                      , cv::GEMM_3_T},
  {"DFT_INVERSE"                   , cv::DFT_INVERSE},
  {"DFT_SCALE"                     , cv::DFT_SCALE},
  {"DFT_ROWS"                      , cv::DFT_ROWS},
  {"DFT_COMPLEX_OUTPUT"            , cv::DFT_COMPLEX_OUTPUT},
  {"DFT_REAL_OUTPUT"               , cv::DFT_REAL_OUTPUT},
  {"DCT_INVERSE"                   , cv::DCT_INVERSE},
  {"DCT_ROWS"                      , cv::DCT_ROWS},
  {"KMEANS_RANDOM_CENTERS"         , cv::KMEANS_RANDOM_CENTERS},
  {"KMEANS_PP_CENTERS"             , cv::KMEANS_PP_CENTERS},
  {"KMEANS_USE_INITIAL_LABELS"     , cv::KMEANS_USE_INITIAL_LABELS},
  {"FONT_HERSHEY_SIMPLEX"          , cv::FONT_HERSHEY_SIMPLEX},
  {"FONT_HERSHEY_PLAIN"            , cv::FONT_HERSHEY_PLAIN},
  {"FONT_HERSHEY_DUPLEX"           , cv::FONT_HERSHEY_DUPLEX},
  {"FONT_HERSHEY_COMPLEX"          , cv::FONT_HERSHEY_COMPLEX},
  {"FONT_HERSHEY_TRIPLEX"          , cv::FONT_HERSHEY_TRIPLEX},
  {"FONT_HERSHEY_COMPLEX_SMALL"    , cv::FONT_HERSHEY_COMPLEX_SMALL},
  {"FONT_HERSHEY_SCRIPT_SIMPLEX"   , cv::FONT_HERSHEY_SCRIPT_SIMPLEX},
  {"FONT_HERSHEY_SCRIPT_COMPLEX"   , cv::FONT_HERSHEY_SCRIPT_COMPLEX},
  {"FONT_ITALIC"                   , cv::FONT_ITALIC},

  {"CV_8UC1"                       , CV_8UC1},
  {"CV_8UC2"                       , CV_8UC2},
  {"CV_8UC3"                       , CV_8UC3},
  {"CV_8UC4"                       , CV_8UC4},
  {"CV_8SC1"                       , CV_8SC1},
  {"CV_8SC2"                       , CV_8SC2},
  {"CV_8SC3"                       , CV_8SC3},
  {"CV_8SC4"                       , CV_8SC4},
  {"CV_16UC1"                      , CV_16UC1},
  {"CV_16UC2"                      , CV_16UC2},
  {"CV_16UC3"                      , CV_16UC3},
  {"CV_16UC4"                      , CV_16UC4},
  {"CV_16SC1"                      , CV_16SC1},
  {"CV_16SC2"                      , CV_16SC2},
  {"CV_16SC3"                      , CV_16SC3},
  {"CV_16SC4"                      , CV_16SC4},
  {"CV_32SC1"                      , CV_32SC1},
  {"CV_32SC2"                      , CV_32SC2},
  {"CV_32SC3"                      , CV_32SC3},
  {"CV_32SC4"                      , CV_32SC4},
  {"CV_32FC1"                      , CV_32FC1},
  {"CV_32FC2"                      , CV_32FC2},
  {"CV_32FC3"                      , CV_32FC3},
  {"CV_32FC4"                      , CV_32FC4},
  {"CV_64FC1"                      , CV_64FC1},
  {"CV_64FC2"                      , CV_64FC2},
  {"CV_64FC3"                      , CV_64FC3},
  {"CV_64FC4"                      , CV_64FC4},
  {"CV_BGR2BGRA"                   , CV_BGR2BGRA},
  {"CV_RGB2RGBA"                   , CV_RGB2RGBA},
  {"CV_BGRA2BGR"                   , CV_BGRA2BGR},
  {"CV_RGBA2RGB"                   , CV_RGBA2RGB},
  {"CV_BGR2RGBA"                   , CV_BGR2RGBA},
  {"CV_RGB2BGRA"                   , CV_RGB2BGRA},
  {"CV_RGBA2BGR"                   , CV_RGBA2BGR},
  {"CV_BGRA2RGB"                   , CV_BGRA2RGB},
  {"CV_BGR2RGB"                    , CV_BGR2RGB},
  {"CV_RGB2BGR"                    , CV_RGB2BGR},
  {"CV_BGRA2RGBA"                  , CV_BGRA2RGBA},
  {"CV_RGBA2BGRA"                  , CV_RGBA2BGRA},
  {"CV_BGR2GRAY"                   , CV_BGR2GRAY},
  {"CV_RGB2GRAY"                   , CV_RGB2GRAY},
  {"CV_GRAY2BGR"                   , CV_GRAY2BGR},
  {"CV_GRAY2RGB"                   , CV_GRAY2RGB},
  {"CV_GRAY2BGRA"                  , CV_GRAY2BGRA},
  {"CV_GRAY2RGBA"                  , CV_GRAY2RGBA},
  {"CV_BGRA2GRAY"                  , CV_BGRA2GRAY},
  {"CV_RGBA2GRAY"                  , CV_RGBA2GRAY},
  {"CV_BGR2BGR565"                 , CV_BGR2BGR565},
  {"CV_RGB2BGR565"                 , CV_RGB2BGR565},
  {"CV_BGR5652BGR"                 , CV_BGR5652BGR},
  {"CV_BGR5652RGB"                 , CV_BGR5652RGB},
  {"CV_BGRA2BGR565"                , CV_BGRA2BGR565},
  {"CV_RGBA2BGR565"                , CV_RGBA2BGR565},
  {"CV_BGR5652BGRA"                , CV_BGR5652BGRA},
  {"CV_BGR5652RGBA"                , CV_BGR5652RGBA},
  {"CV_GRAY2BGR565"                , CV_GRAY2BGR565},
  {"CV_BGR5652GRAY"                , CV_BGR5652GRAY},
  {"CV_BGR2BGR555"                 , CV_BGR2BGR555},
  {"CV_RGB2BGR555"                 , CV_RGB2BGR555},
  {"CV_BGR5552BGR"                 , CV_BGR5552BGR},
  {"CV_BGR5552RGB"                 , CV_BGR5552RGB},
  {"CV_BGRA2BGR555"                , CV_BGRA2BGR555},
  {"CV_RGBA2BGR555"                , CV_RGBA2BGR555},
  {"CV_BGR5552BGRA"                , CV_BGR5552BGRA},
  {"CV_BGR5552RGBA"                , CV_BGR5552RGBA},
  {"CV_GRAY2BGR555"                , CV_GRAY2BGR555},
  {"CV_BGR5552GRAY"                , CV_BGR5552GRAY},
  {"CV_BGR2XYZ"                    , CV_BGR2XYZ},
  {"CV_RGB2XYZ"                    , CV_RGB2XYZ},
  {"CV_XYZ2BGR"                    , CV_XYZ2BGR},
  {"CV_XYZ2RGB"                    , CV_XYZ2RGB},
  {"CV_BGR2YCrCb"                  , CV_BGR2YCrCb},
  {"CV_RGB2YCrCb"                  , CV_RGB2YCrCb},
  {"CV_YCrCb2BGR"                  , CV_YCrCb2BGR},
  {"CV_YCrCb2RGB"                  , CV_YCrCb2RGB},
  {"CV_BGR2HSV"                    , CV_BGR2HSV},
  {"CV_RGB2HSV"                    , CV_RGB2HSV},
  {"CV_BGR2Lab"                    , CV_BGR2Lab},
  {"CV_RGB2Lab"                    , CV_RGB2Lab},
  {"CV_BayerBG2BGR"                , CV_BayerBG2BGR},
  {"CV_BayerGB2BGR"                , CV_BayerGB2BGR},
  {"CV_BayerRG2BGR"                , CV_BayerRG2BGR},
  {"CV_BayerGR2BGR"                , CV_BayerGR2BGR},
  {"CV_BayerBG2RGB"                , CV_BayerBG2RGB},
  {"CV_BayerGB2RGB"                , CV_BayerGB2RGB},
  {"CV_BayerRG2RGB"                , CV_BayerRG2RGB},
  {"CV_BayerGR2RGB"                , CV_BayerGR2RGB},
  {"CV_BGR2Luv"                    , CV_BGR2Luv},
  {"CV_RGB2Luv"                    , CV_RGB2Luv},
  {"CV_BGR2HLS"                    , CV_BGR2HLS},
  {"CV_RGB2HLS"                    , CV_RGB2HLS},
  {"CV_HSV2BGR"                    , CV_HSV2BGR},
  {"CV_HSV2RGB"                    , CV_HSV2RGB},
  {"CV_Lab2BGR"                    , CV_Lab2BGR},
  {"CV_Lab2RGB"                    , CV_Lab2RGB},
  {"CV_Luv2BGR"                    , CV_Luv2BGR},
  {"CV_Luv2RGB"                    , CV_Luv2RGB},
  {"CV_HLS2BGR"                    , CV_HLS2BGR},
  {"CV_HLS2RGB"                    , CV_HLS2RGB},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_cv(lua_State *L) {
#else
extern "C" int luaopen_cv(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "cv", cv_functions);

  // register namespace enums
  register_constants(L, "cv", cv_namespace_constants);

  return 0;
}
