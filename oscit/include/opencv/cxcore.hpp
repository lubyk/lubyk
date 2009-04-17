/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef _CXCORE_HPP_
#define _CXCORE_HPP_

#include "cxmisc.h"

#ifdef __cplusplus

#ifndef SKIP_INCLUDES
#include <algorithm>
#include <complex>
#include <map>
#include <new>
#include <string>
#include <vector>
#endif // SKIP_INCLUDES

namespace cv {

template<typename T> struct CV_EXPORTS Size_;
template<typename T> struct CV_EXPORTS Point_;
template<typename T> struct CV_EXPORTS Rect_;

typedef std::string String;

struct CV_EXPORTS Exception
{
    Exception() { code = 0; line = 0; }
    Exception(int _code, const String& _err, const String& _func, const String& _file, int _line)
        : code(_code), err(_err), func(_func), file(_file), line(_line) {}
    Exception(const Exception& exc)
        : code(exc.code), err(exc.err), func(exc.func), file(exc.file), line(exc.line) {}
    Exception& operator = (const Exception& exc)
    {
        if( this != &exc )
        {
            code = exc.code; err = exc.err; func = exc.func; file = exc.file; line = exc.line;
        }
        return *this;
    }

    int code;
    String err;
    String func;
    String file;
    int line;
};

CV_EXPORTS String format( const char* fmt, ... );
CV_EXPORTS void error( const Exception& exc );

#ifdef __GNUC__
#define CV_Error( code, msg ) cv::error( cv::Exception(code, msg, __func__, __FILE__, __LINE__) )
#define CV_Error_( code, args ) cv::error( cv::Exception(code, cv::format args, __func__, __FILE__, __LINE__) )
#define CV_Assert( expr ) { if(!(expr)) cv::error( cv::Exception(CV_StsAssert, #expr, __func__, __FILE__, __LINE__) ); }
#else
#define CV_Error( code, msg ) cv::error( cv::Exception(code, msg, "", __FILE__, __LINE__) )
#define CV_Error_( code, args ) cv::error( cv::Exception(code, cv::format args, "", __FILE__, __LINE__) )
#define CV_Assert( expr ) { if(!(expr)) cv::error( cv::Exception(CV_StsAssert, #expr, "", __FILE__, __LINE__) ); }
#endif

CV_EXPORTS void setNumThreads(int);
CV_EXPORTS int getNumThreads();
CV_EXPORTS int getThreadNum();

CV_EXPORTS int64 getTickCount();
CV_EXPORTS double getTickFrequency();

CV_EXPORTS void* fastMalloc(size_t);
CV_EXPORTS void fastFree(void* ptr);

template<typename T> static inline T* fastMalloc_(size_t n)
{
    T* ptr = (T*)fastMalloc(n*sizeof(ptr[0]));
    ::new(ptr) T[n];
    return ptr;
}

template<typename T> static inline void fastFree_(T* ptr, size_t n)
{
    for( size_t i = 0; i < n; i++ ) (ptr+i)->~T();
    fastFree(ptr);
}

template<typename T> static inline T* alignPtr(T* ptr, int n=(int)sizeof(T))
{
    return (T*)(((size_t)ptr + n-1) & -n);
}

static inline size_t alignSize(size_t sz, int n)
{
    return (sz + n-1) & -n;
}

CV_EXPORTS void setUseOptimized(bool);
CV_EXPORTS bool useOptimized();

template<typename T> class CV_EXPORTS Allocator
{
public: 
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template<typename U> struct rebind { typedef Allocator<U> other; };

public : 
    explicit Allocator() {}
    ~Allocator() {}
    explicit Allocator(Allocator const&) {}
    template<typename U>
    explicit Allocator(Allocator<U> const&) {}

    // address
    pointer address(reference r) { return &r; }
    const_pointer address(const_reference r) { return &r; }

    pointer allocate(size_type count, const void* =0)
    { return reinterpret_cast<pointer>(fastMalloc(count * sizeof (T))); }

    void deallocate(pointer p, size_type) {fastFree(p); }

    size_type max_size() const
    { return max(static_cast<T>(-1)/sizeof(T), 1); }

    void construct(pointer p, const T& v) { new(static_cast<void*>(p)) T(v); }
    void destroy(pointer p) { p->~T(); }
};

/////////////////////// Vec_ (used as element of multi-channel images ///////////////////// 

template<typename T> struct CV_EXPORTS DataDepth { enum { value = -1, fmt=(int)'\0' }; };

template<> struct DataDepth<uchar> { enum { value = CV_8U, fmt=(int)'u' }; };
template<> struct DataDepth<schar> { enum { value = CV_8S, fmt=(int)'c' }; };
template<> struct DataDepth<ushort> { enum { value = CV_16U, fmt=(int)'w' }; };
template<> struct DataDepth<short> { enum { value = CV_16S, fmt=(int)'s' }; };
template<> struct DataDepth<int> { enum { value = CV_32S, fmt=(int)'i' }; };
template<> struct DataDepth<float> { enum { value = CV_32F, fmt=(int)'f' }; };
template<> struct DataDepth<double> { enum { value = CV_64F, fmt=(int)'d' }; };
template<typename T> struct DataDepth<T*> { enum { value = CV_USRTYPE1, fmt=(int)'r' }; };

template<typename T, int cn> struct CV_EXPORTS Vec_
{
    typedef T value_type;
    enum { depth = DataDepth<T>::value, channels = cn, type = CV_MAKETYPE(depth, channels) };
    
    Vec_();
    Vec_(T v0);
    Vec_(T v0, T v1);
    Vec_(T v0, T v1, T v2);
    Vec_(T v0, T v1, T v2, T v3);
    Vec_(const Vec_<T, cn>& v);
    static Vec_ all(T alpha);
    T dot(const Vec_& v) const;
    double ddot(const Vec_& v) const;
    Vec_ cross(const Vec_& v) const;
    template<typename T2> operator Vec_<T2, cn>() const;
    operator CvScalar() const;
    T operator [](int i) const;
    T& operator[](int i);

    T val[cn];
};

typedef Vec_<uchar, 2> Vec2b;
typedef Vec_<uchar, 3> Vec3b;
typedef Vec_<uchar, 4> Vec4b;

typedef Vec_<short, 2> Vec2s;
typedef Vec_<short, 3> Vec3s;
typedef Vec_<short, 4> Vec4s;

typedef Vec_<int, 2> Vec2i;
typedef Vec_<int, 3> Vec3i;
typedef Vec_<int, 4> Vec4i;

typedef Vec_<float, 2> Vec2f;
typedef Vec_<float, 3> Vec3f;
typedef Vec_<float, 4> Vec4f;

typedef Vec_<double, 2> Vec2d;
typedef Vec_<double, 3> Vec3d;
typedef Vec_<double, 4> Vec4d;

//////////////////////////////// Complex //////////////////////////////

template<typename T> struct CV_EXPORTS Complex
{
    Complex();
    Complex( T _re, T _im=0 );
    template<typename T2> operator Complex<T2>() const;
    Complex conj() const;

    T re, im;
};

typedef Complex<float> Complexf;
typedef Complex<double> Complexd;

//////////////////////////////// Point_ ////////////////////////////////

template<typename T> struct CV_EXPORTS Point_
{
    typedef T value_type;
    
    Point_();
    Point_(T _x, T _y);
    Point_(const Point_& pt);
    Point_(const CvPoint& pt);
    Point_(const CvPoint2D32f& pt);
    Point_(const Size_<T>& sz);
    Point_& operator = (const Point_& pt);
    operator Point_<int>() const;
    operator Point_<float>() const;
    operator Point_<double>() const;
    operator CvPoint() const;
    operator CvPoint2D32f() const;

    T dot(const Point_& pt) const;
    double ddot(const Point_& pt) const;
    bool inside(const Rect_<T>& r) const;
    
    T x, y;
};

template<typename T> struct CV_EXPORTS Point3_
{
    typedef T value_type;
    
    Point3_();
    Point3_(T _x, T _y, T _z);
    Point3_(const Point3_& pt);
    Point3_(const CvPoint3D32f& pt);
    Point3_(const Vec_<T, 3>& t);
    Point3_& operator = (const Point3_& pt);
    Point3_& operator += (const Point3_& pt);
    Point3_& operator -= (const Point3_& pt);
    operator Point3_<int>() const;
    operator Point3_<float>() const;
    operator Point3_<double>() const;
    operator CvPoint3D32f() const;

    T dot(const Point3_& pt) const;
    double ddot(const Point3_& pt) const;
    
    T x, y, z;
};

//////////////////////////////// Size_ ////////////////////////////////

template<typename T> struct CV_EXPORTS Size_
{
    typedef T value_type;
    
    Size_();
    Size_(T _width, T _height);
    Size_(const Size_& sz);
    Size_(const CvSize& sz);
    Size_(const Point_<T>& pt);
    Size_& operator = (const Size_& sz);
    T area() const;

    operator Size_<int>() const;
    operator Size_<float>() const;
    operator Size_<double>() const;
    operator CvSize() const;

    T width, height;
};

//////////////////////////////// Rect_ ////////////////////////////////

template<typename T> struct CV_EXPORTS Rect_
{
    typedef T value_type;
    
    Rect_();
    Rect_(T _x, T _y, T _width, T _height);
    Rect_(const Rect_& r);
    Rect_(const CvRect& r);
    Rect_(const Point_<T>& org, const Size_<T>& sz);
    Rect_(const Point_<T>& pt1, const Point_<T>& pt2);
    Rect_& operator = ( const Rect_& r );
    Point_<T> tl() const;
    Point_<T> br() const;
    
    Size_<T> size() const;
    T area() const;

    operator Rect_<int>() const;
    operator Rect_<float>() const;
    operator Rect_<double>() const;
    operator CvRect() const;

    bool contains(const Point_<T>& pt) const;

    T x, y, width, height;
};

typedef Point_<int> Point2i;
typedef Point2i Point;
typedef Size_<int> Size2i;
typedef Size2i Size;
typedef Rect_<int> Rect;
typedef Point_<float> Point2f;
typedef Point_<double> Point2d;
typedef Size_<float> Size2f;
typedef Point3_<int> Point3i;
typedef Point3_<float> Point3f;
typedef Point3_<double> Point3d;

struct CV_EXPORTS RotatedRect
{
    RotatedRect();
    RotatedRect(const Point2f& _center, const Size2f& _size, float _angle);
    Point2f center;
    Size2f size;
    float angle;
};

//////////////////////////////// Scalar_ ///////////////////////////////

template<typename T> struct CV_EXPORTS Scalar_ : Vec_<T, 4>
{
    Scalar_();
    Scalar_(T v0, T v1, T v2=0, T v3=0);
    Scalar_(const CvScalar& s);
    Scalar_(T v0);
    static Scalar_<T> all(T v0);
    operator CvScalar() const;

    template<typename T2> operator Scalar_<T2>() const;

    Scalar_<T> mul(const Scalar_<T>& t, double scale=1 ) const;
    template<typename T2> void convertTo(T2* buf, int channels, int unroll_to=0) const;
};

typedef Scalar_<double> Scalar;

//////////////////////////////// Range /////////////////////////////////

struct CV_EXPORTS Range
{
    Range();
    Range(int _start, int _end);
    int size() const;
    bool empty() const;
    static Range all();

