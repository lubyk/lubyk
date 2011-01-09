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
 *  one_way_descriptor.cpp
 *  
 *
 *  Created by Victor  Eruhimov on 4/19/09.
 *  Copyright 2009 Argus Corp. All rights reserved.
 *
 */

#include "_cvaux.h"
#include "highgui.h"

namespace cv
{

static const float pi = (float)CV_PI;
    
static void readPCAFeatures(const char* filename, CvMat** avg, CvMat** eigenvectors);
static void eigenvector2image(CvMat* eigenvector, IplImage* img);

inline CvRect resize_rect(CvRect rect, float alpha)
{
	return cvRect(rect.x + cvRound((float)(0.5*(1 - alpha)*rect.width)), rect.y + cvRound((float)(0.5*(1 - alpha)*rect.height)), 
				  cvRound(rect.width*alpha), cvRound(rect.height*alpha));
}

inline CvRect fit_rect_roi_fixedsize(CvRect rect, CvRect roi)
{
	CvRect fit = rect;
	fit.x = MAX(fit.x, roi.x);
	fit.y = MAX(fit.y, roi.y);
    fit.x = MIN(fit.x, roi.x + roi.width - fit.width - 1);
    fit.y = MIN(fit.y, roi.y + roi.height - fit.height - 1);
	return(fit);
}

inline CvRect fit_rect_fixedsize(CvRect rect, IplImage* img)
{
	CvRect roi = cvGetImageROI(img);
	return fit_rect_roi_fixedsize(rect, roi);
}


// AffinePose: defines a parameterized affine transformation of an image patch. 
// An image patch is rotated on angle phi (in degrees), then scaled lambda1 times
// along horizontal and lambda2 times along vertical direction, and then rotated again 
// on angle (theta - phi).
class AffinePose
{
public:
    float phi;
    float theta;
    float lambda1;
    float lambda2;
};

// AffineTransformPatch: generates an affine transformed image patch. 
// - src: source image (roi is supported)
// - dst: output image. ROI of dst image should be 2 times smaller than ROI of src. 
// - pose: parameters of an affine transformation
void AffineTransformPatch(IplImage* src, IplImage* dst, AffinePose pose);

// GenerateAffineTransformFromPose: generates an affine transformation matrix from AffinePose instance
// - size: the size of image patch
// - pose: affine transformation
// - transform: 2x3 transformation matrix
void GenerateAffineTransformFromPose(Size size, AffinePose pose, CvMat* transform);

// Generates a random affine pose
AffinePose GenRandomAffinePose();

const static int num_mean_components = 500;
const static float noise_intensity = 0.15f;


CvMat* ConvertImageToMatrix(IplImage* patch)
{
    CvRect roi = cvGetImageROI(patch);
    CvMat* mat = cvCreateMat(1, roi.width*roi.height, CV_32FC1);
    
    if(patch->depth == 32)
    {
        for(int y = 0; y < roi.height; y++)
        {
            for(int x = 0; x < roi.width; x++)
            {
                mat->data.fl[y*roi.width + x] = *((float*)(patch->imageData + (y + roi.y)*patch->widthStep) + x + roi.x);
            }
        }
    }
    else if(patch->depth == 8)
    {
        for(int y = 0; y < roi.height; y++)
        {
            for(int x = 0; x < roi.width; x++)
            {
                mat->data.fl[y*roi.width + x] = (float)(unsigned char)patch->imageData[(y + roi.y)*patch->widthStep + x + roi.x];
            }
        }
    }
    else
    {
        printf("Image depth %d is not supported\n", patch->depth);
        return 0;
    }
    
    return mat;
}

const OneWayDescriptor* OneWayDescriptorBase::GetDescriptor(int desc_idx) const
{
    return &m_descriptors[desc_idx];
}

OneWayDescriptorBase::OneWayDescriptorBase(Size patch_size, int pose_count, const char* train_path, 
                                               const char* pca_config, const char* pca_hr_config, 
                                               const char* pca_desc_config, int pyr_levels, 
                                               int pca_dim_high, int pca_dim_low) : m_pca_dim_high(pca_dim_high), m_pca_dim_low(pca_dim_low)
{
    m_patch_size = patch_size;
    m_pose_count = pose_count;
    m_pyr_levels = pyr_levels;
    m_poses = 0;
    m_transforms = 0;
    
    m_pca_avg = 0;
    m_pca_eigenvectors = 0;
    m_pca_hr_avg = 0;
    m_pca_hr_eigenvectors = 0;
    m_pca_descriptors = 0;
    
    m_descriptors = 0;
    
    if(train_path == 0 || strlen(train_path) == 0)
    {
        // skip pca loading
        return;
    }
    char pca_config_filename[1024];
    sprintf(pca_config_filename, "%s/%s", train_path, pca_config);
    readPCAFeatures(pca_config_filename, &m_pca_avg, &m_pca_eigenvectors);
    if(pca_hr_config && strlen(pca_hr_config) > 0)
    {
        char pca_hr_config_filename[1024];
        sprintf(pca_hr_config_filename, "%s/%s", train_path, pca_hr_config);
        readPCAFeatures(pca_hr_config_filename, &m_pca_hr_avg, &m_pca_hr_eigenvectors);
    }
    
    m_pca_descriptors = new OneWayDescriptor[m_pca_dim_high + 1];
    if(pca_desc_config && strlen(pca_desc_config) > 0)
//    if(0)
    {
        char pca_desc_config_filename[1024];
        sprintf(pca_desc_config_filename, "%s/%s", train_path, pca_desc_config);
        LoadPCADescriptors(pca_desc_config_filename);
    }
    else
    {
        printf("Initializing the descriptors...\n");
        InitializePoseTransforms();
        CreatePCADescriptors();
        SavePCADescriptors("pca_descriptors.yml");
    }
//    SavePCADescriptors("./pca_descriptors.yml");
    
}

OneWayDescriptorBase::~OneWayDescriptorBase()
{
    cvReleaseMat(&m_pca_avg);
    cvReleaseMat(&m_pca_eigenvectors);
    
    if(m_pca_hr_eigenvectors)
    {
        delete[] m_pca_descriptors;
        cvReleaseMat(&m_pca_hr_avg);
        cvReleaseMat(&m_pca_hr_eigenvectors);
    }
    
    
    delete []m_descriptors;
    
    if(!m_transforms)
    {
        delete []m_poses;
    }
    
    for(int i = 0; i < m_pose_count; i++)
    {
        cvReleaseMat(&m_transforms[i]);
    }
    delete []m_transforms;
}

void OneWayDescriptorBase::InitializePoses()
{
    m_poses = new AffinePose[m_pose_count];
    for(int i = 0; i < m_pose_count; i++)
    {
        m_poses[i] = GenRandomAffinePose();
    }
}

void OneWayDescriptorBase::InitializeTransformsFromPoses()
{
    m_transforms = new CvMat*[m_pose_count];
    for(int i = 0; i < m_pose_count; i++)
    {
        m_transforms[i] = cvCreateMat(2, 3, CV_32FC1);
        GenerateAffineTransformFromPose(cvSize(m_patch_size.width*2, m_patch_size.height*2), m_poses[i], m_transforms[i]);
    }        
}

void OneWayDescriptorBase::InitializePoseTransforms()
{
    InitializePoses();
    InitializeTransformsFromPoses();
}

void OneWayDescriptorBase::InitializeDescriptor(int desc_idx, IplImage* train_image, const char* feature_label)
{
    m_descriptors[desc_idx].SetPCADimHigh(m_pca_dim_high);
    m_descriptors[desc_idx].SetPCADimLow(m_pca_dim_low);
    
    if(!m_pca_hr_eigenvectors)
    {
        m_descriptors[desc_idx].Initialize(m_pose_count, train_image, feature_label);
    }
    else
    {
        m_descriptors[desc_idx].InitializeFast(m_pose_count, train_image, feature_label, 
                                      m_pca_hr_avg, m_pca_hr_eigenvectors, m_pca_descriptors);
    }
    
    if(m_pca_avg)
    {
        m_descriptors[desc_idx].InitializePCACoeffs(m_pca_avg, m_pca_eigenvectors);    
    }
}

void OneWayDescriptorBase::FindDescriptor(IplImage* src, Point2f pt, int& desc_idx, int& pose_idx, float& distance) const
{
    CvRect roi = cvRect(cvRound(pt.x - m_patch_size.width/4),
        cvRound(pt.y - m_patch_size.height/4),
        m_patch_size.width/2, m_patch_size.height/2);
    cvSetImageROI(src, roi);
    
    FindDescriptor(src, desc_idx, pose_idx, distance);
    
    cvResetImageROI(src);
}

void OneWayDescriptorBase::FindDescriptor(IplImage* patch, int& desc_idx, int& pose_idx, float& distance) const
{
#if 1
    findOneWayDescriptor(m_train_feature_count, m_descriptors, patch, desc_idx, pose_idx, distance, m_pca_avg, m_pca_eigenvectors);
#else
    const float scale_min = 0.8f;
    const float scale_max = 1.2f;
    const float scale_step = 1.1f;
    float scale = 1.0f;
    findOneWayDescriptor(m_train_feature_count, m_descriptors, patch, 
                         scale_min, scale_max, scale_step, desc_idx, pose_idx, distance, scale, 
                         m_pca_avg, m_pca_eigenvectors);
#endif
}

void OneWayDescriptorBase::SetPCAHigh(CvMat* avg, CvMat* eigenvectors)
{
    m_pca_hr_avg = cvCloneMat(avg);
    m_pca_hr_eigenvectors = cvCloneMat(eigenvectors);
}

void OneWayDescriptorBase::SetPCALow(CvMat* avg, CvMat* eigenvectors)
{
    m_pca_avg = cvCloneMat(avg);
    m_pca_eigenvectors = cvCloneMat(eigenvectors);
}

void OneWayDescriptorBase::AllocatePCADescriptors()
{
    m_pca_descriptors = new OneWayDescriptor[m_pca_dim_high + 1];
    for(int i = 0; i < m_pca_dim_high + 1; i++)
    {
        m_pca_descriptors[i].SetPCADimHigh(m_pca_dim_high);
        m_pca_descriptors[i].SetPCADimLow(m_pca_dim_low);
    }
}

void OneWayDescriptorBase::CreatePCADescriptors()
{
    if(m_pca_descriptors == 0)
    {
        AllocatePCADescriptors();
    }
    IplImage* frontal = cvCreateImage(m_patch_size, IPL_DEPTH_32F, 1);
    
    eigenvector2image(m_pca_hr_avg, frontal);
    m_pca_descriptors[0].SetTransforms(m_poses, m_transforms);
    m_pca_descriptors[0].Initialize(m_pose_count, frontal, "", 0);
    
    for(int j = 0; j < m_pca_dim_high; j++)
    {
        CvMat eigenvector;
        cvGetSubRect(m_pca_hr_eigenvectors, &eigenvector, cvRect(0, j, m_pca_hr_eigenvectors->cols, 1));
        eigenvector2image(&eigenvector, frontal);
        
        m_pca_descriptors[j + 1].SetTransforms(m_poses, m_transforms);
        m_pca_descriptors[j + 1].Initialize(m_pose_count, frontal, "", 0);
        
        printf("Created descriptor for PCA component %d\n", j);
    }
    
    cvReleaseImage(&frontal);
}


int OneWayDescriptorBase::LoadPCADescriptors(const char* filename)
{
    CvMemStorage* storage = cvCreateMemStorage();
    CvFileStorage* fs = cvOpenFileStorage(filename, storage, CV_STORAGE_READ);
    
    // read affine poses
    CvFileNode* node = cvGetFileNodeByName(fs, 0, "affine poses");
    if(node != 0)
    {
        CvMat* poses = (CvMat*)cvRead(fs, node);
        if(poses->rows != m_pose_count)
        {
            printf("Inconsistency in the number of poses between the class instance and the file! Exiting...\n");
            cvReleaseMat(&poses);
            cvReleaseFileStorage(&fs);
            cvReleaseMemStorage(&storage);
        }
        
        if(m_poses)
        {
            delete m_poses;
        }
        m_poses = new AffinePose[m_pose_count];
        for(int i = 0; i < m_pose_count; i++)
        {
            m_poses[i].phi = (float)cvmGet(poses, i, 0);
            m_poses[i].theta = (float)cvmGet(poses, i, 1);
            m_poses[i].lambda1 = (float)cvmGet(poses, i, 2);
            m_poses[i].lambda2 = (float)cvmGet(poses, i, 3);
        }
        cvReleaseMat(&poses);
        
        // now initialize pose transforms
        InitializeTransformsFromPoses();
    }
    
    node = cvGetFileNodeByName(fs, 0, "pca components number");
    if(node != 0)
    {
        
        m_pca_dim_high = cvReadInt(node);
        if(m_pca_descriptors)
        {
            delete []m_pca_descriptors;
        }
        AllocatePCADescriptors();
        for(int i = 0; i < m_pca_dim_high + 1; i++)
        {
            m_pca_descriptors[i].Allocate(m_pose_count, m_patch_size, 1);
            m_pca_descriptors[i].SetTransforms(m_poses, m_transforms);
            char buf[1024];
            sprintf(buf, "descriptor for pca component %d", i);
            m_pca_descriptors[i].ReadByName(fs, 0, buf);
        }
    }
    cvReleaseFileStorage(&fs);
    cvReleaseMemStorage(&storage);
    
    return 1;
}

void OneWayDescriptorBase::SavePCADescriptors(const char* filename)
{
    CvMemStorage* storage = cvCreateMemStorage();
    CvFileStorage* fs = cvOpenFileStorage(filename, storage, CV_STORAGE_WRITE);
    
    cvWriteInt(fs, "pca components number", m_pca_dim_high);
    cvWriteComment(fs, "The first component is the average vector, so the total number of components is <pca components number> + 1", 0);
    cvWriteInt(fs, "patch width", m_patch_size.width);
    cvWriteInt(fs, "patch height", m_patch_size.height);
    
    // pack the affine transforms into a single CvMat and write them
    CvMat* poses = cvCreateMat(m_pose_count, 4, CV_32FC1);
    for(int i = 0; i < m_pose_count; i++)
    {
        cvmSet(poses, i, 0, m_poses[i].phi);
        cvmSet(poses, i, 1, m_poses[i].theta);
        cvmSet(poses, i, 2, m_poses[i].lambda1);
        cvmSet(poses, i, 3, m_poses[i].lambda2);
    }
    cvWrite(fs, "affine poses", poses);
    cvReleaseMat(&poses);
    
    for(int i = 0; i < m_pca_dim_high + 1; i++)
    {
        char buf[1024];
        sprintf(buf, "descriptor for pca component %d", i);
        m_pca_descriptors[i].Write(fs, buf);
    }
    
    cvReleaseMemStorage(&storage);
    cvReleaseFileStorage(&fs);
}

void OneWayDescriptorBase::Allocate(int train_feature_count)
{
    m_train_feature_count = train_feature_count;
    m_descriptors = new OneWayDescriptor[m_train_feature_count];
    for(int i = 0; i < m_train_feature_count; i++)
    {
        m_descriptors[i].SetPCADimHigh(m_pca_dim_high);
        m_descriptors[i].SetPCADimLow(m_pca_dim_low);
    }
}

void OneWayDescriptorBase::InitializeDescriptors(IplImage* train_image, const vector<KeyPoint>& features, 
                                                 const char* feature_label, int desc_start_idx)
{
    for(int i = 0; i < (int)features.size(); i++)
    {
        Point center = features[i].pt;
        
        CvRect roi = cvRect(center.x - m_patch_size.width/2, center.y - m_patch_size.height/2, m_patch_size.width, m_patch_size.height);
        cvResetImageROI(train_image);
        roi = fit_rect_fixedsize(roi, train_image);
        cvSetImageROI(train_image, roi);
//        roi = cvGetImageROI(train_image);
        if(roi.width != m_patch_size.width || roi.height != m_patch_size.height)
        {
            continue;
        }
        
        InitializeDescriptor(desc_start_idx + i, train_image, feature_label);
        
//        printf("Completed feature %d\n", i);
        
    }
    cvResetImageROI(train_image);
}

void OneWayDescriptorBase::CreateDescriptorsFromImage(IplImage* src, const vector<KeyPoint>& features)
{
    m_train_feature_count = (int)features.size();
    
    m_descriptors = new OneWayDescriptor[m_train_feature_count];
    
    InitializeDescriptors(src, features);
}

void OneWayDescriptorObject::Allocate(int train_feature_count, int object_feature_count)
{
    OneWayDescriptorBase::Allocate(train_feature_count);
    
    m_object_feature_count = object_feature_count;
    m_part_id = new int[m_object_feature_count];
}


void OneWayDescriptorObject::InitializeObjectDescriptors(IplImage* train_image, const vector<KeyPoint>& features, 
                                                         const char* feature_label, int desc_start_idx, float scale)
{
    InitializeDescriptors(train_image, features, feature_label, desc_start_idx);
    
    for(int i = 0; i < (int)features.size(); i++)
    {
        Point center = features[i].pt;
        
        if(m_part_id)
        {
            // remember descriptor part id
            Point center_scaled = cvPoint(cvRound(center.x*scale), cvRound(center.y*scale));
            m_part_id[i + desc_start_idx] = MatchPointToPart(center_scaled);
        }
    }
    cvResetImageROI(train_image);
}

int OneWayDescriptorObject::IsDescriptorObject(int desc_idx) const
{
    return desc_idx < m_object_feature_count ? 1 : 0;
}

int OneWayDescriptorObject::MatchPointToPart(Point pt) const
{
    int idx = -1;
    const float max_dist = 100.f;
    for(int i = 0; i < (int)m_train_features.size(); i++)
    {
        Point2f t(m_train_features[i].pt);
        Point2f delta(pt.x - t.x, pt.y - t.y);
        if(delta.dot(delta) < max_dist)
        {
            idx = i;
            break;
        }
    }
    
    return idx;
}

int OneWayDescriptorObject::GetDescriptorPart(int desc_idx) const
{
    //    return MatchPointToPart(GetDescriptor(desc_idx)->GetCenter());
    return m_part_id[desc_idx];
}

OneWayDescriptorObject::OneWayDescriptorObject(Size patch_size, int pose_count, const char* train_path, 
                                               const char* pca_config, const char* pca_hr_config, const char* pca_desc_config, int pyr_levels) :
OneWayDescriptorBase(patch_size, pose_count, train_path, pca_config, pca_hr_config, pca_desc_config, pyr_levels)
{
    m_part_id = 0;
}

OneWayDescriptorObject::~OneWayDescriptorObject()
{
    delete m_part_id;
}

vector<KeyPoint> OneWayDescriptorObject::_GetTrainFeatures() const
{
    vector<KeyPoint> features;
    for(size_t i = 0; i < m_train_features.size(); i++)
    {
        features.push_back(m_train_features[i]);
    }
    
    return features;
}

static void eigenvector2image(CvMat* eigenvector, IplImage* img)
{
    CvRect roi = cvGetImageROI(img);
    if(img->depth == 32)
    {
        for(int y = 0; y < roi.height; y++)
        {
            for(int x = 0; x < roi.width; x++)
            {
                float val = (float)cvmGet(eigenvector, 0, roi.width*y + x);
                *((float*)(img->imageData + (roi.y + y)*img->widthStep) + roi.x + x) = val;
            }
        }
    }
    else
    {
        for(int y = 0; y < roi.height; y++)
        {
            for(int x = 0; x < roi.width; x++)
            {
                float val = (float)cvmGet(eigenvector, 0, roi.width*y + x);
                img->imageData[(roi.y + y)*img->widthStep + roi.x + x] = (unsigned char)val;
            }
        }
    }
}

static void readPCAFeatures(const char* filename, CvMat** avg, CvMat** eigenvectors)
{
    CvMemStorage* storage = cvCreateMemStorage();
    CvFileStorage* fs = cvOpenFileStorage(filename, storage, CV_STORAGE_READ);
    if(!fs)
    {
        printf("Cannot open file %s! Exiting!", filename);
        cvReleaseMemStorage(&storage);
    }
    
    CvFileNode* node = cvGetFileNodeByName(fs, 0, "avg");
    CvMat* _avg = (CvMat*)cvRead(fs, node);
    node = cvGetFileNodeByName(fs, 0, "eigenvectors");
    CvMat* _eigenvectors = (CvMat*)cvRead(fs, node);
    
    *avg = cvCloneMat(_avg);
    *eigenvectors = cvCloneMat(_eigenvectors);
    
    cvReleaseFileStorage(&fs);
    cvReleaseMemStorage(&storage);
}

static inline Point rect_center(CvRect rect)
{
	return cvPoint(rect.x + rect.width/2, rect.y + rect.height/2);
}

void homography_transform(IplImage* frontal, IplImage* result, CvMat* homography)
{
    cvWarpPerspective(frontal, result, homography);
}

AffinePose perturbate_pose(AffinePose pose, float noise)
{
    // perturbate the matrix
    float noise_mult_factor = 1 + (0.5f - float(rand())/RAND_MAX)*noise;
    float noise_add_factor = noise_mult_factor - 1;
    
    AffinePose pose_pert = pose;
    pose_pert.phi += noise_add_factor;
    pose_pert.theta += noise_mult_factor;
    pose_pert.lambda1 *= noise_mult_factor;
    pose_pert.lambda2 *= noise_mult_factor;

    return pose_pert;
}

void generate_mean_patch(IplImage* frontal, IplImage* result, AffinePose pose, int pose_count, float noise)
{
    IplImage* sum = cvCreateImage(cvSize(result->width, result->height), IPL_DEPTH_32F, 1);
    IplImage* workspace = cvCloneImage(result);
    IplImage* workspace_float = cvCloneImage(sum);
    
    cvSetZero(sum);
    for(int i = 0; i < pose_count; i++)
    {
        AffinePose pose_pert = perturbate_pose(pose, noise);
        
        AffineTransformPatch(frontal, workspace, pose_pert);
        cvConvertScale(workspace, workspace_float);
        cvAdd(sum, workspace_float, sum);
    }
    
    cvConvertScale(sum, result, 1.0f/pose_count);
            
    cvReleaseImage(&workspace);
    cvReleaseImage(&sum);
    cvReleaseImage(&workspace_float);
}

void generate_mean_patch_fast(IplImage* /*frontal*/, IplImage* /*result*/, AffinePose /*pose*/, 
                    CvMat* /*pca_hr_avg*/, CvMat* pca_hr_eigenvectors,
                    const OneWayDescriptor* /*pca_descriptors*/)
{
    for(int i = 0; i < pca_hr_eigenvectors->cols; i++)
    {
    }
}


OneWayDescriptor::OneWayDescriptor()
{
    m_pose_count = 0;
    m_samples = 0;
    m_pca_coeffs = 0;
    m_affine_poses = 0;
    m_transforms = 0;
}

OneWayDescriptor::~OneWayDescriptor()
{
    if(m_pose_count)
    {
        for(int i = 0; i < m_pose_count; i++)
        {
            cvReleaseImage(&m_samples[i]);
            cvReleaseMat(&m_pca_coeffs[i]);
        }
        delete []m_samples;
        delete []m_pca_coeffs;
        
        if(!m_transforms)
        {
            delete []m_affine_poses;
        }
    }
}

void OneWayDescriptor::Allocate(int pose_count, Size size, int nChannels)
{
    m_pose_count = pose_count;
    m_samples = new IplImage* [m_pose_count];
    m_pca_coeffs = new CvMat* [m_pose_count];
    m_patch_size = cvSize(size.width/2, size.height/2);
    
    if(!m_transforms)
    {
        m_affine_poses = new AffinePose[m_pose_count];
    }
    
    int length = m_pca_dim_low;//roi.width*roi.height;
    for(int i = 0; i < m_pose_count; i++)
    {
        m_samples[i] = cvCreateImage(cvSize(size.width/2, size.height/2), IPL_DEPTH_32F, nChannels);
        m_pca_coeffs[i] = cvCreateMat(1, length, CV_32FC1);
    }
}

void cvmSet2DPoint(CvMat* matrix, int row, int col, CvPoint2D32f point)
{
    cvmSet(matrix, row, col, point.x);
    cvmSet(matrix, row, col + 1, point.y);
}

void cvmSet3DPoint(CvMat* matrix, int row, int col, CvPoint3D32f point)
{
    cvmSet(matrix, row, col, point.x);
    cvmSet(matrix, row, col + 1, point.y);
    cvmSet(matrix, row, col + 2, point.z);
}

AffinePose GenRandomAffinePose()
{
    const float scale_min = 0.8f;
    const float scale_max = 1.2f;
    AffinePose pose;
    pose.theta = float(rand())/RAND_MAX*120 - 60;
    pose.phi = float(rand())/RAND_MAX*360;
    pose.lambda1 = scale_min + float(rand())/RAND_MAX*(scale_max - scale_min);
    pose.lambda2 = scale_min + float(rand())/RAND_MAX*(scale_max - scale_min);
    
    return pose;
}

void GenerateAffineTransformFromPose(Size size, AffinePose pose, CvMat* transform)
{
    CvMat* temp = cvCreateMat(3, 3, CV_32FC1);
    CvMat* final = cvCreateMat(3, 3, CV_32FC1);
    cvmSet(temp, 2, 0, 0.0f);
    cvmSet(temp, 2, 1, 0.0f);
    cvmSet(temp, 2, 2, 1.0f);

    CvMat rotation;
    cvGetSubRect(temp, &rotation, cvRect(0, 0, 3, 2));
    
    cv2DRotationMatrix(cvPoint2D32f(size.width/2, size.height/2), pose.phi, 1.0, &rotation);
    cvCopy(temp, final);

    cvmSet(temp, 0, 0, pose.lambda1);
    cvmSet(temp, 0, 1, 0.0f);
    cvmSet(temp, 1, 0, 0.0f);
    cvmSet(temp, 1, 1, pose.lambda2);
    cvmSet(temp, 0, 2, size.width/2*(1 - pose.lambda1));
    cvmSet(temp, 1, 2, size.height/2*(1 - pose.lambda2));
    cvMatMul(temp, final, final);
    
    cv2DRotationMatrix(cvPoint2D32f(size.width/2, size.height/2), pose.theta - pose.phi, 1.0, &rotation);
    cvMatMul(temp, final, final);

    cvGetSubRect(final, &rotation, cvRect(0, 0, 3, 2));
    cvCopy(&rotation, transform);   
    
    cvReleaseMat(&temp);
    cvReleaseMat(&final);
}

void AffineTransformPatch(IplImage* src, IplImage* dst, AffinePose pose)
{
    CvRect src_large_roi = cvGetImageROI(src);
    
    IplImage* temp = cvCreateImage(cvSize(src_large_roi.width, src_large_roi.height), IPL_DEPTH_32F, src->nChannels);
    cvSetZero(temp);
    IplImage* temp2 = cvCloneImage(temp);
    CvMat* rotation_phi = cvCreateMat(2, 3, CV_32FC1);
    
    Size new_size = cvSize(cvRound(temp->width*pose.lambda1), cvRound(temp->height*pose.lambda2));
    IplImage* temp3 = cvCreateImage(new_size, IPL_DEPTH_32F, src->nChannels);

    cvConvertScale(src, temp);
    cvResetImageROI(temp);
    
    
    cv2DRotationMatrix(cvPoint2D32f(temp->width/2, temp->height/2), pose.phi, 1.0, rotation_phi);
    cvWarpAffine(temp, temp2, rotation_phi);
    
    cvSetZero(temp);
    
    cvResize(temp2, temp3);
    
    cv2DRotationMatrix(cvPoint2D32f(temp3->width/2, temp3->height/2), pose.theta - pose.phi, 1.0, rotation_phi);
    cvWarpAffine(temp3, temp, rotation_phi);
    
    cvSetImageROI(temp, cvRect(temp->width/2 - src_large_roi.width/4, temp->height/2 - src_large_roi.height/4, 
                               src_large_roi.width/2, src_large_roi.height/2));
    cvConvertScale(temp, dst);   
    cvReleaseMat(&rotation_phi);
    
    cvReleaseImage(&temp3);
    cvReleaseImage(&temp2);
    cvReleaseImage(&temp);
}

void OneWayDescriptor::GenerateSamples(int pose_count, IplImage* frontal, int norm)
{
/*    if(m_transforms)
    {
        GenerateSamplesWithTransforms(pose_count, frontal);
        return;
    }
*/    
    CvRect roi = cvGetImageROI(frontal);
    IplImage* patch_8u = cvCreateImage(cvSize(roi.width/2, roi.height/2), frontal->depth, frontal->nChannels);
    for(int i = 0; i < pose_count; i++)
    {
        if(!m_transforms)
        {
            m_affine_poses[i] = GenRandomAffinePose();
        }
        //AffineTransformPatch(frontal, patch_8u, m_affine_poses[i]);
        generate_mean_patch(frontal, patch_8u, m_affine_poses[i], num_mean_components, noise_intensity);

        float scale = 1.0f;
        if(norm)
        {
            float sum = (float)cvSum(patch_8u).val[0];
            scale = 1.f/sum;
        }
        cvConvertScale(patch_8u, m_samples[i], scale);
        
#if 0
        double maxval;
        cvMinMaxLoc(m_samples[i], 0, &maxval);
        IplImage* test = cvCreateImage(cvSize(roi.width/2, roi.height/2), IPL_DEPTH_8U, 1);
        cvConvertScale(m_samples[i], test, 255.0/maxval);
        cvNamedWindow("1", 1);
        cvShowImage("1", test);
        cvWaitKey(0);
#endif
    }
    cvReleaseImage(&patch_8u);
}

void OneWayDescriptor::GenerateSamplesFast(IplImage* frontal, CvMat* pca_hr_avg, 
                                             CvMat* pca_hr_eigenvectors, OneWayDescriptor* pca_descriptors)
{
    CvMat* pca_coeffs = cvCreateMat(1, pca_hr_eigenvectors->cols, CV_32FC1);
    double maxval;
    cvMinMaxLoc(frontal, 0, &maxval);
    CvMat* frontal_data = ConvertImageToMatrix(frontal);
    
    float sum = (float)cvSum(frontal_data).val[0];
    cvConvertScale(frontal_data, frontal_data, 1.0f/sum);
    cvProjectPCA(frontal_data, pca_hr_avg, pca_hr_eigenvectors, pca_coeffs);
    for(int i = 0; i < m_pose_count; i++)
    {
        cvSetZero(m_samples[i]);
        for(int j = 0; j < m_pca_dim_high; j++)
        {
            float coeff = (float)cvmGet(pca_coeffs, 0, j);
            IplImage* patch = pca_descriptors[j + 1].GetPatch(i);
            cvAddWeighted(m_samples[i], 1.0, patch, coeff, 0, m_samples[i]);
            
#if 0
            printf("coeff%d = %f\n", j, coeff);
            IplImage* test = cvCreateImage(cvSize(12, 12), IPL_DEPTH_8U, 1);
            double maxval;
            cvMinMaxLoc(patch, 0, &maxval);
            cvConvertScale(patch, test, 255.0/maxval);
            cvNamedWindow("1", 1);
            cvShowImage("1", test);
            cvWaitKey(0);
#endif
        }
        
        cvAdd(pca_descriptors[0].GetPatch(i), m_samples[i], m_samples[i]);
        float sum = (float)cvSum(m_samples[i]).val[0];
        cvConvertScale(m_samples[i], m_samples[i], 1.0/sum);
        
#if 0
        IplImage* test = cvCreateImage(cvSize(12, 12), IPL_DEPTH_8U, 1);
/*        IplImage* temp1 = cvCreateImage(cvSize(12, 12), IPL_DEPTH_32F, 1);
        eigenvector2image(pca_hr_avg, temp1);
        IplImage* test = cvCreateImage(cvSize(12, 12), IPL_DEPTH_8U, 1);
        cvAdd(m_samples[i], temp1, temp1);
        cvMinMaxLoc(temp1, 0, &maxval);
        cvConvertScale(temp1, test, 255.0/maxval);*/
        cvMinMaxLoc(m_samples[i], 0, &maxval);
        cvConvertScale(m_samples[i], test, 255.0/maxval);

        cvNamedWindow("1", 1);
        cvShowImage("1", frontal);
        cvNamedWindow("2", 1);
        cvShowImage("2", test);
        cvWaitKey(0);
#endif
    }
    
    cvReleaseMat(&pca_coeffs);
    cvReleaseMat(&frontal_data);
}

void OneWayDescriptor::SetTransforms(AffinePose* poses, CvMat** transforms)
{
    if(m_affine_poses)
    {
        delete []m_affine_poses;
    }
    
    m_affine_poses = poses;
    m_transforms = transforms;
}

void OneWayDescriptor::Initialize(int pose_count, IplImage* frontal, const char* feature_name, int norm)
{
    m_feature_name = String(feature_name);
    CvRect roi = cvGetImageROI(frontal);
    m_center = rect_center(roi);
    
    Allocate(pose_count, cvSize(roi.width, roi.height), frontal->nChannels);
    
    GenerateSamples(pose_count, frontal, norm);
}

void OneWayDescriptor::InitializeFast(int pose_count, IplImage* frontal, const char* feature_name, 
                    CvMat* pca_hr_avg, CvMat* pca_hr_eigenvectors, OneWayDescriptor* pca_descriptors)
{
    if(pca_hr_avg == 0)
    {
        Initialize(pose_count, frontal, feature_name, 1);
        return;
    }
    m_feature_name = String(feature_name);
    CvRect roi = cvGetImageROI(frontal);
    m_center = rect_center(roi);
    
    Allocate(pose_count, cvSize(roi.width, roi.height), frontal->nChannels);
    
    GenerateSamplesFast(frontal, pca_hr_avg, pca_hr_eigenvectors, pca_descriptors);
}

void OneWayDescriptor::InitializePCACoeffs(CvMat* avg, CvMat* eigenvectors)
{
    for(int i = 0; i < m_pose_count; i++)
    {
        ProjectPCASample(m_samples[i], avg, eigenvectors, m_pca_coeffs[i]);
    }
}

void OneWayDescriptor::ProjectPCASample(IplImage* patch, CvMat* avg, CvMat* eigenvectors, CvMat* pca_coeffs) const
{
    CvMat* patch_mat = ConvertImageToMatrix(patch);
//    CvMat eigenvectorsr;
//    cvGetSubRect(eigenvectors, &eigenvectorsr, cvRect(0, 0, eigenvectors->cols, pca_coeffs->cols));
    CvMat* temp = cvCreateMat(1, eigenvectors->cols, CV_32FC1);
    cvProjectPCA(patch_mat, avg, eigenvectors, temp);
    CvMat temp1;
    cvGetSubRect(temp, &temp1, cvRect(0, 0, pca_coeffs->cols, 1));
    cvCopy(&temp1, pca_coeffs);
    
    cvReleaseMat(&temp);
    cvReleaseMat(&patch_mat);
}

void OneWayDescriptor::EstimatePosePCA(IplImage* patch, int& pose_idx, float& distance, CvMat* avg, CvMat* eigenvectors) const
{
    if(avg == 0)
    {
        // do not use pca
        EstimatePose(patch, pose_idx, distance);
        return;
    }
    
    CvRect roi = cvGetImageROI(patch);
    CvMat* pca_coeffs = cvCreateMat(1, m_pca_dim_low, CV_32FC1);
    
    IplImage* patch_32f = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_32F, 1);
    float sum = (float)cvSum(patch).val[0];
    cvConvertScale(patch, patch_32f, 1.0f/sum);
 
