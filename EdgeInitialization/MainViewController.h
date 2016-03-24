//
//  UIViewController+MainViewController.h
//  EdgeInitialization
//
//  Created by Hartisan on 15/11/18.
//  Copyright © 2015年 Hartisan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLViewController.h"
#import "VideoSource.h"
#import "Initializer.h"

@interface MainViewController : UIViewController <VideoSourceDelegate> {
    
    // 摄像机
    VideoSource* _videoSource;
    
    IBOutlet GLKView* _glView;
    GLViewController* _glVC;
    Initializer* _initializer;
    IBOutlet UIButton* _btnInit;
    cv::Mat _cvMatFromCVImageBufferRef;
    
    // 状态
    bool _initOn;
    
    int _frameCount;
}


@property (nonatomic, strong) VideoSource* _videoSource;
@property (nonatomic, strong) IBOutlet GLKView* _glView;
@property (nonatomic, strong) GLViewController* _glVC;
@property (nonatomic, strong) Initializer* _initializer;
@property (nonatomic, strong) IBOutlet UIButton* _btnInit;
@property cv::Mat _cvMatFromCVImageBufferRef;
@property bool _initOn;
@property bool _detectOn;
@property int _frameCount;


- (cv::Mat)cvMatFromCVImageBufferRef:(CVImageBufferRef)pixelBuffer;
- (void)setGLModelViewMatrixWithCVMatrix:(cv::Mat)cvMatrix;
- (IBAction)initBtnPressed:(id)sender;

@end
