/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

#ifndef __CVAUX_HPP__
#define __CVAUX_HPP__

#ifdef __cplusplus

/****************************************************************************************\
*                                       CamShiftTracker                                  *
\****************************************************************************************/

class CV_EXPORTS CvCamShiftTracker
{
public:

    CvCamShiftTracker();
    virtual ~CvCamShiftTracker();

    /**** Characteristics of the object that are calculated by track_object method *****/
    float   get_orientation() const // orientation of the object in degrees
    { return m_box.angle; }
    float   get_length() const // the larger linear size of the object
    { return m_box.size.height; }
    float   get_width() const // the smaller linear size of the object
    { return m_box.size.width; }
    CvPoint2D32f get_center() const // center of the object
    { return m_box.center; }
    CvRect get_window() const // bounding rectangle for the object
    { return m_comp.rect; }

    /*********************** Tracking parameters ************************/
    int     get_threshold() const // thresholding value that applied to back project
    { return m_threshold; }

    int     get_hist_dims( int* dims = 0 ) const // returns number of histogram dimensions and sets
    { return m_hist ? cvGetDims( m_hist->bins, dims ) : 0; }

    int     get_min_ch_val( int channel ) const // get the minimum allowed value of the specified channel
    { return m_min_ch_val[channel]; }

    int     get_max_ch_val( int channel ) const // get the maximum allowed value of the specified channel
    { return m_max_ch_val[channel]; }

    // set initial object rectangle (must be called before initial calculation of the histogram)
    bool    set_window( CvRect window)
    { m_comp.rect = window; return true; }

    bool    set_threshold( int threshold ) // threshold applied to the histogram bins
    { m_threshold = threshold; return true; }

    bool    set_hist_bin_range( int dim, int min_val, int max_val );

    bool    set_hist_dims( int c_dims, int* dims );// set the histogram parameters

    bool    set_min_ch_val( int channel, int val ) // set the minimum allowed value of the specified channel
    { m_min_ch_val[channel] = val; return true; }
    bool    set_max_ch_val( int channel, int val ) // set the maximum allowed value of the specified channel
    { m_max_ch_val[channel] = val; return true; }

    /************************ The processing methods *********************************/
    // update object position
    virtual bool  track_object( const IplImage* cur_frame );

    // update object histogram
    virtual bool  update_histogram( const IplImage* cur_frame );

    // reset histogram
    virtual void  reset_histogram();

    /************************ Retrieving internal data *******************************/
    // get back project image
    virtual IplImage* get_back_project()
    { return m_back_project; }

    float query( int* bin ) const
    { return m_hist ? (float)cvGetRealND(m_hist->bins, bin) : 0.f; }

protected:

    // internal method for color conversion: fills m_color_planes group
    virtual void color_transform( const IplImage* img );

    CvHistogram* m_hist;

    CvBox2D    m_box;
    CvConnectedComp m_comp;

    float      m_hist_ranges_data[CV_MAX_DIM][2];
    float*     m_hist_ranges[CV_MAX_DIM];

    int        m_min_ch_val[CV_MAX_DIM];
    int        m_max_ch_val[CV_MAX_DIM];
    int        m_threshold;

    IplImage*  m_color_planes[CV_MAX_DIM];
    IplImage*  m_back_project;
    IplImage*  m_temp;
    IplImage*  m_mask;
};

/****************************************************************************************\
*                                   Adaptive Skin Detector                               *
\****************************************************************************************/

class CvAdaptiveSkinDetector
{
private:
	enum {
		GSD_HUE_LT = 3,
		GSD_HUE_UT = 33,
		GSD_INTENSITY_LT = 15,
		GSD_INTENSITY_UT = 250
	};

	class Histogram
	{
	private:
		enum {
			HistogramSize = (GSD_HUE_UT - GSD_HUE_LT + 1)
		};

	protected:
		int findCoverageIndex(double surfaceToCover, int defaultValue = 0);

	public:
		CvHistogram *fHistogram;
		Histogram();
		virtual ~Histogram();