    ProjectPCASample(patch_32f, avg, eigenvectors, pca_coeffs);

    distance = 1e10;
    pose_idx = -1;
    
    for(int i = 0; i < m_pose_count; i++)
    {
        float dist = (float)cvNorm(m_pca_coeffs[i], pca_coeffs);
        if(dist < distance)
        {
            distance = dist;
            pose_idx = i;
        }
    }
    
    cvReleaseMat(&pca_coeffs);
    cvReleaseImage(&patch_32f);
}

void OneWayDescriptor::EstimatePose(IplImage* patch, int& pose_idx, float& distance) const
{
    distance = 1e10;
    pose_idx = -1;
    
    CvRect roi = cvGetImageROI(patch);
    IplImage* patch_32f = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_32F, patch->nChannels);
    float sum = (float)cvSum(patch).val[0];
    cvConvertScale(patch, patch_32f, 1/sum);
    
    for(int i = 0; i < m_pose_count; i++)
    {
        if(m_samples[i]->width != patch_32f->width || m_samples[i]->height != patch_32f->height)
        {
            continue;
        }
        float dist = (float)cvNorm(m_samples[i], patch_32f);
        if(dist < distance)
        {
            distance = dist;
            pose_idx = i;
        }
        
#if 0
        IplImage* img1 = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_8U, 1);
        IplImage* img2 = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_8U, 1);
        double maxval;
        cvMinMaxLoc(m_samples[i], 0, &maxval);
        cvConvertScale(m_samples[i], img1, 255.0/maxval);
        cvMinMaxLoc(patch_32f, 0, &maxval);
        cvConvertScale(patch_32f, img2, 255.0/maxval);
        
        cvNamedWindow("1", 1);
        cvShowImage("1", img1);
        cvNamedWindow("2", 1);
        cvShowImage("2", img2);
        printf("Distance = %f\n", dist);
        cvWaitKey(0);
