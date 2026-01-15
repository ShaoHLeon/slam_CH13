
#include "myslam/map.h"


namespace myslam{

// 增加一个关键帧，or 更新一个关键帧
void Map::InsertKeyframe(Frame::Ptr frame){
    current_frame_ = frame;
    if(keyframes_.find(frame->keyframe_id_) == keyframes_.end()){ // 如果关键帧不存在，则插入
        keyframes_.insert(make_pair(frame->keyframe_id_, frame));
        active_keyframes_.insert(make_pair(frame->keyframe_id_, frame));
    }
    else{
        keyframes_[frame->keyframe_id_] = frame;
        active_keyframes_[frame->keyframe_id_] = frame;
    }

    if(active_keyframes_.size() > num_active_keyframes_){ // 如果活跃关键帧数量超过阈值，则清理
        RemoveOldKeyframe();
    }
}

// 增加一个地图点
void InsertMapPoint(MapPoint::Ptr map_point);

// 清理地图中的冗余关键帧和地图点，即观测数量为0的地图点
void CleanMap();

// 将旧的关键帧设置为不活跃状态
void RemoveOldKeyframe();

}