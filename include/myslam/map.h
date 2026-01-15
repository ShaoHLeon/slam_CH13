// 地图（Map）以散列形式记录所有的关键帧和对应的路标点，同时维护一个被激活的关键帧和地图点
// 激活：窗口，随着时间往前推动
// 后端从地图中取出激活的关键帧和地图点进行优化，忽略其余部分，达到控制优化规模的效果。
// 激活策略由我们自己定义，简单的激活策略：取出最旧的关键帧保持时间上最新的若干个关键帧
/*
// 前端：添加数据
1. 追踪新帧 -> InsertKeyframe()
2. 发现新特征点 -> InsertMapPoint()

// 后端：整理优化数据
1. 定期调用 CleanMap() 清理数据
2. 维护活跃窗口（RemoveOldKeyframe()）
3. 优化地图结构
*/

#pragma once

#include "myslam/common_include.h"
#include "myslam/mappoint.h" // 在使用MapPoint::Ptr时，编译器需要知道MapPoint的完整定义;unordered_map需要知道MapPoint的大小和布局信息;当一个类需要使用另一个类的成员变量时，必须知道该类的完整定义，所以需要包含该类的头文件
#include "myslam/frame.h"

namespace myslam{


/**
 * @brief 地图类
 * 负责管理地图中的所有关键帧和地图点
 * 交互：
 * - 前端调用 InsertKeyframe() 和 InsertMapPoint() 插入新的关键帧和地图点;
 * - 后端维护地图的结构，判定outlier，删除冗余关键帧和地图点
 */
class Map{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::shared_ptr<Map> Ptr;
    typedef std::unordered_map<unsigned long, MapPoint::Ptr> LandmarksType;
    typedef std::unordered_map<unsigned long, Frame::Ptr> KeyframesType;

private:
    std::mutex data_mutex_;
    LandmarksType landmarks_; // 地图点
    LandmarksType active_landmarks_; // 激活的地图点
    KeyframesType keyframes_; // 关键帧
    KeyframesType active_keyframes_; // 激活的关键帧

    Frame::Ptr current_frame_ = nullptr; // 当前帧

    int num_active_keyframes_ = 7;  // 激活的关键帧数量，只保留最新的7个关键帧


public:
    Map(){}

    // =======================前端=======================
    // 增加一个关键帧
    void InsertKeyframe(Frame::Ptr frame);
    // 增加一个地图点
    void InsertMapPoint(MapPoint::Ptr map_point);

    // 获取当前地图中的所有关键帧
    KeyframesType GetAllKeyframes(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return keyframes_;
    }
    // 获取当前地图中的所有地图点
    LandmarksType GetAllMapPoints(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return landmarks_;
    }

    // 获取当前地图中的激活关键帧
    KeyframesType GetActivateKeyframes(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return active_keyframes_;
    }
    // 获取当前地图中的激活地图点
    LandmarksType GetActivateMapPoints(){
        std::unique_lock<std::mutex> lck(data_mutex_);
        return active_landmarks_;
    }

    // 清理地图中的冗余关键帧和地图点，即观测数量为0的地图点
    void CleanMap();

private:
    // 将旧的关键帧设置为不活跃状态
    void RemoveOldKeyframe();

};


}