#include "Synchronizer.h"
#include <iostream>

Synchronizer::Synchronizer(){
    ros::NodeHandle nh;
    // Subscriber and publisher for sync raw_depth and camera_info
    depth_sub_.reset(new message_filters::Subscriber<sensor_msgs::Image>(nh, "/rtp/depth/image_raw", 1));
    depth_info_sub_.reset(new message_filters::Subscriber<sensor_msgs::CameraInfo>(nh, "/rtp/depth/camera_info", 1));

    sync_depth_pub = nh.advertise<sensor_msgs::Image>("/rtp/depth/sync/image_raw", 1);
    sync_camera_info_pub = nh.advertise<sensor_msgs::CameraInfo>("/rtp/depth/sync/camera_info", 1);

    sync_.reset(new message_filters::Synchronizer<Image_Info_Sync_Policy>(Image_Info_Sync_Policy(100), *depth_sub_, *depth_info_sub_));
    sync_->registerCallback(boost::bind(&Synchronizer::depthAndInfoCallback, this, _1, _2));

    // Debug depth_sub and depth_info_sub
    // depth_sub_->registerCallback(boost::bind(&Synchronizer::depth_callback, this, _1));
    // depth_info_sub_->registerCallback(boost::bind(&Synchronizer::depth_info_callback, this, _1));
}

void Synchronizer::depth_callback(const sensor_msgs::ImageConstPtr& depth_msg){
    std::cout << "Got depth image!" << std::endl;
    // print the header
    std::cout << "Header: " << depth_msg->header << std::endl;
}

void Synchronizer::depth_info_callback(const sensor_msgs::CameraInfoConstPtr& camera_info_msg){
    std::cout << "Got camera info!" << std::endl;
    // print the header
    std::cout << "Header: " << camera_info_msg->header << std::endl;
}

void Synchronizer::depthAndInfoCallback(const sensor_msgs::ImageConstPtr& depth_msg, const sensor_msgs::CameraInfoConstPtr& camera_info_msg){
    std::cout << "Got sync depth and camera info!" << std::endl;

    // Current ROS time
    ros::Time current_time = ros::Time::now();

    sensor_msgs::Image sync_depth_msg;
    sync_depth_msg = *depth_msg;
    sync_depth_msg.header.stamp = current_time;
    sync_depth_msg.header.frame_id = "depth_camera_link";
    sync_depth_msg.header.seq = depth_info_count;

    sync_depth_pub.publish(sync_depth_msg);

    // Publish sync camera info
    sensor_msgs::CameraInfo sync_camera_info_msg = *camera_info_msg;
    sync_camera_info_msg.header.stamp = current_time;
    sync_camera_info_msg.header.frame_id = "depth_camera_link";
    sync_camera_info_msg.header.seq = depth_info_count;

    sync_camera_info_pub.publish(sync_camera_info_msg);

    depth_info_count++;
}

Synchronizer::~Synchronizer(){
    std::cout << "Synchronizer destructor called!" << std::endl;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "Synchronizer");
    Synchronizer Synch;
    ros::spin();
    return 0;
}