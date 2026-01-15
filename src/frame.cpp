#include "myslam/frame.h"



namespace myslam{

Frame::Frame(long id, double time_stamp, const SE3 &pose, const cv::Mat &left, const cv::Mat &right)
        : id_(id), time_stamp_(time_stamp), pose_(pose), left_img_(left), right_img_(right) {}


// 设置关键帧并分配关键帧id
void Frame::SetKeyFrame(){
    static long keyframe_factory_id = 0; //
    is_keyframe_ = true;
    keyframe_id_ = keyframe_factory_id++;
}

Frame::Ptr Frame::CreateFrame(){
    static long factory_id = 0; // 静态变量，在程序运行期间一直存在。只会在第一次调用时初始化为0
    Frame::Ptr new_frame(new Frame); //
    new_frame->id_ = factory_id++; // 分配id，将当前的factory_id值赋给新创建的Frame对象的id_成员，然后将factory_id加1
    return new_frame;
}
}