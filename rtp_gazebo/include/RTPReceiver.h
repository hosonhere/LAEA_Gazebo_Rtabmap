#ifndef RTP_RECEIVER_H
#define RTP_RECEIVER_H

#include <iomanip>
#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <memory> // for std::shared_ptr


class RTP_Receiver{
    public: 
        RTP_Receiver();
        ~RTP_Receiver();

        void receive_rgb_stream(int stream_id);
        void receive_depth_stream(int stream_id);
        void receive_pcloud_stream(int stream_id);
        void receive_scan_stream(int stream_id);
        void receive_camera_info_stream(int stream_id);
        void receive_map_pcloud_stream(int stream_id);

        // Publisher
        ros::Publisher rgb_pub;
        ros::Publisher depth_pub;
        ros::Publisher pcloud_pub;
        ros::Publisher scan_pub;
        ros::Publisher camera_info_pub;
        ros::Publisher map_pcloud_pub;

    private:
        std::vector<cv::Mat *> restore_rgb_image; // received rgb data to be restored
        cv::Mat restore_depth_image; // received depth data to be restored
        // pcl::PointCloud<pcl::PointXYZ>::Ptr restore_pcloud; // received point cloud data to be restored
        // pcl::PointCloud<pcl::PointXYZ>::Ptr restore_scan; // received scan data to be restored
        sensor_msgs::CameraInfo restore_camera_info; // received camera info data to be restored
        int depth_image_count = 0;
        int pcloud_count = 0;
        int scan_count = 0;
        int camera_info_count = 0;
        int rgb_count = 0;
        int map_pcloud_count = 0;
};

#endif // RTP_RECEIVER_H