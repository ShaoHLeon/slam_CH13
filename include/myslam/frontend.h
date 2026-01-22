#pragma once


#include "myslam/common_include.h"
#include "myslam/frame.h"
#include "myslam/map.h"
#include "myslam/backend.h"



namespace myslam{


// 前端的状态
enum class FrontendStatus{
    INITING, // 初始化中
    TRACKING_GOOD, // 跟踪良好
    TRACKING_BAD,
    LOST // 失踪
};

/**
 * 前端
 * 估计当前帧的位姿，在满足关键帧条件时向地图加入关键帧并触发优化
 */
class Frontend{

//data
private: 
    FrontendStatus status_ = FrontendStatus::INITING; // 当前状态

    Frame::Ptr current_frame_ = nullptr; // 当前帧
    Frame::Ptr last_frame_ = nullptr; // 上一帧
    

    Map::Ptr map_ = nullptr; // 地图
    Backend::Ptr backend_ = nullptr; // 后端

    SE3 relative_motion_;  // 当前帧与上一帧的相对运动，用于估计当前帧pose初值
    int tracking_inliers_ = 0;  // inliers, used for testing new keyframes

    // params
    int num_features_ = 200;
    int num_features_init_ = 100;
    int num_features_tracking_ = 50;
    int num_features_tracking_bad_ = 20;
    int num_features_needed_for_keyframe_ = 80;


public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::shared_ptr<Frontend> Ptr;

    Frontend();

    // 外部接口，添加一个帧并计算其定位结果
    bool AddFrame(Frame::Ptr frame);

    // Set
    void SetMap(Map::Ptr map) {map_ = map;}
    void SetBackend(Backend::Ptr backend) {backend_ = backend;}
    // void SetViewer()
    FrontendStatus GetStatus() const { return status_; }

private:
    /**
     * Track in normal mode
     * @return true if tracking is good
     */
    bool Track();
};



}