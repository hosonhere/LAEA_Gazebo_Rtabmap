#ifndef RTP_SENDER_H
#define RTP_SENDER_H

#include <iomanip>
#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include "visualization_msgs/Marker.h"
#include "quadrotor_msgs/PositionCommand.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <memory> // for std::shared_ptr

// 點雲 structure
struct point_cloud {
    float x, y, z, r;
    point_cloud(float x0, float y0, float z0, float r0) {
        x = float(x0);
        y = float(y0);
        z = float(z0);
        r = float(r0);
    }
};

class RTP_Sender {
public:
    RTP_Sender();
    ~RTP_Sender();

    // Callback function
    void depth_callback(const sensor_msgs::ImageConstPtr& depth_msg);
    void rgb_callback(const sensor_msgs::ImageConstPtr& rgb_msg);
    void depth_info_callback(const sensor_msgs::CameraInfoConstPtr& depth_info_msg);
    void depth_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg);
    void scan_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg);
    void map_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg);
    void position_vis_callback(const visualization_msgs::Marker::ConstPtr& position_vis_msg);
    void position_command_callback(const quadrotor_msgs::PositionCommand::ConstPtr& position_command_msg);
    void local_odom_callback(const nav_msgs::Odometry::ConstPtr& local_odom_msg);
    void pose_callback(const geometry_msgs::PoseStamped::ConstPtr& pose_msg);

    // RTP Packet Packetization
    void send_rgb_stream(int stream_id, cv::Mat rgb_image);
    void send_depth_stream(int stream_id, cv::Mat depth_image);
    void send_pcloud_stream(int stream_id, pcl::PointCloud<pcl::PointXYZ>::Ptr pcloud);
    void send_camera_info_stream(int stream_id, sensor_msgs::CameraInfo camera_info);
    void send_position_vis_stream(int stream_id, visualization_msgs::Marker position_vis);
    void send_position_command_stream(int stream_id, quadrotor_msgs::PositionCommand position_command);
    void send_local_odom_stream(int stream_id, nav_msgs::Odometry local_odom);
    void send_pose_stream(int stream_id, geometry_msgs::PoseStamped pose);

    // Process depth image by making it 16-bit unsigned integer
    void process_depth_data(cv::Mat &depth_image, cv::Mat &depth_image_uint16);

private:
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::Image>> depth_sub_; // depth image
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::Image>> rgb_sub_; // rgb image

    // camera_info
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::CameraInfo>> depth_info_sub_;

    // point cloud
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::PointCloud2>> pcloud_sub_;
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::PointCloud2>> scan_sub_;
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::PointCloud2>> map_pcloud_sub_;

    // position visualization
    std::shared_ptr<message_filters::Subscriber<visualization_msgs::Marker>> position_vis_sub_;

    // position command
    std::shared_ptr<message_filters::Subscriber<quadrotor_msgs::PositionCommand>> position_command_sub_;

    // local odometry
    std::shared_ptr<message_filters::Subscriber<nav_msgs::Odometry>> local_odom_sub_;

    // pose
    std::shared_ptr<message_filters::Subscriber<geometry_msgs::PoseStamped>> pose_sub_;

    // RTP Packet Packetization
    double last_depth_time;
};

#endif // RTP_SENDER_H
