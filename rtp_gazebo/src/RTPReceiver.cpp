#include "RTPReceiver.h"
#include "RTPSession.h"
#include "Codec.h"
// #include <iostream>

std::string local_ip = "140.114.208.49";
std::string remote_ip = "140.114.208.49";

RTPSession *session = new RTPSession();
VideoCodec *rgb_codec = new VideoCodec();
DepthImageCodec *depth_codec = new DepthImageCodec();
PointCloudCodec *pcloud_codec = new PointCloudCodec();
CameraInfoCodec *camera_info_codec = new CameraInfoCodec();
MarkerCodec *position_vis_codec = new MarkerCodec();
CommandCodec *position_command_codec = new CommandCodec();
OdomCodec *local_odom_codec = new OdomCodec();
PoseCodec *pose_codec = new PoseCodec();

RTP_Receiver::RTP_Receiver(){
    ros::NodeHandle nh;
    // rgb_pub = nh.advertise<sensor_msgs::Image>("/rtp/depth/rgb_image_raw", 1);
    depth_pub = nh.advertise<sensor_msgs::Image>("/rtp/depth/image_raw", 1);
    pcloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/rtp/pointcloud/depth", 1);
    scan_pub = nh.advertise<sensor_msgs::PointCloud2>("/rtp/pointcloud/scan", 1);
    // camera_info_pub = nh.advertise<sensor_msgs::CameraInfo>("/rtp/depth/camera_info", 1);
    map_pcloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/rtp/pointcloud/map", 1);
    marker_pub = nh.advertise<visualization_msgs::Marker>("/rtp/position_vis", 1);
    pos_cmd_pub = nh.advertise<quadrotor_msgs::PositionCommand>("/rtp/position_cmd", 1);
    local_odom_pub = nh.advertise<nav_msgs::Odometry>("/rtp/local_odom", 1);
    pose_pub = nh.advertise<geometry_msgs::PoseStamped>("/rtp/pose", 1);
}

void RTP_Receiver::depth_image_timer_cb(){
    // Current time
    double current_time = ros::Time::now().toSec();
    std::cout<<"Time duration between each received depth image: "<<current_time-depth_image_timer<<std::endl;
    depth_image_timer = current_time;
}

