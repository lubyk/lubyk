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

#ifndef _CXCORE_OPERATIONS_H_
#define _CXCORE_OPERATIONS_H_

#ifndef SKIP_INCLUDES
#include <limits.h>
#endif // SKIP_INCLUDES

#ifdef __cplusplus

namespace cv
{

/////////////// saturate_cast (used in image & signal processing) ///////////////////

template<typename T> static inline T saturate_cast(uchar v) { return T(v); }
template<typename T> static inline T saturate_cast(schar v) { return T(v); }
template<typename T> static inline T saturate_cast(ushort v) { return T(v); }
template<typename T> static inline T saturate_cast(short v) { return T(v); }
template<typename T> static inline T saturate_cast(unsigned v) { return T(v); }
template<typename T> static inline T saturate_cast(int v) { return T(v); }
template<typename T> static inline T saturate_cast(float v) { return T(v); }
template<typename T> static inline T saturate_cast(double v) { return T(v); }

template<> inline uchar saturate_cast<uchar>(schar v)
{ return (uchar)std::max((int)v, 0); }
template<> inline uchar saturate_cast<uchar>(ushort v)
{ return (uchar)std::min((unsigned)v, (unsigned)UCHAR_MAX); }
template<> inline uchar saturate_cast<uchar>(int v)
{ return (uchar)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
template<> inline uchar saturate_cast<uchar>(short v)
{ return saturate_cast<uchar>((int)v); }
template<> inline uchar saturate_cast<uchar>(unsigned v)
{ return (uchar)std::min(v, (unsigned)UCHAR_MAX); }
template<> inline uchar saturate_cast<uchar>(float v)
{ int iv = cvRound(v); return saturate_cast<uchar>(iv); }
template<> inline uchar saturate_cast<uchar>(double v)
{ int iv = cvRound(v); return saturate_cast<uchar>(iv); }

template<> inline schar saturate_cast<schar>(uchar v)
{ return (schar)std::min((int)v, SCHAR_MAX); }
template<> inline schar saturate_cast<schar>(ushort v)
{ return (schar)std::min((unsigned)v, (unsigned)SCHAR_MAX); }
template<> inline schar saturate_cast<schar>(int v)
{
    return (schar)((unsigned)(v-SCHAR_MIN) <= (unsigned)UCHAR_MAX ?
                v : v > 0 ? SCHAR_MAX : SCHAR_MIN);
}
template<> inline schar saturate_cast<schar>(short v)
{ return saturate_cast<schar>((int)v); }
template<> inline schar saturate_cast<schar>(unsigned v)
{ return (schar)std::min(v, (unsigned)SCHAR_MAX); }

template<> inline schar saturate_cast<schar>(float v)
{ int iv = cvRound(v); return saturate_cast<schar>(iv); }
template<> inline schar saturate_cast<schar>(double v)
{ int iv = cvRound(v); return saturate_cast<schar>(iv); }

template<> inline ushort saturate_cast<ushort>(schar v)
{ return (ushort)std::max((int)v, 0); }
template<> inline ushort saturate_cast<ushort>(short v)
{ return (ushort)std::max((int)v, 0); }
template<> inline ushort saturate_cast<ushort>(int v)
{ return (ushort)((unsigned)v <= (unsigned)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
template<> inline ushort saturate_cast<ushort>(unsigned v)
{ return (ushort)std::min(v, (unsigned)USHRT_MAX); }
template<> inline ushort saturate_cast<ushort>(float v)
{ int iv = cvRound(v); return saturate_cast<ushort>(iv); }
template<> inline ushort saturate_cast<ushort>(double v)
{ int iv = cvRound(v); return saturate_cast<ushort>(iv); }

template<> inline short saturate_cast<short>(ushort v)
{ return (short)std::min((int)v, SHRT_MAX); }
template<> inline short saturate_cast<short>(int v)
{
    return (short)((unsigned)(v - SHRT_MIN) <= (unsigned)USHRT_MAX ?
            v : v > 0 ? SHRT_MAX : SHRT_MIN);
}
template<> inline short saturate_cast<short>(unsigned v)
{ return (short)std::min(v, (unsigned)SHRT_MAX); }
template<> inline short saturate_cast<short>(float v)
{ int iv = cvRound(v); return saturate_cast<short>(iv); }
template<> inline short saturate_cast<short>(double v)
{ int iv = cvRound(v); return saturate_cast<short>(iv); }

template<> inline int saturate_cast<int>(float v) { return cvRound(v); }
template<> inline int saturate_cast<int>(double v) { return cvRound(v); }

// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
template<> inline unsigned saturate_cast<unsigned>(float v){ return cvRound(v); }
template<> inline unsigned saturate_cast<unsigned>(double v) { return cvRound(v); }


/////////////////////////// short vector (Vec_) /////////////////////////////

template<typename T, int cn> inline Vec_<T, cn>::Vec_() {}
template<typename T, int cn> inline Vec_<T, cn>::Vec_(T v0)
{
    val[0] = v0;
    for(int i = 1; i < cn; i++) val[i] = T();
}

template<typename T, int cn> inline Vec_<T, cn>::Vec_(T v0, T v1)
{
    val[0] = v0; val[1] = v1;
    for(int i = 2; i < cn; i++) val[i] = T(0);
}

template<typename T, int cn> inline Vec_<T, cn>::Vec_(T v0, T v1, T v2)
{
    val[0] = v0; val[1] = v1; val[2] = v2;
    for(int i = 3; i < cn; i++) val[i] = T(0);
}

template<typename T, int cn> inline Vec_<T, cn>::Vec_(T v0, T v1, T v2, T v3)
{
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    for(int i = 4; i < cn; i++) val[i] = T(0);
}

template<typename T, int cn> inline Vec_<T, cn>::Vec_(const Vec_<T, cn>& v)
{
    for( int i = 0; i < cn; i++ ) val[i] = v.val[i];
}

template<typename T, int cn> inline Vec_<T, cn> Vec_<T, cn>::all(T alpha)
{
    Vec_ v;
    for( int i = 0; i < cn; i++ ) v.val[i] = alpha;
    return v;
}

template<typename T, int cn> inline T Vec_<T, cn>::dot(const Vec_<T, cn>& v) const
{
    T s = 0;
    for( int i = 0; i < cn; i++ ) s += val[i]*v.val[i];
    return s;
}

template<typename T, int cn> inline double Vec_<T, cn>::ddot(const Vec_<T, cn>& v) const
{
    double s = 0;
    for( int i = 0; i < cn; i++ ) s += (double)val[i]*v.val[i];
    return s;
}

template<typename T, int cn> inline Vec_<T, cn> Vec_<T, cn>::cross(const Vec_<T, cn>& v) const
{
    return Vec_<T, cn>(); // for arbitrary-size vector there is no cross-product defined
}

template<typename T, int cn> template<typename T2>
inline Vec_<T, cn>::operator Vec_<T2, cn>() const
{
    Vec_<T2, cn> v;
    for( int i = 0; i < cn; i++ ) v.val[i] = saturate_cast<T2>(val[i]);
    return v;
}

template<typename T, int cn> inline Vec_<T, cn>::operator CvScalar() const
{
    CvScalar s = {{0,0,0,0}};
    int i;
    for( i = 0; i < std::min(cn, 4); i++ ) s.val[i] = val[i];
    for( ; i < 4; i++ ) s.val[i] = 0;
    return s;
}

template<typename T, int cn> inline T Vec_<T, cn>::operator [](int i) const { return val[i]; }
template<typename T, int cn> inline T& Vec_<T, cn>::operator[](int i) { return val[i]; }

template<typename T, int cn> static inline Vec_<T, cn>
operator + (const Vec_<T, cn>& a, const Vec_<T, cn>& b)
{
    Vec_<T, cn> c = a;
    return c += b;
}

template<typename T, int cn> static inline Vec_<T, cn>
operator - (const Vec_<T, cn>& a, const Vec_<T, cn>& b)
{
    Vec_<T, cn> c = a;
    return c -= b;
}

template<typename T> static inline
Vec_<T, 2>& operator *= (Vec_<T, 2>& a, T alpha)
{
    a[0] *= alpha; a[1] *= alpha;
    return a;
}

template<typename T> static inline
Vec_<T, 3>& operator *= (Vec_<T, 3>& a, T alpha)
{
    a[0] *= alpha; a[1] *= alpha; a[2] *= alpha;
    return a;
}

template<typename T> static inline
Vec_<T, 4>& operator *= (Vec_<T, 4>& a, T alpha)
{
    a[0] *= alpha; a[1] *= alpha; a[2] *= alpha; a[3] *= alpha;
    return a;
}

template<typename T, int cn> static inline Vec_<T, cn>
operator * (const Vec_<T, cn>& a, T alpha)
{
    Vec_<T, cn> c = a;
    return c *= alpha;
}

template<typename T, int cn> static inline Vec_<T, cn>
operator * (T alpha, const Vec_<T, cn>& a)
{
    return a * alpha;
}

template<typename T, int cn> static inline Vec_<T, cn>
operator - (const Vec_<T, cn>& a)
{
    Vec_<T,cn> t;
    for( int i = 0; i < cn; i++ ) t.val[i] = saturate_cast<T>(-a.val[i]);
    return t;
}

template<> inline Vec_<float, 3> Vec_<float, 3>::cross(const Vec_<float, 3>& v) const
{
    return Vec_<float,3>(val[1]*v.val[2] - val[2]*v.val[1],
                     val[2]*v.val[0] - val[0]*v.val[2],
                     val[0]*v.val[1] - val[1]*v.val[0]);
}

template<> inline Vec_<double, 3> Vec_<double, 3>::cross(const Vec_<double, 3>& v) const
{
    return Vec_<double,3>(val[1]*v.val[2] - val[2]*v.val[1],
                     val[2]*v.val[0] - val[0]*v.val[2],
                     val[0]*v.val[1] - val[1]*v.val[0]);
}

template<typename T1, typename T2> static inline
Vec_<T1, 2>& operator += (Vec_<T1, 2>& a, const Vec_<T2, 2>& b)
{
    a[0] = saturate_cast<T1>(a[0] + b[0]);
    a[1] = saturate_cast<T1>(a[1] + b[1]);
    return a;
}

template<typename T1, typename T2> static inline
Vec_<T1, 3>& operator += (Vec_<T1, 3>& a, const Vec_<T2, 3>& b)
{
    a[0] = saturate_cast<T1>(a[0] + b[0]);
    a[1] = saturate_cast<T1>(a[1] + b[1]);
    a[2] = saturate_cast<T1>(a[2] + b[2]);
    return a;
}

template<typename T1, typename T2> static inline
Vec_<T1, 4>& operator += (Vec_<T1, 4>& a, const Vec_<T2, 4>& b)
{
    a[0] = saturate_cast<T1>(a[0] + b[0]);
    a[1] = saturate_cast<T1>(a[1] + b[1]);
    a[2] = saturate_cast<T1>(a[2] + b[2]);
    a[3] = saturate_cast<T1>(a[3] + b[3]);
    return a;
}

//////////////////////////////// Complex //////////////////////////////

template<typename T> inline Complex<T>::Complex() : re(0), im(0) {}
template<typename T> inline Complex<T>::Complex( T _re, T _im ) : re(_re), im(_im) {}
template<typename T> template<typename T2> inline Complex<T>::operator Complex<T2>() const
{ return Complex<T2>(saturate_cast<T2>(re), saturate_cast<T2>(im)); }
template<typename T> inline Complex<T> Complex<T>::conj() const
{ return Complex<T>(re, -im); }

template<typename T> static inline
Complex<T> operator + (const Complex<T>& a, const Complex<T>& b)
{ return Complex<T>( a.re + b.re, a.im + b.im ); }

template<typename T> static inline
Complex<T>& operator += (Complex<T>& a, const Complex<T>& b)
{ a.re += b.re; a.im += b.im; return a; }

template<typename T> static inline
Complex<T> operator - (const Complex<T>& a, const Complex<T>& b)
{ return Complex<T>( a.re - b.re, a.im - b.im ); }

template<typename T> static inline
Complex<T>& operator -= (Complex<T>& a, const Complex<T>& b)
{ a.re -= b.re; a.im -= b.im; return a; }

template<typename T> static inline
Complex<T> operator - (const Complex<T>& a)
{ return Complex<T>(-a.re, -a.im); }

template<typename T> static inline
Complex<T> operator * (const Complex<T>& a, const Complex<T>& b)
{ return Complex<T>( a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re ); }

template<typename T> static inline
Complex<T> operator * (const Complex<T>& a, T b)
{ return Complex<T>( a.re*b, a.im*b ); }

template<typename T> static inline
Complex<T> operator * (T b, const Complex<T>& a)
{ return Complex<T>( a.re*b, a.im*b ); }

template<typename T> static inline
Complex<T> operator + (const Complex<T>& a, T b)
{ return Complex<T>( a.re + b, a.im ); }

template<typename T> static inline
Complex<T> operator - (const Complex<T>& a, T b)
{ return Complex<T>( a.re - b, a.im ); }

template<typename T> static inline
Complex<T> operator + (T b, const Complex<T>& a)
{ return Complex<T>( a.re + b, a.im ); }

template<typename T> static inline
Complex<T> operator - (T b, const Complex<T>& a)
{ return Complex<T>( b - a.re, -a.im ); }

template<typename T> static inline
Complex<T>& operator += (Complex<T>& a, T b)
{ a.re += b; return a; }

template<typename T> static inline
Complex<T>& operator -= (Complex<T>& a, T b)
{ a.re -= b; return a; }

template<typename T> static inline
Complex<T>& operator *= (Complex<T>& a, T b)
{ a.re *= b; a.im *= b; return a; }

template<typename T> static inline
double abs(const Complex<T>& a)
{ return std::sqrt( (double)a.re.a.re + (double)a.im*a.im); }

template<typename T> static inline
Complex<T> operator / (const Complex<T>& a, const Complex<T>& b)
{
    double t = 1./((double)b.re*b.re + (double)b.im*b.im);
    return Complex<T>( (T)((a.re*b.re + a.im*b.im)*t),
                        (T)((-a.re*b.im + a.im*b.re)*t) );
}

template<typename T> static inline
Complex<T>& operator /= (Complex<T>& a, const Complex<T>& b)
{
    return (a = a / b);
}

template<typename T> static inline
Complex<T> operator / (const Complex<T>& a, T b)
{
    T t = (T)1/b;
    return Complex<T>( a.re*t, a.im*t );
}

template<typename T> static inline
Complex<T> operator / (T b, const Complex<T>& a)
{
    return Complex<T>(b)/a;
}

template<typename T> static inline
Complex<T> operator /= (const Complex<T>& a, T b)
{
    T t = (T)1/b;
    a.re *= t; a.im *= t; return a;
}

//////////////////////////////// 2D Point ////////////////////////////////

template<typename T> inline Point_<T>::Point_() : x(0), y(0) {}
template<typename T> inline Point_<T>::Point_(T _x, T _y) : x(_x), y(_y) {}
template<typename T> inline Point_<T>::Point_(const Point_& pt) : x(pt.x), y(pt.y) {}
template<typename T> inline Point_<T>::Point_(const CvPoint& pt) : x((T)pt.x), y((T)pt.y) {}
template<typename T> inline Point_<T>::Point_(const CvPoint2D32f& pt) : x((T)pt.x), y((T)pt.y) {}
template<typename T> inline Point_<T>::Point_(const Size_<T>& sz) : x(sz.width), y(sz.height) {}
template<typename T> inline Point_<T>& Point_<T>::operator = (const Point_& pt)
{ x = pt.x; y = pt.y; return *this; }

template<typename T> inline Point_<T>::operator Point_<int>() const
{ return Point_<int>(cvRound(x), cvRound(y)); }
template<typename T> inline Point_<T>::operator Point_<float>() const
{ return Point_<float>(float(x), float(y)); }
template<typename T> inline Point_<T>::operator Point_<double>() const
{ return Point_<double>(x, y); }
template<typename T> inline Point_<T>::operator CvPoint() const
{ return cvPoint(cvRound(x), cvRound(y)); }
template<typename T> inline Point_<T>::operator CvPoint2D32f() const
{ return cvPoint2D32f((float)x, (float)y); }

template<typename T> inline T Point_<T>::dot(const Point_& pt) const
{ return x*pt.x + y*pt.y; }
template<typename T> inline double Point_<T>::ddot(const Point_& pt) const
{ return (double)x*pt.x + (double)y*pt.y; }

template<typename T> static inline Point_<T>&
operator += (Point_<T>& a, const Point_<T>& b) { a.x += b.x; a.y += b.y; return a; }

template<typename T> static inline Point_<T>&
operator -= (Point_<T>& a, const Point_<T>& b) { a.x -= b.x; a.y -= b.y; return a; }

template<typename T> static inline double norm(const Point_<T>& pt)
{ return sqrt((double)pt.x*pt.x + (double)pt.y*pt.y); }

template<typename T> static inline bool operator == (const Point_<T>& a, const Point_<T>& b)
{ return a.x == b.x && a.y == b.y; }

template<typename T> static inline bool operator != (const Point_<T>& a, const Point_<T>& b)
{ return !(a == b); }

template<typename T> static inline Point_<T> operator + (const Point_<T>& a, const Point_<T>& b)
{ return Point_<T>( a.x + b.x, a.y + b.y ); }

template<typename T> static inline Point_<T> operator - (const Point_<T>& a, const Point_<T>& b)
{ return Point_<T>( a.x - b.x, a.y - b.y ); }

template<typename T> static inline Point_<T> operator - (const Point_<T>& a)
{ return Point_<T>( -a.x, -a.y ); }

template<typename T> static inline Point_<T> operator * (const Point_<T>& a, T b)
{ return Point_<T>( a.x*b, a.y*b ); }

template<typename T> static inline Point_<T> operator * (T a, const Point_<T>& b)
{ return Point_<T>( a*b.x, a*b.y ); }

template<> inline Point_<int>::Point_(const CvPoint2D32f& pt) : x(cvRound(pt.x)), y(cvRound(pt.y)) {}
template<> inline Point_<int>::operator CvPoint() const { return cvPoint(x,y); }

//////////////////////////////// 3D Point ////////////////////////////////

template<typename T> inline Point3_<T>::Point3_() : x(0), y(0), z(0) {}
template<typename T> inline Point3_<T>::Point3_(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
template<typename T> inline Point3_<T>::Point3_(const Point3_& pt) : x(pt.x), y(pt.y), z(pt.z) {}
template<typename T> inline Point3_<T>::Point3_(const CvPoint3D32f& pt) :
x((T)pt.x), y((T)pt.y), z((T)pt.z) {}
template<typename T> inline Point3_<T>::Point3_(const Vec_<T, 3>& t) : x(t[0]), y(t[1]), z(t[2]) {}

template<typename T> inline Point3_<T>::operator Point3_<int>() const
{ return Point3_<int>(cvRound(x), cvRound(y), cvRound(z)); }
template<typename T> inline Point3_<T>::operator Point3_<float>() const
{ return Point3_<float>(float(x), float(y), float(z)); }
template<typename T> inline Point3_<T>::operator Point3_<double>() const
{ return Point3_<double>(x, y, z); }
template<typename T> inline Point3_<T>::operator CvPoint3D32f() const
{ return cvPoint3D32f((float)x, (float)y, (float)z); }

template<typename T> inline Point3_<T>& Point3_<T>::operator = (const Point3_& pt)
{ x = pt.x; y = pt.y; z = pt.z; return *this; }

template<typename T> inline T Point3_<T>::dot(const Point3_& pt) const
{ return x*pt.x + y*pt.y + z*pt.z; }
template<typename T> inline double Point3_<T>::ddot(const Point3_& pt) const
{ return (double)x*pt.x + (double)y*pt.y + (double)z*pt.z; }

template<typename T> static inline Point3_<T>&
operator += (Point3_<T>& a, const Point3_<T>& b) { a += b.x; a += b.y; a += b.z; return a; }
template<typename T> static inline Point3_<T>&
operator -= (Point3_<T>& a, const Point3_<T>& b) { a -= b.x; a -= b.y; a -= b.z; return a; }

template<typename T> static inline double norm(const Point3_<T>& pt)
{ return sqrt((double)pt.x*pt.x + (double)pt.y*pt.y + (double)pt.z*pt.z); }

template<typename T> static inline bool operator == (const Point3_<T>& a, const Point3_<T>& b)
{ return a.x == b.x && a.y == b.y && a.z == b.z; }

template<typename T> static inline Point3_<T> operator + (const Point3_<T>& a, const Point3_<T>& b)
{ return Point3_<T>( a.x + b.x, a.y + b.y, a.z + b.z ); }

template<typename T> static inline Point3_<T> operator - (const Point3_<T>& a, const Point3_<T>& b)
{ return Point3_<T>( a.x - b.x, a.y - b.y, a.z - b.z ); }

template<typename T> static inline Point3_<T> operator - (const Point3_<T>& a)
{ return Point3_<T>( -a.x, -a.y, -a.z ); }

template<typename T> static inline Point3_<T> operator * (const Point3_<T>& a, T b)
{ return Point3_<T>( a.x*b, a.y*b, a.z*b ); }

template<typename T> static inline Point3_<T> operator * (T a, const Point3_<T>& b)
{ return Point3_<T>( a*b.x, a*b.y, a*b.z ); }

template<> inline Point3_<int>::Point3_(const CvPoint3D32f& pt)
    : x(cvRound(pt.x)), y(cvRound(pt.y)), z(cvRound(pt.z)) {}

//////////////////////////////// Size ////////////////////////////////

template<typename T> inline Size_<T>::Size_()
    : width(0), height(0) {}
template<typename T> inline Size_<T>::Size_(T _width, T _height)
    : width(_width), height(_height) {}
template<typename T> inline Size_<T>::Size_(const Size_& sz)
    : width(sz.width), height(sz.height) {}
template<typename T> inline Size_<T>::Size_(const CvSize& sz)
    : width((T)sz.width), height((T)sz.height) {}
template<typename T> inline Size_<T>::Size_(const Point_<T>& pt) : width(pt.x), height(pt.y) {}

template<typename T> inline Size_<T>::operator Size_<int>() const
{ return Size_<int>(cvRound(width), cvRound(height)); }
template<typename T> inline Size_<T>::operator Size_<float>() const
{ return Size_<float>(float(width), float(height)); }
template<typename T> inline Size_<T>::operator Size_<double>() const
{ return Size_<double>(width, height); }
template<typename T> inline Size_<T>::operator CvSize() const
{ return cvSize(cvRound(width), cvRound(height)); }

template<typename T> inline Size_<T>& Size_<T>::operator = (const Size_<T>& sz)
{ width = sz.width; height = sz.height; return *this; }
template<typename T> inline T Size_<T>::area() const { return width*height; }

template<typename T> static inline Size_<T>& operator += (Size_<T>& a, const Size_<T>& b)
{ a.width += b.width; a.height += b.height; return a; }
template<typename T> static inline Size_<T>& operator -= (Size_<T>& a, const Size_<T>& b)
{ a.width -= b.width; a.height -= b.height; return a; }

template<typename T> static inline bool operator == (const Size_<T>& a, const Size_<T>& b)
{ return a.width == b.width && a.height == b.height; }
template<typename T> static inline bool operator != (const Size_<T>& a, const Size_<T>& b)
{ return a.width != b.width || a.height != b.height; }

template<> inline Size_<int>::operator CvSize() const { return cvSize(width,height); }

//////////////////////////////// Rect ////////////////////////////////


template<typename T> inline Rect_<T>::Rect_() : x(0), y(0), width(0), height(0) {}
template<typename T> inline Rect_<T>::Rect_(T _x, T _y, T _width, T _height) : x(_x), y(_y), width(_width), height(_height) {}
template<typename T> inline Rect_<T>::Rect_(const Rect_<T>& r) : x(r.x), y(r.y), width(r.width), height(r.height) {}
template<typename T> inline Rect_<T>::Rect_(const CvRect& r) : x((T)r.x), y((T)r.y), width((T)r.width), height((T)r.height) {}
template<typename T> inline Rect_<T>::Rect_(const Point_<T>& org, const Size_<T>& sz) :
    x(org.x), y(org.y), width(sz.width), height(sz.height) {}
template<typename T> inline Rect_<T>::Rect_(const Point_<T>& pt1, const Point_<T>& pt2)
{
    x = min(pt1.x, pt2.x); y = min(pt1.y, pt2.y);
    width = max(pt1.x, pt2.x) - x; height = max(pt1.y, pt2.y) - y;
}
template<typename T> inline Rect_<T>& Rect_<T>::operator = ( const Rect_<T>& r )
{ x = r.x; y = r.y; width = r.width; height = r.height; return *this; }

template<typename T> inline Point_<T> Rect_<T>::tl() const { return Point_<T>(x,y); }
template<typename T> inline Point_<T> Rect_<T>::br() const { return Point_<T>(x+width, y+height); }

template<typename T> static inline Rect_<T>& operator += ( Rect_<T>& a, const Point_<T>& b )
{ a.x += b.x; a.y += b.y; return a; }
template<typename T> static inline Rect_<T>& operator -= ( Rect_<T>& a, const Point_<T>& b )
{ a.x -= b.x; a.y -= b.y; return a; }

template<typename T> static inline Rect_<T>& operator += ( Rect_<T>& a, const Size_<T>& b )
{ a.width += b.width; a.height += b.height; return a; }

template<typename T> static inline Rect_<T>& operator -= ( Rect_<T>& a, const Size_<T>& b )
{ a.width -= b.width; a.height -= b.height; return a; }

template<typename T> static inline Rect_<T>& operator &= ( Rect_<T>& a, const Rect_<T>& b )
{
    T x1 = max(a.x, b.x), y1 = max(a.y, b.y);
    a.width = min(a.x + a.width, b.x + b.width) - x1;
    a.height = min(a.y + a.height, b.y + b.height) - y1;
    a.x = x1; a.y = y1;
    if( a.width <= 0 || a.height <= 0 )
        a = Rect();
    return a;
}

template<typename T> static inline Rect_<T>& operator |= ( Rect_<T>& a, const Rect_<T>& b )
{
    T x1 = min(a.x, b.x), y1 = min(a.y, b.y);
    a.width = max(a.x + a.width, b.x + b.width) - x1;
    a.height = max(a.y + a.height, b.y + b.height) - y1;
    a.x = x1; a.y = y1;
    return a;
}

template<typename T> inline Size_<T> Rect_<T>::size() const { return Size_<T>(width, height); }
template<typename T> inline T Rect_<T>::area() const { return width*height; }

template<typename T> inline Rect_<T>::operator Rect_<int>() const { return Rect_<int>(cvRound(x), cvRound(y), cvRound(width), cvRound(height)); }
template<typename T> inline Rect_<T>::operator Rect_<float>() const { return Rect_<float>(float(x), float(y), float(width), float(height)); }
template<typename T> inline Rect_<T>::operator Rect_<double>() const { return Rect_<double>(x, y, width, height); }
template<typename T> inline Rect_<T>::operator CvRect() const { return cvRect(cvRound(x), cvRound(y), cvRound(width), cvRound(height)); }

template<typename T> inline bool Rect_<T>::contains(const Point_<T>& pt) const
{ return x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height; }

template<typename T> static inline bool operator == (const Rect_<T>& a, const Rect_<T>& b)
{
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

template<typename T> static inline Rect_<T> operator + (const Rect_<T>& a, const Point_<T>& b)
{
    return Rect_<T>( a.x + b.x, a.y + b.y, a.width, a.height );
}

template<typename T> static inline Rect_<T> operator - (const Rect_<T>& a, const Point_<T>& b)
{
    return Rect_<T>( a.x - b.x, a.y - b.y, a.width, a.height );
}

template<typename T> static inline Rect_<T> operator + (const Rect_<T>& a, const Size_<T>& b)
{
    return Rect_<T>( a.x, a.y, a.width + b.width, a.height + b.height );
}

template<typename T> static inline Rect_<T> operator & (const Rect_<T>& a, const Rect_<T>& b)
{
    Rect_<T> c = a;
    return c &= b;
}

template<typename T> static inline Rect_<T> operator | (const Rect_<T>& a, const Rect_<T>& b)
{
    Rect_<T> c = a;
    return c |= b;
}

template<typename T> inline bool Point_<T>::inside( const Rect_<T>& r ) const
{
    return r.contains(*this);
}

template<> inline Rect_<int>::operator CvRect() const { return cvRect(x, y, width, height); }

inline RotatedRect::RotatedRect() { angle = 0; }
inline RotatedRect::RotatedRect(const Point2f& _center, const Size2f& _size, float _angle)
    : center(_center), size(_size), angle(_angle) {}

//////////////////////////////// Scalar_ ///////////////////////////////

template<typename T> inline Scalar_<T>::Scalar_()
{ this->val[0] = this->val[1] = this->val[2] = this->val[3] = 0; }

template<typename T> inline Scalar_<T>::Scalar_(T v0, T v1, T v2, T v3)
{ this->val[0] = v0; this->val[1] = v1; this->val[2] = v2; this->val[3] = v3; }

template<typename T> inline Scalar_<T>::Scalar_(const CvScalar& s)
{
    this->val[0] = saturate_cast<T>(s.val[0]);
    this->val[1] = saturate_cast<T>(s.val[1]);
    this->val[2] = saturate_cast<T>(s.val[2]);
    this->val[3] = saturate_cast<T>(s.val[3]);
}

template<typename T> inline Scalar_<T>::Scalar_(T v0)
{ this->val[0] = v0; this->val[1] = this->val[2] = this->val[3] = 0; }

template<typename T> inline Scalar_<T> Scalar_<T>::all(T v0)
{ return Scalar_<T>(v0, v0, v0, v0); }
template<typename T> inline Scalar_<T>::operator CvScalar() const
{ return cvScalar(this->val[0], this->val[1], this->val[2], this->val[3]); }

template<typename T> template<typename T2> inline Scalar_<T>::operator Scalar_<T2>() const
{
    return Scalar_<T2>(saturate_cast<T2>(this->val[0]),
                  saturate_cast<T2>(this->val[1]),
                  saturate_cast<T2>(this->val[2]),
                  saturate_cast<T2>(this->val[3]));
}

template<typename T> static inline Scalar_<T>& operator += (Scalar_<T>& a, const Scalar_<T>& b)
{
    a.val[0] = saturate_cast<T>(a.val[0] + b.val[0]);
    a.val[1] = saturate_cast<T>(a.val[1] + b.val[1]);
    a.val[2] = saturate_cast<T>(a.val[2] + b.val[2]);
    a.val[3] = saturate_cast<T>(a.val[3] + b.val[3]);
    return a;
}

template<typename T> static inline Scalar_<T>& operator -= (Scalar_<T>& a, const Scalar_<T>& b)
{
    a.val[0] = saturate_cast<T>(a.val[0] - b.val[0]);
    a.val[1] = saturate_cast<T>(a.val[1] - b.val[1]);
    a.val[2] = saturate_cast<T>(a.val[2] - b.val[2]);
    a.val[3] = saturate_cast<T>(a.val[3] - b.val[3]);
    return a;
}

template<typename T> static inline Scalar_<T>& operator *= ( Scalar_<T>& a, T v )
{
    a.val[0] = saturate_cast<T>(a.val[0] * v);
    a.val[1] = saturate_cast<T>(a.val[1] * v);
    a.val[2] = saturate_cast<T>(a.val[2] * v);
    a.val[3] = saturate_cast<T>(a.val[3] * v);
    return a;
}

template<typename T> inline Scalar_<T> Scalar_<T>::mul(const Scalar_<T>& t, double scale ) const
{
    return Scalar_<T>( saturate_cast<T>(this->val[0]*t.val[0]*scale),
                       saturate_cast<T>(this->val[1]*t.val[1]*scale),
                       saturate_cast<T>(this->val[2]*t.val[2]*scale),
                       saturate_cast<T>(this->val[3]*t.val[3]*scale));
}

template<typename T> static inline bool operator == ( const Scalar_<T>& a, const Scalar_<T>& b )
{
    return a.val[0] == b.val[0] && a.val[1] == b.val[1] && 
        a.val[2] == b.val[2] && a.val[3] == b.val[3];
}

template<typename T> static inline bool operator != ( const Scalar_<T>& a, const Scalar_<T>& b )
{
    return a.val[0] != b.val[0] || a.val[1] != b.val[1] || 
        a.val[2] != b.val[2] || a.val[3] != b.val[3];
}

template<typename T> template<typename T2> inline void Scalar_<T>::convertTo(T2* buf, int cn, int unroll_to) const
{
    int i;
    CV_Assert(cn <= 4);
    for( i = 0; i < cn; i++ )
        buf[i] = saturate_cast<T2>(this->val[i]);
    for( ; i < unroll_to; i++ )
        buf[i] = buf[i-cn];
}

static inline void scalarToRawData(const Scalar& s, void* buf, int type, int unroll_to=0)
{
    int depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    switch(depth)
    {
    case CV_8U:
        s.convertTo((uchar*)buf, cn, unroll_to);
        break;
    case CV_8S:
        s.convertTo((schar*)buf, cn, unroll_to);
        break;
    case CV_16U:
        s.convertTo((ushort*)buf, cn, unroll_to);
        break;
    case CV_16S:
        s.convertTo((short*)buf, cn, unroll_to);
        break;
    case CV_32S:
        s.convertTo((int*)buf, cn, unroll_to);
        break;
    case CV_32F:
        s.convertTo((float*)buf, cn, unroll_to);
        break;
    case CV_64F:
        s.convertTo((double*)buf, cn, unroll_to);
        break;
    default:
        CV_Error(CV_StsUnsupportedFormat,"");
    }
}


template<typename T> static inline Scalar_<T> operator + (const Scalar_<T>& a, const Scalar_<T>& b)
{
    return Scalar_<T>(saturate_cast<T>(a.val[0] + b.val[0]),
                      saturate_cast<T>(a.val[1] + b.val[1]),
                      saturate_cast<T>(a.val[2] + b.val[2]),
                      saturate_cast<T>(a.val[3] + b.val[3]));
}

template<typename T> static inline Scalar_<T> operator - (const Scalar_<T>& a, const Scalar_<T>& b)
{
    return Scalar_<T>(saturate_cast<T>(a.val[0] - b.val[0]),
                      saturate_cast<T>(a.val[1] - b.val[1]),
                      saturate_cast<T>(a.val[2] - b.val[2]),
                      saturate_cast<T>(a.val[3] - b.val[3]));
}

template<typename T> static inline Scalar_<T> operator * (const Scalar_<T>& a, T alpha)
{
    return Scalar_<T>(saturate_cast<T>(a.val[0] * alpha),
                      saturate_cast<T>(a.val[1] * alpha),
                      saturate_cast<T>(a.val[2] * alpha),
                      saturate_cast<T>(a.val[3] * alpha));
}

template<typename T> static inline Scalar_<T> operator * (T alpha, const Scalar_<T>& a)
{
    return a*alpha;
}

template<typename T> static inline Scalar_<T> operator - (const Scalar_<T>& a)
{
    return Scalar_<T>(saturate_cast<T>(-a.val[0]), saturate_cast<T>(-a.val[1]),
                      saturate_cast<T>(-a.val[2]), saturate_cast<T>(-a.val[3]));
}

//////////////////////////////// Range /////////////////////////////////

inline Range::Range() : start(0), end(0) {}
inline Range::Range(int _start, int _end) : start(_start), end(_end) {}
inline int Range::size() const { return end - start; }
inline bool Range::empty() const { return start == end; }
inline Range Range::all() { return Range(INT_MIN, INT_MAX); }

static inline bool operator == (const Range& r1, const Range& r2)
{ return r1.start == r2.start && r1.end == r2.end; }

static inline bool operator != (const Range& r1, const Range& r2)
{ return !(r1 == r2); }

static inline bool operator !(const Range& r)
{ return r.start == r.end; }

static inline Range operator & (const Range& r1, const Range& r2)
{
    Range r(std::max(r1.start, r2.start), std::min(r2.start, r2.end));
    r.end = std::max(r.end, r.start);
    return r;
}

static inline Range& operator &= (Range& r1, const Range& r2)
{
    r1 = r1 & r2;
    return r1;
}

static inline Range operator + (const Range& r1, int delta)
{
    return Range(r1.start + delta, r1.end + delta);
}

static inline Range operator + (int delta, const Range& r1)
{
    return Range(r1.start + delta, r1.end + delta);
}

static inline Range operator - (const Range& r1, int delta)
{
    return r1 + (-delta);
}


template <typename T> inline Vector<T>::Vector() {}
template <typename T> inline Vector<T>::Vector(size_t _size) { resize(_size); }
template <typename T> inline Vector<T>::Vector(size_t _size, const T& val)
{
    resize(_size);
    for(size_t i = 0; i < _size; i++)
        hdr.data[i] = val;
}
template <typename T> inline Vector<T>::Vector(T* _data, size_t _size, bool _copyData)
{ set(_data, _size, _copyData); }
template <typename T> inline Vector<T>::Vector(const std::vector<T>& vec, bool _copyData)
{ set(&vec[0], vec.size(), _copyData); }
template <typename T> inline Vector<T>::Vector(const Vector& d)
{ *this = d; }
template <typename T> inline Vector<T>::Vector(const Vector& d, const Range& r)
{
    if( r == Range::all() )
        r = Range(0, d.size());
    if( r.size() > 0 && r.start >= 0 && r.end <= d.size() )
    {
        if( d.hdr.refcount )
            ++*d.hdr.refcount;
        hdr.refcount = d.hdr.refcount;
        hdr.datastart = d.hdr.datastart;
        hdr.data = d.hdr.data + r.start;
        hdr.capacity = hdr.size = r.size();
    }
}

template <typename T> inline Vector<T>& Vector<T>::operator = (const Vector& d)
{
    if( this == &d )
        return *this;
    if( d.hdr.refcount )
        ++*d.hdr.refcount;
    release();
    hdr = d.hdr;
    return *this;
}

template <typename T> inline Vector<T>::~Vector() { release(); }
template <typename T> inline Vector<T> Vector<T>::clone() const
{
    return Vector(hdr.data, hdr.size, true);
}

template <typename T> inline T& Vector<T>::operator [] (size_t i) { assert( i < size() ); return hdr.data[i]; }
template <typename T> inline const T& Vector<T>::operator [] (size_t i) const { assert( i < size() ); return hdr.data[i]; }
template <typename T> inline T& Vector<T>::operator [] (int i) { assert( (size_t)i < size() ); return hdr.data[i]; }
template <typename T> inline const T& Vector<T>::operator [] (int i) const { assert( (size_t)i < size() ); return hdr.data[i]; }
template <typename T> inline Vector<T> Vector<T>::operator() (const Range& r) const { return Vector(*this, r); }
template <typename T> inline T& Vector<T>::back() { assert(!empty()); return hdr.data[hdr.size-1]; }
template <typename T> inline const T& Vector<T>::back() const { assert(!empty()); return hdr.data[hdr.size-1]; }
template <typename T> inline T& Vector<T>::front() { assert(!empty()); return hdr.data[0]; }
template <typename T> inline const T& Vector<T>::front() const { assert(!empty()); return hdr.data[0]; }

template <typename T> inline T* Vector<T>::begin() { return hdr.data; }
template <typename T> inline T* Vector<T>::end() { return hdr.data + hdr.size; }
template <typename T> inline const T* Vector<T>::begin() const { return hdr.data; }
template <typename T> inline const T* Vector<T>::end() const { return hdr.data + hdr.size; }

template <typename T> inline void Vector<T>::release()
{
    if( hdr.refcount && --*hdr.refcount == 0 )
        fastFree_<T>(hdr.datastart, hdr.capacity);
    hdr = Hdr();
}

template <typename T> inline void Vector<T>::set(T* _data, size_t _size, bool _copyData)
{
    if( !_copyData )
    {
        release();
        hdr.data = hdr.datastart = _data;
        hdr.size = hdr.capacity = _size;
        hdr.refcount = 0;
    }
    else
    {
        reserve(_size);
        for( size_t i = 0; i < _size; i++ )
            hdr.data[i] = _data[i];
        hdr.size = _size;
    }
}

template <typename T> inline void Vector<T>::reserve(size_t newCapacity)
{
    T* newData;
    int* newRefcount;
    size_t i, oldSize = hdr.size;
    if( (!hdr.refcount || *hdr.refcount == 1) && hdr.capacity >= newCapacity )
        return;
    newCapacity = std::max(newCapacity, oldSize);
    size_t datasize = alignSize(newCapacity*sizeof(T), (size_t)sizeof(*newRefcount));
    newData = (T*)fastMalloc(datasize + sizeof(*newRefcount));
    for( i = 0; i < newCapacity; i++ )
        ::new(newData + i) T();
    newRefcount = (int*)((uchar*)newData + datasize);
    *newRefcount = 1;
    for( i = 0; i < oldSize; i++ )
        newData[i] = hdr.data[i];
    release();
    hdr.data = hdr.datastart = newData;
    hdr.capacity = newCapacity;
    hdr.size = oldSize;
    hdr.refcount = newRefcount;
}

template <typename T> inline void Vector<T>::resize(size_t newSize)
{
    size_t i;
    newSize = std::max(newSize, (size_t)0);
    if( (!hdr.refcount || *hdr.refcount == 1) && hdr.size == newSize )
        return;
    reserve(newSize);
    for( i = hdr.size; i < newSize; i++ )
        hdr.data[i] = T();
    hdr.size = newSize;
}

template <typename T> inline Vector<T>& Vector<T>::push_back(const T& elem)
{
    if( hdr.size == hdr.capacity )
        reserve( std::max((size_t)8, hdr.capacity*3/2) );
    hdr.data[hdr.size++] = elem;
    return *this;
}
template <typename T> inline Vector<T>& Vector<T>::pop_back()
{
    if( hdr.size > 0 )
        --hdr.size;
    return *this;
}

template <typename T> inline size_t Vector<T>::size() const { return hdr.size; }
template <typename T> inline size_t Vector<T>::capacity() const { return hdr.capacity; }
template <typename T> inline bool Vector<T>::empty() const { return hdr.size == 0; }
template <typename T> inline void Vector<T>::clear() { resize(0); }

//////////////////////////////// Mat ////////////////////////////////

inline Mat::Mat()
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0) {}

inline Mat::Mat(int _rows, int _cols, int _type)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0)
{
    if( _rows > 0 && _cols > 0 )
        create( _rows, _cols, _type );
}

inline Mat::Mat(int _rows, int _cols, int _type, const Scalar& _s)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0),
    datastart(0), dataend(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

inline Mat::Mat(Size _size, int _type)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0),
    datastart(0), dataend(0)
{
    if( _size.height > 0 && _size.width > 0 )
        create( _size.height, _size.width, _type );
}

inline Mat::Mat(const Mat& m)
    : flags(m.flags), rows(m.rows), cols(m.cols), step(m.step), data(m.data),
    refcount(m.refcount), datastart(m.datastart), dataend(m.dataend)
{
    if( refcount )
        ++*refcount;
}

inline Mat::Mat(int _rows, int _cols, int _type, void* _data, int _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), rows(_rows), cols(_cols),
    step(_step), data((uchar*)_data), refcount(0),
    datastart((uchar*)_data), dataend((uchar*)_data)
{
    int minstep = cols*elemSize();
    if( step == AUTO_STEP )
    {
        step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) step = minstep;
        assert( step >= minstep );
        flags |= step == minstep ? CONTINUOUS_FLAG : 0;
    }
    dataend += step*(rows-1) + minstep;
}

inline Mat::Mat(Size _size, int _type, void* _data, int _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), rows(_size.height), cols(_size.width),
    step(_step), data((uchar*)_data), refcount(0),
    datastart((uchar*)_data), dataend((uchar*)_data)
{
    int minstep = cols*elemSize();
    if( step == AUTO_STEP )
    {
        step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) step = minstep;
        assert( step >= minstep );
        flags |= step == minstep ? CONTINUOUS_FLAG : 0;
    }
    dataend += step*(rows-1) + minstep;
}

inline Mat::Mat(const Mat& m, const Range& rowRange, const Range& colRange)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;

