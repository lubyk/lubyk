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

#ifndef _CV_HPP_
#define _CV_HPP_

#ifdef __cplusplus

namespace cv
{

enum { BORDER_REPLICATE=IPL_BORDER_REPLICATE, BORDER_CONSTANT=IPL_BORDER_CONSTANT,
       BORDER_REFLECT=IPL_BORDER_REFLECT, BORDER_REFLECT_101=IPL_BORDER_REFLECT_101,
       BORDER_REFLECT101=BORDER_REFLECT_101, BORDER_WRAP=IPL_BORDER_WRAP,
       BORDER_TRANSPARENT, BORDER_DEFAULT=BORDER_REFLECT_101, BORDER_ISOLATED=16 };

CV_EXPORTS int borderInterpolate( int p, int len, int borderType );

struct CV_EXPORTS BaseRowFilter
{
    BaseRowFilter();
    virtual ~BaseRowFilter();
    virtual void operator()(const uchar* src, uchar* dst,
                            int width, int cn) = 0;
    int ksize, anchor;
};


struct CV_EXPORTS BaseColumnFilter
{
    BaseColumnFilter();
    virtual ~BaseColumnFilter();
    virtual void operator()(const uchar** src, uchar* dst, int dststep,
                            int dstcount, int width) = 0;
    virtual void reset();
    int ksize, anchor;
};


struct CV_EXPORTS BaseFilter
{
    BaseFilter();
    virtual ~BaseFilter();
    virtual void operator()(const uchar** src, uchar* dst, int dststep,
                            int dstcount, int width, int cn) = 0;
    virtual void reset();
    Size ksize;
    Point anchor;
};


struct CV_EXPORTS FilterEngine
{
    FilterEngine();
    FilterEngine(const Ptr<BaseFilter>& _filter2D,
                 const Ptr<BaseRowFilter>& _rowFilter,
                 const Ptr<BaseColumnFilter>& _columnFilter,
                 int srcType, int dstType, int bufType,
                 int _rowBorderType=BORDER_REPLICATE,
                 int _columnBorderType=-1,
                 const Scalar& _borderValue=Scalar());
    virtual ~FilterEngine();
    void init(const Ptr<BaseFilter>& _filter2D,
              const Ptr<BaseRowFilter>& _rowFilter,
              const Ptr<BaseColumnFilter>& _columnFilter,
              int srcType, int dstType, int bufType,
              int _rowBorderType=BORDER_REPLICATE, int _columnBorderType=-1,
              const Scalar& _borderValue=Scalar());
    virtual int start(Size wholeSize, Rect roi, int maxBufRows=-1);
    virtual int start(const Mat& src, const Rect& srcRoi=Rect(0,0,-1,-1),
                      bool isolated=false, int maxBufRows=-1);
    virtual int proceed(const uchar* src, int srcStep, int srcCount,
                        uchar* dst, int dstStep);
    virtual void apply( const Mat& src, Mat& dst,
                        const Rect& srcRoi=Rect(0,0,-1,-1),
                        Point dstOfs=Point(0,0),
                        bool isolated=false);
    bool isSeparable() const { return filter2D.obj == 0; }
    int remainingInputRows() const;
    int remainingOutputRows() const;
    
    int srcType, dstType, bufType;
    Size ksize;
    Point anchor;
    int maxWidth;
    Size wholeSize;
    Rect roi;
    int dx1, dx2;
    int rowBorderType, columnBorderType;
    Vector<int> borderTab;
    int borderElemSize;
    Vector<uchar> ringBuf;
    Vector<uchar> srcRow;
    Vector<uchar> constBorderValue;
    Vector<uchar> constBorderRow;
    int bufStep, startY, startY0, endY, rowCount, dstY;
    Vector<uchar*> rows;
    
