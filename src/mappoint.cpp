#include "myslam/mappoint.h"
#include "myslam/feature.h"


namespace myslam{

MapPoint::MapPoint(long id, Vec3 position): id_(id), pose_(position){}

// MapPoint类的成员函数，用于移除一个观测特征
void MapPoint::RemoveObservation(std::shared_ptr<Feature> feature){
    std::unique_lock<std::mutex> loc(data_mutex_);     // unique_lock会在构造时加锁，析构时自动解锁
    
    // 遍历observations_列表（存储所有观测到这个地图点的特征）
    for (auto iter = observations_.begin(); iter != observations_.end(); iter++){
        // iter是迭代器，指向 weak_ptr<Feature>，lock()将weak_ptr转换为shared_ptr
        if (iter->lock() == feature){
            observations_.erase(iter);
            feature->map_point_.reset(); // reset()将shared_ptr置空，断开与MapPoint的连接
            observed_times_--;
            break;
        }
    }
}


// 创建新的地图点对象
MapPoint::Ptr MapPoint::CreateNewMappoint(){
    static long factory_id = 0;
    MapPoint::Ptr new_mappoint(new MapPoint);
    new_mappoint->id_ = factory_id++;
    return new_mappoint;
}


}