    if( rowRange == Range::all() )
        rows = m.rows;
    else
    {
        CV_Assert( 0 <= rowRange.start && rowRange.start <= rowRange.end && rowRange.end <= m.rows );
        rows = rowRange.size();
        data += step*rowRange.start;
    }

    if( colRange == Range::all() )
        cols = m.cols;
    else
    {
        CV_Assert( 0 <= rowRange.start && rowRange.start <= rowRange.end && rowRange.end <= m.rows );
        cols = colRange.size();
        data += colRange.start*elemSize();
        flags &= cols < m.cols ? ~CONTINUOUS_FLAG : -1;
    }

    if( refcount )
        ++*refcount;
    if( rows <= 0 || cols <= 0 )
        rows = cols = 0;
}

inline Mat::Mat(const Mat& m, const Rect& roi)
    : flags(m.flags), rows(roi.height), cols(roi.width),
    step(m.step), data(m.data + roi.y*step), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend)
{
    flags &= roi.width < m.cols ? ~CONTINUOUS_FLAG : -1;
    data += roi.x*elemSize();
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
        0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    if( refcount )
        ++*refcount;
    if( rows <= 0 || cols <= 0 )
        rows = cols = 0;
}

inline Mat::Mat(const CvMat* m, bool copyData)
    : flags(MAGIC_VAL + (m->type & (CV_MAT_TYPE_MASK|CV_MAT_CONT_FLAG))),
    rows(m->rows), cols(m->cols), step(m->step), data(m->data.ptr), refcount(0),
    datastart(m->data.ptr), dataend(m->data.ptr)
{
    if( step == 0 )
        step = cols*elemSize();
    int minstep = cols*elemSize();
    dataend += step*(rows-1) + minstep;
    if( copyData )
    {
        refcount = 0;
        data = datastart = dataend = 0;
        Mat(m->rows, m->cols, m->type, m->data.ptr, m->step).copyTo(*this);
    }
}

