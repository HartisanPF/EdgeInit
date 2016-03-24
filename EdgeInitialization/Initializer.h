//
//  NSObject+Initializer.h
//  EdgeInitialization
//
//  Created by Hartisan on 15/11/23.
//  Copyright © 2015年 Hartisan. All rights reserved.
//

#ifdef __cplusplus
#import <opencv2/opencv.hpp>
#endif

#import <Foundation/Foundation.h>
#import <opencv2/imgcodecs/ios.h>
#import <opencv2/imgproc.hpp>
#import <opencv2/features2d.hpp>
#import <opencv2/highgui.hpp>
#import <opencv2/core/core.hpp>
#import <opencv2/imgproc.hpp>
#import <opencv2/line_descriptor/descriptor.hpp>
#import <CoreMotion/CoreMotion.h>

#define IPAD_CAMERA_PARAM_FX 536.84710693359375
#define IPAD_CAMERA_PARAM_FY 536.7637939453125
#define IPAD_CAMERA_PARAM_U 316.23187255859375
#define IPAD_CAMERA_PARAM_V 223.457733154296875
#define VOTE_BOX_INTERVAL 2.0
#define VOTE_BOX_RATE 0.9
#define LINE_VERTICAL_THREASH 4.0
#define LINE_PARALLEL_THRESH 5.0
#define LINE_DISTANCE_THRESH 15.0
#define LINE_ALIGN_RATE 0.2
#define LINE_OVERLAP_PARAL 3.0
#define LINE_OVERLAP_DIST 2.0
#define PRE_ASSUME_X -0.059
#define PRE_ASSUME_Y 0.018
#define PRE_ASSUME_Z 3.3

@interface Initializer : NSObject {
    
    NSString* _targetName;
    cv::Mat _Ri;
    cv::Mat _cameraParamMatrix;
    cv::Mat _cameraParamMatrixInvert;
    std::vector<cv::Mat> _modelLinesVec;
    int _modelLineVertsNum;
    double* _modelLineVerts;
    std::vector<std::vector<cv::line_descriptor::KeyLine>> _classifiedImgLines;
    std::vector<std::vector<cv::Point2i>> _candidateCorrespondences;
    CMMotionManager* _motionManager;
    double _xTheta;
    double _yTheta;
    double _zTheta;
    cv::Mat _approxTranslation;
    bool _translationSolved;
}


@property (nonatomic, strong) NSString* _targetName;
@property cv::Mat _Ri;
@property cv::Mat _cameraParamMatrix;
@property cv::Mat _cameraParamMatrixInvert;
@property std::vector<cv::Mat> _modelLinesVec;
@property int _modelLineVertsNum;
@property double* _modelLineVerts;
@property std::vector<std::vector<cv::line_descriptor::KeyLine>> _classifiedImgLines;
@property std::vector<std::vector<cv::Point2i>> _candidateCorrespondences;
@property (nonatomic, strong) CMMotionManager* _motionManager;
@property double _xTheta;
@property double _yTheta;
@property double _zTheta;
@property cv::Mat _approxTranslation;
@property bool _translationSolved;


- (cv::Mat)initWithImage:(cv::Mat)cvImage;
- (void)setInclinationAngle;
- (cv::Mat)getCameraParamMatrix;
- (std::vector<cv::Mat>)getModelLinesWithTargetName:(NSString*)targetName;
- (cv::Mat)getMaskfromHSVMat:(cv::Mat)hsvMat ofTarget:(NSString*)targetName;
- (bool)isCorrespondingToVerticalLines:(cv::line_descriptor::KeyLine)imgLine;
- (cv::Mat)convertImageCoord2CameraCoordAtU:(double)u andV:(double)v;
- (std::vector<std::vector<cv::line_descriptor::KeyLine>>)classifyImgLinesBySlope:(std::vector<cv::line_descriptor::KeyLine>)imgLines;
- (std::vector<double>)getCandidateAzimuthsWithImgLines:(std::vector<cv::line_descriptor::KeyLine>)imgLines;
- (cv::Mat)getInclinationMatrix;
- (cv::Mat)getRotationMatrixWithAzimuth:(double)azimuth andInclinationMatrix:(cv::Mat)inclination;
- (std::vector<std::vector<int>>)findAlignsWithRotation:(cv::Mat)rotationMatrix andImgLines:(std::vector<cv::line_descriptor::KeyLine>)imgLines;
- (cv::Mat)getProjectionOf3DPointX:(double)x Y:(double)y Z:(double)z withR:(cv::Mat)rotat andT:(cv::Mat)trans;
- (float)distanceFromPoint:(cv::Point2f)ptC ToLineDeterminedByPoint:(cv::Point2f)ptA andPoint:(cv::Point2f)ptB;
- (bool)isProjectedLineDetermByPoint:(cv::Point2f)projPtS andPoint:(cv::Point2f)projPtE alignedWithImgLineDetermByPoint:(cv::Point2f)imgPtS andPoint:(cv::Point2f)imgPtE;
- (cv::Mat)calcTransInAligns:(std::vector<std::vector<int>>)aligns withRot:(cv::Mat)rotation andImgLines:(std::vector<cv::line_descriptor::KeyLine>)imgLines;
- (cv::Mat)getTranslationWithThreePairsOfModelLines:(std::vector<int>)modelLinesIndex andImgLines:(std::vector<cv::line_descriptor::KeyLine>)imgLines andRotationMatrix:(cv::Mat)rotation;
- (bool)isModelVecAtIndex:(int)indexA ParallelWithVecAtIndex:(int)indexB;
- (cv::Mat)calcApproxTranslationWithColorMask:(cv::Mat)mask;
- (cv::Point2f)getMassCenterOfContour:(std::vector<cv::Point>)contour;
- (cv::Mat)combineRotation:(cv::Mat)rot andTranslation:(cv::Mat)trans;
- (int)calcPoseScoreWithRot:(cv::Mat)rot andTrans:(cv::Mat)trans andAligns:(std::vector<std::vector<int>>)aligns andImgLines:(std::vector<cv::line_descriptor::KeyLine>)imgLines;
- (bool)isProjectedLineDetermByPoint:(cv::Point2f)projPtS andPoint:(cv::Point2f)projPtE overlapWithImgLineDetermByPoint:(cv::Point2f)imgPtS andPoint:(cv::Point2f)imgPtE;

@end