		void findCurveThresholds(int &x1, int &x2, double percent = 0.05);
		void mergeWith(Histogram *source, double weight);
	};

	int nStartCounter, nFrameCount, nSkinHueLowerBound, nSkinHueUpperBound, nMorphingMethod, nSamplingDivider;
	double fHistogramMergeFactor, fHuePercentCovered;
	Histogram histogramHueMotion, skinHueHistogram;
	IplImage *imgHueFrame, *imgSaturationFrame, *imgLastGrayFrame, *imgMotionFrame, *imgFilteredFrame;
	IplImage *imgShrinked, *imgTemp, *imgGrayFrame, *imgHSVFrame;

protected:
	void initData(IplImage *src, int widthDivider, int heightDivider);
	void adaptiveFilter();

public:

	enum {
		MORPHING_METHOD_NONE = 0,
		MORPHING_METHOD_ERODE = 1,
		MORPHING_METHOD_ERODE_ERODE	= 2,
		MORPHING_METHOD_ERODE_DILATE = 3
	};

	CvAdaptiveSkinDetector(int samplingDivider = 1, int morphingMethod = MORPHING_METHOD_NONE);
	virtual ~CvAdaptiveSkinDetector();

	virtual void process(IplImage *inputBGRImage, IplImage *outputHueMask);
};


/****************************************************************************************\
*                                  Fuzzy MeanShift Tracker                               *
\****************************************************************************************/

class CvFuzzyPoint {
public:
	double x, y, value;

	CvFuzzyPoint(double _x, double _y);
};

class CvFuzzyCurve {
private:
    std::vector<CvFuzzyPoint> points;
	double value, centre;

	bool between(double x, double x1, double x2);

public:
	CvFuzzyCurve();
	~CvFuzzyCurve();

	void setCentre(double _centre);
	double getCentre();
	void clear();
	void addPoint(double x, double y);
	double calcValue(double param);
	double getValue();
	void setValue(double _value);
};

class CvFuzzyFunction {
public:
    std::vector<CvFuzzyCurve> curves;

	CvFuzzyFunction();
	~CvFuzzyFunction();
	void addCurve(CvFuzzyCurve *curve, double value = 0);
	void resetValues();
	double calcValue();
	CvFuzzyCurve *newCurve();
};

class CvFuzzyRule {
private:
	CvFuzzyCurve *fuzzyInput1, *fuzzyInput2;
	CvFuzzyCurve *fuzzyOutput;
public:
	CvFuzzyRule();
	~CvFuzzyRule();
	void setRule(CvFuzzyCurve *c1, CvFuzzyCurve *c2, CvFuzzyCurve *o1);
	double calcValue(double param1, double param2);
	CvFuzzyCurve *getOutputCurve();
};

class CvFuzzyController {
private:
    std::vector<CvFuzzyRule*> rules;
public:
	CvFuzzyController();
	~CvFuzzyController();
	void addRule(CvFuzzyCurve *c1, CvFuzzyCurve *c2, CvFuzzyCurve *o1);
	double calcOutput(double param1, double param2);
};

class CvFuzzyMeanShiftTracker
{
private:
	class FuzzyResizer
	{
	private:
		CvFuzzyFunction iInput, iOutput;
		CvFuzzyController fuzzyController;
	public:
		FuzzyResizer();
		int calcOutput(double edgeDensity, double density);
	};

	class SearchWindow
	{
	public:
		FuzzyResizer *fuzzyResizer;
		int x, y;
		int width, height, maxWidth, maxHeight, ellipseHeight, ellipseWidth;
		int ldx, ldy, ldw, ldh, numShifts, numIters;
		int xGc, yGc;
		long m00, m01, m10, m11, m02, m20;
		double ellipseAngle;
		double density;
		unsigned int depthLow, depthHigh;
		int verticalEdgeLeft, verticalEdgeRight, horizontalEdgeTop, horizontalEdgeBottom;