inline Mat::~Mat()
{
    release();
}

inline Mat& Mat::operator = (const Mat& m)
{
    if( this != &m )
    {
        if( m.refcount )
            ++*m.refcount;
        release();
        flags = m.flags;
        rows = m.rows; cols = m.cols;
        step = m.step; data = m.data;
        datastart = m.datastart; dataend = m.dataend;
        refcount = m.refcount;
    }
    return *this;
}

inline Mat Mat::row(int y) const { return Mat(*this, Range(y, y+1), Range::all()); }
inline Mat Mat::col(int x) const { return Mat(*this, Range::all(), Range(x, x+1)); }
inline Mat Mat::rowRange(int startrow, int endrow) const
    { return Mat(*this, Range(startrow, endrow), Range::all()); }
inline Mat Mat::rowRange(const Range& r) const
    { return Mat(*this, r, Range::all()); }
inline Mat Mat::colRange(int startcol, int endcol) const
    { return Mat(*this, Range::all(), Range(startcol, endcol)); }
inline Mat Mat::colRange(const Range& r) const
    { return Mat(*this, Range::all(), r); }

inline Mat Mat::diag(int d) const
{
    Mat m = *this;
    int esz = elemSize();
    int len;

    if( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    else
    {
        len = std::min(rows + d, cols);
        m.data -= step*d;
    }
    if( len <= 0 )
    {
        assert(0);
        return Mat();
    }
    m.rows = len;
    m.cols = 1;
    m.step += esz;
    if( m.rows > 1 )
        m.flags &= ~CONTINUOUS_FLAG;
    else
        m.flags |= CONTINUOUS_FLAG;
    return m;
}

inline Mat Mat::diag(const Mat& d)
{
    Mat m(d.rows, d.rows, d.type(), Scalar(0)), md = m.diag();
    d.copyTo(md);
    return m;
}

inline Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    return m;
}

inline void Mat::assignTo( Mat& m, int type ) const
{
    if( type < 0 )
        m = *this;
    else
        convertTo(m, type);
}

inline void Mat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( rows == _rows && cols == _cols && type() == _type )
        return;
    if( data )
        release();
    assert( _rows >= 0 && _cols >= 0 );
    if( _rows > 0 && _cols > 0 )
    {
        flags = MAGIC_VAL + CONTINUOUS_FLAG + _type;
        rows = _rows;
        cols = _cols;
        step = elemSize()*cols;
        size_t nettosize = (size_t)step*rows;
        size_t datasize = alignSize(nettosize, (int)sizeof(*refcount));
        datastart = data = (uchar*)fastMalloc(datasize + sizeof(*refcount));
        dataend = data + nettosize;
        refcount = (int*)(data + datasize);
        *refcount = 1;
    }
}

inline void Mat::create(Size _size, int _type)
{
    create(_size.height, _size.width, _type);
}

inline void Mat::release()
{
    if( refcount && --*refcount == 0 )
        fastFree(datastart);
    data = datastart = dataend = 0;
    step = rows = cols = 0;
    refcount = 0;
}

