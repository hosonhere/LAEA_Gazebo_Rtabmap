#include "RTPSender.h"
#include "RTPSession.h"
#include "Data.h"
#include "Codec.h"
#include <iostream>

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

RTP_Sender::RTP_Sender(){
    ros::NodeHandle nh;
    depth_sub_.reset(new message_filters::Subscriber<sensor_msgs::Image>(nh, "/camera/depth/image_raw", 1));
    // rgb_sub_.reset(new message_filters::Subscriber<sensor_msgs::Image>(nh, "/camera/depth/rgb_image_raw", 1));
    // depth_info_sub_.reset(new message_filters::Subscriber<sensor_msgs::CameraInfo>(nh, "/camera/depth/camera_info", 1));
    pcloud_sub_.reset(new message_filters::Subscriber<sensor_msgs::PointCloud2>(nh, "/depth_scan_pointcloud", 1));
    scan_sub_.reset(new message_filters::Subscriber<sensor_msgs::PointCloud2>(nh, "/scan_pointcloud", 1));
    map_pcloud_sub_.reset(new message_filters::Subscriber<sensor_msgs::PointCloud2>(nh, "/sdf_map/occupancy_all", 1));
    position_vis_sub_.reset(new message_filters::Subscriber<visualization_msgs::Marker>(nh, "/planning/position_cmd_vis", 1));
    position_command_sub_.reset(new message_filters::Subscriber<quadrotor_msgs::PositionCommand>(nh, "/position_cmd", 1));
    local_odom_sub_.reset(new message_filters::Subscriber<nav_msgs::Odometry>(nh, "/mavros/local_position/odom", 1));
    pose_sub_.reset(new message_filters::Subscriber<geometry_msgs::PoseStamped>(nh, "/mavros/camera/pose", 1));

    // Callback function
    depth_sub_->registerCallback(boost::bind(&RTP_Sender::depth_callback, this, _1));
    // rgb_sub_->registerCallback(boost::bind(&RTP_Sender::rgb_callback, this, _1));
    // depth_info_sub_->registerCallback(boost::bind(&RTP_Sender::depth_info_callback, this, _1));
    pcloud_sub_->registerCallback(boost::bind(&RTP_Sender::depth_point_cloud_callback, this, _1));
    scan_sub_->registerCallback(boost::bind(&RTP_Sender::scan_point_cloud_callback, this, _1));
    map_pcloud_sub_->registerCallback(boost::bind(&RTP_Sender::map_point_cloud_callback, this, _1));
    position_vis_sub_->registerCallback(boost::bind(&RTP_Sender::position_vis_callback, this, _1));
    position_command_sub_->registerCallback(boost::bind(&RTP_Sender::position_command_callback, this, _1));
    local_odom_sub_->registerCallback(boost::bind(&RTP_Sender::local_odom_callback, this, _1));
    pose_sub_->registerCallback(boost::bind(&RTP_Sender::pose_callback, this, _1));
}

RTP_Sender::~RTP_Sender(){
    depth_sub_.reset();
    rgb_sub_.reset();
    depth_info_sub_.reset();
    pcloud_sub_.reset();
    scan_sub_.reset();
    map_pcloud_sub_.reset();
    position_vis_sub_.reset();
    position_command_sub_.reset();
    local_odom_sub_.reset();
    pose_sub_.reset();
}

void RTP_Sender::process_depth_data(cv::Mat &depth_image, cv::Mat &depth_image_uint16){
    int rows = depth_image.size().height;
    int cols = depth_image.size().width;

    int is_nan_num = 0, is_excceed_value_num = 0, is_normal_value_num = 0;
    
    depth_image_uint16 = cv::Mat::zeros(rows, cols, CV_16UC1);
    uint16_t bad_point = 0;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            float value = depth_image.at<float>(i, j);
            int milli_value;
            uint16_t convert_value;
            if(std::isnan(value)){
                is_nan_num++;
                convert_value = 0;
            }else if((milli_value = value * 1000) >= 65536){
                is_excceed_value_num++;
                convert_value = 0;
            }else{
                is_normal_value_num++;
                convert_value = (uint16_t)(value * 1000);
            }
            depth_image_uint16.at<uint16_t>(i, j) = convert_value;
        }
    }
}