    Ptr<BaseFilter> filter2D;
    Ptr<BaseRowFilter> rowFilter;
    Ptr<BaseColumnFilter> columnFilter;
};

enum { KERNEL_GENERAL=0, KERNEL_SYMMETRICAL=1, KERNEL_ASYMMETRICAL=2,
       KERNEL_SMOOTH=4, KERNEL_INTEGER=8 };

CV_EXPORTS int getKernelType(const Mat& kernel, Point anchor);

CV_EXPORTS Ptr<BaseRowFilter> getLinearRowFilter(int srcType, int bufType,
                                            const Mat& kernel, int anchor,
                                            int symmetryType);

CV_EXPORTS Ptr<BaseColumnFilter> getLinearColumnFilter(int bufType, int dstType,
                                            const Mat& kernel, int anchor,
                                            int symmetryType, double delta=0,
                                            int bits=0);

CV_EXPORTS Ptr<BaseFilter> getLinearFilter(int srcType, int dstType,
                                           const Mat& kernel,
                                           Point anchor=Point(-1,-1),
                                           double delta=0, int bits=0);

CV_EXPORTS Ptr<FilterEngine> createSeparableLinearFilter(int srcType, int dstType,
                          const Mat& rowKernel, const Mat& columnKernel,
                          Point _anchor=Point(-1,-1), double delta=0,
                          int _rowBorderType=BORDER_DEFAULT,
                          int _columnBorderType=-1,
                          const Scalar& _borderValue=Scalar());

CV_EXPORTS Ptr<FilterEngine> createLinearFilter(int srcType, int dstType,
                 const Mat& kernel, Point _anchor=Point(-1,-1),
                 double delta=0, int _rowBorderType=BORDER_DEFAULT,
                 int _columnBorderType=-1, const Scalar& _borderValue=Scalar());

CV_EXPORTS Mat getGaussianKernel( int ksize, double sigma, int ktype=CV_64F );

CV_EXPORTS Ptr<FilterEngine> createGaussianFilter( int type, Size ksize,
                                    double sigma1, double sigma2=0,
                                    int borderType=BORDER_DEFAULT);

CV_EXPORTS void getDerivKernels( Mat& kx, Mat& ky, int dx, int dy, int ksize,
                                 bool normalize=false, int ktype=CV_32F );

CV_EXPORTS Ptr<FilterEngine> createDerivFilter( int srcType, int dstType,
                                        int dx, int dy, int ksize,
                                        int borderType=BORDER_DEFAULT );

CV_EXPORTS Ptr<BaseRowFilter> getRowSumFilter(int srcType, int sumType,
                                                 int ksize, int anchor=-1);
CV_EXPORTS Ptr<BaseColumnFilter> getColumnSumFilter(int sumType, int dstType,
                                                       int ksize, int anchor=-1,
                                                       double scale=1);
CV_EXPORTS Ptr<FilterEngine> createBoxFilter( int srcType, int dstType, Size ksize,
                                                 Point anchor=Point(-1,-1),
                                                 bool normalize=true,
                                                 int borderType=BORDER_DEFAULT);

enum { MORPH_ERODE=0, MORPH_DILATE=1, MORPH_OPEN=2, MORPH_CLOSE=3,
       MORPH_GRADIENT=4, MORPH_TOPHAT=5, MORPH_BLACKHAT=6 };

CV_EXPORTS Ptr<BaseRowFilter> getMorphologyRowFilter(int op, int type, int ksize, int anchor=-1);
CV_EXPORTS Ptr<BaseColumnFilter> getMorphologyColumnFilter(int op, int type, int ksize, int anchor=-1);
CV_EXPORTS Ptr<BaseFilter> getMorphologyFilter(int op, int type, const Mat& kernel,
                                               Point anchor=Point(-1,-1));

static inline Scalar morphologyDefaultBorderValue() { return Scalar::all(DBL_MAX); }

CV_EXPORTS Ptr<FilterEngine> createMorphologyFilter(int op, int type, const Mat& kernel,
                    Point anchor=Point(-1,-1), int _rowBorderType=BORDER_CONSTANT,
                    int _columnBorderType=-1,
                    const Scalar& _borderValue=morphologyDefaultBorderValue());

enum { MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2 };
CV_EXPORTS Mat getStructuringElement(int shape, Size ksize, Point anchor=Point(-1,-1));

CV_EXPORTS void copyMakeBorder( const Mat& src, Mat& dst,
                                int top, int bottom, int left, int right,
                                int borderType );

CV_EXPORTS void medianBlur( const Mat& src, Mat& dst, int ksize );
CV_EXPORTS void GaussianBlur( const Mat& src, Mat& dst, Size ksize,
                              double sigma1, double sigma2=0,
                              int borderType=BORDER_DEFAULT );
CV_EXPORTS void bilateralFilter( const Mat& src, Mat& dst, int d,
                                 double sigmaColor, double sigmaSpace,
                                 int borderType=BORDER_DEFAULT );
CV_EXPORTS void boxFilter( const Mat& src, Mat& dst, int ddepth,
                           Size ksize, Point anchor=Point(-1,-1),
                           bool normalize=true,
                           int borderType=BORDER_DEFAULT );
static inline void blur( const Mat& src, Mat& dst,
                         Size ksize, Point anchor=Point(-1,-1),
                         int borderType=BORDER_DEFAULT )
{
    boxFilter( src, dst, -1, ksize, anchor, true, borderType );
}

CV_EXPORTS void filter2D( const Mat& src, Mat& dst, int ddepth,
                          const Mat& kernel, Point anchor=Point(-1,-1),
                          double delta=0, int borderType=BORDER_DEFAULT );

CV_EXPORTS void sepFilter2D( const Mat& src, Mat& dst, int ddepth,
                             const Mat& kernelX, const Mat& kernelY,
                             Point anchor=Point(-1,-1),
                             double delta=0, int borderType=BORDER_DEFAULT );

CV_EXPORTS void Sobel( const Mat& src, Mat& dst, int ddepth,
                       int dx, int dy, int ksize=3,
                       double scale=1, double delta=0,
                       int borderType=BORDER_DEFAULT );

CV_EXPORTS void Scharr( const Mat& src, Mat& dst, int ddepth,
                        int dx, int dy, double scale=1, double delta=0,
                        int borderType=BORDER_DEFAULT );

CV_EXPORTS void Laplacian( const Mat& src, Mat& dst, int ddepth,
                           int ksize=1, double scale=1, double delta=0,
                           int borderType=BORDER_DEFAULT );

CV_EXPORTS void erode( const Mat& src, Mat& dst, const Mat& kernel,
                       Point anchor=Point(-1,-1), int iterations=1,
                       int borderType=BORDER_CONSTANT,
                       const Scalar& borderValue=morphologyDefaultBorderValue() );
CV_EXPORTS void dilate( const Mat& src, Mat& dst, const Mat& kernel,
                        Point anchor=Point(-1,-1), int iterations=1,
                        int borderType=BORDER_CONSTANT,
                        const Scalar& borderValue=morphologyDefaultBorderValue() );
CV_EXPORTS void morphologyEx( const Mat& src, Mat& dst, int op, const Mat& kernel,
                              Point anchor=Point(-1,-1), int iterations=1,
                              int borderType=BORDER_CONSTANT,
                              const Scalar& borderValue=morphologyDefaultBorderValue() );

enum { INTER_NEAREST=0, INTER_LINEAR=1, INTER_CUBIC=2, INTER_AREA=3,
       INTER_LANCZOS4=4, INTER_MAX=7, WARP_INVERSE_MAP=16 };

CV_EXPORTS void resize( const Mat& src, Mat& dst,
                        Size dsize=Size(), double fx=0, double fy=0,
                        int interpolation=INTER_LINEAR );

CV_EXPORTS void warpAffine( const Mat& src, Mat& dst,
                            const Mat& M, Size dsize,
                            int flags=INTER_LINEAR,
                            int borderMode=BORDER_CONSTANT,
                            const Scalar& borderValue=Scalar());
CV_EXPORTS void warpPerspective( const Mat& src, Mat& dst,
                                 const Mat& M, Size dsize,
                                 int flags=INTER_LINEAR,
                                 int borderMode=BORDER_CONSTANT,
                                 const Scalar& borderValue=Scalar());

CV_EXPORTS void remap( const Mat& src, Mat& dst, const Mat& map1, const Mat& map2,
                       int interpolation, int borderMode=BORDER_CONSTANT,
                       const Scalar& borderValue=Scalar());

CV_EXPORTS void convertMaps( const Mat& map1, const Mat& map2, Mat& dstmap1, Mat& dstmap2,
                             int dstmap1type, bool nninterpolation=false );

CV_EXPORTS Mat getRotationMatrix2D( Point2f center, double angle, double scale );
CV_EXPORTS Mat getPerspectiveTransform( const Point2f src[], const Point2f dst[] );
CV_EXPORTS Mat getAffineTransform( const Point2f src[], const Point2f dst[] );

CV_EXPORTS void integral( const Mat& src, Mat& sum, int sdepth=-1 );
CV_EXPORTS void integral( const Mat& src, Mat& sum, Mat& sqsum, int sdepth=-1 );
CV_EXPORTS void integral( const Mat& src, Mat& sum, Mat& sqsum, Mat& tilted, int sdepth=-1 );

CV_EXPORTS void accumulate( const Mat& src, Mat& dst, const Mat& mask=Mat() );
CV_EXPORTS void accumulateSquare( const Mat& src, Mat& dst, const Mat& mask=Mat() );
CV_EXPORTS void accumulateProduct( const Mat& src1, const Mat& src2,
                                   Mat& dst, const Mat& mask=Mat() );
CV_EXPORTS void accumulateWeighted( const Mat& src, Mat& dst,
                                    double alpha, const Mat& mask=Mat() );

enum { THRESH_BINARY=0, THRESH_BINARY_INV=1, THRESH_TRUNC=2, THRESH_TOZERO=3,
       THRESH_TOZERO_INV=4, THRESH_MASK=7, THRESH_OTSU=8 };

CV_EXPORTS double threshold( const Mat& src, Mat& dst, double thresh, double maxval, int type );

enum { ADAPTIVE_THRESH_MEAN_C=0, ADAPTIVE_THRESH_GAUSSIAN_C=1 };

CV_EXPORTS void adaptiveThreshold( const Mat& src, Mat& dst, double maxValue,
                                   int adaptiveMethod, int thresholdType,
                                   int blockSize, double C );

CV_EXPORTS void pyrDown( const Mat& src, Mat& dst, const Size& dstsize=Size());
CV_EXPORTS void pyrUp( const Mat& src, Mat& dst, const Size& dstsize=Size());
CV_EXPORTS void buildPyramid( const Mat& src, Vector<Mat>& dst, int maxlevel );


CV_EXPORTS void undistort( const Mat& src, Mat& dst, const Mat& cameraMatrix,
                           const Mat& distCoeffs, const Mat& newCameraMatrix=Mat() );
CV_EXPORTS void initUndistortRectifyMap( const Mat& cameraMatrix, const Mat& distCoeffs,
                           const Mat& R, const Mat& newCameraMatrix,
                           Size size, int m1type, Mat& map1, Mat& map2 );
CV_EXPORTS Mat_<double> getDefaultNewCameraMatrix( const Mat_<double>& A, Size imgsize );

}