#endif
    }
    
    cvReleaseImage(&patch_32f);
}

void OneWayDescriptor::Save(const char* path)
{
    for(int i = 0; i < m_pose_count; i++)
    {
        char buf[1024];
        sprintf(buf, "%s/patch_%04d.jpg", path, i);
        IplImage* patch = cvCreateImage(cvSize(m_samples[i]->width, m_samples[i]->height), IPL_DEPTH_8U, m_samples[i]->nChannels);
        
        double maxval;
        cvMinMaxLoc(m_samples[i], 0, &maxval);
        cvConvertScale(m_samples[i], patch, 255/maxval);
        
        cvSaveImage(buf, patch);
        
        cvReleaseImage(&patch);
    }
}

void OneWayDescriptor::Write(CvFileStorage* fs, const char* name)
{
    CvMat* mat = cvCreateMat(m_pose_count, m_samples[0]->width*m_samples[0]->height, CV_32FC1);
    
    // prepare data to write as a single matrix
    for(int i = 0; i < m_pose_count; i++)
    {
        for(int y = 0; y < m_samples[i]->height; y++)
        {
            for(int x = 0; x < m_samples[i]->width; x++)
            {
                float val = *((float*)(m_samples[i]->imageData + m_samples[i]->widthStep*y) + x);
                cvmSet(mat, i, y*m_samples[i]->width + x, val);
            }
        }
    }
    
    cvWrite(fs, name, mat);
    
    cvReleaseMat(&mat);
}

