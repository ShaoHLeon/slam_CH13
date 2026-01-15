#pragma once

//std
#include <memory> // 提供内存管理相关的功能，提供智能指针类，如shared_ptr和unique_ptr，用于自动管理动态分配的内存，避免内存泄漏
#include <mutex>
#include <vector>
#include <list> // STL标准模板库中的容器类，实现一个双向链表数据结构，每个节点包含指向前一个节点和后一个节点的指针，以及存储的数据
#include <unordered_map>




#include <Eigen/Core>
#include <Eigen/Geometry>


// double vectors，元素类型是double双精度浮点数
typedef Eigen::Matrix<double, 3, 1> Vec3; // 3维列向量，



#include <sophus/se3.hpp> 
#include <sophus/so3.hpp>
typedef Sophus::SE3d SE3;
typedef Sophus::SO3d SO3;


#include <opencv2/core/core.hpp>