void RTP_Sender::rgb_callback(const sensor_msgs::ImageConstPtr& rgb_msg){
    // std::cout << "RGB image callback" << " frame id: "<< rgb_msg->header.frame_id << std::endl;

    // Convert ROS image message to OpenCV image
    cv_bridge::CvImagePtr cv_ptr;
    try{
        cv_ptr = cv_bridge::toCvCopy(rgb_msg, sensor_msgs::image_encodings::BGR8);
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    // Successfully convert ROS image message to OpenCV image
    cv::Mat rgb_image = cv_ptr->image;

    // Send RGB image
    send_rgb_stream(0, rgb_image);
}

void RTP_Sender::depth_callback(const sensor_msgs::ImageConstPtr& depth_msg){
    // std::cout << "Depth image callback" <<" frame id: "<< depth_msg->header.frame_id << std::endl;

    // Print Sent time
    double current_time = ros::Time::now().toSec();
    // std::cout<<"Time duration between each sent depth image: "<<current_time-last_depth_time<<std::endl;
    last_depth_time = current_time;

    // Convert ROS image message to OpenCV image
    cv_bridge::CvImagePtr cv_ptr;
    try{
        cv_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_32FC1);
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    
    // Successfully convert ROS image message to OpenCV image
    cv::Mat depth_image = cv_ptr->image;

    // Process depth image by making it 16-bit unsigned integer
    cv::Mat depth_image_uint16;
    process_depth_data(depth_image, depth_image_uint16);

    // Send depth image
    send_depth_stream(1, depth_image_uint16);
}

void RTP_Sender::depth_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg){
    // std::cout << "Depth point cloud callback" << " frame id: "<< pcloud_msg->header.frame_id << std::endl;

    // Convert ROS point cloud message to PCL point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*pcloud_msg, *pcloud);

    // Send point cloud
    send_pcloud_stream(2, pcloud);
}

void RTP_Sender::scan_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg){
    // std::cout << "Scan point cloud callback" << " frame id: "<< pcloud_msg->header.frame_id << std::endl;

    // Convert ROS point cloud message to PCL point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*pcloud_msg, *pcloud);

    // Send point cloud
    send_pcloud_stream(3, pcloud);
}

void RTP_Sender::depth_info_callback(const sensor_msgs::CameraInfoConstPtr& depth_info_msg){
    // std::cout << "Depth camera info callback" << " frame id: "<< depth_info_msg->header.frame_id << std::endl;

    // Send camera info
    send_camera_info_stream(4, *depth_info_msg);
}

void RTP_Sender::map_point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& pcloud_msg){
    // std::cout << "Map point cloud callback" << " frame id: "<< pcloud_msg->header.frame_id << std::endl;

    // Convert ROS point cloud message to PCL point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*pcloud_msg, *pcloud);

    // Send point cloud
    send_pcloud_stream(5, pcloud);
}

void RTP_Sender::position_vis_callback(const visualization_msgs::Marker::ConstPtr& position_vis_msg){
    // std::cout << "Position visualization callback" << " frame id: "<< position_vis_msg->header.frame_id << std::endl;

    // Send position visualization
    send_position_vis_stream(6, *position_vis_msg);
}

void RTP_Sender::position_command_callback(const quadrotor_msgs::PositionCommand::ConstPtr& position_command_msg){
    // std::cout << "Position command callback" << " frame id: "<< position_command_msg->header.frame_id << std::endl;

    // Send position command
    send_position_command_stream(7, *position_command_msg);
}

void RTP_Sender::local_odom_callback(const nav_msgs::Odometry::ConstPtr& local_odom_msg){
    // std::cout << "Local odometry callback" << " frame id: "<< local_odom_msg->header.frame_id << std::endl;

    // Send local odometry
    send_local_odom_stream(8, *local_odom_msg);
}