int OneWayDescriptor::ReadByName(CvFileStorage* fs, CvFileNode* parent, const char* name)
{
    CvMat* mat = (CvMat*)cvReadByName(fs, parent, name);
    if(!mat)
    {
        return 0;
    }
    

    for(int i = 0; i < m_pose_count; i++)
    {
        for(int y = 0; y < m_samples[i]->height; y++)
        {
            for(int x = 0; x < m_samples[i]->width; x++)
            {
                float val = (float)cvmGet(mat, i, y*m_samples[i]->width + x);
                *((float*)(m_samples[i]->imageData + y*m_samples[i]->widthStep) + x) = val;
            }
        }
    }
            
    cvReleaseMat(&mat);
    return 1;
}

IplImage* OneWayDescriptor::GetPatch(int index)
{
    return m_samples[index];
}

AffinePose OneWayDescriptor::GetPose(int index) const
{
    return m_affine_poses[index];
}

void findOneWayDescriptor(int desc_count, const OneWayDescriptor* descriptors,
                          IplImage* patch, int& desc_idx, int& pose_idx, float& distance, 
                          CvMat* avg, CvMat* eigenvectors)
{
    desc_idx = -1;
    pose_idx = -1;
    distance = 1e10;
    for(int i = 0; i < desc_count; i++)
    {
        int _pose_idx = -1;
        float _distance = 0;

#if 0
        descriptors[i].EstimatePose(patch, _pose_idx, _distance);
#else
        descriptors[i].EstimatePosePCA(patch, _pose_idx, _distance, avg, eigenvectors);
#endif
        
        if(_distance < distance)
        {
            desc_idx = i;
            pose_idx = _pose_idx;
            distance = _distance;
        }
    }
}

