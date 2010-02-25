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

/*
  This is a variation of
  "Stereo Processing by Semiglobal Matching and Mutual Information"
  by Heiko Hirschmuller.
  
  We match blocks rather than individual pixels, thus the algorithm is called
  SGBM (Semi-global block matching)
*/ 

#include "_cv.h"
#include <limits.h>

namespace cv
{

typedef uchar PixType;
typedef short CostType;
typedef short DispType;
    
enum { NR = 16, NR2 = NR/2 };
    
StereoSGBM::StereoSGBM()
{
    minDisparity = numberOfDisparities = 0;
    SADWindowSize = 0;
    P1 = P2 = 0;
    disp12MaxDiff = 0;
    preFilterCap = 0;
    uniquenessRatio = 0;
    speckleWindowSize = 0;
    speckleRange = 0;
}

    
StereoSGBM::StereoSGBM( int _minDisparity, int _numDisparities, int _SADWindowSize,
                    int _P1, int _P2, int _disp12MaxDiff, int _preFilterCap,
                    int _uniquenessRatio, int _speckleWindowSize, int _speckleRange )
{
    minDisparity = _minDisparity;
    numberOfDisparities = _numDisparities;
    SADWindowSize = _SADWindowSize;
    P1 = _P1;
    P2 = _P2;
    disp12MaxDiff = _disp12MaxDiff;
    preFilterCap = _preFilterCap;
    uniquenessRatio = _uniquenessRatio;
    speckleWindowSize = _speckleWindowSize;
    speckleRange = _speckleRange;
}

    
StereoSGBM::~StereoSGBM()
{
}

/*
  For each pixel row1[x], max(-maxD, 0) <= minX <= x < maxX <= width - max(0, -minD),
  and for each disparity minD<=d<maxD the function
  computes the cost (cost[(x-minX)*(maxD - minD) + (d - minD)]), depending on the difference between
  row1[x] and row2[x-d]. The subpixel algorithm from
  "Depth Discontinuities by Pixel-to-Pixel Stereo" by Stan Birchfield and C. Tomasi
  is used, hence the suffix BT.
 
  the temporary buffer should contain width2*2 elements
 */
static void calcPixelCostBT( const Mat& img1, const Mat& img2, int y,
                             int minD, int maxD, CostType* cost,
                             PixType* buffer, const PixType* tab, int tabOfs )
{
    int x, c, width = img1.cols, cn = img1.channels();
    int minX1 = max(maxD, 0), maxX1 = width + min(minD, 0);
    int minX2 = max(minX1 - maxD, 0), maxX2 = min(maxX1 - minD, width);
    int D = maxD - minD, width1 = maxX1 - minX1, width2 = maxX2 - minX2;
    const PixType *row1 = img1.ptr<PixType>(y), *row2 = img2.ptr<PixType>(y);
    PixType *prow1 = buffer + width2*2, *prow2 = prow1 + width*cn;
    
    tab += tabOfs;
    
    for( c = 0; c < cn; c++ )
    {
        prow1[width*c] = prow1[width*c + width-1] = 
        prow2[width*c] = prow2[width*c + width-1] = tab[0];
    }
    
    if( cn == 1 )
    {
        int n1 = y > 0 ? -img1.step : 0, s1 = y < img1.rows-1 ? img1.step : 0;
        int n2 = y > 0 ? -img2.step : 0, s2 = y < img2.rows-1 ? img2.step : 0;
        
        for( x = 1; x < width-1; x++ )
        {
            //prow1[x] = tab[row1[x+1] - row1[x-1]];
            //prow2[width-1-x] = tab[row2[x+1] - row2[x-1]];
            prow1[x] = tab[(row1[x+1] - row1[x-1])*2 + row1[x+n1+1] - row1[x+n1-1] + row1[x+s1+1] - row1[x+s1-1]];
            prow2[width-1-x] = tab[(row2[x+1] - row2[x-1])*2 + row2[x+n2+1] - row2[x+n2-1] + row2[x+s2+1] - row2[x+s2-1]];
        }
    }
    else
    {
        for( x = 1; x < width-1; x++ )
        {
            prow1[x] = tab[row1[x*3+3] - row1[x*3-3]];
            prow1[x+width] = tab[row1[x*3+4] - row1[x*3-2]];
            prow1[x+width*2] = tab[row1[x*3+5] - row1[x*3-1]];
            
            prow2[width-1-x] = tab[row2[x*3+3] - row2[x*3-3]];
            prow2[width-1-x+width] = tab[row2[x*3+4] - row2[x*3-2]];
            prow2[width-1-x+width*2] = tab[row2[x*3+5] - row2[x*3-1]];
        }
    }
    
    memset( cost, 0, width1*D*sizeof(cost[0]) );
    
    buffer -= minX2;
    cost -= minX1*D + minD; // simplify the cost indices inside the loop
    
#if 1    
    for( c = 0; c < cn; c++, prow1 += width, prow2 += width )
    {
        // precompute
        //   v0 = min(row2[x-1/2], row2[x], row2[x+1/2]) and
        //   v1 = max(row2[x-1/2], row2[x], row2[x+1/2]) and
        for( x = minX2; x < maxX2; x++ )
        {
            int v = prow2[x];
            int vl = x > 0 ? (v + prow2[x-1])/2 : v;
            int vr = x < width-1 ? (v + prow2[x+1])/2 : v;
            int v0 = min(vl, vr); v0 = min(v0, v);
            int v1 = max(vl, vr); v1 = max(v1, v);
            buffer[x] = (PixType)v0;
            buffer[x + width2] = (PixType)v1;
        }
        
        for( x = minX1; x < maxX1; x++ )
        {
            int u = prow1[x];
            int ul = x > 0 ? (u + prow1[x-1])/2 : u;
            int ur = x < width-1 ? (u + prow1[x+1])/2 : u;
            int u0 = min(ul, ur); u0 = min(u0, u);
            int u1 = max(ul, ur); u1 = max(u1, u);
            
        #if CV_SSE2
            __m128i _u = _mm_set1_epi8(u), _u0 = _mm_set1_epi8(u0);
            __m128i _u1 = _mm_set1_epi8(u1), z = _mm_setzero_si128();
            
            for( int d = minD; d < maxD; d += 16 )
            {
                __m128i _v = _mm_loadu_si128((const __m128i*)(prow2 + width-x-1 + d));
                __m128i _v0 = _mm_loadu_si128((const __m128i*)(buffer + width-x-1 + d));
                __m128i _v1 = _mm_loadu_si128((const __m128i*)(buffer + width-x-1 + d + width2));
                __m128i c0 = _mm_max_epu8(_mm_subs_epu8(_u, _v1), _mm_subs_epu8(_v0, _u));
                __m128i c1 = _mm_max_epu8(_mm_subs_epu8(_v, _u1), _mm_subs_epu8(_u0, _v));
                __m128i diff = _mm_min_epu8(c0, c1);
                
                c0 = _mm_load_si128((__m128i*)(cost + x*D + d));
                c1 = _mm_load_si128((__m128i*)(cost + x*D + d + 8));
                
                _mm_store_si128((__m128i*)(cost + x*D + d), _mm_adds_epi16(c0, _mm_unpacklo_epi8(diff,z)));
                _mm_store_si128((__m128i*)(cost + x*D + d + 8), _mm_adds_epi16(c1, _mm_unpackhi_epi8(diff,z)));
            }
        #else
            for( int d = minD; d < maxD; d++ )
            {
                int v = prow2[width-x-1 + d];
                int v0 = buffer[width-x-1 + d];
                int v1 = buffer[width-x-1 + d + width2];
                int c0 = max(0, u - v1); c0 = max(c0, v0 - u);
                int c1 = max(0, v - u1); c1 = max(c1, u0 - v);
                
                cost[x*D + d] = (CostType)(cost[x*D+d] + min(c0, c1));
            }
        #endif
        }
    }
#else
    for( c = 0; c < cn; c++, prow1 += width, prow2 += width )
    {
        for( x = minX1; x < maxX1; x++ )
        {
            int u = prow1[x];
        #if CV_SSE2
            __m128i _u = _mm_set1_epi8(u), z = _mm_setzero_si128();
            
            for( int d = minD; d < maxD; d += 16 )
            {
                __m128i _v = _mm_loadu_si128((const __m128i*)(prow2 + width-1-x + d));
                __m128i diff = _mm_adds_epu8(_mm_subs_epu8(_u,_v), _mm_subs_epu8(_v,_u));
                __m128i c0 = _mm_load_si128((__m128i*)(cost + x*D + d));
                __m128i c1 = _mm_load_si128((__m128i*)(cost + x*D + d + 8));
                
                _mm_store_si128((__m128i*)(cost + x*D + d), _mm_adds_epi16(c0, _mm_unpacklo_epi8(diff,z)));
                _mm_store_si128((__m128i*)(cost + x*D + d + 8), _mm_adds_epi16(c1, _mm_unpackhi_epi8(diff,z)));
            }
        #else
            for( int d = minD; d < maxD; d++ )
            {
                int v = prow2[width-1-x + d];
                cost[x*D + d] = (CostType)(cost[x*D + d] + std::abs(u - v));
            }
        #endif
        }
    }
#endif
}

    
/*
  computes disparity for "roi" in img1 w.r.t. img2 and write it to disp1buf.
  that is, disp1buf(x, y)=d means that img1(x+roi.x, y+roi.y) ~ img2(x+roi.x-d, y+roi.y).
  minD <= d < maxD.
  disp2full is the reverse disparity map, that is:
    disp2full(x+roi.x,y+roi.y)=d means that img2(x+roi.x, y+roi.y) ~ img1(x+roi.x+d, y+roi.y)
 
  note that disp1buf will have the same size as the roi and
  disp2full will have the same size as img1 (or img2).
  On exit disp2buf is not the final disparity, it is an intermediate result that becomes
  final after all the tiles are processed.
 
  the disparity in disp1buf is written with sub-pixel accuracy
  (4 fractional bits, see CvStereoSGBM::DISP_SCALE),
  using quadratic interpolation, while the disparity in disp2buf
  is written as is, without interpolation.
 
  disp2cost also has the same size as img1 (or img2).
  It contains the minimum current cost, used to find the best disparity, corresponding to the minimal cost.
*/ 
static void computeDisparitySGBM( const Mat& img1, const Mat& img2,
                                  Mat& disp1, const StereoSGBM& params, 
                                  Mat& buffer )
{
    const int ALIGN = 16;
    const int DISP_SHIFT = StereoSGBM::DISP_SHIFT;
    const int DISP_SCALE = StereoSGBM::DISP_SCALE;
    const CostType MAX_COST = SHRT_MAX;
    
    int minD = params.minDisparity, maxD = minD + params.numberOfDisparities;
    Size SADWindowSize;
    SADWindowSize.width = SADWindowSize.height = params.SADWindowSize > 0 ? params.SADWindowSize : 5;
    int ftzero = max(params.preFilterCap, 15) | 1;
    int uniquenessRatio = params.uniquenessRatio > 0 ? params.uniquenessRatio : 10;
    int disp12MaxDiff = params.disp12MaxDiff > 0 ? params.disp12MaxDiff : 1;
    int P1 = params.P1 > 0 ? params.P1 : 2, P2 = max(params.P2 > 0 ? params.P2 : 5, P1+1);
    int k, width = disp1.cols, height = disp1.rows;
    int minX1 = max(maxD, 0), maxX1 = width + min(minD, 0);
    int D = maxD - minD, width1 = maxX1 - minX1;
    int INVALID_DISP = minD - 1, INVALID_DISP_SCALED = INVALID_DISP*DISP_SCALE;
    int SW2 = SADWindowSize.width/2, SH2 = SADWindowSize.height/2;
    const int TAB_OFS = 256*4, TAB_SIZE = 256 + TAB_OFS*2;
    PixType clipTab[TAB_SIZE];
    
    for( k = 0; k < TAB_SIZE; k++ )
        clipTab[k] = (PixType)(min(max(k - TAB_OFS, -ftzero), ftzero) + ftzero);
    
    if( minX1 >= maxX1 )
    {
        disp1 = Scalar::all(INVALID_DISP_SCALED);
        return;
    }
    
    CV_Assert( D % 16 == 0 );
    
    // NR - the number of directions. the loop on x below that computes Lr assumes that NR == 8.
    // if you change NR, please, modify the loop as well.
    int D2 = D+16, NRD2 = NR2*D2;
    
    // the number of L_r(.,.) and min_k L_r(.,.) lines in the buffer:
    // for 8-way dynamic programming we need the current row and
    // the previous row, i.e. 2 rows in total
    const int NLR = 3;
    const int LrBorder = NLR - 1;
    
    // for each possible stereo match (img1(x,y) <=> img2(x-d,y))
    // we keep pixel difference cost (C) and the summary cost over NR directions (S).
    // we also keep all the partial costs for the previous line L_r(x,d) and also min_k L_r(x, k)
    size_t costBufSize = width1*D;
    size_t minLrSize = (width1 + LrBorder*2)*NR2, LrSize = minLrSize*D2;
    int hsumBufNRows = SH2*2 + 2;
    size_t totalBufSize = (LrSize + minLrSize)*NLR*sizeof(CostType) + // minLr[] and Lr[]
        costBufSize*(hsumBufNRows + 3)*sizeof(CostType) + // hsumBuf, pixdiff, C and S
        width*8*img1.channels()*sizeof(PixType) + // temp buffer for computing per-pixel cost
        width*(sizeof(CostType) + sizeof(DispType)) + 1024; // disp2cost + disp2
    
    if( !buffer.data || !buffer.isContinuous() ||
        buffer.cols*buffer.rows*buffer.elemSize() < totalBufSize )
        buffer.create(1, totalBufSize, CV_8U);
    
    // summary cost over different (nDirs) directions
    CostType* C = (CostType*)alignPtr(buffer.data, ALIGN);
    CostType* hsumBuf = C + costBufSize;
    CostType* pixDiff = hsumBuf + costBufSize*hsumBufNRows;
    CostType* S = pixDiff + costBufSize;
    CostType *Lr[NLR]={0}, *minLr[NLR]={0};
    for( k = 0; k < NLR; k++ )
    {
        // shift Lr[k] and minLr[k] pointers, because we allocated them with the borders,
        // and will occasionally use negative indices with the arrays
        Lr[k] = S + costBufSize + LrSize*k + NRD2*LrBorder + 8;
        memset( Lr[k] - LrBorder*NRD2 - 8, 0, LrSize*sizeof(CostType) );
        minLr[k] = S + costBufSize + LrSize*NLR + minLrSize*k + NR2*2;
        memset( minLr[k] - LrBorder*NR2, 0, minLrSize*sizeof(CostType) );
    }
    
    CostType* disp2cost = S + costBufSize + (LrSize + minLrSize)*NLR;
    DispType* disp2ptr = (DispType*)(disp2cost + width);
    
    PixType* tempBuf = (PixType*)(disp2ptr + width);
    
    for( k = 0; k < width1*D; k++ )
        C[k] = P2;
    
    for( int y = 0; y < height; y++ )
    {
        int x, d, y1 = y == 0 ? 0 : y + SH2, y2 = y == 0 ? SH2 : y1;
        DispType* disp1ptr = disp1.ptr<DispType>(y);
        
        for( x = 0; x < width; x++ )
        {
            disp1ptr[x] = disp2ptr[x] = INVALID_DISP_SCALED;
            disp2cost[x] = MAX_COST;
        }
        
        for( k = y1; k <= y2; k++ )
        {
            CostType* hsumAdd = hsumBuf + (min(k, height-1) % hsumBufNRows)*costBufSize;
            
            if( k < height )
            {
                calcPixelCostBT( img1, img2, k, minD, maxD, pixDiff, tempBuf, clipTab, TAB_OFS );
                
                memset(hsumAdd, 0, D*sizeof(CostType));
                for( x = 0; x <= SW2*D; x += D )
                {
                    int scale = x == 0 ? SW2 + 1 : 1;
                    for( d = 0; d < D; d++ )
                        hsumAdd[d] = (CostType)(hsumAdd[d] + pixDiff[x + d]*scale);
                }

                if( y > 0 )
                {
                    const CostType* hsumSub = hsumBuf + (max(y - SH2 - 1, 0) % hsumBufNRows)*costBufSize;
                    for( x = D; x < width1*D; x += D )
                    {
                        const CostType* pixAdd = pixDiff + min(x + SW2*D, (width1-1)*D);
                        const CostType* pixSub = pixDiff + max(x - (SW2+1)*D, 0);
                        
                    #if CV_SSE2
                        for( d = 0; d < D; d += 8 )
                        {
                            __m128i hv = _mm_load_si128((const __m128i*)(hsumAdd + x - D + d));
                            __m128i Cx = _mm_load_si128((__m128i*)(C + x + d));
                            hv = _mm_adds_epi16(_mm_subs_epi16(hv,
                                            _mm_load_si128((const __m128i*)(pixSub + d))),
                                            _mm_load_si128((const __m128i*)(pixAdd + d)));
                            Cx = _mm_adds_epi16(_mm_subs_epi16(Cx,
                                                _mm_load_si128((const __m128i*)(hsumSub + x + d))),
                                                hv);
                            _mm_store_si128((__m128i*)(hsumAdd + x + d), hv);
                            _mm_store_si128((__m128i*)(C + x + d), Cx);
                        }
                    #else
                        for( d = 0; d < D; d++ )
                        {
                            int hv = hsumAdd[x + d] = (CostType)(hsumAdd[x - D + d] + pixAdd[d] - pixSub[d]);
                            C[x + d] = (CostType)(C[x + d] + hv - hsumSub[x + d]);
                        }
                    #endif
                    }
                }
                else
                {
                    for( x = D; x < width1*D; x += D )
                    {
                        const CostType* pixAdd = pixDiff + min(x + SW2*D, (width1-1)*D);
                        const CostType* pixSub = pixDiff + max(x - (SW2+1)*D, 0);
                        
                        for( d = 0; d < D; d++ )
                            hsumAdd[x + d] = (CostType)(hsumAdd[x - D + d] + pixAdd[d] - pixSub[d]);
                    }
                }
            }
            
            if( y == 0 )
            {
                int scale = k == 0 ? SH2 + 1 : 1;
                for( x = 0; x < width1*D; x++ )
                    C[x] = (CostType)(C[x] + hsumAdd[x]*scale);
            }
        }
        
        // clear the left and the right borders
        memset( Lr[0] - NRD2*LrBorder - 8, 0, NRD2*LrBorder*sizeof(CostType) );
        memset( Lr[0] + width1*NRD2 - 8, 0, NRD2*LrBorder*sizeof(CostType) );
        memset( minLr[0] - NR2*LrBorder, 0, NR2*LrBorder*sizeof(CostType) );
        memset( minLr[0] + width1*NR2, 0, NR2*LrBorder*sizeof(CostType) );
        
        /*
           [formula 13 in the paper]
           compute L_r(p, d) = C(p, d) +
                min(L_r(p-r, d),
                    L_r(p-r, d-1) + P1,
                    L_r(p-r, d+1) + P1,
                    min_k L_r(p-r, k) + P2) - min_k L_r(p-r, k)
           where p = (x,y), r is one of the directions.
           we process all the directions at once:
             0: r=(-dx, 0)
             1: r=(-1, -dy)
             2: r=(0, -dy)
             3: r=(1, -dy)
             4: r=(-2, -dy)
             5: r=(-1, -dy*2)
             6: r=(1, -dy*2)
             7: r=(2, -dy)
        */
        for( x = 0; x < width1; x++ )
        {
            int xm = x*NR2, xd = xm*D2;
            
            int delta0 = minLr[0][xm - NR2] + P2, delta1 = minLr[1][xm - NR2 + 1] + P2;
            int delta2 = minLr[1][xm + 2] + P2, delta3 = minLr[1][xm + NR2 + 3] + P2;
            
            CostType* Lr_p0 = Lr[0] + xd - NRD2;
            CostType* Lr_p1 = Lr[1] + xd - NRD2 + D2;
            CostType* Lr_p2 = Lr[1] + xd + D2*2;
            CostType* Lr_p3 = Lr[1] + xd + NRD2 + D2*3;
            
            Lr_p0[-1] = Lr_p0[D] = Lr_p1[-1] = Lr_p1[D] =
            Lr_p2[-1] = Lr_p2[D] = Lr_p3[-1] = Lr_p3[D] = MAX_COST;
            
            CostType* Lr_p = Lr[0] + xd;
            const CostType* Cp = C + x*D;
            CostType* Sp = S + x*D;
            
        #if CV_SSE2
            __m128i _P1 = _mm_set1_epi16((short)P1);
            
            __m128i _delta0 = _mm_set1_epi16((short)delta0);
            __m128i _delta1 = _mm_set1_epi16((short)delta1);
            __m128i _delta2 = _mm_set1_epi16((short)delta2);
            __m128i _delta3 = _mm_set1_epi16((short)delta3);
            __m128i _minL0 = _mm_set1_epi16((short)MAX_COST);
            
            for( d = 0; d < D; d += 8 )
            {
                __m128i Cpd = _mm_load_si128((const __m128i*)(Cp + d));
                __m128i L0, L1, L2, L3;
                
                L0 = _mm_load_si128((const __m128i*)(Lr_p0 + d));
                L1 = _mm_load_si128((const __m128i*)(Lr_p1 + d));
                L2 = _mm_load_si128((const __m128i*)(Lr_p2 + d));
                L3 = _mm_load_si128((const __m128i*)(Lr_p3 + d));
                
                L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d - 1)), _P1));
                L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d + 1)), _P1));
                
                L1 = _mm_min_epi16(L1, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p1 + d - 1)), _P1));
                L1 = _mm_min_epi16(L1, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p1 + d + 1)), _P1));
                
                L2 = _mm_min_epi16(L2, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p2 + d - 1)), _P1));
                L2 = _mm_min_epi16(L2, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p2 + d + 1)), _P1));
                
                L3 = _mm_min_epi16(L3, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p3 + d - 1)), _P1));
                L3 = _mm_min_epi16(L3, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p3 + d + 1)), _P1));
                
                L0 = _mm_min_epi16(L0, _delta0);
                L0 = _mm_adds_epi16(_mm_subs_epi16(L0, _delta0), Cpd);
                
                L1 = _mm_min_epi16(L1, _delta1);
                L1 = _mm_adds_epi16(_mm_subs_epi16(L1, _delta1), Cpd);
                
                L2 = _mm_min_epi16(L2, _delta2);
                L2 = _mm_adds_epi16(_mm_subs_epi16(L2, _delta2), Cpd);
                
                L3 = _mm_min_epi16(L3, _delta3);
                L3 = _mm_adds_epi16(_mm_subs_epi16(L3, _delta3), Cpd);
                
                _mm_store_si128( (__m128i*)(Lr_p + d), L0);
                _mm_store_si128( (__m128i*)(Lr_p + d + D2), L1);
                _mm_store_si128( (__m128i*)(Lr_p + d + D2*2), L2);
                _mm_store_si128( (__m128i*)(Lr_p + d + D2*3), L3);
                
                __m128i t0 = _mm_min_epi16(_mm_unpacklo_epi16(L0, L2), _mm_unpackhi_epi16(L0, L2));
                __m128i t1 = _mm_min_epi16(_mm_unpacklo_epi16(L1, L3), _mm_unpackhi_epi16(L1, L3));
                t0 = _mm_min_epi16(_mm_unpacklo_epi16(t0, t1), _mm_unpackhi_epi16(t0, t1));
                _minL0 = _mm_min_epi16(_minL0, t0);
                
                L0 = _mm_adds_epi16(L0, L1);
                L2 = _mm_adds_epi16(L2, L3);
                L0 = _mm_adds_epi16(L0, L2);
                
                _mm_store_si128((__m128i*)(Sp + d), L0);
            }
            
            _minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 8));
            _mm_storel_epi64((__m128i*)&minLr[0][xm], _minL0);
            
        #else
            int minL0 = MAX_COST, minL1 = MAX_COST, minL2 = MAX_COST, minL3 = MAX_COST;
            
            for( d = 0; d < D; d++ )
            {
                int Cpd = Cp[d], L0, L1, L2, L3;
                
                L0 = Cpd + min((int)Lr_p0[d], min(Lr_p0[d-1] + P1, min(Lr_p0[d+1] + P1, delta0))) - delta0;
                L1 = Cpd + min((int)Lr_p1[d], min(Lr_p1[d-1] + P1, min(Lr_p1[d+1] + P1, delta1))) - delta1;                    
                L2 = Cpd + min((int)Lr_p2[d], min(Lr_p2[d-1] + P1, min(Lr_p2[d+1] + P1, delta2))) - delta2;
                L3 = Cpd + min((int)Lr_p3[d], min(Lr_p3[d-1] + P1, min(Lr_p3[d+1] + P1, delta3))) - delta3;
                
                Lr_p[d] = (CostType)L0;
                minL0 = min(minL0, L0);
                
                Lr_p[d + D2] = (CostType)L1;
                minL1 = min(minL1, L1);
                
                Lr_p[d + D2*2] = (CostType)L2;
                minL2 = min(minL2, L2);
                
                Lr_p[d + D2*3] = (CostType)L3;
                minL3 = min(minL3, L3);
                
                Sp[d] = saturate_cast<CostType>(L0 + L1 + L2 + L3);
            }
            minLr[0][xm] = (CostType)minL0;
            minLr[0][xm+1] = (CostType)minL1;
            minLr[0][xm+2] = (CostType)minL2;
            minLr[0][xm+3] = (CostType)minL3;
        #endif
        }
        
        for( x = width1 - 1; x >= 0; x-- )
        {
            int xm = x*NR2, xd = xm*D2;
            
            int minL0 = MAX_COST;
            int delta0 = minLr[0][xm + NR2] + P2;
            CostType* Lr_p0 = Lr[0] + xd + NRD2;
            Lr_p0[-1] = Lr_p0[D] = MAX_COST;
            CostType* Lr_p = Lr[0] + xd;
            
            const CostType* Cp = C + x*D;
            CostType* Sp = S + x*D;
            CostType minS = MAX_COST;
            int bestDisp = -1;

        #if CV_SSE2
            __m128i _P1 = _mm_set1_epi16((short)P1);
            __m128i _delta0 = _mm_set1_epi16((short)delta0);
            
            __m128i _minL0 = _mm_set1_epi16((short)minL0);
            __m128i _minS = _mm_set1_epi16(MAX_COST), _bestDisp = _mm_set1_epi16(-1);
            __m128i _d8 = _mm_setr_epi16(0, 1, 2, 3, 4, 5, 6, 7), _8 = _mm_set1_epi16(8);
            
            for( d = 0; d < D; d += 8 )
            {
                __m128i Cpd = _mm_load_si128((const __m128i*)(Cp + d)), L0;
                
                L0 = _mm_load_si128((const __m128i*)(Lr_p0 + d));
                L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d - 1)), _P1));
                L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d + 1)), _P1));
                L0 = _mm_min_epi16(L0, _delta0);
                L0 = _mm_adds_epi16(_mm_subs_epi16(L0, _delta0), Cpd);
                
                _mm_store_si128((__m128i*)(Lr_p + d), L0);
                _minL0 = _mm_min_epi16(_minL0, L0);
                L0 = _mm_adds_epi16(L0, *(__m128i*)(Sp + d));
                _mm_store_si128((__m128i*)(Sp + d), L0);
                
                __m128i mask = _mm_cmpgt_epi16(_minS, L0);
                _minS = _mm_min_epi16(_minS, L0);
                _bestDisp = _mm_xor_si128(_bestDisp, _mm_and_si128(_mm_xor_si128(_bestDisp,_d8), mask));
                _d8 = _mm_adds_epi16(_d8, _8);
            }
            
            short CV_DECL_ALIGNED(16) bestDispBuf[8];
            _mm_store_si128((__m128i*)bestDispBuf, _bestDisp);
            
            _minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 8));
            _minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 4));
            _minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 2));
            
            __m128i _S = _mm_min_epi16(_minS, _mm_srli_si128(_minS, 8));
            _S = _mm_min_epi16(_S, _mm_srli_si128(_S, 4));
            _S = _mm_min_epi16(_S, _mm_srli_si128(_S, 2));
            
            minLr[0][xm] = (CostType)_mm_cvtsi128_si32(_minL0);
            minS = (CostType)_mm_cvtsi128_si32(_S);
            
            _S = _mm_shuffle_epi32(_mm_unpacklo_epi16(_S, _S), 0);
            _S = _mm_cmpeq_epi16(_minS, _S);
            int idx = _mm_movemask_epi8(_mm_packs_epi16(_S, _S)) & 255;
            
            static const uchar LSBTab[] =
            {
                0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
                5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
            };
            
            bestDisp = bestDispBuf[LSBTab[idx]];
        #else
            for( d = 0; d < D; d++ )
            {
                int L0 = Cp[d] + min((int)Lr_p0[d], min(Lr_p0[d-1] + P1, min(Lr_p0[d+1] + P1, delta0))) - delta0;
                
                Lr_p[d] = (CostType)L0;
                minL0 = min(minL0, L0);
                
                int Sval = Sp[d] = saturate_cast<CostType>(Sp[d] + L0);
                if( Sval < minS )
                {
                    minS = Sval;
                    bestDisp = d;
                }
            }
            minLr[0][xm] = (CostType)minL0;
        #endif
            
            for( d = 0; d < D; d++ )
            {
                if( Sp[d]*(100 - uniquenessRatio) < minS*100 && std::abs(bestDisp - d) > 1 )
                    break;
            }
            if( d < D )
                continue;
            d = bestDisp;
            int x2 = x + minX1 - d - minD;
            if( disp2cost[x2] > minS )
            {
                disp2cost[x2] = (CostType)minS;
                disp2ptr[x2] = (DispType)(d + minD);
            }
            
            if( 0 < d && d < D-1 )
            {
                // do subpixel quadratic interpolation:
                //   fit parabola into (x1=d-1, y1=Sp[d-1]), (x2=d, y2=Sp[d]), (x3=d+1, y3=Sp[d+1])
                //   then find minimum of the parabola.
                int denom2 = max(Sp[d-1] + Sp[d+1] - 2*Sp[d], 1);
                d = d*DISP_SCALE + ((Sp[d-1] - Sp[d+1])*DISP_SCALE + denom2)/(denom2*2);
            }
            else
                d *= DISP_SCALE;
            disp1ptr[x + minX1] = (DispType)(d + minD*DISP_SCALE);
        }
        
        for( x = minX1; x < maxX1; x++ )
        {
            // we round the computed disparity both towards -inf and +inf and check
            // if either of the corresponding disparities in disp2 is consistent.
            // This is to give the computed disparity a chance to look valid if it is.
            int d = disp1ptr[x];
            if( d == INVALID_DISP_SCALED )
                continue;
            int _d = d >> DISP_SHIFT;
            int d_ = (d + DISP_SCALE-1) >> DISP_SHIFT;
            int _x = x - _d, x_ = x - d_;
            if( 0 <= _x && _x < width && disp2ptr[_x] >= minD && std::abs(disp2ptr[_x] - _d) > disp12MaxDiff &&
               0 <= x_ && x_ < width && disp2ptr[x_] >= minD && std::abs(disp2ptr[x_] - d_) > disp12MaxDiff )
                disp1ptr[x] = (DispType)INVALID_DISP_SCALED;
        }
        
        // now shift the cyclic buffers
        std::swap( Lr[0], Lr[1] );
        std::swap( Lr[0], Lr[2] );
        std::swap( minLr[0], minLr[1] );
        std::swap( minLr[0], minLr[2] );
    }
}