inline void Mat::locateROI( Size& wholeSize, Point& ofs ) const
{
    int esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;
    assert( step > 0 );
    if( delta1 == 0 )
        ofs.x = ofs.y = 0;
    else
    {
        ofs.y = (int)(delta1/step);
        ofs.x = (int)((delta1 - step*ofs.y)/esz);
        assert( data == datastart + ofs.y*step + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (int)((delta2 - minstep)/step + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (delta2 - step*(wholeSize.height-1))/esz;
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

inline Mat& Mat::adjustROI( int dtop, int dbottom, int dleft, int dright )
{
    Size wholeSize; Point ofs;
    int esz = elemSize();
    locateROI( wholeSize, ofs );
    int row1 = std::max(ofs.y - dtop, 0), row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);
    int col1 = std::max(ofs.x - dleft, 0), col2 = std::min(ofs.x + cols + dright, wholeSize.width);
    data += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    if( esz*cols == step )
        flags |= CONTINUOUS_FLAG;
    else
        flags &= ~CONTINUOUS_FLAG;
    return *this;
}

inline Mat Mat::operator()( Range rowRange, Range colRange ) const
{
    return Mat(*this, rowRange, colRange);
}

inline Mat Mat::operator()( const Rect& roi ) const
{ return Mat(*this, roi); }

inline Mat::operator CvMat() const
{
    CvMat m = cvMat(rows, cols, type(), data);
    m.step = step;
    m.type = (m.type & ~CONTINUOUS_FLAG) | (flags & CONTINUOUS_FLAG);
    return m;
}

inline bool Mat::isContinuous() const { return (flags & CONTINUOUS_FLAG) != 0; }
inline int Mat::elemSize() const { return CV_ELEM_SIZE(flags); }
inline int Mat::elemSize1() const { return CV_ELEM_SIZE1(flags); }
inline int Mat::type() const { return CV_MAT_TYPE(flags); }
inline int Mat::depth() const { return CV_MAT_DEPTH(flags); }
inline int Mat::channels() const { return CV_MAT_CN(flags); }
inline int Mat::step1() const { return step/elemSize1(); }
inline Size Mat::size() const { return Size(cols, rows); }

inline uchar* Mat::ptr(int y)
{
    assert( (unsigned)y < (unsigned)rows );
    return data + step*y;
}

inline const uchar* Mat::ptr(int y) const
{
    assert( (unsigned)y < (unsigned)rows );
    return data + step*y;
}

static inline void swap( Mat& a, Mat& b )
{
    std::swap( a.flags, b.flags );
    std::swap( a.rows, b.rows ); std::swap( a.cols, b.cols );
    std::swap( a.step, b.step ); std::swap( a.data, b.data );
    std::swap( a.datastart, b.datastart );
    std::swap( a.dataend, b.dataend );
    std::swap( a.refcount, b.refcount );
}

// Multiply-with-Carry RNG
inline RNG::RNG() { state = 0xffffffff; }
inline RNG::RNG(unsigned seed) { state = seed ? seed : 0xffffffff; }
inline RNG::RNG(uint64 _state) { state = _state ? _state : 0xffffffff; }
inline unsigned RNG::next()
{
    state = (uint64)(unsigned)state*A + (unsigned)(state >> 32);
    return (unsigned)state;
}

inline RNG::operator uchar() { return (uchar)next(); }
inline RNG::operator schar() { return (schar)next(); }
inline RNG::operator ushort() { return (ushort)next(); }
inline RNG::operator short() { return (short)next(); }
inline RNG::operator unsigned() { return next(); }
inline RNG::operator int() { return (int)next(); }
// * (2^32-1)^-1
inline RNG::operator float() { return next()*2.3283064365386962890625e-10f; }
inline RNG::operator double()
{
    unsigned t = next();
    return (((uint64)t << 32) | next())*5.4210108624275221700372640043497e-20;
}

inline TermCriteria::TermCriteria() : type(0), maxCount(0), epsilon(0) {}
inline TermCriteria::TermCriteria(int _type, int _maxCount, double _epsilon)
    : type(_type), maxCount(_maxCount), epsilon(_epsilon) {}

inline SVD::SVD() {}
inline SVD::SVD( const Mat& m, int flags ) { operator ()(m, flags); }
inline void SVD::solveZ( const Mat& m, Mat& dst )
{
    SVD svd(m);
    svd.vt.row(svd.vt.rows-1).reshape(1,svd.vt.cols).copyTo(dst);
}

inline uchar* LineIterator::operator *() { return ptr; }
inline LineIterator& LineIterator::operator ++()
{
    int mask = err < 0 ? -1 : 0;
    err += minusDelta + (plusDelta & mask);
    ptr += minusStep + (plusStep & mask);
    return *this;
}
inline LineIterator LineIterator::operator ++(int)
{
    LineIterator it = *this;
    ++(*this);
    return it;
}

///////////////////////////////// Mat_<T> ////////////////////////////////////

template<typename T> inline Mat_<T>::Mat_() :
Mat() { flags = (flags & ~CV_MAT_TYPE_MASK) | DataType<T>::type; }
template<typename T> inline Mat_<T>::Mat_(int _rows, int _cols) :
    Mat(_rows, _cols, DataType<T>::type) {}

template<typename T> inline Mat_<T>::Mat_(int _rows, int _cols, const T& value) :
    Mat(_rows, _cols, DataType<T>::type) { *this = value; }

template<typename T> inline Mat_<T>::Mat_(const Mat& m) : Mat()
{ flags = (flags & ~CV_MAT_TYPE_MASK) | DataType<T>::type; *this = m; }

template<typename T> inline Mat_<T>::Mat_(const Mat_& m) : Mat(m) {}

template<typename T> inline Mat_<T>::Mat_(int _rows, int _cols, T* _data, int _step)
    : Mat(_rows, _cols, DataType<T>::type, _data, _step) {}

template<typename T> inline Mat_<T>::Mat_(const Mat_& m, const Range& rowRange, const Range& colRange)
    : Mat(m, rowRange, colRange) {}

template<typename T> inline Mat_<T>::Mat_(const Mat_& m, const Rect& roi)
    : Mat(m, roi) {}

template<typename T> inline Mat_<T>::~Mat_() { release(); }

template<typename T> inline Mat_<T>& Mat_<T>::operator = (const Mat& m)
{
    if( DataType<T>::type == m.type() )
    {
        Mat::operator = (m);
        return *this;
    }
    if( DataType<T>::depth == m.depth() )
    {
        return (*this = m.reshape(DataType<T>::channels));
    }
    assert(DataType<T>::channels == m.channels());
    m.convertTo(*this, type());
    return *this;
}

template<typename T> inline Mat_<T>& Mat_<T>::operator = (const Mat_& m)
{
    Mat::operator=(m);
    return *this;
}

template<typename T> inline Mat_<T>& Mat_<T>::operator = (const T& s)
{
    Mat::operator=(Scalar(s));
    return *this;
}

template<typename T> inline Mat_<T>& Mat_<T>::create(int _rows, int _cols)
{
    Mat::create(_rows, _cols, DataType<T>::type);
    return *this;
}

template<typename T> inline Mat_<T>& Mat_<T>::create(Size _size)
{
    Mat::create(_size, DataType<T>::type);
    return *this;
}

template<typename T> inline Mat_<T> Mat_<T>::cross(const Mat_& m) const
{ return Mat_<T>(Mat::cross(m)); }

template<typename T> template<typename T2> inline Mat_<T>::operator Mat_<T2>() const
{ return Mat_<T2>(*this); }

template<typename T> inline Mat_<T> Mat_<T>::row(int y) const
{ return Mat_(*this, Range(y, y+1), Range::all()); }
template<typename T> inline Mat_<T> Mat_<T>::col(int x) const
{ return Mat_(*this, Range::all(), Range(x, x+1)); }
template<typename T> inline Mat_<T> Mat_<T>::diag(int d) const
{ return Mat_(Mat::diag(d)); }
template<typename T> inline Mat_<T> Mat_<T>::clone() const
{ return Mat_(Mat::clone()); }

template<typename T> inline int Mat_<T>::elemSize() const
{
    assert( Mat::elemSize() == (int)sizeof(T) );
    return sizeof(T);
}

template<typename T> inline int Mat_<T>::elemSize1() const
{
    assert( Mat::elemSize1() == (int)sizeof(T)/DataType<T>::channels );
    return sizeof(T)/DataType<T>::channels;
}
template<typename T> inline int Mat_<T>::type() const
{
    assert( Mat::type() == DataType<T>::type );
    return DataType<T>::type;
}
template<typename T> inline int Mat_<T>::depth() const
{
    assert( Mat::depth() == DataType<T>::depth );
    return DataType<T>::depth;
}
template<typename T> inline int Mat_<T>::channels() const
{
    assert( Mat::channels() == DataType<T>::channels );
    return DataType<T>::channels;
}
template<typename T> inline int Mat_<T>::stepT() const { return step/elemSize(); }
template<typename T> inline int Mat_<T>::step1() const { return step/elemSize1(); }

template<typename T> inline Mat_<T> Mat_<T>::reshape(int _rows) const
{ return Mat_<T>(Mat::reshape(0,_rows)); }

template<typename T> inline Mat_<T>& Mat_<T>::adjustROI( int dtop, int dbottom, int dleft, int dright )
{ return (Mat_<T>&)(Mat::adjustROI(dtop, dbottom, dleft, dright));  }

template<typename T> inline Mat_<T> Mat_<T>::operator()( const Range& rowRange, const Range& colRange ) const
{ return Mat_<T>(*this, rowRange, colRange); }

template<typename T> inline Mat_<T> Mat_<T>::operator()( const Rect& roi ) const
{ return Mat_<T>(roi); }

template<typename T> inline T* Mat_<T>::operator [](int y)
{ return (T*)ptr(y); }
template<typename T> inline const T* Mat_<T>::operator [](int y) const
{ return (const T*)ptr(y); }

template<typename T> inline T& Mat_<T>::operator ()(int row, int col)
{
    assert( (unsigned)col < (unsigned)cols );
    return (*this)[row][col];
}

template<typename T> inline T Mat_<T>::operator ()(int row, int col) const
{
    assert( (unsigned)col < (unsigned)cols );
    return (*this)[row][col];
}


template<typename T1, typename T2, typename Op> inline void
process( const Mat_<T1>& m1, Mat_<T2>& m2, Op op )
{
    int y, x, rows = m1.rows, cols = m1.cols;
    int c1 = m1.channels(), c2 = m2.channels();

    assert( m1.size() == m2.size() );

    for( y = 0; y < rows; y++ )
    {
        const T1* src = m1[y];
        T2* dst = m2[y];

        for( x = 0; x < cols; x++ )
            dst[x] = op(src[x]);
    }
}

template<typename T1, typename T2, typename T3, typename Op> inline void
process( const Mat_<T1>& m1, const Mat_<T2>& m2, Mat_<T3>& m3, Op op )
{
    int y, x, rows = m1.rows, cols = m1.cols;

    assert( m1.size() == m2.size() );

    for( y = 0; y < rows; y++ )
    {
        const T1* src1 = m1[y];
        const T2* src2 = m2[y];
        T3* dst = m3[y];

        for( x = 0; x < cols; x++ )
            dst[x] = op( src1[x], src2[x] );
    }
}

struct CV_EXPORTS MatExpr_Base
{
    MatExpr_Base() {}
    virtual ~MatExpr_Base() {}
    virtual void assignTo(Mat& m, int type=-1) const = 0;
};

template<typename E, typename M> struct CV_EXPORTS MatExpr_ : MatExpr_Base
{
    MatExpr_(const E& _e) : e(_e) {}
    ~MatExpr_() {}
    operator M() const { return (M)e; }
    void assignTo(Mat& m, int type=-1) const { e.assignTo(m, type); }

    M row(int y) const { return ((M)e).row(y); }
    M col(int x) const { return ((M)e).col(x); }
    M diag(int d=0) const { return ((M)e).diag(d); }

    M operator()( const Range& rowRange, const Range& colRange ) const
    { return ((M)e)(rowRange, colRange); }
    M operator()( const Rect& roi ) const { return ((M)e)(roi); }

    M cross(const M& m) const { return ((M)e).cross(m); }
    double dot(const M& m) const { return ((M)e).dot(m); }

    MatExpr_<MatExpr_Op2_<M, double, M, MatOp_T_<Mat> >, M> t() const
    { return ((M)e).t(); }
    MatExpr_<MatExpr_Op2_<M, int, M, MatOp_Inv_<Mat> >, M> inv(int method=DECOMP_LU) const
    { return ((M)e).inv(method); }

    MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
    mul(const M& m, double scale=1) const
    { return ((M)e).mul(m, scale); }
    template<typename A> MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M >
    mul(const MatExpr_<A, M>& m, double scale=1) const
    { return ((M)e).mul(m, scale); }

    E e;
};


inline Mat::Mat(const MatExpr_Base& expr)
 : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0)
{
    expr.assignTo(*this);
}

inline Mat& Mat::operator = (const MatExpr_Base& expr)
{
    expr.assignTo(*this);
    return *this;
}

template<typename T> inline Mat_<T>::Mat_(const MatExpr_Base& e) : Mat()
{
    e.assignTo(*this, DataType<T>::type);
}

template<typename T> inline Mat_<T>& Mat_<T>::operator = (const MatExpr_Base& e)
{
    e.assignTo(*this, DataType<T>::type);
    return *this;
}

template<typename T> inline Mat_<T>::operator MatExpr_<Mat_<T>, Mat_<T> >() const
{ return MatExpr_<Mat_<T>, Mat_<T> >(*this); }

inline Mat::operator MatExpr_<Mat, Mat>() const
{ return MatExpr_<Mat, Mat>(*this); }

template<typename M> struct CV_EXPORTS MatOp_Sub_
{
    MatOp_Sub_() {}

    static void apply(const M& a, const M& b, M& c, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            subtract( a, b, c );
        }
        else
        {
            Mat temp;
            apply(a, b, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_Scale_
{
    MatOp_Scale_() {}

    static void apply(const M& a, double alpha, M& c, int type=-1)
    {
        a.convertTo(c, type, alpha, 0);
    }
};

template<typename M> struct CV_EXPORTS MatOp_ScaleAddS_
{
    MatOp_ScaleAddS_() {}

    static void apply(const M& a, double alpha, double beta, M& c, int type=-1)
    {
        a.convertTo(c, type, alpha, beta);
    }
};

template<typename M> struct CV_EXPORTS MatOp_AddS_
{
    MatOp_AddS_() {}

    static void apply(const M& a, const Scalar& s, M& c, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            add(a, s, c);
        }
        else
        {
            Mat temp;
            apply(a, s, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_AddEx_
{
    MatOp_AddEx_() {}

    static void apply(const M& a, double alpha, const M& b,
                      double beta, double gamma, M& c, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            addWeighted(a, alpha, b, beta, gamma, c);
        }
        else
        {
            Mat temp;
            apply(a, alpha, b, beta, gamma, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_Bin_
{
    MatOp_Bin_() {}

    static void apply(const M& a, const M& b, int _op, M& c, int type=-1)
    {
        char op = (char)_op;
        if( type == a.type() || type < 0 )
        {
            if( op == '&' )
                bitwise_and( a, b, c );
            else if( op == '|' )
                bitwise_or( a, b, c );
            else if( op == '^' )
                bitwise_xor( a, b, c );
            else if( op == 'm' )
                min( a, b, c );
            else if( op == 'M' )
                max( a, b, c );
            else if( op == 'a' )
                absdiff( a, b, c );
            else
                assert(0);
        }
        else
        {
            Mat temp;
            apply(a, b, op, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_BinS_
{
    MatOp_BinS_() {}

    static void apply(const M& a, const Scalar& s, int _op, M& c, int type=-1)
    {
        char op = (char)_op;
        if( type == a.type() || type < 0 )
        {
            if( op == '&' )
                bitwise_and( a, s, c );
            else if( op == '|' )
                bitwise_or( a, s, c );
            else if( op == '^' )
                bitwise_xor( a, s, c );
            else if( op == 'm' )
                min( a, s[0], c );
            else if( op == 'M' )
                max( a, s[0], c );
            else if( op == 'a' )
                absdiff( a, s, c );
            else if( op == '~' )
                bitwise_not( a, c );
            else
                assert(0);
        }
        else
        {
            Mat temp;
            apply(a, s, op, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_T_
{
    MatOp_T_() {}

    static void apply(const M& a, double scale, M& c, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            transpose(a, c);
            if( fabs(scale - 1) > DBL_EPSILON )
                c.convertTo(c, -1, scale, 0);
        }
        else
        {
            Mat temp;
            apply(a, scale, temp);
            temp.convertTo(c, type);
        }
    }
};


template<typename M> struct CV_EXPORTS MatOp_MatMul_
{
    MatOp_MatMul_() {}

    static void apply(const M& a, const M& b, double scale, int flags, M& c, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            gemm(a, b, scale, Mat(), 0, c, flags);
        }
        else
        {
            Mat temp;
            apply(a, b, scale, flags, temp);
            temp.convertTo(c, type);
        }
    }
};


template<typename M> struct CV_EXPORTS MatOp_MatMulAdd_
{
    MatOp_MatMulAdd_() {}

    static void apply(const M& a, const M& b, double alpha,
        const M& c, double beta, int flags, M& d, int type=-1)
    {
        if( type == a.type() || type < 0 )
        {
            gemm(a, b, alpha, c, beta, d, flags);
        }
        else
        {
            Mat temp;
            apply(a, b, alpha, c, beta, flags, temp);
            temp.convertTo(d, type);
        }
    }
};


template<typename M> struct CV_EXPORTS MatOp_Cmp_
{
    MatOp_Cmp_() {}

    static void apply(const M& a, const M& b, int op, M& c, int type=-1)
    {
        if( type == CV_8UC1 || type == -1 )
        {
            compare(a, b, c, op);
        }
        else
        {
            Mat temp;
            apply(a, b, op, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_CmpS_
{
    MatOp_CmpS_() {}

    static void apply(const M& a, double alpha, int op, M& c, int type=-1)
    {
        if( type == CV_8UC1 || type == -1 )
        {
            compare(a, alpha, c, op);
        }
        else
        {
            Mat temp;
            apply(a, alpha, op, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_MulDiv_
{
    MatOp_MulDiv_() {}

    static void apply(const M& a, const M& b, double alpha, char op, M& c, int type=-1)
    {
        if( type == a.type() || type == -1 )
        {
            if( op == '*' )
                multiply( a, b, c, alpha );
            else
                divide( a, b, c, alpha );
        }
        else
        {
            Mat temp;
            apply(a, b, alpha, op, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_DivRS_
{
    MatOp_DivRS_() {}

    static void apply(const M& a, double alpha, M& c, int type=-1)
    {
        if( type == a.type() || type == -1 )
        {
            c.create(a.rows, a.cols, a.type());
            divide( alpha, a, c );
        }
        else
        {
            Mat temp;
            apply(a, alpha, temp);
            temp.convertTo(c, type);
        }
    }
};


template<typename M> struct CV_EXPORTS MatOp_Inv_
{
    MatOp_Inv_() {}

    static void apply(const M& a, int method, M& c, int type=-1)
    {
        if( type == a.type() || type == -1 )
        {
            invert(a, c, method);
        }
        else
        {
            Mat temp;
            apply(a, method, temp);
            temp.convertTo(c, type);
        }
    }
};


template<typename M> struct CV_EXPORTS MatOp_Solve_
{
    MatOp_Solve_() {}

    static void apply(const M& a, const M& b, int method, M& c, int type=-1)
    {
        if( type == a.type() || type == -1 )
        {
            solve(a, b, c, method);
        }
        else
        {
            Mat temp;
            apply(a, b, method, temp);
            temp.convertTo(c, type);
        }
    }
};

template<typename M> struct CV_EXPORTS MatOp_Set_
{
    MatOp_Set_() {}

    static void apply(Size size, int type0, const Scalar& s, int mtype, M& c, int type=-1)
    {
        if( type < 0 )
            type = type0;
        c.create(size.height, size.width, type);
        if( mtype == 0 )
            c = Scalar(0);
        else if( mtype == 1 )
            c = s;
        else if( mtype == 2 )
            setIdentity(c, s);
    }
};

template<typename A1, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op1_
{
    MatExpr_Op1_(const A1& _a1) : a1(_a1) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1;
};

template<typename A1, typename A2, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op2_
{
    MatExpr_Op2_(const A1& _a1, const A2& _a2) : a1(_a1), a2(_a2) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, a2, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1; A2 a2;
};

template<typename A1, typename A2, typename A3, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op3_
{
    MatExpr_Op3_(const A1& _a1, const A2& _a2, const A3& _a3) : a1(_a1), a2(_a2), a3(_a3) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, a2, a3, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1; A2 a2; A3 a3;
};

template<typename A1, typename A2, typename A3, typename A4, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op4_
{
    MatExpr_Op4_(const A1& _a1, const A2& _a2, const A3& _a3, const A4& _a4)
        : a1(_a1), a2(_a2), a3(_a3), a4(_a4) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, a2, a3, a4, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1; A2 a2; A3 a3; A4 a4;
};

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op5_
{
    MatExpr_Op5_(const A1& _a1, const A2& _a2, const A3& _a3, const A4& _a4, const A5& _a5)
        : a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, a2, a3, a4, a5, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5;
};

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename M, typename Op>
struct CV_EXPORTS MatExpr_Op6_
{
    MatExpr_Op6_(const A1& _a1, const A2& _a2, const A3& _a3,
                    const A4& _a4, const A5& _a5, const A6& _a6)
        : a1(_a1), a2(_a2), a3(_a3), a4(_a4), a5(_a5), a6(_a6) {}
    void assignTo(Mat& m, int type=-1) const { Op::apply(a1, a2, a3, a4, a5, a6, (M&)m, type); }
    operator M() const { M result; assignTo(result); return result; }

    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6;
};

///////////////////////////////// Arithmetical Operations ///////////////////////////////////

// A + B
static inline MatExpr_<MatExpr_Op5_<Mat, double, Mat, double, double, Mat, MatOp_AddEx_<Mat> >, Mat>
operator + (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op5_<Mat, double, Mat, double, double, Mat, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, 1, b, 1, 0));
}

template<typename T> static inline
MatExpr_<MatExpr_Op5_<Mat_<T>, double, Mat_<T>,
double, double, Mat_<T>, MatOp_AddEx_<Mat> >, Mat_<T> >
operator + (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op5_<Mat_<T>, double, Mat_<T>, double, double, Mat_<T>, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, 1, b, 1, 0));
}

// E1 + E2
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<M, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b )
{
    typedef MatExpr_Op5_<M, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, 1, (M)b, 1, 0));
}

// A - B
static inline MatExpr_<MatExpr_Op2_<Mat, Mat, Mat, MatOp_Sub_<Mat> >, Mat>
operator - (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op2_<Mat, Mat, Mat, MatOp_Sub_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b));
}

template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, Mat_<T>, Mat_<T>, MatOp_Sub_<Mat> >, Mat_<T> >
operator - (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op2_<Mat_<T>, Mat_<T>, Mat_<T>, MatOp_Sub_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, b));
}

// E1 - E2
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op2_<M, M, M, MatOp_Sub_<Mat> >, M>
operator - (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b )
{
    typedef MatExpr_Op2_<M, M, M, MatOp_Sub_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, (M)b));
}

// -(E1 - E2)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op2_<B, A, M, MatOp_Sub_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, B, M, MatOp_Sub_<Mat> >, M>& a )
{
    typedef MatExpr_Op2_<B, A, M, MatOp_Sub_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a2, a.e.a1));
}

// (A - B)*alpha
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op2_<A, B, M, MatOp_Sub_<Mat> >, M>& a,
            double alpha)
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, alpha, a.e.a2, -alpha, 0));
}

// alpha*(A - B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator * (double alpha,
            const MatExpr_<MatExpr_Op2_<A, B, M, MatOp_Sub_<Mat> >, M>& a)
{ return a*alpha; }


// A*alpha
static inline
MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>
operator * (const Mat& a, double alpha)
{
    typedef MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha));
}

// A*alpha
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> >, Mat_<T> >
operator * (const Mat_<T>& a, double alpha)
{
    typedef MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, alpha));
}

// alpha*A
static inline
MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>
operator * (double alpha, const Mat& a)
{ return a*alpha; }

// alpha*A
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> >, Mat_<T> >
operator * (double alpha, const Mat_<T>& a)
{ return a*alpha; }

// A/alpha
static inline
MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>
operator / (const Mat& a, double alpha)
{ return a*(1./alpha); }

// A/alpha
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> >, Mat_<T> >
operator / (const Mat_<T>& a, double alpha)
{ return a*(1./alpha); }

// -A
static inline
MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>
operator - (const Mat& a)
{ return a*(-1); }

// -A
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> >, Mat_<T> >
operator - (const Mat_<T>& a)
{ return a*(-1); }

// E*alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_Scale_<Mat> >, M>
operator * (const MatExpr_<A, M>& a, double alpha)
{
    typedef MatExpr_Op2_<M, double, M, MatOp_Scale_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, alpha));
}

// alpha*E
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_Scale_<Mat> >, M>
operator * (double alpha, const MatExpr_<A, M>& a)
{ return a*alpha; }

// E/alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_Scale_<Mat> >, M>
operator / (const MatExpr_<A, M>& a, double alpha)
{ return a*(1./alpha); }

// (E*alpha)*beta ~ E*(alpha*beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            double beta)
{ return a.e.a1*(a.e.a2*beta); }

// beta*(E*alpha) ~ E*(alpha*beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>
operator * (double beta,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return a.e.a1*(a.e.a2*beta); }

// (E*alpha)/beta ~ E*(alpha/beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>
operator / (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            double beta)
{ return a.e.a1*(a.e.a2/beta); }

// -E ~ E*(-1)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<MatExpr_<A, M>, double, M, MatOp_Scale_<Mat> >, M>
operator - (const MatExpr_<A, M>& a)
{ return a*(-1); }

// -(E*alpha) ~ E*(-alpha)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return a.e.a1*(-a.e.a2); }

// A + alpha
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>, MatOp_ScaleAddS_<Mat> >, Mat_<T> >
operator + (const Mat_<T>& a, double alpha)
{
    typedef MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>,
        MatOp_ScaleAddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, 1, alpha));
}

// A + alpha
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, Scalar, Mat_<T>, MatOp_AddS_<Mat> >, Mat_<T> >
operator + (const Mat_<T>& a, const Scalar& alpha)
{
    typedef MatExpr_Op2_<Mat_<T>, Scalar, Mat_<T>,
        MatOp_AddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, alpha));
}

