//
//  UIViewController+MainViewController.m
//  EdgeInitialization
//
//  Created by Hartisan on 15/11/18.
//  Copyright © 2015年 Hartisan. All rights reserved.
//

#import "MainViewController.h"

@implementation MainViewController

@synthesize _glView, _glVC, _videoSource, _initializer, _btnInit, _initOn, _cvMatFromCVImageBufferRef, _detectOn, _frameCount;


- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    self._initOn = false;
    self._frameCount = 0;
    
    // 摄像头
    self._videoSource = [[VideoSource alloc] init];
    self._videoSource._delegate = self;
    
    // 初始化分析器
    self._initializer = [[Initializer alloc] init];
    
    // 初始化GL层
    self._glVC = [[GLViewController alloc] initWithGLKView:self._glView];
    [self._glVC setModelNameWithImg:self._initializer._targetName];
    
    // 开启摄像头
    [self._videoSource startWithDevicePosition:AVCaptureDevicePositionBack];
}


// 当视频流被送来后，更新纹理并图像处理
- (void)frameReady:(CVImageBufferRef)pixelBuffer {
    
    self._frameCount++;
    
    // 更新背景
    dispatch_sync(dispatch_get_main_queue(), ^{
        
        [self._glVC updateBackgroundTexture:pixelBuffer];
    });
    
    // 把图像保存下来，供cv处理
    if (self._initOn && self._frameCount % 1 == 0) {
        
        self._cvMatFromCVImageBufferRef = [self cvMatFromCVImageBufferRef:pixelBuffer];
        
        __block cv::Mat cvMatrix;
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            
            // 异步计算矩阵
            cvMatrix = [self._initializer initWithImage:self._cvMatFromCVImageBufferRef];
            
            if (cvMatrix.cols == 4) {
                
                // 设置给GL
                [self setGLModelViewMatrixWithCVMatrix:cvMatrix];
                self._glVC._drawModels = true;
                
            } else {
                
                self._glVC._drawModels = false;
            }
        });
    }
}


// 把CVImageBufferRef转为cvMat
- (cv::Mat)cvMatFromCVImageBufferRef:(CVImageBufferRef)pixelBuffer {
    
    // 彩图
    uint8_t* baseAddress = (uint8_t*)CVPixelBufferGetBaseAddress(pixelBuffer);
    size_t width = CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    size_t stride = CVPixelBufferGetBytesPerRow(pixelBuffer);
    cv::Mat cvMat((int)height, (int)width, CV_8UC4, baseAddress, stride);
    
    return cvMat.clone();
}


// 把CV环境下的大矩阵转为GL环境下的ModelView矩阵
- (void)setGLModelViewMatrixWithCVMatrix:(cv::Mat)cvMatrix {
    
    float matrix[16];
    double conv[] = {1.0,  0.0,  0.0, 0.0,
        0.0, -1.0,  0.0, 0.0,
        0.0,  0.0, -1.0, 0.0,
        0.0,  0.0,  0.0, 1.0};
    cv::Mat convert = cv::Mat(4, 4, CV_64FC1, conv);
    cvMatrix = convert * cvMatrix;
    
    matrix[0] = cvMatrix.at<double>(0, 0);
    matrix[1] = cvMatrix.at<double>(1, 0);
    matrix[2] = cvMatrix.at<double>(2, 0);
    matrix[3] = 0.0f;
    
    matrix[4] = cvMatrix.at<double>(0, 1);
    matrix[5] = cvMatrix.at<double>(1, 1);
    matrix[6] = cvMatrix.at<double>(2, 1);
    matrix[7] = 0.0f;
    
    matrix[8] = cvMatrix.at<double>(0, 2);
    matrix[9] = cvMatrix.at<double>(1, 2);
    matrix[10] = cvMatrix.at<double>(2, 2);
    matrix[11] = 0.0f;
    
    matrix[12] = cvMatrix.at<double>(0, 3);
    matrix[13] = cvMatrix.at<double>(1, 3);
    matrix[14] = cvMatrix.at<double>(2, 3);
    matrix[15] = 1.0f;
    
    self._glVC._modelViewMatrix = GLKMatrix4MakeWithArray(matrix);
}


// 按钮事件
- (IBAction)initBtnPressed:(id)sender {
    
    if (self._initOn) {
        
        self._initOn = false;
        
    } else {
        
        self._initOn = true;
    }
}

@end
