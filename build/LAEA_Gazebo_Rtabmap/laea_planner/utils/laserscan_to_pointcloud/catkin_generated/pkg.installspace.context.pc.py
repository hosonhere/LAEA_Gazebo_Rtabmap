# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include;/usr/include;/usr/include/eigen3".split(';') if "${prefix}/include;/usr/include;/usr/include/eigen3" != "" else []
PROJECT_CATKIN_DEPENDS = "dynamic_reconfigure;geometry_msgs;nav_msgs;rosconsole;roscpp;sensor_msgs;tf2;tf2_ros".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lpolar_to_cartesian_matrix_cache;-ltf_rosmsg_eigen_conversions;-ltf_collector;-llaserscan_to_pointcloud".split(';') if "-lpolar_to_cartesian_matrix_cache;-ltf_rosmsg_eigen_conversions;-ltf_collector;-llaserscan_to_pointcloud" != "" else []
PROJECT_NAME = "laserscan_to_pointcloud"
PROJECT_SPACE_DIR = "/home/hoson/laea_rtabmap/install"
PROJECT_VERSION = "1.0.0"