// alpha + A
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>, MatOp_ScaleAddS_<Mat> >, Mat_<T> >
operator + (double alpha, const Mat_<T>& a)
{ return a + alpha; }

// alpha + A
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, Scalar, Mat_<T>, MatOp_AddS_<Mat> >, Mat_<T> >
operator + (const Scalar& alpha, const Mat_<T>& a)
{ return a + alpha; }

// A - alpha
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>, MatOp_ScaleAddS_<Mat> >, Mat_<T> >
operator - (const Mat_<T>& a, double alpha)
{ return a + (-alpha); }

// A - alpha
template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, Scalar, Mat_<T>, MatOp_AddS_<Mat> >, Mat_<T> >
operator - (const Mat_<T>& a, const Scalar& alpha)
{ return a + (-alpha); }

// alpha - A
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>, MatOp_ScaleAddS_<Mat> >, Mat_<T> >
operator - (double alpha, const Mat_<T>& a)
{
    typedef MatExpr_Op3_<Mat_<T>, double, double, Mat_<T>,
        MatOp_ScaleAddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, -1, alpha));
}

// E + alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (const MatExpr_<A, M>& a, double alpha)
{
    typedef MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, 1, alpha));
}

// E + alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, Scalar, M, MatOp_AddS_<Mat> >, M>
operator + (const MatExpr_<A, M>& a, const Scalar& alpha)
{
    typedef MatExpr_Op2_<M, Scalar, M, MatOp_AddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, alpha));
}

// alpha + E
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (double alpha, const MatExpr_<A, M>& a)
{ return a + alpha; }

// alpha + E
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, Scalar, M, MatOp_AddS_<Mat> >, M>
operator + (const Scalar& alpha, const MatExpr_<A, M>& a)
{ return a + alpha; }

// E - alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (const MatExpr_<A, M>& a, double alpha)
{ return a + (-alpha); }

// E - alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, Scalar, M, MatOp_AddS_<Mat> >, M>
operator - (const MatExpr_<A, M>& a, const Scalar& alpha)
{ return a + (-alpha); }

// alpha - E
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (double alpha, const MatExpr_<A, M>& a)
{
    typedef MatExpr_Op3_<M, double, double, M, MatOp_ScaleAddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a, -1, alpha));
}

// E*alpha + beta
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            double beta)
{
    typedef MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, beta));
}

// beta + E*alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (double beta,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return a + beta; }

// E*alpha - beta
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            double beta)
{ return a + (-beta); }

// beta - E*alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (double beta,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return (a.e.a1*(-a.e.a2)) + beta; }

// (E*alpha + gamma) + beta ~ E*alpha + (gamma + beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            double beta)
{ return a.e.a1*a.e.a2 + (a.e.a3 + beta); }

// beta + (E*alpha + gamma)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator + (double beta, const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a + beta; }

// (E*alpha + gamma) - beta
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            double beta)
{ return a + (-beta); }

// beta - (E*alpha + gamma)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (double beta, const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a.e.a1*(-a.e.a2) + (beta - a.e.a3); }

// (E*alpha + gamma)*beta
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            double beta)
{ return a.e.a1*(a.e.a2*beta) + (a.e.a3*beta); }

// beta*(E*alpha + gamma)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator * (double beta, const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a*beta; }

// -(E*alpha + beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a*(-1); }

// (A*u + B*v + w) + beta
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a,
            double beta )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, a.e.a3, a.e.a4, a.e.a5 + beta));
}

// beta + (A*u + B*v + w)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (double beta,
            const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a)
{ return a + beta; }

// (A*u + B*v + w) - beta
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a,
            double beta)
{ return a + (-beta); }

// beta - (A*u + B*v + w)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (double beta,
            const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a)
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, -a.e.a2, a.e.a3, -a.e.a4, -a.e.a5 + beta));
}

// (A*u + B*v + w)*beta
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a,
            double beta )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1,
        a.e.a2*beta, a.e.a3, a.e.a4*beta, a.e.a5*beta));
}

// beta*(A*u + B*v + w)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator * (double beta,
            const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a)
{ return a * beta; }

// -(A*u + B*v + w)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>& a)
{ return a*(-1); }

// A*alpha + B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const M& b )
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b, 1, 0));
}

// B + A*alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const M& b,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return a + b; }

// (A*alpha + beta) + B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const M& b )
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b, 1, a.e.a3));
}

// B + (A*alpha + beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const M& b,
            const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a + b; }


// A*alpha + E
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const MatExpr_<B, M>& b )
{ return a + (M)b; }

// E + A*alpha
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<B, M>& b,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return a + (M)b; }

// (A*alpha + beta) + E
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<B, M>& b )
{ return a + (M)b; }

// E + (A*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<B, M>& b,
            const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return a + b; }

// A*alpha + B*beta
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, b.e.a2, 0));
}

// (A*alpha + beta) + B*gamma
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, b.e.a2, a.e.a3));
}

// B*gamma + (A*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b,
            const MatExpr_<MatExpr_Op3_<B, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a )
{ return a + b; }

// (A*alpha + beta) + (B*gamma + theta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator + (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op3_<B, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, b.e.a2, a.e.a3 + b.e.a3));
}

// A*alpha - B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const M& b )
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b, -1, 0));
}

// B - A*alpha
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const M& b,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, -a.e.a2, b, 1, 0));
}

// (A*alpha + beta) - B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const M& b )
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b, -1, a.e.a3));
}

// B - (A*alpha + beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const M& b,
            const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{
    typedef MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b, -1, a.e.a3));
}

// A*alpha - E
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const MatExpr_<B, M>& b )
{ return a - (M)b; }

// E - A*alpha
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<B, M>& b,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return (M)b - a; }

// (A*alpha + beta) - E
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<B, M>& b )
{ return a - (M)b; }

// E - (A*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, M, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<B, M>& b,
            const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a)
{ return (M)b - a; }

// A*alpha - B*beta
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, -b.e.a2, 0));
}

// (A*alpha + beta) - B*gamma
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, -b.e.a2, a.e.a3));
}

// B*gamma - (A*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b,
            const MatExpr_<MatExpr_Op3_<B, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, -a.e.a2, b.e.a1, b.e.a2, -a.e.a3));
}

// (A*alpha + beta) - (B*gamma + theta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> >, M>
operator - (const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op3_<B, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& b )
{
    typedef MatExpr_Op5_<A, double, B, double, double, M, MatOp_AddEx_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, b.e.a1, -b.e.a2, a.e.a3 - b.e.a3));
}

/////////////////////////////// Mat Multiplication ///////////////////////////////////

// A^t
inline MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_T_<Mat> >, Mat>
Mat::t() const
{
    typedef MatExpr_Op2_<Mat, double, Mat, MatOp_T_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(*this, 1));
}

template<typename T> inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_T_<Mat> >, Mat_<T> >
Mat_<T>::t() const
{
    typedef MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_T_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(*this, 1));
}

// A*B
static inline
MatExpr_<MatExpr_Op4_<Mat, Mat, double, int, Mat, MatOp_MatMul_<Mat> >, Mat>
operator * ( const Mat& a, const Mat& b )
{
    typedef MatExpr_Op4_<Mat, Mat, double, int, Mat, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, 1, 0));
}

template<typename T> static inline
MatExpr_<MatExpr_Op4_<Mat_<T>, Mat_<T>, double, int, Mat_<T>,
MatOp_MatMul_<Mat> >, Mat_<T> >
operator * ( const Mat_<T>& a, const Mat_<T>& b )
{
    typedef MatExpr_Op4_<Mat_<T>, Mat_<T>, double, int, Mat_<T>,
        MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, b, 1, 0));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<A, M>& a, const MatExpr_<B, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a, (M)b, 1, 0));
}

// (A*alpha)*B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a, const M& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, b, a.e.a2, 0));
}

// A*(B*alpha)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const M& b, const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(b, (M)a.e.a1, a.e.a2, 0));
}

// A^t*B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_T_<Mat> >, M>& a, const M& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, b, a.e.a2, GEMM_1_T));
}

// A*B^t
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const M& a, const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a, (M)b.e.a1, b.e.a2, GEMM_2_T));
}

// (A*alpha)*(B*beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1, a.e.a2*b.e.a2, 0));
}

// A^t*(B*alpha)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_T_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1, a.e.a2*b.e.a2, GEMM_1_T));
}

// (A*alpha)*B^t
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1, a.e.a2*b.e.a2, GEMM_2_T));
}

// A^t*B^t
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_T_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1,
        (M)b.e.a1, a.e.a2*b.e.a2, GEMM_1_T+GEMM_2_T));
}

// (A*B)*alpha
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             double alpha )
{
    typedef MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2, a.e.a3*alpha, a.e.a4));
}

// alpha*(A*B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> >, M>
operator * ( double alpha,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    return a*alpha;
}

// -(A*B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    return a*(-1);
}

// (A*alpha + beta)*B
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& a, const M& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, b, a.e.a2, b, a.e.a3, 0));
}

// A*(B*alpha + beta)
template<typename A, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const M& a, const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a, (M)b.e.a1, b.e.a2, a, b.e.a3, 0));
}

// (A*alpha + beta)*(B*gamma)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1,
        a.e.a2*b.e.a2, (M)b.e.a1, a.e.a3*b.e.a2, 0));
}

// (A*gamma)*(B*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1,
        a.e.a2*b.e.a2, (M)a.e.a1, a.e.a2*b.e.a3, 0));
}

// (A*alpha + beta)*B^t
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1,
        a.e.a2*b.e.a2, (M)b.e.a1, a.e.a3*b.e.a2, GEMM_2_T));
}

// A^t*(B*alpha + beta)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_T_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_ScaleAddS_<Mat> >, M>& b )
{
    typedef MatExpr_Op4_<M, M, double, int, M, MatOp_MatMul_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)b.e.a1,
        a.e.a2*b.e.a2, (M)a.e.a1, a.e.a2*b.e.a3, GEMM_1_T));
}

// (A*B + C)*alpha
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<A, B, double, C, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( const MatExpr_<MatExpr_Op6_<A, B, double, C,
             double, int, M, MatOp_MatMulAdd_<Mat> >, M>& a, double alpha )
{
    typedef MatExpr_Op6_<A, B, double, C, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(a.e.a1, a.e.a2,
        a.e.a3*alpha, a.e.a4, a.e.a5*alpha, a.e.a6));
}

// alpha*(A*B + C)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<A, B, double, C, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator * ( double alpha, const MatExpr_<MatExpr_Op6_<A, B, double, C,
             double, int, M, MatOp_MatMulAdd_<Mat> >, M>& a )
{ return a*alpha; }

// -(A*B + C)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<A, B, double, C, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op6_<A, B, double, C,
             double, int, M, MatOp_MatMulAdd_<Mat> >, M>& a )
{ return a*(-1); }


// (A*B) + C
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const M& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, b, 1, a.e.a4));
}

// C + (A*B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const M& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{ return a + b; }


// (A*B) - C
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const M& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, b, -1, a.e.a4));
}

// C - (A*B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const M& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, -a.e.a3, b, 1, a.e.a4));
}


// (A*B) + C
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<C, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b, 1, a.e.a4));
}

// C + (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<C, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{ return a + b; }


// (A*B) - C
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<C, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b, -1, a.e.a4));
}

// C - (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<C, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, -a.e.a3, (M)b, 1, a.e.a4));
}


// (A*B) + C*alpha
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b.e.a1, b.e.a2, a.e.a4));
}

// C*alpha + (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_Scale_<Mat> >, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{ return a + b; }


// (A*B) - (C*alpha)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_Scale_<Mat> >, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b.e.a1, -b.e.a2, a.e.a4));
}

// (C*alpha) - (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_Scale_<Mat> >, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, -a.e.a3, (M)b.e.a1, b.e.a2, a.e.a4));
}


// (A*B) + C^t
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b.e.a1, b.e.a2, a.e.a4 + GEMM_3_T));
}

// C^t + (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator + ( const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_T_<Mat> >, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{ return a + b; }


// (A*B) - C^t
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a,
             const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_T_<Mat> >, M>& b )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, a.e.a3, (M)b.e.a1, -b.e.a2, a.e.a4+GEMM_3_T));
}

// C^t - (A*B)
template<typename A, typename B, typename C, typename M> static inline
MatExpr_<MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> >, M>
operator - ( const MatExpr_<MatExpr_Op2_<C, double, M, MatOp_T_<Mat> >, M>& b,
             const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& a )
{
    typedef MatExpr_Op6_<M, M, double, M, double, int, M, MatOp_MatMulAdd_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp(
        (M)a.e.a1, (M)a.e.a2, -a.e.a3, (M)b.e.a1, b.e.a2, a.e.a4+GEMM_3_T));
}


