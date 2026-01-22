/*
前端处理逻辑：
1. 前端有初始化、正常追踪、追踪丢失三种状态；
2. 初始化状态中，根据左右目之间的光流匹配（得到特征点对），通过三角化计算得到3D路标点，构成初始地图；
3. 追踪阶段，前端计算上一帧的特征点到当前帧的光流，根据光流结果计算图像位姿。计算只使用左目相机；
4. 如果追踪到的点较少，就判定当前帧为关键帧。对于关键帧：
    - 提取新的特征点；
    - 找到这些特征点在右图中对应点，用三角化建立新的路标点；
    - 将新的关键帧和路标点加入地图，并触发一次后端优化。
5. 如果追踪丢失，重置前端系统，重新初始化。
*/


#include "myslam/frontend.h"


namespace myslam{

bool Frontend::AddFrame(myslam::Frame::Ptr frame){
    current_frame_ = frame;
    switch(status_){ // 状态机处理机制
        case FrontendStatus::INITING:
            StereoInit();
            break;
        case FrontendStatus::TRACKING_GOOD:
        case FrontendStatus::TRACKING_BAD:
            Track();    
            break;
        case FrontendStatus::LOST:
            Reset();
            break;
    }

    last_frame_ = current_frame_; // 存疑？？？？？
    return true;
}

bool Frontend::Track(){
    if(last_frame_) {
        current_frame_->SetPose(relative_motion_ * last_frame_->Pose()); // 计算当前帧的位姿
    }

    int num_track_last = TrackLastFrame(); // 计算当前帧与上一帧之间的特征点匹配
    tracking_inliers_ = EstimateCurrentPose(); // 

    if (tracking_inliers_ > num_features_tracking_) {
        // tracking good
        status_ = FrontendStatus::TRACKING_GOOD;
    } else if (tracking_inliers_ > num_features_tracking_bad_) {
        // tracking bad
        status_ = FrontendStatus::TRACKING_BAD;
    } else {
        // lost
        status_ = FrontendStatus::LOST;
    }

    InsertKeyframe();
    relative_motion_ = current_frame_->Pose() * last_frame_->Pose().inverse();

    if (viewer_) viewer_->AddCurrentFrame(current_frame_);
    return true;

}


}