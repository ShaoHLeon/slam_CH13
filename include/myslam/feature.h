// Feature(struct): 图像自身的2D位置

#pragma once

#include "myslam/common_include.h"


namespace myslam{

// 前向声明
struct Frame;
struct MapPoint;


struct Feature{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::shared_ptr<Feature> Ptr;

    std::weak_ptr<Frame> frame_; // 持有该feature的frame的弱指针
    cv::KeyPoint position_; // 特征点在图像中的位置，2D提取位置
    std::weak_ptr<MapPoint> map_point_; // 特征点对应的地图点
    
    bool is_outlier_ = false; // 是否是异常点
    bool is_on_left_image_ = true; // 标识是否在左图

public:
    Feature(){}

    Feature(std::shared_ptr<Frame> frame, const cv::KeyPoint& kp)
    : frame_(frame), position_(kp){}
};

}