////////////////////////////// Augmenting algebraic operations //////////////////////////////////

static inline Mat& operator += (const Mat& a, const Mat& b)
{
    add(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator -= (const Mat& a, const Mat& b)
{
    subtract(a, b, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator *= (const Mat& a, const Mat& b)
{
    gemm(a, b, 1, Mat(), 0, (Mat&)a, 0);
    return (Mat&)a;
}

static inline Mat& operator *= (const Mat& a, double alpha)
{
    a.convertTo((Mat&)a, -1, alpha);
    return (Mat&)a;
}

static inline Mat& operator += (const Mat& a, const Scalar& s)
{
    add(a, s, (Mat&)a);
    return (Mat&)a;
}

static inline Mat& operator -= (const Mat& a, const Scalar& s)
{ return (a += -s); }

template<typename T> static inline
Mat_<T>& operator += (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a += (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename T> static inline
Mat_<T>& operator -= (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a -= (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename T> static inline
Mat_<T>& operator *= (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a *= (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename T> static inline
Mat_<T>& operator += (const Mat_<T>& a, const Scalar& s)
{
    (Mat&)a += s;
    return (Mat_<T>&)a;
}

template<typename T> static inline
Mat_<T>& operator -= (const Mat_<T>& a, const Scalar& s)
{
    (Mat&)a -= s;
    return (Mat_<T>&)a;
}

template<typename A, typename M> static inline
M& operator += (const M& a, const MatExpr_<A, M>& b)
{ return (a += (M)b); }

template<typename A, typename M> static inline
M& operator -= (const M& a, const MatExpr_<A, M>& b)
{ return (a -= (M)b); }

template<typename A, typename M> static inline
M& operator *= (const M& a, const MatExpr_<A, M>& b)
{ return (a *= (M)b); }

template<typename A, typename M> static inline
M& operator += (const M& a,
                const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b)
{
    scaleAdd( b.e.a1, Scalar(b.e.a2), a, a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator -= (const M& a,
                const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b)
{
    CvMat _a = a;
    CvMat _b = b.e.a1;
    cvScaleAdd( &_b, -b.e.a2, &_a, &_a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator += (const M& a,
                const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& b)
{
    MatOp_AddEx_<Mat>::apply( a, 1, (M)b.e.a1, b.e.a2, b.e.a3, a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator -= (const M& a,
                const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& b)
{
    MatOp_AddEx_<Mat>::apply( a, 1, (M)b.e.a1, -b.e.a2, -b.e.a3, a );
    return (M&)a;
}

template<typename A, typename B, typename M> static inline
M& operator += (const M& a,
                const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& b)
{
    MatOp_MatMulAdd_<Mat>::apply( (M)b.e.a1, (M)b.e.a2, b.e.a3, a, 1, b.e.a4, a );
    return (M&)a;
}

template<typename A, typename B, typename M> static inline
M& operator -= (const M& a,
                const MatExpr_<MatExpr_Op4_<A, B, double, int, M, MatOp_MatMul_<Mat> >, M>& b)
{
    MatOp_MatMulAdd_<Mat>::apply( (M)b.e.a1, (M)b.e.a2, -b.e.a3, a, 1, b.e.a4, a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator *= (const M& a,
                const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b)
{
    MatOp_MatMul_<Mat>::apply( a, (M)b.e.a1, b.e.a2, 0, a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator *= (const M& a,
                const MatExpr_<MatExpr_Op3_<A, double, double, M, MatOp_ScaleAddS_<Mat> >, M>& b)
{
    MatOp_MatMulAdd_<Mat>::apply( a, (M)b.e.a1, b.e.a2, a, b.e.a3, 0, a );
    return (M&)a;
}

template<typename A, typename M> static inline
M& operator *= (const M& a,
                const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_T_<Mat> >, M>& b)
{
    MatOp_MatMul_<Mat>::apply( a, (M)b.e.a1, b.e.a2, GEMM_2_T, a );
    return (M&)a;
}

////////////////////////////// Logical operations ///////////////////////////////

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> >, Mat>
operator & (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, '&'));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> >, Mat>
operator | (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, '|'));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> >, Mat>
operator ^ (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, '^'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
            MatOp_Bin_<Mat> >, Mat_<T> >
operator & (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, b, '&'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
            MatOp_Bin_<Mat> >, Mat_<T> >
operator | (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, b, '|'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
            MatOp_Bin_<Mat> >, Mat_<T> >
operator ^ (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, b, '^'));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator & (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return (M)a & (M)b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator & (const MatExpr_<A, M>& a, const M& b)
{ return (M)a & b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator & (const M& a, const MatExpr_<A, M>& b)
{ return a & (M)b; }

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator | (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return (M)a | (M)b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator | (const MatExpr_<A, M>& a, const M& b)
{ return (M)a | b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator | (const M& a, const MatExpr_<A, M>& b)
{ return a | (M)b; }

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator ^ (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return (M)a ^ (M)b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator ^ (const MatExpr_<A, M>& a, const M& b)
{ return (M)a ^ b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
operator ^ (const M& a, const MatExpr_<A, M>& b)
{ return a ^ (M)b; }

static inline Mat& operator &= (const Mat& a, const Mat& b)
{
    MatOp_Bin_<Mat>::apply( a, b, '&', (Mat&)a );
    return (Mat&)a;
}

static inline Mat& operator |= (const Mat& a, const Mat& b)
{
    MatOp_Bin_<Mat>::apply( a, b, '|', (Mat&)a );
    return (Mat&)a;
}

static inline Mat& operator ^= (const Mat& a, const Mat& b)
{
    MatOp_Bin_<Mat>::apply( a, b, '^', (Mat&)a );
    return (Mat&)a;
}

template<typename T> static inline Mat_<T>&
operator &= (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a &= (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename T> static inline Mat_<T>&
operator |= (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a |= (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename T> static inline Mat_<T>&
operator ^= (const Mat_<T>& a, const Mat_<T>& b)
{
    (Mat&)a ^= (const Mat&)b;
    return (Mat_<T>&)a;
}

template<typename A, typename M> static inline M&
operator &= (const M& a, const MatExpr_<A, M>& b)
{ return (a &= (M)b); }

template<typename A, typename M> static inline M&
operator |= (const M& a, const MatExpr_<A, M>& b)
{ return (a |= (M)b); }

template<typename A, typename M> static inline M&
operator ^= (const M& a, const MatExpr_<A, M>& b)
{ return (a ^= (M)b); }

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator & (const Mat& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, s, '&'));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator & (const Scalar& s, const Mat& a)
{ return a & s; }

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator | (const Mat& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, s, '|'));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator | (const Scalar& s, const Mat& a)
{ return a | s; }

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator ^ (const Mat& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, s, '^'));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator ^ (const Scalar& s, const Mat& a)
{ return a ^ s; }

static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
operator ~ (const Mat& a)
{
    typedef MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, Scalar(), '~'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator & (const Mat_<T>& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, s, '&'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator & (const Scalar& s, const Mat_<T>& a)
{ return a & s; }

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator | (const Mat_<T>& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, s, '|'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator | (const Scalar& s, const Mat_<T>& a)
{ return a | s; }

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator ^ (const Mat_<T>& a, const Scalar& s)
{
    typedef MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, s, '^'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator ^ (const Scalar& s, const Mat_<T>& a)
{ return a ^ s; }

template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> >, Mat_<T> >
operator ~ (const Mat_<T>& a)
{
    typedef MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, Scalar(), '~'));
}

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator & (const MatExpr_<A, M>& a, const Scalar& s)
{ return (M)a & s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator & (const Scalar& s, const MatExpr_<A, M>& a)
{ return (M)a & s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator | (const MatExpr_<A, M>& a, const Scalar& s)
{ return (M)a | s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator | (const Scalar& s, const MatExpr_<A, M>& a)
{ return (M)a | s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator ^ (const MatExpr_<A, M>& a, const Scalar& s)
{ return (M)a ^ s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator ^ (const Scalar& s, const MatExpr_<A, M>& a)
{ return (M)a ^ s; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, Scalar, int, M, MatOp_BinS_<Mat> >, M >
operator ~ (const MatExpr_<A, M>& a)
{ return ~(M)a; }

static inline Mat& operator &= (const Mat& a, const Scalar& s)
{
    MatOp_BinS_<Mat>::apply( a, s, '&', (Mat&)a );
    return (Mat&)a;
}

static inline Mat& operator |= (const Mat& a, const Scalar& s)
{
    MatOp_BinS_<Mat>::apply( a, s, '|', (Mat&)a );
    return (Mat&)a;
}

static inline Mat& operator ^= (const Mat& a, const Scalar& s)
{
    MatOp_BinS_<Mat>::apply( a, s, '^', (Mat&)a );
    return (Mat&)a;
}

template<typename T> static inline Mat_<T>&
operator &= (const Mat_<T>& a, const Scalar& s)
{
    (Mat&)a &= s;
    return (Mat_<T>&)a;
}

template<typename T> static inline Mat_<T>&
operator |= (const Mat_<T>& a, const Scalar& s)
{
    (Mat&)a |= s;
    return (Mat_<T>&)a;
}

template<typename T> static inline Mat_<T>&
operator ^= (const Mat_<T>& a, const Scalar& s)
{
    (Mat&)a ^= s;
    return (Mat_<T>&)a;
}

////////////////////////////// Comparison operations ///////////////////////////////

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator == (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, CMP_EQ));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator >= (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, CMP_GE));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator > (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, CMP_GT));
}

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator <= (const Mat& a, const Mat& b)
{ return b >= a; }

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator < (const Mat& a, const Mat& b)
{ return b > a; }

static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> >, Mat>
operator != (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Cmp_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, CMP_NE));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator == (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_EQ));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator >= (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_GE));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator > (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_GT));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator <= (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_LE));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator < (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_LT));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator != (const Mat& a, double alpha)
{
    typedef MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha, CMP_NE));
}

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator == (double alpha, const Mat& a)
{ return a == alpha; }

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator >= (double alpha, const Mat& a)
{ return a <= alpha; }

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator > (double alpha, const Mat& a)
{ return a < alpha; }

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator <= (double alpha, const Mat& a)
{ return a >= alpha; }

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator < (double alpha, const Mat& a)
{ return a > alpha; }

static inline MatExpr_<MatExpr_Op3_<Mat, double, int, Mat, MatOp_CmpS_<Mat> >, Mat>
operator != (double alpha, const Mat& a)
{ return a != alpha; }

/////////////////////////////// Miscellaneous operations //////////////////////////////

// max(A, B)
static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> >, Mat>
max(const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, 'M'));
}

// min(A, B)
static inline MatExpr_<MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> >, Mat>
min(const Mat& a, const Mat& b)
{
    typedef MatExpr_Op3_<Mat, Mat, int, Mat, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, 'm'));
}

// abs(A)
static inline MatExpr_<MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> >, Mat>
abs(const Mat& a)
{
    typedef MatExpr_Op3_<Mat, Scalar, int, Mat, MatOp_BinS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, Scalar(0), 'a'));
}

// max(A, B)
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
            MatOp_Bin_<Mat> >, Mat_<T> >
max(const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, b, 'M'));
}

// min(A, B)
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
            MatOp_Bin_<Mat> >, Mat_<T> >
min(const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Mat_<T>, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, b, 'm'));
}

// abs(A)
template<typename T> static inline
MatExpr_<MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>,
            MatOp_BinS_<Mat> >, Mat_<T> >
abs(const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op3_<Mat_<T>, Scalar, int, Mat_<T>,
        MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(
        a, Scalar(0), 'a'));
}

// max(A, B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
max(const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return max((M)a, (M)b); }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
max(const MatExpr_<A, M>& a, const M& b)
{ return max((M)a, b); }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
max(const M& a, const MatExpr_<A, M>& b)
{ return max(a, (M)b); }

// min(A, B)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
min(const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return min((M)a, (M)b); }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
min(const MatExpr_<A, M>& a, const M& b)
{ return min((M)a, b); }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
min(const M& a, const MatExpr_<A, M>& b)
{ return min(a, (M)b); }

// abs(A)
template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> >, M>
abs(const MatExpr_<MatExpr_Op2_<A, B, M, MatOp_Sub_<Mat> >, M>& a)
{
    typedef MatExpr_Op3_<M, M, int, M, MatOp_Bin_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)a.e.a2, 'a'));
}

template<typename T> void merge(const Vector<Mat_<T> >& mv, Mat& dst)
{ merge( (const Vector<Mat>&)mv, dst ); }

template<typename T> void split(const Mat& src, Vector<Mat_<T> >& mv)
{ split(src, (Vector<Mat>&)mv ); }

///// Element-wise multiplication

inline MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
Mat::mul(const Mat& m, double scale) const
{
    typedef MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(*this, m, scale, '*'));
}

inline MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
Mat::mul(const MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_Scale_<Mat> >, Mat>& m, double scale) const
{
    typedef MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(*this, m.e.a1, m.e.a2*scale, '*'));
}

inline MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
Mat::mul(const MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_DivRS_<Mat> >, Mat>& m, double scale) const
{
    typedef MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(*this, m.e.a1, scale/m.e.a2, '/'));
}

template<typename T> inline
MatExpr_<MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> >, Mat_<T> >
Mat_<T>::mul(const Mat_<T>& m, double scale) const
{
    typedef MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(*this, m, scale, '*'));
}

template<typename T> inline
MatExpr_<MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> >, Mat_<T> >
Mat_<T>::mul(const MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_Scale_<Mat> >, Mat_<T> >& m, double scale) const
{
    typedef MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(*this, m.e.a1, m.e.a2*scale, '*'));
}

template<typename T> inline
MatExpr_<MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> >, Mat_<T> >
Mat_<T>::mul(const MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_DivRS_<Mat> >, Mat_<T> >& m, double scale) const
{
    typedef MatExpr_Op4_<Mat_<T>, Mat_<T>, double, char, Mat_<T>, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(*this, m.e.a1, scale/m.e.a2, '/'));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op4_<A, B, double, char, M, MatOp_MulDiv_<Mat> >, M>& a,
            double alpha)
{
    typedef MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, (M)a.e.a2, a.e.a3*alpha, a.e.a4));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator * (double alpha,
            const MatExpr_<MatExpr_Op4_<A, B, double, char, M, MatOp_MulDiv_<Mat> >, M>& a)
{ return a*alpha; }


////// Element-wise division

static inline MatExpr_<MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> >, Mat>
operator / (const Mat& a, const Mat& b)
{
    typedef MatExpr_Op4_<Mat, Mat, double, char, Mat, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, b, 1, '/'));
}

template<typename T> static inline
MatExpr_<MatExpr_Op4_<Mat_<T>, Mat_<T>, double,
char, Mat_<T>, MatOp_MulDiv_<Mat> >, Mat_<T> >
operator / (const Mat_<T>& a, const Mat_<T>& b)
{
    typedef MatExpr_Op4_<Mat_<T>, Mat_<T>, double,
        char, Mat_<T>, MatOp_MulDiv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, b, 1, '/'));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const MatExpr_<A, M>& a, const MatExpr_<B, M>& b)
{ return (M)a/(M)b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const MatExpr_<A, M>& a, const M& b)
{ return (M)a/b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const M& a, const MatExpr_<A, M>& b)
{ return a/(M)b; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const M& b)
{ return ((M)a.e.a1/b)*a.e.a2; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const M& a,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& b)
{ return (a/(M)b.e.a1)*(1./b.e.a2); }

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_Scale_<Mat> >, M>& b)
{ return ((M)a.e.a1/(M)b.e.a1)*(a.e.a2/b.e.a2); }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const M& a,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_DivRS_<Mat> >, M>& b)
{ return a.mul((M)b.e.a1, 1./b.e.a2); }

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op4_<M, M, double, char, M, MatOp_MulDiv_<Mat> >, M>
operator / (const MatExpr_<A, M>& a,
            const MatExpr_<MatExpr_Op2_<B, double, M, MatOp_DivRS_<Mat> >, M>& b)
{ return ((M)a).mul((M)b.e.a1, 1./b.e.a2); }

static inline
MatExpr_<MatExpr_Op2_<Mat, double, Mat, MatOp_DivRS_<Mat> >, Mat >
operator / (double alpha, const Mat& a)
{
    typedef MatExpr_Op2_<Mat, double, Mat, MatOp_DivRS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(a, alpha));
}

static inline Mat& operator /= (const Mat& a, double alpha)
{
    MatOp_Scale_<Mat>::apply( a, 1./alpha, (Mat&)a );
    return (Mat&)a;
}

template<typename T>
static inline Mat_<T>& operator /= (const Mat_<T>& a, double alpha)
{
    MatOp_Scale_<Mat>::apply( a, 1./alpha, (Mat&)a );
    return (Mat_<T>&)a;
}

