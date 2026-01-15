// 记录路标点的3D位置
// observation_ 记录该路标点被哪些特征点Feature观测到，如果有Feature被判断为outlier，会被修改

#pragma once

#include "myslam/common_include.h"

namespace myslam{

struct Feature;

struct MapPoint{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::shared_ptr<MapPoint> Ptr;

    unsigned long id_; // 路标点 ID
    bool is_outlier_ = false; // 是否是异常点
    Vec3 pose_ = Vec3::Zero(); // 路标点在 世界 坐标系下的坐标
    std::mutex data_mutex_; // 数据同步互斥锁
    int observed_times_ = 0; // 被特征匹配算法观测到的次数，通过统计观测次数，可以评估特征点的稳定性和可靠性
    std::list<std::weak_ptr<Feature>> observations_; // 路标点被观测到的特征

public:
    MapPoint() = default;
    MapPoint(long id, Vec3 position);

    Vec3 Pose(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return pose_;
    }

    void SetPose(const Vec3& pose){
        std::unique_lock<std::mutex> lck(data_mutex_);
        pose_ = pose;
    }

    void AddObservation(std::shared_ptr<Feature> feature){
        std::unique_lock<std::mutex> lck(data_mutex_);
        observations_.push_back(feature);
        observed_times_++;
    }

    // 当地图点不再被某个特征观测到时，需要移除这种关联关系，特征点可能被遮挡、移除视野
    void RemoveObservation(std::shared_ptr<Feature> feature){}

    std::list<std::weak_ptr<Feature>> GetObservations(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return observations_;
    }

    //factory function
    static MapPoint::Ptr CreateNewMappoint();

};
}