#pragma once

//std
#include <memory> // 
#include <mutex>
#include <vector>



#include <Eigen/Core>
#include <Eigen/Geometry>


// double vectors
typedef Eigen::Matrix<double, 3, 1> Vec3;



#include <sophus/se3.hpp> 
#include <sophus/so3.hpp>
typedef Sophus::SE3d SE3;
typedef Sophus::SO3d SO3;


#include <opencv2/core/core.hpp>
