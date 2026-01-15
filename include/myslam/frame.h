// Frame(struct): 帧id、相机位姿pose_、左右图像及其特征点
// pose_会被前后端同时设置、访问 -> 定义位姿的 Set和Get函数，并使用互斥锁保护
// Frame可以由静态函数构建，在静态函数中可以自动分配id，确保每个Frame都有唯一的ID，不需要外部调用者关心ID的分配逻辑。


#pragma once // 防止头文件被重复包含

#include "myslam/common_include.h"


namespace myslam{

// 前向声明
struct Feature;
struct MapPoint;



struct Frame
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW // Eigen库的矩阵运算需要内存对齐，否则会报错
    typedef std::shared_ptr<Frame> Ptr; // 类型定义--Frame智能指针类型Ptr，该类型指针使用引用计数管理内存

    unsigned long id_ = 0; // 当前帧的id
    unsigned long keyframe_id_ = 0; // 关键帧的id
    bool is_keyframe_ = false; // 是否为关键帧
    double time_stamp_; // 时间戳
    SE3 pose_; // 相机位姿,Tcw形式,从世界坐标系到相机坐标系的变换矩阵, [R t; 0 1]
    std::mutex pose_mutex_; // 位姿互斥锁，互斥量
    cv::Mat left_img_, right_img_;   // stereo images
    
    // extract features in left image
    std::vector<std::shared_ptr<Feature>> left_features_; // 左图特征点
    // corresponding features in right image, set to nullptr if no corresponding
    std::vector<std::shared_ptr<Feature>> features_right_; // 右图特征点

public: // 数据成员
    Frame() {}; // 默认构造函数
    Frame(long id, double time_stamp, const SE3& pose, const cv::Mat& left_img, const cv::Mat& right_img);

    // set and get pose, thread safe
    SE3 Pose(){
        std::unique_lock<std::mutex> lck(pose_mutex_); // 使用互斥锁保护共享数据pose_的访问, 确保在任何时刻只有一个线程可以访问pose_

        // 在这个作用域内，所有对共享资源的访问都被保护

        return pose_;
    }
    void SetPose(const SE3 &pose) {
        std::unique_lock<std::mutex> lck(pose_mutex_);
        pose_ = pose;
    }

    // 设置关键帧并分配并键帧id
    void SetKeyFrame();

    /// 工厂构建模式，分配id 
    static std::shared_ptr<Frame> CreateFrame();
};



}