//////////////////////////////////////////////////////////////////////////////////////////

struct CV_EXPORTS CvLevMarq
{
    CvLevMarq();
    CvLevMarq( int nparams, int nerrs, CvTermCriteria criteria=
        cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,DBL_EPSILON),
        bool completeSymmFlag=false );
    ~CvLevMarq();
    void init( int nparams, int nerrs, CvTermCriteria criteria=
        cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,DBL_EPSILON),
        bool completeSymmFlag=false );
    bool update( const CvMat*& param, CvMat*& J, CvMat*& err );
    bool updateAlt( const CvMat*& param, CvMat*& JtJ, CvMat*& JtErr, double*& errNorm );

    void clear();
    void step();
    enum { DONE=0, STARTED=1, CALC_J=2, CHECK_ERR=3 };

    CvMat* mask;
    CvMat* prevParam;
    CvMat* param;
    CvMat* J;
    CvMat* err;
    CvMat* JtJ;
    CvMat* JtJN;
    CvMat* JtErr;
    CvMat* JtJV;
    CvMat* JtJW;
    double prevErrNorm, errNorm;
    int lambdaLg10;
    CvTermCriteria criteria;
    int state;
    int iters;
    bool completeSymmFlag;
};


// 2009-01-12, Xavier Delacour <xavier.delacour@gmail.com>

struct lsh_hash {
  int h1, h2;
};

struct CvLSHOperations {
  virtual ~CvLSHOperations() {}

  virtual int vector_add(const void* data) = 0;
  virtual void vector_remove(int i) = 0;
  virtual const void* vector_lookup(int i) = 0;
  virtual void vector_reserve(int n) = 0;
  virtual unsigned int vector_count() = 0;

  virtual void hash_insert(lsh_hash h, int l, int i) = 0;
  virtual void hash_remove(lsh_hash h, int l, int i) = 0;
  virtual int hash_lookup(lsh_hash h, int l, int* ret_i, int ret_i_max) = 0;
};


#endif /* __cplusplus */

#endif /* _CV_HPP_ */

/* End of file. */