void findOneWayDescriptor(int desc_count, const OneWayDescriptor* descriptors, IplImage* patch, 
                          float scale_min, float scale_max, float scale_step,
                          int& desc_idx, int& pose_idx, float& distance, float& scale, 
                          CvMat* avg, CvMat* eigenvectors)
{
    Size patch_size = descriptors[0].GetPatchSize();
    IplImage* input_patch = cvCreateImage(patch_size, IPL_DEPTH_8U, 1);
    CvRect roi = cvGetImageROI(patch);
    int _desc_idx, _pose_idx;
    float _distance;
    distance = 1e10;
    for(float cur_scale = scale_min; cur_scale < scale_max; cur_scale *= scale_step)
    {
        CvRect roi_scaled = resize_rect(roi, cur_scale);
        cvSetImageROI(patch, roi_scaled);
        cvResize(patch, input_patch);
        
#if 0
        if(roi.x > 244 && roi.y < 200)
        {
            cvNamedWindow("1", 1);
            cvShowImage("1", input_patch);
            cvWaitKey(0);
        }
#endif
        
        findOneWayDescriptor(desc_count, descriptors, input_patch, _desc_idx, _pose_idx, _distance, avg, eigenvectors);
        if(_distance < distance)
        {
            distance = _distance;
            desc_idx = _desc_idx;
            _pose_idx = pose_idx;
            scale = cur_scale;
        }
    }
    cvSetImageROI(patch, roi);
    
    cvReleaseImage(&input_patch);
}

const char* OneWayDescriptor::GetFeatureName() const
{
    return m_feature_name.c_str();
}

Point OneWayDescriptor::GetCenter() const
{
    return m_center;
}

}