template<typename T> static inline
MatExpr_<MatExpr_Op2_<Mat_<T>, double, Mat_<T>, MatOp_DivRS_<Mat> >, Mat_<T> >
operator / (double alpha, const Mat_<T>& a)
{
    typedef MatExpr_Op2_<Mat_<T>, double, Mat_<T>,
        MatOp_DivRS_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(a, alpha));
}

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_DivRS_<Mat> >, M>
operator / (double alpha, const MatExpr_<A, M>& a)
{ return alpha/(M)a; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_DivRS_<Mat> >, M>
operator / (double alpha,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_Scale_<Mat> >, M>& a)
{ return (alpha/a.e.a2)/(M)a.e.a1; }

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op2_<M, double, M, MatOp_Scale_<Mat> >, M>
operator / (double alpha,
            const MatExpr_<MatExpr_Op2_<A, double, M, MatOp_DivRS_<Mat> >, M>& a)
{ return (M)a.e.a1*(alpha/a.e.a2); }

static inline Mat& operator /= (const Mat& a, const Mat& b)
{
    MatOp_MulDiv_<Mat>::apply( a, b, 1, '/', (Mat&)a );
    return (Mat&)a;
}

template<typename A, typename M>
static inline M& operator /= (const M& a, const MatExpr_<MatExpr_Op2_<A, double,
                              M, MatOp_Scale_<Mat> >, M>& b)
{
    MatOp_MulDiv_<Mat>::apply( a, (M)b.e.a1, 1./b.e.a2, '/', (M&)a );
    return (M&)a;
}

template<typename A, typename M>
static inline M& operator /= (const M& a, const MatExpr_<MatExpr_Op2_<A, double,
                              M, MatOp_DivRS_<Mat> >, M>& b)
{
    MatOp_MulDiv_<Mat>::apply( a, (M)b.e.a1, 1./b.e.a2, '*', (M&)a );
    return (M&)a;
}

// Mat Inversion and solving linear systems

inline MatExpr_<MatExpr_Op2_<Mat, int, Mat, MatOp_Inv_<Mat> >, Mat>
Mat::inv(int method) const
{
    typedef MatExpr_Op2_<Mat, int, Mat, MatOp_Inv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(*this, method));
}

template<typename T> inline
MatExpr_<MatExpr_Op2_<Mat_<T>, int, Mat_<T>, MatOp_Inv_<Mat> >, Mat_<T> >
Mat_<T>::inv(int method) const
{
    typedef MatExpr_Op2_<Mat_<T>, int, Mat_<T>, MatOp_Inv_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat_<T> >(MatExpr_Temp(*this, method));
}

template<typename A, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Solve_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op2_<A, int, M, MatOp_Inv_<Mat> >, M>& a,
            const M& b)
{
    typedef MatExpr_Op3_<M, M, int, M, MatOp_Solve_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, M>(MatExpr_Temp((M)a.e.a1, b, a.e.a2));
}

template<typename A, typename B, typename M> static inline
MatExpr_<MatExpr_Op3_<M, M, int, M, MatOp_Solve_<Mat> >, M>
operator * (const MatExpr_<MatExpr_Op2_<A, int, M, MatOp_Inv_<Mat> >, M>& a,
            const MatExpr_<B, M>& b)
{ return a*(M)b; }


/////////////////////////////// Initialization ////////////////////////////////////////

inline MatExpr_Initializer Mat::zeros(int rows, int cols, int type)
{
    typedef MatExpr_Op4_<Size, int, Scalar, int, Mat, MatOp_Set_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(Size(cols, rows), type, 0, 0));
}

inline MatExpr_Initializer Mat::zeros(Size size, int type)
{
    return zeros(size.height, size.width, type);
}

inline MatExpr_Initializer Mat::ones(int rows, int cols, int type)
{
    typedef MatExpr_Op4_<Size, int, Scalar, int, Mat, MatOp_Set_<Mat> > MatExpr_Temp;
    return MatExpr_<MatExpr_Temp, Mat>(MatExpr_Temp(Size(cols, rows), type, 1, 1));
}

inline MatExpr_Initializer Mat::ones(Size size, int type)
{
    return ones(size.height, size.width, type);
}

inline MatExpr_Initializer Mat::eye(int rows, int cols, int type)
{
    typedef MatExpr_Op4_<Size, int, Scalar, int, Mat, MatOp_Set_<Mat> > MatExpr_Temp;
    return MatExpr_Initializer(MatExpr_Temp(Size(cols, rows), type, 1, 2));
}

inline MatExpr_Initializer Mat::eye(Size size, int type)
{
    return eye(size.height, size.width, type);
}

static inline MatExpr_Initializer operator * (const MatExpr_Initializer& a, double alpha)
{
    typedef MatExpr_Op4_<Size, int, Scalar, int, Mat, MatOp_Set_<Mat> > MatExpr_Temp;
    return MatExpr_Initializer(MatExpr_Temp(a.e.a1, a.e.a2, a.e.a3*alpha, a.e.a4));
}

static inline MatExpr_Initializer operator * (double alpha, MatExpr_Initializer& a)
{ return a*alpha; }

template<typename T> inline MatExpr_Initializer Mat_<T>::zeros(int rows, int cols)
{ return Mat::zeros(rows, cols, DataType<T>::type); }

template<typename T> inline MatExpr_Initializer Mat_<T>::zeros(Size size)
{ return Mat::zeros(size, DataType<T>::type); }

template<typename T> inline MatExpr_Initializer Mat_<T>::ones(int rows, int cols)
{ return Mat::ones(rows, cols, DataType<T>::type); }

template<typename T> inline MatExpr_Initializer Mat_<T>::ones(Size size)
{ return Mat::ones(size, DataType<T>::type); }

template<typename T> inline MatExpr_Initializer Mat_<T>::eye(int rows, int cols)
{ return Mat::eye(rows, cols, DataType<T>::type); }

template<typename T> inline MatExpr_Initializer Mat_<T>::eye(Size size)
{ return Mat::eye(size, DataType<T>::type); }


//////////// Iterators & Comma initializers //////////////////

template<typename T> inline MatConstIterator_<T>::MatConstIterator_()
    : m(0), ptr(0), sliceEnd(0) {}

template<typename T> inline MatConstIterator_<T>::MatConstIterator_(const Mat_<T>* _m) : m(_m)
{
    if( !_m )
        ptr = sliceEnd = 0;
    else
    {
        ptr = (T*)_m->data;
        sliceEnd = ptr + (_m->isContinuous() ? _m->rows*_m->cols : _m->cols);
    }
}

template<typename T> inline MatConstIterator_<T>::
    MatConstIterator_(const Mat_<T>* _m, int _row, int _col) : m(_m)
{
    if( !_m )
        ptr = sliceEnd = 0;
    else
    {
        assert( (unsigned)_row < _m->rows && (unsigned)_col < _m->cols );
        ptr = (T*)(_m->data + _m->step*_row);
        sliceEnd = _m->isContinuous() ? (T*)_m->data + _m->rows*_m->cols : ptr + _m->cols;
        ptr += _col;
    }
}

template<typename T> inline MatConstIterator_<T>::
    MatConstIterator_(const Mat_<T>* _m, Point _pt) : m(_m)
{
    if( !_m )
        ptr = sliceEnd = 0;
    else
    {
        assert( (unsigned)_pt.y < (unsigned)_m->rows && (unsigned)_pt.x < (unsigned)_m->cols );
        ptr = (T*)(_m->data + _m->step*_pt.y);
        sliceEnd = _m->isContinuous() ? (T*)_m->data + _m->rows*_m->cols : ptr + _m->cols;
        ptr += _pt.x;
    }
}

template<typename T> inline MatConstIterator_<T>::
    MatConstIterator_(const MatConstIterator_& it)
    : m(it.m), ptr(it.ptr), sliceEnd(it.sliceEnd) {}

template<typename T> inline MatConstIterator_<T>&
    MatConstIterator_<T>::operator = (const MatConstIterator_& it )
{
    m = it.m; ptr = it.ptr; sliceEnd = it.sliceEnd;
    return *this;
}

template<typename T> inline T MatConstIterator_<T>::operator *() const { return *ptr; }

template<typename T> inline MatConstIterator_<T>& MatConstIterator_<T>::operator += (int ofs)
{
    if( !m || ofs == 0 )
        return *this;
    ptr += ofs;
    if( m->isContinuous() )
    {
        if( ptr > sliceEnd )
            ptr = sliceEnd;
        else if( ptr < (T*)m->data )
            ptr = (T*)m->data;
    }
    else if( ptr >= sliceEnd || ptr < sliceEnd - m->cols )
    {
        ptr -= ofs;
        Point pt = pos();
        int cols = m->cols;
        ofs += pt.y*cols + pt.x;
        if( ofs > cols*m->rows )
            ofs = cols*m->rows;
        else if( ofs < 0 )
            ofs = 0;
        pt.y = ofs/cols;
        pt.x = ofs - pt.y*cols;
        ptr = (T*)(m->data + m->step*pt.y);
        sliceEnd = ptr + cols;
        ptr += pt.x;
    }
    return *this;
}

template<typename T> inline MatConstIterator_<T>& MatConstIterator_<T>::operator -= (int ofs)
{ return (*this += -ofs); }

template<typename T> inline MatConstIterator_<T>& MatConstIterator_<T>::operator --()
{ return (*this += -1); }

template<typename T> inline MatConstIterator_<T> MatConstIterator_<T>::operator --(int)
{
    MatConstIterator_ b = *this;
    *this += -1;
    return b;
}

template<typename T> inline MatConstIterator_<T>& MatConstIterator_<T>::operator ++()
{
    if( m && ++ptr >= sliceEnd )
    {
        --ptr;
        *this += 1;
    }
    return *this;
}

template<typename T> inline MatConstIterator_<T> MatConstIterator_<T>::operator ++(int)
{
    MatConstIterator_ b = *this;
    if( m && ++ptr >= sliceEnd )
    {
        --ptr;
        *this += 1;
    }
    return b;
}

template<typename T> inline Point MatConstIterator_<T>::pos() const
{
    if( !m )
        return Point();
    if( m->isContinuous() )
    {
        int ofs = ptr - (T*)m->data, y = ofs / m->cols, x = ofs - y*m->cols;
        return Point(x,y);
    }
    else
    {
        int stepT = m->stepT(), y = (ptr - (T*)m->data)/stepT, x = (ptr - (T*)m->data) - y*stepT;
        return Point(x,y);
    }
}

template<typename T> inline MatIterator_<T>::MatIterator_() : MatConstIterator_<T>() {}

template<typename T> inline MatIterator_<T>::MatIterator_(Mat_<T>* _m)
    : MatConstIterator_<T>(_m) {}

template<typename T> inline MatIterator_<T>::MatIterator_(Mat_<T>* _m, int _row, int _col)
    : MatConstIterator_<T>(_m, _row, _col) {}

template<typename T> inline MatIterator_<T>::MatIterator_(const Mat_<T>* _m, Point _pt)
    : MatConstIterator_<T>(_m, _pt) {}

template<typename T> inline MatIterator_<T>::MatIterator_(const MatIterator_& it)
    : MatConstIterator_<T>(it) {}

template<typename T> inline MatIterator_<T>& MatIterator_<T>::operator = (const MatIterator_<T>& it )
{
    this->m = it.m; this->ptr = it.ptr; this->sliceEnd = it.sliceEnd;
    return *this;
}

template<typename T> inline T& MatIterator_<T>::operator *() const { return *(this->ptr); }

template<typename T> inline MatIterator_<T>& MatIterator_<T>::operator += (int ofs)
{
    MatConstIterator_<T>::operator += (ofs);
    return *this;
}

template<typename T> inline MatIterator_<T>& MatIterator_<T>::operator -= (int ofs)
{
    MatConstIterator_<T>::operator += (-ofs);
    return *this;
}

template<typename T> inline MatIterator_<T>& MatIterator_<T>::operator --()
{
    MatConstIterator_<T>::operator += (-1);
    return *this;
}

template<typename T> inline MatIterator_<T> MatIterator_<T>::operator --(int)
{
    MatIterator_ b = *this;
    MatConstIterator_<T>::operator += (-1);
    return b;
}

template<typename T> inline MatIterator_<T>& MatIterator_<T>::operator ++()
{
    if( this->m && ++this->ptr >= this->sliceEnd )
    {
        --this->ptr;
        MatConstIterator_<T>::operator += (1);
    }
    return *this;
}

template<typename T> inline MatIterator_<T> MatIterator_<T>::operator ++(int)
{
    MatIterator_ b = *this;
    if( this->m && ++this->ptr >= this->sliceEnd )
    {
        --this->ptr;
        MatConstIterator_<T>::operator += (1);
    }
    return b;
}

template<typename T> static inline bool
operator == (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return a.m == b.m && a.ptr == b.ptr; }

template<typename T> static inline bool
operator != (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return !(a == b); }

template<typename T> static inline bool
operator < (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return a.ptr < b.ptr; }

template<typename T> static inline bool
operator > (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return a.ptr > b.ptr; }

template<typename T> static inline bool
operator <= (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return a.ptr <= b.ptr; }

template<typename T> static inline bool
operator >= (const MatConstIterator_<T>& a, const MatConstIterator_<T>& b)
{ return a.ptr >= b.ptr; }

template<typename T> static inline int
operator - (const MatConstIterator_<T>& b, const MatConstIterator_<T>& a)
{
    if( a.m != b.m )
        return INT_MAX;
    if( a.sliceEnd == b.sliceEnd )
        return b.ptr - a.ptr;
    {
        Point ap = a.pos(), bp = b.pos();
        if( bp.y > ap.y )
            return (bp.y - ap.y - 1)*a.m->cols + (a.m->cols - ap.x) + bp.x;
        if( bp.y < ap.y )
            return -((ap.y - bp.y - 1)*a.m->cols + (a.m->cols - bp.x) + ap.x);
        return bp.x - ap.x;
    }
}

template<typename T> static inline MatConstIterator_<T>
operator + (const MatConstIterator_<T>& a, int ofs)
{ MatConstIterator_<T> b = a; return b += ofs; }

template<typename T> static inline MatConstIterator_<T>
operator + (int ofs, const MatConstIterator_<T>& a)
{ MatConstIterator_<T> b = a; return b += ofs; }

template<typename T> static inline MatConstIterator_<T>
operator - (const MatConstIterator_<T>& a, int ofs)
{ MatConstIterator_<T> b = a; return b += -ofs; }

template<typename T> inline T MatConstIterator_<T>::operator [](int i) const
{ return *(*this + i); }

template<typename T> static inline MatIterator_<T>
operator + (const MatIterator_<T>& a, int ofs)
{ MatIterator_<T> b = a; return b += ofs; }

template<typename T> static inline MatIterator_<T>
operator + (int ofs, const MatIterator_<T>& a)
{ MatIterator_<T> b = a; return b += ofs; }

template<typename T> static inline MatIterator_<T>
operator - (const MatIterator_<T>& a, int ofs)
{ MatIterator_<T> b = a; return b += -ofs; }

template<typename T> inline T& MatIterator_<T>::operator [](int i) const
{ return *(*this + i); }

template<typename T> inline MatConstIterator_<T> Mat_<T>::begin() const
{ return MatConstIterator_<T>(this); }

template<typename T> inline MatConstIterator_<T> Mat_<T>::end() const
{
    MatConstIterator_<T> it(this);
    it.ptr = it.sliceEnd = (T*)(data + step*(rows-1)) + cols;
    return it;
}

template<typename T> inline MatIterator_<T> Mat_<T>::begin()
{ return MatIterator_<T>(this); }

template<typename T> inline MatIterator_<T> Mat_<T>::end()
{
    MatIterator_<T> it(this);
    it.ptr = it.sliceEnd = (T*)(data + step*(rows-1)) + cols;
    return it;
}

template<typename T> struct CV_EXPORTS MatOp_Iter_
{
    MatOp_Iter_() {}

    static void apply(const MatIterator_<T>& a, Mat& c, int type=-1)
    {
        if( type < 0 )
            c = *a.m;
        else
            a.m->convertTo(c, type);
    }
};

template<typename T> inline MatCommaInitializer_<T>::MatCommaInitializer_(Mat_<T>* _m) :
    MatExpr_<MatExpr_Op1_<MatIterator_<T>, Mat_<T>,
        MatOp_Iter_<T> >, Mat_<T> >(MatIterator_<T>(_m)) {}

template<typename T> template<typename T2> inline MatCommaInitializer_<T>&
MatCommaInitializer_<T>::operator , (T2 v)
{
    assert( this->e.a1 < this->e.a1.m->end() );
    *this->e.a1 = T(v); ++this->e.a1;
    return *this;
}

template<typename T> inline MatCommaInitializer_<T>::operator Mat_<T>() const
{
    assert( this->e.a1 == this->e.a1.m->end() );
    return *this->e.a1.m;
}

template<typename T> inline Mat_<T> MatCommaInitializer_<T>::operator *() const
{
    assert( this->e.a1 == this->e.a1.m->end() );
    return *this->e.a1.m;
}

template<typename T> inline void
MatCommaInitializer_<T>::assignTo(Mat& m, int type) const
{
    Mat_<T>(*this).assignTo(m, type);
}

template<typename T, typename T2> static inline MatCommaInitializer_<T>
operator << (const Mat_<T>& m, T2 val)
{
    MatCommaInitializer_<T> commaInitializer((Mat_<T>*)&m);
    return (commaInitializer, val);
}

template<typename T> inline VectorCommaInitializer_<T>::
VectorCommaInitializer_(Vector<T>* _vec) : vec(_vec), idx(0) {}

template<typename T> template<typename T2> inline VectorCommaInitializer_<T>&
VectorCommaInitializer_<T>::operator , (T2 val)
{
    if( (size_t)idx < vec->size() )
        (*vec)[idx] = T(val);
    else
        vec->push_back(T(val));
    idx++;
    return *this;
}

template<typename T> inline VectorCommaInitializer_<T>::operator Vector<T>() const
{ return *vec; }

template<typename T> inline Vector<T> VectorCommaInitializer_<T>::operator *() const
{ return *vec; }

template<typename T, typename T2> static inline VectorCommaInitializer_<T>
operator << (const Vector<T>& vec, T2 val)
{
    VectorCommaInitializer_<T> commaInitializer((Vector<T>*)&vec);
    return (commaInitializer, val);
}

/////////////////////////////// AutoBuffer ////////////////////////////////////////