		SearchWindow();
		~SearchWindow();
		void setSize(int _x, int _y, int _width, int _height);
		void initDepthValues(IplImage *maskImage, IplImage *depthMap);
		bool shift();
		void extractInfo(IplImage *maskImage, IplImage *depthMap, bool initDepth);
		void getResizeAttribsEdgeDensityLinear(int &resizeDx, int &resizeDy, int &resizeDw, int &resizeDh);
		void getResizeAttribsInnerDensity(int &resizeDx, int &resizeDy, int &resizeDw, int &resizeDh);
		void getResizeAttribsEdgeDensityFuzzy(int &resizeDx, int &resizeDy, int &resizeDw, int &resizeDh);
		bool meanShift(IplImage *maskImage, IplImage *depthMap, int maxIteration, bool initDepth);
	};

public:
	enum TrackingState
	{
		tsNone 			= 0,
		tsSearching 	= 1,
		tsTracking 		= 2,
		tsSetWindow 	= 3,
		tsDisabled		= 10
	};

	enum ResizeMethod {
		rmEdgeDensityLinear		= 0,
		rmEdgeDensityFuzzy		= 1,
		rmInnerDensity			= 2
	};

	enum {
		MinKernelMass			= 1000
	};

	SearchWindow kernel;
	int searchMode;

private:
	enum
	{
		MaxMeanShiftIteration 	= 5,
		MaxSetSizeIteration 	= 5
	};

	void findOptimumSearchWindow(SearchWindow &searchWindow, IplImage *maskImage, IplImage *depthMap, int maxIteration, int resizeMethod, bool initDepth);

public:
	CvFuzzyMeanShiftTracker();
	~CvFuzzyMeanShiftTracker();

	void track(IplImage *maskImage, IplImage *depthMap, int resizeMethod, bool resetSearch, int minKernelMass = MinKernelMass);
};


namespace cv
{

class CV_EXPORTS OctTree
{
public:    
    struct Node
    {
        Node() {}
        int begin, end;
        float x_min, x_max, y_min, y_max, z_min, z_max;		
        int maxLevels;
        bool isLeaf;
        int children[8];
    };

    OctTree();
    OctTree( const Vector<Point3f>& points, int maxLevels = 10, int minPoints = 20 );
    virtual ~OctTree();

    virtual void buildTree( const Vector<Point3f>& points, int maxLevels = 10, int minPoints = 20 );
    virtual void getPointsWithinSphere( const Point3f& center, float radius,
                                        Vector<Point3f>& points ) const;
    const Vector<Node>& getNodes() const { return nodes; }
private:
    int minPoints;
    Vector<Point3f> points;
    Vector<Node> nodes;
	
	virtual void buildNext(Node& node);
};


CV_EXPORTS void computeNormals( const OctTree& octtree, 
                     const Vector<Point3f>& centers, 
                     Vector<Point3f>& normals, 
                     Vector<uchar>& mask, 
                     float normalRadius,
                     int minNeighbors = 20);

CV_EXPORTS void computeSpinImages( const OctTree& octtree, 
                        const Vector<Point3f>& points,
                        const Vector<Point3f>& normals,
                        Vector<uchar>& mask,
                        Mat& spinImages,						  
                        float support, 
                        float pixelsPerMeter );

struct CV_EXPORTS HOGParams
{
    HOGParams();
    HOGParams( int cell_n, int cell_x, int cell_y,
               int bin_num, int stride_x, int stride_y );
    
    int histSize() const { return cellN * cellN * nbins; }

    int cellN; // each block will have cell_n * cell_n cells;
    int cellX; // cell width in pixels
    int cellY; // cell height in pixels

    int nbins; // number of histogram bins per cell
    int strideX; // horizontal block shift
    int strideY; // vertical block shift 
};

CV_EXPORTS void extractHOG( const Mat& image, Mat& hogs,
                const HOGParams& params = HOGParams()); 

}

#endif /* __cplusplus */

#endif /* __CVAUX_HPP__ */

/* End of file. */