typedef cv::Point_<short> Point2s;
    
void filterSpeckles( Mat& img, double _newval, int maxSpeckleSize, double _maxDiff, Mat& _buf )
{
    CV_Assert( img.type() == CV_16SC1 );
    
    int newVal = cvRound(_newval);
    int maxDiff = cvRound(_maxDiff);
    int width = img.cols, height = img.rows, npixels = width*height;
    size_t bufSize = npixels*(int)(sizeof(Point2s) + sizeof(int) + sizeof(uchar));
    if( !_buf.isContinuous() || !_buf.data || _buf.cols*_buf.rows*_buf.elemSize() < bufSize )
        _buf.create(1, bufSize, CV_8U);
    
    uchar* buf = _buf.data;
    int i, j, dstep = img.step/sizeof(short);
    int* labels = (int*)buf;
    buf += npixels*sizeof(labels[0]);
    Point2s* wbuf = (Point2s*)buf;
    buf += npixels*sizeof(wbuf[0]);
    uchar* rtype = (uchar*)buf;
    int curlabel = 0;
    
    // clear out label assignments
    memset(labels, 0, npixels*sizeof(labels[0]));
    
    for( i = 1; i < height-1; i++ )
    {
        short* ds = img.ptr<short>(i);
        int* ls = labels + width*i;
        
        for( j = 1; j < width-1; j++ )
        {
            if( ds[j] != newVal )	// not a bad disparity
            {
                if( ls[j] )		// has a label, check for bad label
                {  
                    if( rtype[ls[j]] ) // small region, zero out disparity
                        ds[j] = newVal;
                }
                // no label, assign and propagate
                else
                {
                    Point2s* ws = wbuf;	// initialize wavefront
                    Point2s p(j, i);	// current pixel
                    curlabel++;	// next label
                    int count = 0;	// current region size
                    ls[j] = curlabel;
                    
                    // wavefront propagation
                    while( ws >= wbuf ) // wavefront not empty
                    {
                        count++;
                        // put neighbors onto wavefront
                        short* dpp = &img.at<short>(p.y, p.x);
                        short dp = *dpp;
                        int* lpp = labels + width*p.y + p.x;
                        
                        if( p.x < width-1 && !lpp[+1] && dpp[+1] != newVal && std::abs(dp - dpp[+1]) <= maxDiff )
                        {
                            lpp[+1] = curlabel;
                            *ws++ = Point2s(p.x+1, p.y);
                        }
                        
                        if( p.x > 0 && !lpp[-1] && dpp[-1] != newVal && std::abs(dp - dpp[-1]) <= maxDiff )
                        {
                            lpp[-1] = curlabel;
                            *ws++ = Point2s(p.x-1, p.y);
                        }
                        
                        if( p.y < height-1 && !lpp[+width] && dpp[+dstep] != newVal && std::abs(dp - dpp[+dstep]) <= maxDiff )
                        {
                            lpp[+width] = curlabel;
                            *ws++ = Point2s(p.x, p.y+1);
                        }
                        
                        if( p.y > 0 && !lpp[-width] && dpp[-dstep] != newVal && std::abs(dp - dpp[-dstep]) <= maxDiff )
                        {
                            lpp[-width] = curlabel;
                            *ws++ = Point2s(p.x, p.y-1);
                        }
                        
                        // pop most recent and propagate
                        // NB: could try least recent, maybe better convergence
                        p = *--ws;
                    }
                    
                    // assign label type
                    if( count <= maxSpeckleSize )	// speckle region
                    {
                        rtype[ls[j]] = 1;	// small region label
                        ds[j] = newVal;
                    }
                    else
                        rtype[ls[j]] = 0;	// large region label
                }
            }
        }
    }
}    
    
    
void StereoSGBM::operator ()( const Mat& left, const Mat& right, Mat& disp )
{
    CV_Assert( left.size() == right.size() && left.type() == right.type() &&
               left.depth() == DataType<PixType>::depth );
    
    disp.create( left.size(), CV_16S );
    
    computeDisparitySGBM( left, right, disp, *this, buffer );
    medianBlur(disp, disp, 3);
    
    if( speckleWindowSize > 0 )
        filterSpeckles(disp, (minDisparity - 1)*DISP_SCALE, 100, DISP_SCALE, buffer);
}
    
}

