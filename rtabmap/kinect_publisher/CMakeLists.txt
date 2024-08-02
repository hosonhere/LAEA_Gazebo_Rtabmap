cmake_minimum_required(VERSION 2.8.3)
project(kinect_publisher)

find_package(catkin REQUIRED COMPONENTS
  rospy
  sensor_msgs
  tf
)

catkin_package()

catkin_install_python(PROGRAMS
  src/kinect_publisher.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
