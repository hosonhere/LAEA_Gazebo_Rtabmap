cmake_minimum_required(VERSION 3.16)

project(rtp_gazebo)
aux_source_directory(./src SRC)
include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(RTPDevice_DIR "/home/hoson/laea_rtabmap/src/LAEA_Gazebo_Rtabmap/rtp")
set(OTHER_PLUGIN_DIR "/home/hoson/laea_rtabmap/src/LAEA_Gazebo_Rtabmap/3rd")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  visualization_msgs
  cv_bridge
  message_filters
  sensor_msgs
)

include_directories(${RTPDevice_DIR}/include)
include_directories(${RTPDevice_DIR}/Codec/include)
include_directories(${OTHER_PLUGIN_DIR}/Zdepth/include)
include_directories(${OTHER_PLUGIN_DIR}/Zdepth/zstd/include)
include_directories(${OTHER_PLUGIN_DIR}/uvgRTP/include)

find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PCL REQUIRED)
find_package(gazebo REQUIRED)
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

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rtp_gazebo
  CATKIN_DEPENDS roscpp std_msgs cv_bridge message_filters sensor_msgs
  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

add_executable(RTPSender ./src/RTPSender.cpp)
target_link_libraries(RTPSender ${catkin_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} ${GAZEBO_LIBRARIES})
target_link_libraries(RTPSender /home/hoson/laea_rtabmap/devel/lib/librtp.a)
target_link_libraries(RTPSender ${OTHER_PLUGIN_DIR}/uvgRTP/build/libuvgrtp.a)
target_link_libraries(RTPSender ${OTHER_PLUGIN_DIR}/Zdepth/libzdepth.a)
target_link_libraries(RTPSender ${OTHER_PLUGIN_DIR}/Zdepth/zstd/libzstd.a)
target_link_libraries(RTPSender PkgConfig::LIBAV)
target_link_libraries(RTPSender -lpthread)

add_executable(RTPReceiver ./src/RTPReceiver.cpp)
target_link_libraries(RTPReceiver ${catkin_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} ${GAZEBO_LIBRARIES})
target_link_libraries(RTPReceiver /home/hoson/laea_rtabmap/devel/lib/librtp.a)
target_link_libraries(RTPReceiver ${OTHER_PLUGIN_DIR}/uvgRTP/build/libuvgrtp.a)
target_link_libraries(RTPReceiver ${OTHER_PLUGIN_DIR}/Zdepth/libzdepth.a)
target_link_libraries(RTPReceiver ${OTHER_PLUGIN_DIR}/Zdepth/zstd/libzstd.a)
target_link_libraries(RTPReceiver PkgConfig::LIBAV)
target_link_libraries(RTPReceiver -lpthread)

add_executable(Synchronizer ./src/Synchronizer.cpp)
target_link_libraries(Synchronizer ${catkin_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} ${GAZEBO_LIBRARIES})
target_link_libraries(Synchronizer /home/hoson/laea_rtabmap/devel/lib/librtp.a)
target_link_libraries(Synchronizer ${OTHER_PLUGIN_DIR}/uvgRTP/build/libuvgrtp.a)
target_link_libraries(Synchronizer ${OTHER_PLUGIN_DIR}/Zdepth/libzdepth.a)
target_link_libraries(Synchronizer ${OTHER_PLUGIN_DIR}/Zdepth/zstd/libzstd.a)
target_link_libraries(Synchronizer PkgConfig::LIBAV)
target_link_libraries(Synchronizer -lpthread)