void RTP_Receiver::receive_rgb_stream(int stream_id){
    
    int ret = 1;

    while(true){
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = rgb_codec->decode(data, restore_rgb_image);
            if (ret == 0){

                // std::cout << "Got rgb image frame!" << std::endl;

                // Convert cv::Mat to sensor_msgs::Image
                for(int i = 0; i < restore_rgb_image.size(); i++){
                    cv::Mat rgb_image = *restore_rgb_image[i];
                    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", rgb_image).toImageMsg();

                    msg->header.stamp = ros::Time::now();  // 使用当前的 ROS 时间戳
                    msg->header.frame_id = "depth_camera_link";  // 使用相机的坐标系
                    msg->header.seq = rgb_count++;
                    rgb_pub.publish(msg);
                }

                restore_rgb_image.clear();
            }
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_depth_stream(int stream_id){
    
    int ret = 1;

    while(true){
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            // Timer Callback
            // depth_image_timer_cb();
            ret = depth_codec->decode(data, restore_depth_image);
            if(ret == 0){
                // std::cout << "Got depth image frame!" << std::endl;
                
                // Convert cv::Mat to sensor_msgs::Image and publish it
                sensor_msgs::Image depth_msg;

                std_msgs::Header header;
                header.stamp = ros::Time::now();  // 使用当前的 ROS 时间戳
                header.frame_id = "depth_camera_link";  // 使用相机的坐标系
                header.seq = depth_image_count++;
                cv_bridge::CvImage(header, "32FC1", restore_depth_image).toImageMsg(depth_msg);

                depth_pub.publish(depth_msg);

                // std::cout << "Published depth image!" << std::endl;

            }
            destroy_data(data);
        }
    }
}


void RTP_Receiver::receive_pcloud_stream(int stream_id){
    int ret = 1;
    
    while(true){
        pcl::PointCloud<pcl::PointXYZ>::Ptr restore_pcloud(new pcl::PointCloud<pcl::PointXYZ>());
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = pcloud_codec->decode(data, restore_pcloud);
            if(ret == 0){
                // std::cout << "Got point cloud frame!" << std::endl;

                // Convert PCL point cloud to sensor_msgs::PointCloud2
                sensor_msgs::PointCloud2 pcloud_msg;
                pcl::toROSMsg(*restore_pcloud, pcloud_msg);
                pcloud_msg.header.stamp = ros::Time::now();
                pcloud_msg.header.frame_id = "map";
                pcloud_msg.header.seq = pcloud_count++;

                pcloud_pub.publish(pcloud_msg);

                // Destroy msg after publish
                pcloud_msg.data.clear();
            }
            restore_pcloud.reset();
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_scan_stream(int stream_id){
    int ret = 1;
    
    while(true){
        pcl::PointCloud<pcl::PointXYZ>::Ptr restore_scan(new pcl::PointCloud<pcl::PointXYZ>());
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = pcloud_codec->decode(data, restore_scan);
            if(ret == 0){
                // std::cout << "Got scan point cloud frame!" << std::endl;

                // Convert PCL point cloud to sensor_msgs::PointCloud2
                sensor_msgs::PointCloud2 pcloud_msg;
                pcl::toROSMsg(*restore_scan, pcloud_msg);

                pcloud_msg.header.stamp = ros::Time::now();
                pcloud_msg.header.frame_id = "map";
                pcloud_msg.header.seq = scan_count++;
                scan_pub.publish(pcloud_msg);

                // Destroy msg after publish
                pcloud_msg.data.clear();
            }
            restore_scan.reset();
            destroy_data(data);
        }

    }
}

void RTP_Receiver::receive_camera_info_stream(int stream_id){
    int ret = 1;
    
    while(true){
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = camera_info_codec->decode(data, restore_camera_info);
            if(ret == 0){
                // std::cout << "Got camera info frame!" << std::endl;

                // Setup message header
                restore_camera_info.header.stamp = ros::Time::now();
                restore_camera_info.header.frame_id = "depth_camera_link";
                restore_camera_info.header.seq = camera_info_count++;
                
                camera_info_pub.publish(restore_camera_info);
                
            }
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_map_pcloud_stream(int stream_id){
    int ret = 1;
    
    while(true){
        pcl::PointCloud<pcl::PointXYZ>::Ptr restore_map_pcloud(new pcl::PointCloud<pcl::PointXYZ>());
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = pcloud_codec->decode(data, restore_map_pcloud);
            if(ret == 0){
                // std::cout << "Got map point cloud frame!" << std::endl;

                // Convert PCL point cloud to sensor_msgs::PointCloud2
                sensor_msgs::PointCloud2 pcloud_msg;
                pcl::toROSMsg(*restore_map_pcloud, pcloud_msg);
                pcloud_msg.header.stamp = ros::Time::now();
                pcloud_msg.header.frame_id = "world";
                pcloud_msg.header.seq = map_pcloud_count++;

                map_pcloud_pub.publish(pcloud_msg);

                // Destroy msg after publish
                pcloud_msg.data.clear();
            }
            restore_map_pcloud.reset();
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_marker_stream(int stream_id){
    int ret = 1;
    
    while(true){
        visualization_msgs::Marker restore_marker;
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = position_vis_codec->decode(data, restore_marker);
            if(ret == 0){
                // std::cout << "Got marker frame!" << std::endl;

                // Setup message header
                restore_marker.header.stamp = ros::Time::now();
                restore_marker.header.frame_id = "world";
                restore_marker.header.seq = marker_count++;
                
                marker_pub.publish(restore_marker);
                
            }
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_pos_cmd_stream(int stream_id){
    int ret = 1;
    
    while(true){
        quadrotor_msgs::PositionCommand restore_pos_cmd;
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = position_command_codec->decode(restore_pos_cmd, data);
            if(ret == 0){
                // std::cout << "Got position command frame!" << std::endl;

                // Setup message header
                restore_pos_cmd.header.stamp = ros::Time::now();
                restore_pos_cmd.header.frame_id = "world";
                restore_pos_cmd.header.seq = pos_cmd_count++;
                
                pos_cmd_pub.publish(restore_pos_cmd);
                
            }
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_local_odom_stream(int stream_id){
    int ret = 1;
    
    while(true){
        nav_msgs::Odometry restore_local_odom;
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = local_odom_codec->decode(data, restore_local_odom);
            if(ret == 0){
                // std::cout << "Got local odom frame!" << std::endl;

                // Setup message header
                restore_local_odom.header.stamp = ros::Time::now();
                restore_local_odom.header.frame_id = "map"; // Remember to change this 
                restore_local_odom.header.seq = local_odom_count++;
                
                local_odom_pub.publish(restore_local_odom);
                
            }
            destroy_data(data);
        }
    }
}

void RTP_Receiver::receive_pose_stream(int stream_id){
    int ret = 1;
    
    while(true){
        geometry_msgs::PoseStamped restore_pose;
        struct Data *data;
        data = session->get_data(stream_id);
        if(data){
            ret = pose_codec->decode(data, restore_pose);
            if(ret == 0){
                // std::cout << "Got pose frame!" << std::endl;

                // Setup message header
                restore_pose.header.stamp = ros::Time::now();
                restore_pose.header.frame_id = "camera_link"; // Remember to change this 
                restore_pose.header.seq = pose_count++;
                
                pose_pub.publish(restore_pose);
                
            }
            destroy_data(data);
        }
    }
}

RTP_Receiver::~RTP_Receiver(){

}

int main(int argc, char **argv){

    rgb_codec->init(
        640, 480, AV_CODEC_ID_H264, AVMEDIA_TYPE_VIDEO, AV_PIX_FMT_YUV420P, AV_PIX_FMT_BGR24
    );

    int ret;
    
    if ((ret = session->create_session(local_ip, remote_ip)) != 0){
        exit(0);
    }
    
    // create two stream, rgb and depth stream
    if((ret = session->create_stream(
        0, 11000, 10000, "rgb_stream", "video", 96, "recvonly")) != 0){
        exit(0);
    }
    
    if((ret = session->create_stream(
        1, 13000, 12000, "depth_stream", "video", 97, "recvonly")) != 0){
        exit(0);
    }
    
    // create point cloud stream
    if((ret = session->create_stream(
        2, 15000, 14000, "point_cloud", "pointcloud", 98, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        3, 17000, 16000, "scan_point_cloud", "pointcloud", 99, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        4, 19000, 18000, "camera_info", "camera_info", 100, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        5, 21000, 20000, "map_point_cloud", "pointcloud", 101, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        6, 23000, 22000, "position_visualization", "marker", 102, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        7, 25000, 24000, "position_command", "command", 103, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        8, 27000, 26000, "local_odom", "odom", 104, "recvonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        9, 29000, 28000, "pose", "pose", 105, "recvonly")) != 0){
        exit(0);
    }

    ros::init(argc, argv, "rtp_receiver");

    RTP_Receiver rtp_receiver;
    // std::thread recv_rgb_thread = std::thread(&RTP_Receiver::receive_rgb_stream, &rtp_receiver, 0);
    std::thread recv_depth_thread = std::thread(&RTP_Receiver::receive_depth_stream, &rtp_receiver, 1);
    std::thread recv_pcloud_thread = std::thread(&RTP_Receiver::receive_pcloud_stream, &rtp_receiver, 2);
    std::thread recv_scan_thread = std::thread(&RTP_Receiver::receive_scan_stream, &rtp_receiver, 3);
    // std::thread recv_camera_info_thread = std::thread(&RTP_Receiver::receive_camera_info_stream, &rtp_receiver, 4);
    std::thread recv_map_pcloud_thread = std::thread(&RTP_Receiver::receive_map_pcloud_stream, &rtp_receiver, 5);
    std::thread recv_marker_thread = std::thread(&RTP_Receiver::receive_marker_stream, &rtp_receiver, 6);
    std::thread recv_pos_cmd_thread = std::thread(&RTP_Receiver::receive_pos_cmd_stream, &rtp_receiver, 7);
    std::thread recv_local_odom_thread = std::thread(&RTP_Receiver::receive_local_odom_stream, &rtp_receiver, 8);
    std::thread recv_pose_thread = std::thread(&RTP_Receiver::receive_pose_stream, &rtp_receiver, 9);

    // recv_rgb_thread.join();
    recv_depth_thread.join();
    recv_pcloud_thread.join();
    recv_scan_thread.join();
    // recv_camera_info_thread.join();
    recv_map_pcloud_thread.join();
    recv_marker_thread.join();
    recv_pos_cmd_thread.join(); 
    recv_local_odom_thread.join();
    recv_pose_thread.join();

    ros::spin();

    return 0;
}