void RTP_Sender::pose_callback(const geometry_msgs::PoseStamped::ConstPtr& pose_msg){
    // std::cout << "Pose callback" << " frame id: "<< pose_msg->header.frame_id << std::endl;

    // Send pose
    send_pose_stream(9, *pose_msg);
}

void RTP_Sender::send_rgb_stream(int stream_id, cv::Mat rgb_image){
    // std::cout << "Send RGB stream" << std::endl;

    int ret;
    struct Data data;

    ret = rgb_codec->encode(rgb_image, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send rgb data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_depth_stream(int stream_id, cv::Mat depth_image){
    // std::cout << "Send depth stream" << std::endl;

    int ret = 1;
    struct Data data;

    ret = depth_codec->encode(depth_image, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send depth data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_pcloud_stream(int stream_id, pcl::PointCloud<pcl::PointXYZ>::Ptr pcloud){
    // std::cout << "Send point cloud stream, id: " << stream_id<< std::endl;

    int ret;
    struct Data data;
    
    ret = pcloud_codec->encode(pcloud, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send point cloud data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_camera_info_stream(int stream_id, sensor_msgs::CameraInfo camera_info){
    // std::cout << "Send camera info stream" << std::endl;

    int ret;
    struct Data data;

    ret = camera_info_codec->encode(camera_info, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send camera info data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_position_vis_stream(int stream_id, visualization_msgs::Marker position_vis){
    // std::cout << "Send position visualization stream" << std::endl;

    int ret;
    struct Data data;

    ret = position_vis_codec->encode(position_vis, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send position visualization data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_position_command_stream(int stream_id, quadrotor_msgs::PositionCommand position_command){
    // std::cout << "Send position command stream" << std::endl;

    int ret;
    struct Data data;

    ret = position_command_codec->encode(&data, position_command);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send position command data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_local_odom_stream(int stream_id, nav_msgs::Odometry local_odom){
    // std::cout << "Send local odometry stream" << std::endl;

    int ret;
    struct Data data;

    ret = local_odom_codec->encode(local_odom, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send local odometry data" << std::endl;
    }

    destroy_data(&data);
}

void RTP_Sender::send_pose_stream(int stream_id, geometry_msgs::PoseStamped pose){
    // std::cout << "Send pose stream" << std::endl;

    int ret;
    struct Data data;

    ret = pose_codec->encode(pose, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        // std::cout << "send pose data" << std::endl;
    }

    destroy_data(&data);
}

int main(int argc, char** argv){

    rgb_codec->init(
        640, 480, AV_CODEC_ID_H264, AVMEDIA_TYPE_VIDEO, AV_PIX_FMT_YUV420P, AV_PIX_FMT_BGR24
    );

    int ret;
    
    if ((ret = session->create_session(local_ip, remote_ip)) != 0){
        exit(0);
    }
    
    // create two stream, rgb and depth stream
    if((ret = session->create_stream(
        0, 10000, 11000, "rgb_stream", "video", 96, "sendonly")) != 0){
        exit(0);
    }
    
    if((ret = session->create_stream(
        1, 12000, 13000, "depth_stream", "video", 97, "sendonly")) != 0){
        exit(0);
    }
    
    //create point cloud stream

    if((ret = session->create_stream(
        2, 14000, 15000, "point_cloud", "pointcloud", 98, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        3, 16000, 17000, "scan_point_cloud", "pointcloud", 99, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        4, 18000, 19000, "camera_info", "camera_info", 100, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        5, 20000, 21000, "map_point_cloud", "pointcloud", 101, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        6, 22000, 23000, "position_visualization", "marker", 102, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        7, 24000, 25000, "position_command", "command", 103, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        8, 26000, 27000, "local_odom", "odom", 104, "sendonly")) != 0){
        exit(0);
    }

    if((ret = session->create_stream(
        9, 28000, 29000, "pose", "pose", 105, "sendonly")) != 0){
        exit(0);
    }

    ros::init(argc, argv, "rtp_sender");
    RTP_Sender client;
    ros::MultiThreadedSpinner spinner(6); 	// 开两个spinner并行处理
    spinner.spin();
    return 0;
}