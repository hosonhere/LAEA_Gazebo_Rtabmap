# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include".split(';') if "${prefix}/include" != "" else []
PROJECT_CATKIN_DEPENDS = "dynamic_reconfigure;image_geometry;image_transport;nodelet;roscpp;sensor_msgs".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lDepthImageToLaserScan;-lDepthImageToLaserScanROS;-lDepthImageToLaserScanNodelet".split(';') if "-lDepthImageToLaserScan;-lDepthImageToLaserScanROS;-lDepthImageToLaserScanNodelet" != "" else []
PROJECT_NAME = "depthimage_to_laserscan"
PROJECT_SPACE_DIR = "/home/hoson/laea_rtabmap/install"
PROJECT_VERSION = "1.0.8"