template<typename T, size_t fixed_size> inline AutoBuffer<T, fixed_size>::AutoBuffer()
: ptr(buf), size(fixed_size) {}

template<typename T, size_t fixed_size> inline AutoBuffer<T, fixed_size>::AutoBuffer(size_t _size)
: ptr(buf), size(fixed_size) { allocate(_size); }

template<typename T, size_t fixed_size> inline AutoBuffer<T, fixed_size>::~AutoBuffer()
{ deallocate(); }

template<typename T, size_t fixed_size> inline void AutoBuffer<T, fixed_size>::allocate(size_t _size)
{
    if(_size <= size)
        return;
    deallocate();
    if(_size > fixed_size)
    {
        ptr = (T*)fastMalloc(_size*sizeof(T));
        size = _size;
    }
}

template<typename T, size_t fixed_size> inline void AutoBuffer<T, fixed_size>::deallocate()
{
    if( ptr != buf )
    {
        fastFree(ptr);
        ptr = buf;
        size = fixed_size;
    }
}

template<typename T, size_t fixed_size> inline AutoBuffer<T, fixed_size>::operator T* ()
{ return ptr; }

template<typename T, size_t fixed_size> inline AutoBuffer<T, fixed_size>::operator const T* () const
{ return ptr; }


/////////////////////////////////// Ptr ////////////////////////////////////////

template<typename T> inline Ptr<T>::Ptr() : obj(0), refcount(0) {}
template<typename T> inline Ptr<T>::Ptr(T* _obj) : obj(_obj)
{
    if(obj)
    {
        refcount = (int*)fastMalloc(sizeof(*refcount));
        *refcount = 1;
    }
    else
        refcount = 0;
}

template<typename T> inline void Ptr<T>::addref()
{ if( refcount ) ++*refcount; }

template<typename T> inline void Ptr<T>::release()
{
    if( refcount && --*refcount == 0 )
    {
        delete_obj();
        fastFree(refcount);
    }
    refcount = 0;
    obj = 0;
}

template<typename T> inline void Ptr<T>::delete_obj()
{
    if( obj ) delete obj;
}

template<typename T> inline Ptr<T>::~Ptr() { release(); }

template<typename T> inline Ptr<T>::Ptr(const Ptr<T>& ptr)
{
    obj = ptr.obj;
    refcount = ptr.refcount;
    addref();
}

template<typename T> inline Ptr<T>& Ptr<T>::operator = (const Ptr<T>& ptr)
{
    int* _refcount = ptr.refcount;
    if( _refcount )
        ++*_refcount;
    release();
    obj = ptr.obj;
    refcount = _refcount;
    return *this;
}

template<typename T> inline T* Ptr<T>::operator -> () { return obj; }
template<typename T> inline const T* Ptr<T>::operator -> () const { return obj; }

template<typename T> inline Ptr<T>::operator T* () { return obj; }
template<typename T> inline Ptr<T>::operator const T*() const { return obj; }

//////////////////////////////////////// XML & YAML I/O ////////////////////////////////////

template<> inline void Ptr<CvFileStorage>::delete_obj()
{ cvReleaseFileStorage(&obj); }

static inline void write( FileStorage& fs, const String& name, int value )
{ cvWriteInt( *fs, name.size() ? name.c_str() : 0, value ); }

static inline void write( FileStorage& fs, const String& name, float value )
{ cvWriteReal( *fs, name.size() ? name.c_str() : 0, value ); }

static inline void write( FileStorage& fs, const String& name, double value )
{ cvWriteReal( *fs, name.size() ? name.c_str() : 0, value ); }

static inline void write( FileStorage& fs, const String& name, const String& value )
{ cvWriteString( *fs, name.size() ? name.c_str() : 0, value.c_str() ); }

template<typename T> static inline void write(FileStorage& fs, const T& value)
{ write(fs, String(), value); }

template<> inline void write(FileStorage& fs, const int& value )
{ cvWriteInt( *fs, 0, value ); }

template<> inline void write(FileStorage& fs, const float& value )
{ cvWriteReal( *fs, 0, value ); }

template<> inline void write(FileStorage& fs, const double& value )
{ cvWriteReal( *fs, 0, value ); }

template<> inline void write(FileStorage& fs, const String& value )
{ cvWriteString( *fs, 0, value.c_str() ); }

template<typename T, int numflag> struct CV_EXPORTS VecWriterProxy
{
    VecWriterProxy( FileStorage* _fs ) : fs(_fs) {}
    void operator()(const Vector<T>& vec) const
    {
        size_t i, count = vec.size();
        for( i = 0; i < count; i++ )
            write( *fs, vec[i] );
    }
    FileStorage* fs;
};

template<typename T> struct CV_EXPORTS VecWriterProxy<T,1>
{
    VecWriterProxy( FileStorage* _fs ) : fs(_fs) {}
    void operator()(const Vector<T>& vec) const
    {
        int _fmt = DataType<T>::fmt;
        char fmt[] = { (char)((_fmt>>8)+'1'), (char)_fmt, '\0' };
        fs->writeRaw( String(fmt), Vector<uchar>((uchar*)&*vec, vec.size()*sizeof(T)) );
    }
    FileStorage* fs;
};


template<typename T> static inline void write( FileStorage& fs, const Vector<T>& vec )
{
    VecWriterProxy<T, DataType<T>::fmt != 0> w(&fs);
    w(vec);
}

template<typename T> static inline FileStorage&
operator << ( FileStorage& fs, const Vector<T>& vec )
{
    VecWriterProxy<T, DataType<T>::fmt != 0> w(&fs);
    w(vec);
    return fs;
}

CV_EXPORTS void write( FileStorage& fs, const String& name, const Mat& value );

template<typename T> static inline FileStorage& operator << (FileStorage& fs, const T& value)
{
    if( !fs.isOpened() )
        return fs;
    if( fs.state == FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP )
        CV_Error( CV_StsError, "No element name has been given" );
    write( fs, fs.elname, value );
    if( fs.state & FileStorage::INSIDE_MAP )
        fs.state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
    return fs;
}

CV_EXPORTS FileStorage& operator << (FileStorage& fs, const String& str);

static inline FileStorage& operator << (FileStorage& fs, const char* str)
{ return (fs << String(str)); }

inline FileNode FileStorage::operator[](const String& nodename) const
{
    return FileNode(fs.obj, cvGetFileNodeByName(fs.obj, 0, nodename.c_str()));
}
inline FileNode FileStorage::operator[](const char* nodename) const
{
    return FileNode(fs.obj, cvGetFileNodeByName(fs.obj, 0, nodename));
}

inline FileNode::FileNode() : fs(0), node(0) {}
inline FileNode::FileNode(const CvFileStorage* _fs, const CvFileNode* _node)
    : fs(_fs), node(_node) {}

inline FileNode::FileNode(const FileNode& _node) : fs(_node.fs), node(_node.node) {}
inline FileNode FileNode::operator[](const String& nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, node, nodename.c_str()));
}
inline FileNode FileNode::operator[](const char* nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, node, nodename));
}

inline FileNode FileNode::operator[](int i) const
{
    return isSeq() ? FileNode(fs, (CvFileNode*)cvGetSeqElem(node->data.seq, i)) :
        i == 0 ? *this : FileNode();
}

inline int FileNode::type() const { return !node ? NONE : (node->tag & TYPE_MASK); }
inline bool FileNode::isNone() const { return type() == NONE; }
inline bool FileNode::isSeq() const { return type() == SEQ; }
inline bool FileNode::isMap() const { return type() == MAP; }
inline bool FileNode::isInt() const { return type() == INT; }
inline bool FileNode::isReal() const { return type() == REAL; }
inline bool FileNode::isString() const { return type() == STR; }
inline bool FileNode::isNamed() const { return !node ? false : (node->tag & NAMED) != 0; }
inline String FileNode::name() const
{
    const char* str;
    return !node || (str = cvGetFileNodeName(node)) == 0 ? String() : String(str);
}
inline size_t FileNode::count() const
{
    int t = type();
    return t == MAP ? ((CvSet*)node->data.map)->active_count :
        t == SEQ ? node->data.seq->total : node != 0;
}

static inline void read(const FileNode& node, uchar& value, uchar default_value)
{ value = saturate_cast<uchar>(cvReadInt(node.node, default_value)); }

static inline void read(const FileNode& node, schar& value, schar default_value)
{ value = saturate_cast<schar>(cvReadInt(node.node, default_value)); }

static inline void read(const FileNode& node, ushort& value, ushort default_value)
{ value = saturate_cast<ushort>(cvReadInt(node.node, default_value)); }

static inline void read(const FileNode& node, short& value, short default_value)
{ value = saturate_cast<short>(cvReadInt(node.node, default_value)); }

static inline void read(const FileNode& node, int& value, int default_value)
{ value = cvReadInt(node.node, default_value); }

static inline void read(const FileNode& node, float& value, float default_value)
{ value = (float)cvReadReal(node.node, default_value); }

static inline void read(const FileNode& node, double& value, double default_value)
{ value = cvReadReal(node.node, default_value); }

static inline void read(const FileNode& node, String& value, const String& default_value)
{ value = String(cvReadString(node.node, default_value.c_str())); }

inline FileNode::operator int() const
{
    return cvReadInt(node, 0);
}
inline FileNode::operator float() const
{
    return (float)cvReadReal(node, 0);
}
inline FileNode::operator double() const
{
    return cvReadReal(node, 0);
}
inline FileNode::operator String() const
{
    return String(cvReadString(node, ""));
}

inline void FileNode::readRaw( const String& fmt, Vector<uchar>& vec ) const
{
    begin().readRaw( fmt, vec );
}

template<typename T, int numflag> struct CV_EXPORTS VecReaderProxy
{
    VecReaderProxy( FileNodeIterator* _it ) : it(_it) {}
    void operator()(Vector<T>& vec, size_t count) const
    {
        count = std::min(count, it->remaining);
        vec.resize(count);
        for( size_t i = 0; i < count; i++, ++(*it) )
            read(**it, vec[i], T());
    }
    FileNodeIterator* it;
};

template<typename T> struct CV_EXPORTS VecReaderProxy<T,1>
{
    VecReaderProxy( FileNodeIterator* _it ) : it(_it) {}
    void operator()(Vector<T>& vec, size_t count) const
    {
        size_t remaining = it->remaining, cn = DataType<T>::channels;
        int _fmt = DataType<T>::fmt;
        char fmt[] = { (char)((_fmt>>8)+'1'), (char)_fmt, '\0' };
        count = std::min(count, remaining/cn);
        vec.resize(count);
        Vector<uchar> _vec((uchar*)&*vec, count*sizeof(T), false);
        it->readRaw( String(fmt), _vec, count );
    }
    FileNodeIterator* it;
};

template<typename T> static inline void
read( FileNodeIterator& it, Vector<T>& vec, size_t maxCount=(size_t)INT_MAX )
{
    VecReaderProxy<T, DataType<T>::fmt != 0> r(&it);
    r(vec, maxCount);
}

template<typename T> static inline void
read( FileNode& node, Vector<T>& vec, const Vector<T>& /*default_value*/ )
{
    read( node.begin(), vec );
}

inline FileNodeIterator FileNode::begin() const
{
    return FileNodeIterator(fs, node);
}

inline FileNodeIterator FileNode::end() const
{
    return FileNodeIterator(fs, node, count());
}

inline FileNode FileNodeIterator::operator *() const
{ return FileNode(fs, (const CvFileNode*)reader.ptr); }

inline FileNode FileNodeIterator::operator ->() const
{ return FileNode(fs, (const CvFileNode*)reader.ptr); }

template<typename T> static inline FileNodeIterator& operator >> (FileNodeIterator& it, T& value)
{ read( *it, value, T()); return ++it; }

template<typename T> static inline
FileNodeIterator& operator >> (FileNodeIterator& it, Vector<T>& vec)
{
    VecReaderProxy<T, DataType<T>::fmt != 0> r(&it);
    r(vec, (size_t)INT_MAX);
    return it;
}

template<typename T> static inline void operator >> (const FileNode& n, T& value)
{ FileNodeIterator it = n.begin(); it >> value; }

static inline bool operator == (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return it1.fs == it2.fs && it1.container == it2.container &&
        it1.reader.ptr == it2.reader.ptr && it1.remaining == it2.remaining;
}

static inline bool operator != (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return !(it1 == it2);
}

static inline int operator - (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return it2.remaining - it1.remaining;
}

static inline bool operator < (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return it1.remaining > it2.remaining;
}

/****************************************************************************************\

  Generic implementation of QuickSort algorithm
  Use it as: Vector<T> a; ... sort(a,<less_than_predictor>);

  The current implementation was derived from *BSD system qsort():

    * Copyright (c) 1992, 1993
    *  The Regents of the University of California.  All rights reserved.
    *
    * Redistribution and use in source and binary forms, with or without
    * modification, are permitted provided that the following conditions
    * are met:
    * 1. Redistributions of source code must retain the above copyright
    *    notice, this list of conditions and the following disclaimer.
    * 2. Redistributions in binary form must reproduce the above copyright
    *    notice, this list of conditions and the following disclaimer in the
    *    documentation and/or other materials provided with the distribution.
    * 3. All advertising materials mentioning features or use of this software
    *    must display the following acknowledgement:
    *  This product includes software developed by the University of
    *  California, Berkeley and its contributors.
    * 4. Neither the name of the University nor the names of its contributors
    *    may be used to endorse or promote products derived from this software
    *    without specific prior written permission.
    *
    * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
    * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
    * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    * SUCH DAMAGE.

\****************************************************************************************/

template<typename T, class _LT> void sort( Vector<T>& vec, _LT LT=_LT() )
{
    int isort_thresh = 7;
    int sp = 0;

    struct
    {
        T *lb;
        T *ub;
    }
    stack[48];

    T* array = &vec[0];
    int total = vec.size();

    if( total <= 1 )
        return;

    stack[0].lb = array;
    stack[0].ub = array + (total - 1);

    while( sp >= 0 )
    {
        T* left = stack[sp].lb;
        T* right = stack[sp--].ub;

        for(;;)
        {
            int i, n = (int)(right - left) + 1, m;
            T* ptr;
            T* ptr2;

            if( n <= isort_thresh )
            {
            insert_sort:
                for( ptr = left + 1; ptr <= right; ptr++ )
                {
                    for( ptr2 = ptr; ptr2 > left && LT(ptr2[0],ptr2[-1]); ptr2--)
                        std::swap( ptr2[0], ptr2[-1] );
                }
                break;
            }
            else
            {
                T* left0;
                T* left1;
                T* right0;
                T* right1;
                T* pivot;
                T* a;
                T* b;
                T* c;
                int swap_cnt = 0;

                left0 = left;
                right0 = right;
                pivot = left + (n/2);

                if( n > 40 )
                {
                    int d = n / 8;
                    a = left, b = left + d, c = left + 2*d;
                    left = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));

                    a = pivot - d, b = pivot, c = pivot + d;
                    pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));

                    a = right - 2*d, b = right - d, c = right;
                    right = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));
                }

                a = left, b = pivot, c = right;
                pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))
                                   : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));
                if( pivot != left0 )
                {
                    std::swap( *pivot, *left0 );
                    pivot = left0;
                }
                left = left1 = left0 + 1;
                right = right1 = right0;

                for(;;)
                {
                    while( left <= right && !LT(*pivot, *left) )
                    {
                        if( !LT(*left, *pivot) )
                        {
                            if( left > left1 )
                                std::swap( *left1, *left );
                            swap_cnt = 1;
                            left1++;
                        }
                        left++;
                    }

                    while( left <= right && !LT(*right, *pivot) )
                    {
                        if( !LT(*pivot, *right) )
                        {
                            if( right < right1 )
                                std::swap( *right1, *right );
                            swap_cnt = 1;
                            right1--;
                        }
                        right--;
                    }

                    if( left > right )
                        break;
                    std::swap( *left, *right );
                    swap_cnt = 1;
                    left++;
                    right--;
                }

                if( swap_cnt == 0 )
                {
                    left = left0, right = right0;
                    goto insert_sort;
                }

                n = MIN( (int)(left1 - left0), (int)(left - left1) );
                for( i = 0; i < n; i++ )
                    std::swap( left0[i], left[i-n] );

                n = MIN( (int)(right0 - right1), (int)(right1 - right) );
                for( i = 0; i < n; i++ )
                    std::swap( left[i], right0[i-n+1] );
                n = (int)(left - left1);
                m = (int)(right1 - right);
                if( n > 1 )
                {
                    if( m > 1 )
                    {
                        if( n > m )
                        {
                            stack[++sp].lb = left0;
                            stack[sp].ub = left0 + n - 1;
                            left = right0 - m + 1, right = right0;
                        }
                        else
                        {
                            stack[++sp].lb = right0 - m + 1;
                            stack[sp].ub = right0;
                            left = left0, right = left0 + n - 1;
                        }
                    }
                    else
                        left = left0, right = left0 + n - 1;
                }
                else if( m > 1 )
                    left = right0 - m + 1, right = right0;
                else
                    break;
            }
        }
    }
}

template<typename T> struct CV_EXPORTS LessThan
{
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template<typename T> struct CV_EXPORTS GreaterEq
{
    bool operator()(const T& a, const T& b) const { return a >= b; }
};

template<typename T> struct CV_EXPORTS LessThanIdx
{
    LessThanIdx( const T* _arr ) : arr(_arr) {}
    bool operator()(int a, int b) const { return arr[a] < arr[b]; }
    const T* arr;
};

template<typename T> struct CV_EXPORTS GreaterEqIdx
{
    GreaterEqIdx( const T* _arr ) : arr(_arr) {}
    bool operator()(int a, int b) const { return arr[a] >= arr[b]; }
    const T* arr;
};

}

#endif
#endif