    int start, end;
};

/////////////////////////////// DataType ////////////////////////////////

template<typename T> struct DataType
{
    typedef T value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<uchar>
{
    typedef uchar value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<schar>
{
    typedef schar value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<ushort>
{
    typedef ushort value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<short>
{
    typedef short value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<int>
{
    typedef int value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<float>
{
    typedef float value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<double>
{
    typedef double value_type;
    typedef value_type channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 1,
           fmt=DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T, int cn> struct DataType<Vec_<T, cn> >
{
    typedef Vec_<T, cn> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = cn,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<std::complex<T> >
{
    typedef std::complex<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 2,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Complex<T> >
{
    typedef Complex<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 2,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Point_<T> >
{
    typedef Point_<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 2,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Point3_<T> >
{
    typedef Point3_<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 3,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Size_<T> >
{
    typedef Size_<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 2,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Rect_<T> >
{
    typedef Rect_<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 4,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<typename T> struct DataType<Scalar_<T> >
{
    typedef Scalar_<T> value_type;
    typedef T channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 4,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};

template<> struct DataType<Range>
{
    typedef Range value_type;
    typedef int channel_type;
    enum { depth = DataDepth<channel_type>::value, channels = 2,
           fmt = ((channels-1)<<8) + DataDepth<channel_type>::fmt,
           type = CV_MAKETYPE(depth, channels) };
};


//////////////////////////////// Vector ////////////////////////////////

// template vector class. It is similar to STL's vector,
// with a few important differences:
//   1) it can be created on top of user-allocated data w/o copying it
//   2) Vector b = a means copying the header,
//      not the underlying data (use clone() to make a deep copy)
template <typename T> class CV_EXPORTS Vector
{
public:
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;

    struct CV_EXPORTS Hdr
    {
        Hdr() : data(0), datastart(0), refcount(0), size(0), capacity(0) {};
        T* data;
        T* datastart;
        volatile int* refcount;
        size_t size;
        size_t capacity;
    };

    Vector();
    Vector(size_t _size);
    Vector(size_t _size, const T& val);
    Vector(T* _data, size_t _size, bool _copyData=false);
    Vector(const std::vector<T>& vec, bool _copyData=false);
    Vector(const Vector& d);
    Vector(const Vector& d, const Range& r);

    Vector& operator = (const Vector& d);

    ~Vector();
    Vector clone() const;

    T& operator [] (size_t i);
    const T& operator [] (size_t i) const;
    T& operator [] (int i);
    const T& operator [] (int i) const;
    Vector operator() (const Range& r) const;
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;

    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    void release();
    void set(T* _data, size_t _size, bool _copyData=false);

    void reserve(size_t newCapacity);
    void resize(size_t newSize);
    Vector<T>& push_back(const T& elem);
    Vector<T>& pop_back();
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();

protected:
    Hdr hdr;
};

//////////////////// Generic ref-cointing pointer class for C/C++ objects ////////////////////////

template<typename T> struct CV_EXPORTS Ptr
{
    Ptr();
    Ptr(T* _obj);
    ~Ptr();
    Ptr(const Ptr& ptr);
    Ptr& operator = (const Ptr& ptr);
    void addref();
    void release();
    void delete_obj();

    T* operator -> ();
    const T* operator -> () const;

    operator T* ();
    operator const T*() const;

    T* obj;
    int* refcount;
};

//////////////////////////////// Mat ////////////////////////////////

struct MatExpr_Base;
template<typename E, typename M> struct MatExpr_;
template<typename A1, typename M, typename Op> struct MatExpr_Op1_;
template<typename A1, typename A2, typename M, typename Op> struct MatExpr_Op2_;
template<typename A1, typename A2, typename A3, typename M, typename Op> struct MatExpr_Op3_;
template<typename A1, typename A2, typename A3, typename A4,
        typename M, typename Op> struct MatExpr_Op4_;
template<typename A1, typename A2, typename A3, typename A4,
        typename A5, typename M, typename Op> struct MatExpr_Op5_;
template<typename M> struct CV_EXPORTS MatOp_DivRS_;
template<typename M> struct CV_EXPORTS MatOp_Inv_;
template<typename M> struct CV_EXPORTS MatOp_MulDiv_;
template<typename M> struct CV_EXPORTS MatOp_Repeat_;
template<typename M> struct CV_EXPORTS MatOp_Set_;
template<typename M> struct CV_EXPORTS MatOp_Scale_;
template<typename M> struct CV_EXPORTS MatOp_T_;

struct Mat;

typedef MatExpr_<MatExpr_Op4_<Size, int, Scalar,
    int, Mat, MatOp_Set_<Mat> >, Mat> MatExpr_Initializer;

template<typename T> struct MatIterator_;
template<typename T> struct MatConstIterator_;

enum { MAGIC_MASK=0xFFFF0000, TYPE_MASK=0x00000FFF, DEPTH_MASK=7 };

static inline int getElemSize(int type) { return CV_ELEM_SIZE(type); }

// matrix decomposition types
enum { DECOMP_LU=0, DECOMP_SVD=1, DECOMP_EIG=2, DECOMP_CHOLESKY=3, DECOMP_QR=4, DECOMP_NORMAL=16 };
enum { NORM_INF=1, NORM_L1=2, NORM_L2=4, NORM_RELATIVE=8};
enum { CMP_EQ=0, CMP_GT=1, CMP_GE=2, CMP_LT=3, CMP_LE=4, CMP_NE=5 };
enum { GEMM_1_T=1, GEMM_2_T=2, GEMM_3_T=4 };
enum { DFT_INVERSE=1, DFT_SCALE=2, DFT_ROWS=4, DFT_COMPLEX_OUTPUT=16, DFT_REAL_OUTPUT=32,
    DCT_INVERSE = DFT_INVERSE, DCT_ROWS=DFT_ROWS };

struct CV_EXPORTS Mat
{
    Mat();
    Mat(int _rows, int _cols, int _type);
    Mat(int _rows, int _cols, int _type, const Scalar& _s);
    Mat(Size _size, int _type);
    Mat(const Mat& m);
    Mat(int _rows, int _cols, int _type, void* _data, int _step=AUTO_STEP);
    Mat(Size _size, int _type, void* _data, int _step=AUTO_STEP);
    Mat(const Mat& m, const Range& rowRange, const Range& colRange);
    Mat(const Mat& m, const Rect& roi);
    Mat(const CvMat* m, bool copyData=false);
    Mat(const IplImage* img, bool copyData=false);
    Mat( const MatExpr_Base& expr );
    ~Mat();
    Mat& operator = (const Mat& m);
    Mat& operator = (const MatExpr_Base& expr);

    operator MatExpr_<Mat, Mat>() const;

    Mat row(int y) const;
    Mat col(int x) const;
    Mat rowRange(int startrow, int endrow) const;
    Mat rowRange(const Range& r) const;
    Mat colRange(int startcol, int endcol) const;
    Mat colRange(const Range& r) const;
    Mat diag(int d=0) const;
    static Mat diag(const Mat& d);

    Mat clone() const;
    void copyTo( Mat& m ) const;
    void copyTo( Mat& m, const Mat& mask ) const;
    void convertTo( Mat& m, int rtype, double alpha=1, double beta=0 ) const;

    void assignTo( Mat& m, int type=-1 ) const;
    Mat& operator = (const Scalar& s);
    Mat& setTo(const Scalar& s, const Mat& mask=Mat());
    Mat reshape(int _cn, int _rows=0) const;

    MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_T_<Mat> >, Mat>
    t() const;
    MatExpr_<MatExpr_Op2_<Mat, int, Mat, MatOp_Inv_<Mat> >, Mat>
        inv(int method=DECOMP_LU) const;
    MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
    mul(const Mat& m, double scale=1) const;
    MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
    mul(const MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>& m, double scale=1) const;
    MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>    
    mul(const MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_DivRS_<Mat> >, Mat>& m, double scale=1) const;

    Mat cross(const Mat& m) const;
    double dot(const Mat& m) const;

    static MatExpr_Initializer zeros(int rows, int cols, int type);
    static MatExpr_Initializer zeros(Size size, int type);
    static MatExpr_Initializer ones(int rows, int cols, int type);
    static MatExpr_Initializer ones(Size size, int type);
    static MatExpr_Initializer eye(int rows, int cols, int type);
    static MatExpr_Initializer eye(Size size, int type);

    void create(int _rows, int _cols, int _type);
    void create(Size _size, int _type);
    void release();

    void locateROI( Size& wholeSize, Point& ofs ) const;
    Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );
    Mat operator()( Range rowRange, Range colRange ) const;
    Mat operator()( const Rect& roi ) const;

    operator CvMat() const;
    bool isContinuous() const;
    int elemSize() const;
    int elemSize1() const;
    int type() const;
    int depth() const;
    int channels() const;
    int step1() const;
    Size size() const;

    uchar* ptr(int y=0);
    const uchar* ptr(int y=0) const;

    enum { MAGIC_VAL=0x42FF0000, AUTO_STEP=-1, CONTINUOUS_FLAG=CV_MAT_CONT_FLAG };

    int flags;
    int rows, cols;
    int step;
    uchar* data;
    volatile int* refcount;

    uchar* datastart;
    uchar* dataend;
};


// Multiply-with-Carry RNG
struct CV_EXPORTS RNG
{
    enum { A=4164903690U, UNIFORM=0, NORMAL=1 };

    RNG();
    RNG(unsigned seed);
    RNG(uint64 _state);
    unsigned next();

    operator uchar();
    operator schar();
    operator ushort();
    operator short();
    operator unsigned();
    operator int();
    operator float();
    operator double();
    void fill( Mat& mat, int distType, const Scalar& a, const Scalar& b );

    uint64 state;
};

struct CV_EXPORTS TermCriteria
{
    enum { COUNT=1, EPS=2 };

    TermCriteria();
    TermCriteria(int _type, int _maxCount, double _epsilon);
    
    int type;
    int maxCount;
    double epsilon;
};

CV_EXPORTS Mat cvarrToMat(const CvArr* arr, bool copyData=false, bool allowND=true);
CV_EXPORTS Mat extractImageCOI(const CvArr* arr);

CV_EXPORTS void add(const Mat& a, const Mat& b, Mat& c, const Mat& mask);
CV_EXPORTS void subtract(const Mat& a, const Mat& b, Mat& c, const Mat& mask);
CV_EXPORTS void add(const Mat& a, const Mat& b, Mat& c);
CV_EXPORTS void subtract(const Mat& a, const Mat& b, Mat& c);
CV_EXPORTS void add(const Mat& a, const Scalar& s, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void subtract(const Mat& a, const Scalar& s, Mat& c, const Mat& mask=Mat());

CV_EXPORTS void multiply(const Mat& a, const Mat& b, Mat& c, double scale=1);
CV_EXPORTS void divide(const Mat& a, const Mat& b, Mat& c, double scale=1);
CV_EXPORTS void divide(double scale, const Mat& b, Mat& c);

CV_EXPORTS void subtract(const Scalar& s, const Mat& a, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void scaleAdd(const Mat& a, double alpha, const Mat& b, Mat& c);
CV_EXPORTS void addWeighted(const Mat& a, double alpha, const Mat& b,
                            double beta, double gamma, Mat& c);
CV_EXPORTS void convertScaleAbs(const Mat& a, Mat& c, double alpha=1, double beta=0);
CV_EXPORTS void LUT(const Mat& a, const Mat& lut, Mat& b);

CV_EXPORTS Scalar sum(const Mat& m);
CV_EXPORTS int countNonZero( const Mat& m );

CV_EXPORTS Scalar mean(const Mat& m);
CV_EXPORTS Scalar mean(const Mat& m, const Mat& mask);
CV_EXPORTS void meanStdDev(const Mat& m, Scalar& mean, Scalar& stddev, const Mat& mask=Mat());
CV_EXPORTS double norm(const Mat& a, int normType=NORM_L2);
CV_EXPORTS double norm(const Mat& a, const Mat& b, int normType=NORM_L2);
CV_EXPORTS double norm(const Mat& a, int normType, const Mat& mask);
CV_EXPORTS double norm(const Mat& a, const Mat& b,
                       int normType, const Mat& mask);
CV_EXPORTS void normalize( const Mat& a, Mat& b, double alpha=1, double beta=0,
                          int norm_type=NORM_L2, int rtype=-1, const Mat& mask=Mat());

CV_EXPORTS void minMaxLoc(const Mat& a, double* minVal,
                          double* maxVal=0, Point* minLoc=0,
                          Point* maxLoc=0, const Mat& mask=Mat());
CV_EXPORTS void reduce(const Mat& m, Mat& dst, int dim, int rtype, int dtype=-1);
CV_EXPORTS void merge(const Vector<Mat>& mv, Mat& dst);
CV_EXPORTS void split(const Mat& m, Vector<Mat>& mv);
CV_EXPORTS void mixChannels(const Vector<Mat>& src, Vector<Mat>& dst,
                            const Vector<int>& fromTo);
CV_EXPORTS void flip(const Mat& a, Mat& b, int flipCode);

CV_EXPORTS void repeat(const Mat& a, int ny, int nx, Mat& b);
static inline Mat repeat(const Mat& src, int ny, int nx)
{
    if( nx == 1 && ny == 1 ) return src;
    Mat dst; repeat(src, ny, nx, dst); return dst;
}

CV_EXPORTS void bitwise_and(const Mat& a, const Mat& b, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_or(const Mat& a, const Mat& b, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_xor(const Mat& a, const Mat& b, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_and(const Mat& a, const Scalar& s, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_or(const Mat& a, const Scalar& s, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_xor(const Mat& a, const Scalar& s, Mat& c, const Mat& mask=Mat());
CV_EXPORTS void bitwise_not(const Mat& a, Mat& c);
CV_EXPORTS void absdiff(const Mat& a, const Mat& b, Mat& c);
CV_EXPORTS void absdiff(const Mat& a, const Scalar& s, Mat& c);
CV_EXPORTS void inRange(const Mat& src, const Mat& lowerb,
                        const Mat& upperb, Mat& dst);
CV_EXPORTS void inRange(const Mat& src, const Scalar& lowerb,
                        const Scalar& upperb, Mat& dst);
CV_EXPORTS void compare(const Mat& a, const Mat& b, Mat& c, int cmpop);
CV_EXPORTS void compare(const Mat& a, double s, Mat& c, int cmpop);
CV_EXPORTS void min(const Mat& a, const Mat& b, Mat& c);
CV_EXPORTS void min(const Mat& a, double alpha, Mat& c);
CV_EXPORTS void max(const Mat& a, const Mat& b, Mat& c);
CV_EXPORTS void max(const Mat& a, double alpha, Mat& c);

CV_EXPORTS void sqrt(const Mat& a, Mat& b);
CV_EXPORTS void pow(const Mat& a, double power, Mat& b);
CV_EXPORTS void exp(const Mat& a, Mat& b);
CV_EXPORTS void log(const Mat& a, Mat& b);
CV_EXPORTS float cubeRoot(float val);
CV_EXPORTS float fastAtan2(float y, float x);
CV_EXPORTS void polarToCart(const Mat& magnitude, const Mat& angle,
                            Mat& x, Mat& y, bool angleInDegrees=false);
CV_EXPORTS void cartToPolar(const Mat& x, const Mat& y,
                            const Mat& magnitude, const Mat& angle,
                            bool angleInDegrees=false);
CV_EXPORTS bool checkRange(const Mat& a, bool quiet=true, Point* pt=0,
                           double minVal=-DBL_MAX, double maxVal=DBL_MAX);

CV_EXPORTS void gemm(const Mat& a, const Mat& b, double alpha,
                     const Mat& c, double gamma, Mat& d, int flags=0);
CV_EXPORTS void mulTransposed( const Mat& a, Mat& c, bool aTa,
                               const Mat& delta=Mat(),
                               double scale=1, int rtype=-1 );
CV_EXPORTS void transpose(const Mat& a, Mat& b);
CV_EXPORTS void transform(const Mat& src, Mat& dst, const Mat& m );
CV_EXPORTS void perspectiveTransform(const Mat& src, Mat& dst, const Mat& m );

CV_EXPORTS void completeSymm(Mat& a, bool lowerToUpper=false);
CV_EXPORTS void setIdentity(Mat& c, const Scalar& s=Scalar(1));
CV_EXPORTS double determinant(const Mat& m);
CV_EXPORTS Scalar trace(const Mat& m);
CV_EXPORTS double invert(const Mat& a, Mat& c, int flags=DECOMP_LU);
CV_EXPORTS bool solve(const Mat& a, const Mat& b, Mat& x, int flags=DECOMP_LU);
CV_EXPORTS void sort(const Mat& a, Mat& b, int flags);
CV_EXPORTS void sortIdx(const Mat& a, Mat& b, int flags);
CV_EXPORTS void solveCubic(const Mat& coeffs, Mat& roots);
CV_EXPORTS void solvePoly(const Mat& coeffs, Mat& roots, int maxIters=20, int fig=100);
CV_EXPORTS bool eigen(const Mat& a, Mat& eigenvalues);
CV_EXPORTS bool eigen(const Mat& a, Mat& eigenvalues, Mat& eigenvectors);

CV_EXPORTS void calcCovariation( const Vector<Mat>& data, Mat& covar, Mat& mean,
                                 int flags, int ctype=CV_64F);
CV_EXPORTS void calcCovariation( const Mat& data, Mat& covar, Mat& mean,
                                 int flags, int ctype=CV_64F);

struct CV_EXPORTS PCA
{
    PCA();
    PCA(const Mat& data, const Mat& mean, int flags, int maxComponents=0);
    PCA& operator()(const Mat& data, const Mat& mean, int flags, int maxComponents=0);
    Mat project(const Mat& vec) const;
    Mat backProject(const Mat& vec) const;

    Mat eigenvectors;
    Mat eigenvalues;
    Mat mean;
};

struct CV_EXPORTS SVD
{
    enum { MODIFY_A=1, NO_UV=2, FULL_UV=4 };
    SVD();
    SVD( const Mat& m, int flags=0 );
    SVD& operator ()( const Mat& m, int flags=0 );

    static void solveZ( const Mat& m, Mat& dst );
    void backSubst( const Mat& rhs, Mat& dst ) const;

    Mat u, w, vt;
};

CV_EXPORTS double mahalanobis(const Mat& v1, const Mat& v2, const Mat& icovar);
static inline double mahalonobis(const Mat& v1, const Mat& v2, const Mat& icovar)
{ return mahalanobis(v1, v2, icovar); }

CV_EXPORTS void dft(const Mat& src, Mat& dst, int flags=0, int nonzeroRows=0);
CV_EXPORTS void idft(const Mat& src, Mat& dst, int flags=0, int nonzeroRows=0);
CV_EXPORTS void dct(const Mat& src, Mat& dst, int flags=0);
CV_EXPORTS void idct(const Mat& src, Mat& dst, int flags=0);
CV_EXPORTS void mulSpectrums(const Mat& a, const Mat& b, Mat& c,
                             int flags, bool conjB=false);
CV_EXPORTS int getOptimalDFTSize(int vecsize);

CV_EXPORTS int kmeans( const Mat& samples, int K,
                       Mat& labels, Mat& centers,
                       TermCriteria crit, int attempts=1,
                       int flags=0, double* compactness=0);

CV_EXPORTS void seqToVector( const CvSeq* ptseq, Vector<Point>& pts );

CV_EXPORTS RNG& theRNG();
static inline int randi() { return (int)theRNG(); }
static inline unsigned randu() { return (unsigned)theRNG(); }
static inline float randf() { return (float)theRNG(); }
static inline double randd() { return (double)theRNG(); }
static inline void randu(Mat& dst, const Scalar& low, const Scalar& high)
{ theRNG().fill(dst, RNG::UNIFORM, low, high); }
static inline void randn(Mat& dst, const Scalar& mean, const Scalar& stddev)
{ theRNG().fill(dst, RNG::NORMAL, mean, stddev); }
CV_EXPORTS void randShuffle(Mat& dst, RNG& rng, double iterFactor=1.);
static inline void randShuffle(Mat& dst, double iterFactor=1.)
{ randShuffle(dst, theRNG(), iterFactor); }

CV_EXPORTS void line(Mat& img, Point pt1, Point pt2, const Scalar& color,
                     int thickness=1, int lineType=8, int shift=0);

CV_EXPORTS void rectangle(Mat& img, Point pt1, Point pt2,
                          const Scalar& color, int thickness=1,
                          int lineType=8, int shift=0);

CV_EXPORTS void circle(Mat& img, Point center, int radius,
                       const Scalar& color, int thickness=1,
                       int lineType=8, int shift=0);

CV_EXPORTS void ellipse(Mat& img, Point center, Size axes,
                        double angle, double startAngle, double endAngle,
                        const Scalar& color, int thickness=1,
                        int lineType=8, int shift=0);

CV_EXPORTS void ellipse(Mat& img, const RotatedRect& box, const Scalar& color,
                        int thickness=1, int lineType=8, int shift=0 );

CV_EXPORTS void fillConvexPoly(Mat& img, const Vector<Point>& pts,
                               const Scalar& color, int lineType=8,
                               int shift=0);

CV_EXPORTS void fillPoly(Mat& img, const Vector<Vector<Point> >& pts,
                         const Scalar& color, int lineType=8, int shift=0,
                         Point offset=Point() );

CV_EXPORTS void polylines(Mat& img, const Vector<Vector<Point> >& pts, bool isClosed,
                          const Scalar& color, int thickness=1, int lineType=8, int shift=0 );

CV_EXPORTS bool clipLine(Size imgSize, Point& pt1, Point& pt2);

struct CV_EXPORTS LineIterator
{
    LineIterator(const Mat& img, Point pt1, Point pt2,
                 int connectivity=8, bool leftToRight=false);
    uchar* operator *();
    LineIterator& operator ++();
    LineIterator operator ++(int);

    uchar* ptr;
    int err, count;
    int minusDelta, plusDelta;
    int minusStep, plusStep;
};

CV_EXPORTS void ellipse2Poly( Point center, Size axes, int angle,
                              int arcStart, int arcEnd, int delta, Vector<Point>& pts );

enum
{
    FONT_HERSHEY_SIMPLEX = 0,
    FONT_HERSHEY_PLAIN = 1,
    FONT_HERSHEY_DUPLEX = 2,
    FONT_HERSHEY_COMPLEX = 3,
    FONT_HERSHEY_TRIPLEX = 4,
    FONT_HERSHEY_COMPLEX_SMALL = 5,
    FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
    FONT_HERSHEY_SCRIPT_COMPLEX = 7,
    FONT_ITALIC = 16
};

CV_EXPORTS void putText( Mat& img, const String& text, Point org,
                         int fontFace, double fontScale, Scalar color,
                         int thickness=1, int linetype=8,
                         bool bottomLeftOrigin=false );

CV_EXPORTS Size getTextSize(const String& text, int fontFace,
                            double fontScale, int thickness,
                            int* baseLine);

///////////////////////////////// Mat_<T> ////////////////////////////////////

template<typename T> struct CV_EXPORTS Mat_ : public Mat
{
    typedef T value_type;
    typedef typename DataType<T>::channel_type channel_type;
    typedef MatIterator_<T> iterator;
    typedef MatConstIterator_<T> const_iterator;
    
    Mat_();
    Mat_(int _rows, int _cols);
    Mat_(int _rows, int _cols, const T& value);
    Mat_(const Mat& m);
    Mat_(const Mat_& m);
    Mat_(int _rows, int _cols, T* _data, int _step=AUTO_STEP);
    Mat_(const Mat_& m, const Range& rowRange, const Range& colRange);
    Mat_(const Mat_& m, const Rect& roi);
    Mat_(const MatExpr_Base& expr);
    ~Mat_();

    Mat_& operator = (const Mat& m);
    Mat_& operator = (const Mat_& m);
    Mat_& operator = (const T& s);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    Mat_& create(int _rows, int _cols);
    Mat_& create(Size _size);
    Mat_ cross(const Mat_& m) const;
    Mat_& operator = (const MatExpr_Base& expr);
    template<typename T2> operator Mat_<T2>() const;
    Mat_ row(int y) const;
    Mat_ col(int x) const;
    Mat_ diag(int d=0) const;
    Mat_ clone() const;

    MatExpr_<MatExpr_Op2_<Mat_, double, Mat_, MatOp_T_<Mat> >, Mat_> t() const;
    MatExpr_<MatExpr_Op2_<Mat_, int, Mat_, MatOp_Inv_<Mat> >, Mat_> inv(int method=DECOMP_LU) const;

    MatExpr_<MatExpr_Op4_<Mat_, Mat_, double, char, Mat_, MatOp_MulDiv_<Mat> >, Mat_>
    mul(const Mat_& m, double scale=1) const;
    MatExpr_<MatExpr_Op4_<Mat_, Mat_, double, char, Mat_, MatOp_MulDiv_<Mat> >, Mat_>
    mul(const MatExpr_<MatExpr_Op2_<Mat_, double, Mat_,
        MatOp_Scale_<Mat> >, Mat_>& m, double scale=1) const;
    MatExpr_<MatExpr_Op4_<Mat_, Mat_, double, char, Mat_, MatOp_MulDiv_<Mat> >, Mat_>    
    mul(const MatExpr_<MatExpr_Op2_<Mat_, double, Mat_,
        MatOp_DivRS_<Mat> >, Mat_>& m, double scale=1) const;

    int elemSize() const;
    int elemSize1() const;
    int type() const;
    int depth() const;
    int channels() const;
    int stepT() const;
    int step1() const;

    static MatExpr_Initializer zeros(int rows, int cols);
    static MatExpr_Initializer zeros(Size size);
    static MatExpr_Initializer ones(int rows, int cols);
    static MatExpr_Initializer ones(Size size);
    static MatExpr_Initializer eye(int rows, int cols);
    static MatExpr_Initializer eye(Size size);

    Mat_ reshape(int _rows) const;
    Mat_& adjustROI( int dtop, int dbottom, int dleft, int dright );
    Mat_ operator()( const Range& rowRange, const Range& colRange ) const;
    Mat_ operator()( const Rect& roi ) const;

    T* operator [](int y);
    const T* operator [](int y) const;

    T& operator ()(int row, int col);
    T operator ()(int row, int col) const;

    operator MatExpr_<Mat_, Mat_>() const;
};

//////////// Iterators & Comma initializers //////////////////

template<typename T>
struct CV_EXPORTS MatConstIterator_
{
    typedef T value_type;
    typedef int difference_type;

    MatConstIterator_();
    MatConstIterator_(const Mat_<T>* _m);
    MatConstIterator_(const Mat_<T>* _m, int _row, int _col=0);
    MatConstIterator_(const Mat_<T>* _m, Point _pt);
    MatConstIterator_(const MatConstIterator_& it);

    MatConstIterator_& operator = (const MatConstIterator_& it );
    T operator *() const;
    T operator [](int i) const;
    
    MatConstIterator_& operator += (int ofs);
    MatConstIterator_& operator -= (int ofs);
    MatConstIterator_& operator --();
    MatConstIterator_ operator --(int);
    MatConstIterator_& operator ++();
    MatConstIterator_ operator ++(int);
    Point pos() const;

    const Mat_<T>* m;
    T* ptr;
    T* sliceEnd;
};


template<typename T>
struct CV_EXPORTS MatIterator_ : MatConstIterator_<T>
{
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;

    MatIterator_();
    MatIterator_(Mat_<T>* _m);
    MatIterator_(Mat_<T>* _m, int _row, int _col=0);
    MatIterator_(const Mat_<T>* _m, Point _pt);
    MatIterator_(const MatIterator_& it);
    MatIterator_& operator = (const MatIterator_<T>& it );

    T& operator *() const;
    T& operator [](int i) const;

    MatIterator_& operator += (int ofs);
    MatIterator_& operator -= (int ofs);
    MatIterator_& operator --();
    MatIterator_ operator --(int);
    MatIterator_& operator ++();
    MatIterator_ operator ++(int);
};

template<typename T> struct CV_EXPORTS MatOp_Iter_;

template<typename T> struct CV_EXPORTS MatCommaInitializer_ :
    MatExpr_<MatExpr_Op1_<MatIterator_<T>, Mat_<T>, MatOp_Iter_<T> >, Mat_<T> >
{
    MatCommaInitializer_(Mat_<T>* _m);
    template<typename T2> MatCommaInitializer_<T>& operator , (T2 v);
    operator Mat_<T>() const;
    Mat_<T> operator *() const;
    void assignTo(Mat& m, int type=-1) const;
};

template<typename T> struct VectorCommaInitializer_
{
    VectorCommaInitializer_(Vector<T>* _vec);
    template<typename T2> VectorCommaInitializer_<T>& operator , (T2 val);
    operator Vector<T>() const;
    Vector<T> operator *() const;

    Vector<T>* vec;
    int idx;
};

template<typename T, size_t fixed_size=CV_MAX_LOCAL_SIZE> struct CV_EXPORTS AutoBuffer
{
    typedef T value_type;

    AutoBuffer();
    AutoBuffer(size_t _size);
    ~AutoBuffer();

    void allocate(size_t _size);
    void deallocate();
    operator T* ();
    operator const T* () const;

    T* ptr;
    size_t size;
    T buf[fixed_size];
};

//////////////////////////////////////// XML & YAML I/O ////////////////////////////////////

struct CV_EXPORTS FileNode;

struct CV_EXPORTS FileStorage
{
    enum { READ=0, WRITE=1, APPEND=2 };
    enum { UNDEFINED=0, VALUE_EXPECTED=1, NAME_EXPECTED=2, INSIDE_MAP=4 };
    FileStorage();
    FileStorage(const String& filename, int flags);
    FileStorage(CvFileStorage* fs);
    virtual ~FileStorage();

    virtual bool open(const String& filename, int flags);
    virtual bool isOpened() const;
    virtual void release();

    FileNode root(int streamidx=0) const;
    FileNode operator[](const String& nodename) const;
    FileNode operator[](const char* nodename) const;

    CvFileStorage* operator *() { return fs.obj; }
    const CvFileStorage* operator *() const { return fs.obj; }
    void writeRaw( const String& fmt, const Vector<uchar>& vec );
    void writeObj( const String& name, const void* obj );

    Ptr<CvFileStorage> fs;
    String elname;
    Vector<char> structs;
    int state;
};

struct CV_EXPORTS FileNodeIterator;

struct CV_EXPORTS FileNode
{
    enum { NONE=0, INT=1, REAL=2, FLOAT=REAL, STR=3, STRING=STR, REF=4, SEQ=5, MAP=6, TYPE_MASK=7,
        FLOW=8, USER=16, EMPTY=32, NAMED=64 };
    FileNode();
    FileNode(const CvFileStorage* fs, const CvFileNode* node);
    FileNode(const FileNode& node);
    FileNode operator[](const String& nodename) const;
    FileNode operator[](const char* nodename) const;
    FileNode operator[](int i) const;
    int type() const;
    int rawDataSize(const String& fmt) const;
    bool isNone() const;
    bool isSeq() const;
    bool isMap() const;
    bool isInt() const;
    bool isReal() const;
    bool isString() const;
    bool isNamed() const;
    String name() const;
    size_t count() const;
    operator int() const;
    operator float() const;
    operator double() const;
    operator String() const;

    FileNodeIterator begin() const;
    FileNodeIterator end() const;

    void readRaw( const String& fmt, Vector<uchar>& vec ) const;
    void* readObj() const;

    // do not use wrapper pointer classes for better efficiency
    const CvFileStorage* fs;
    const CvFileNode* node;
};

struct CV_EXPORTS FileNodeIterator
{
    FileNodeIterator();
    FileNodeIterator(const CvFileStorage* fs, const CvFileNode* node, size_t ofs=0);
    FileNodeIterator(const FileNodeIterator& it);
    FileNode operator *() const;
    FileNode operator ->() const;

    FileNodeIterator& operator ++();
    FileNodeIterator operator ++(int);
    FileNodeIterator& operator --();
    FileNodeIterator operator --(int);
    FileNodeIterator& operator += (int);
    FileNodeIterator& operator -= (int);

    FileNodeIterator& readRaw( const String& fmt, Vector<uchar>& vec,
                               size_t maxCount=(size_t)INT_MAX );

    const CvFileStorage* fs;
    const CvFileNode* container;
    CvSeqReader reader;
    size_t remaining;
};

}

#endif // __cplusplus

#include "cxoperations.hpp"

#endif /*_CXCORE_HPP_*/
