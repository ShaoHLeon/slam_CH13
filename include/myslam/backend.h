#pragma once

#include "myslam/common_include.h"
#include "myslam/frame.h"
#include "myslam/map.h"


namespace myslam{

/**
 * 后端
 * 有单独优化线程，在Map更新时启动优化
 * Map更新由前端触发
 */
class Backend{


public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    typedef std::shared_ptr<Backend> Ptr;




};
}