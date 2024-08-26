cmake_minimum_required(VERSION 2.8.3)
project(rtp)

# 设置RTPDevice_DIR的路径
set(RTPDevice_DIR "/home/hoson/laea_rtabmap/src/LAEA_Gazebo_Rtabmap")

# 包含必要的头文件目录
include_directories(${RTPDevice_DIR}/3rd/uvgRTP/include/uvgRTP)
include_directories(${RTPDevice_DIR}/3rd/Zdepth/include)
include_directories(${RTPDevice_DIR}/3rd/Zdepth/zstd/include)
include_directories(${RTPDevice_DIR}/3rd/uvgRTP/include)

# 找到并链接PCL库
find_package(PCL REQUIRED)
message(STATUS "Using PCL version: ${PCL_VERSION}")
include_directories(${PCL_INCLUDE_DIRS})

# 找到并链接libav库
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

# 找到并链接OpenCV库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# ROS 组件
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  visualization_msgs
  quadrotor_msgs
  cv_bridge
  message_filters
  sensor_msgs
  nav_msgs
  geometry_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

# 定义静态库rtp
add_library(rtp STATIC
    "${RTPDevice_DIR}/rtp/Codec/src/VideoCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/DepthImageCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/PointCloudCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/CameraInfoCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/MarkerCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/CommandCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/OdomCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/PoseCodec.cpp"
    "${RTPDevice_DIR}/rtp/Codec/src/Data.cpp"
    "${RTPDevice_DIR}/rtp/src/Stream.cpp"
    "${RTPDevice_DIR}/rtp/src/RTPSession.cpp"
)

# 包含目录配置
target_include_directories(rtp
    PUBLIC ${RTPDevice_DIR}/rtp/include
    PUBLIC ${RTPDevice_DIR}/rtp/src
    PUBLIC ${RTPDevice_DIR}/rtp/Codec/include
    PUBLIC ${RTPDevice_DIR}/rtp/Codec/src
)

# 链接库
target_link_libraries(rtp
    ${PCL_LIBRARIES}
    ${OpenCV_LIBS}
    PkgConfig::LIBAV
    ${catkin_LIBRARIES}
)

# catkin包配置
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rtp
  CATKIN_DEPENDS roscpp std_msgs cv_bridge message_filters sensor_msgs
  DEPENDS PCL OpenCV
)
