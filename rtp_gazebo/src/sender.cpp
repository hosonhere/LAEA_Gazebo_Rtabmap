#include "RTPSession.h"
#include "AirsimClient.h"
#include "Codec.h"


// ip 設定
std::string local_ip = "140.114.77.83";
std::string remote_ip = "140.114.77.83";

RTPSession *session = new RTPSession();
VideoCodec *rgb_codec = new VideoCodec();
DepthImageCodec *depth_codec = new DepthImageCodec();
PointCloudCodec *pcloud_codec = new PointCloudCodec();

void send_rgb_stream(int stream_id, cv::Mat &rgb_image){
    
    int ret;
    struct Data data;

    ret = rgb_codec->encode(rgb_image, &data);
    //std::cout << "ret: " << ret << std::endl;
    if(ret == 0){
        session->send_data(stream_id, &data);
        std::cout << "send rgb data" << std::endl;
    }
    destroy_data(&data);
}

void send_depth_stream(int stream_id, cv::Mat &depth_image){
    
    int ret;
    struct Data data;

    ret = depth_codec->encode(depth_image, &data);

    if(ret == 0){
        session->send_data(stream_id, &data);
        std::cout << "send depth data" << std::endl;
    }
    destroy_data(&data);
}

void send_pcloud_stream(int stream_id, pcl::PointCloud <pcl::PointXYZ>::Ptr pcloud){
    
    int ret;
    struct Data data;
    
    ret = pcloud_codec->encode(pcloud, &data);
    if(ret == 0){
        session->send_data(stream_id, &data);
        std::cout << "send point cloud data" << std::endl;
    }
    destroy_data(&data);
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

    // airsim client
    AirsimClient airsim_client;
    airsim_client.start();
    std::vector<ImageResponse> image_response;
    
    msr::airlib::LidarData lidar_data;
    std::vector <point_cloud> pcloud_data;

    for(int i = 0; i < 100; i++){
        airsim_client.get_image_data(image_response);
        airsim_client.get_lidar_data(lidar_data, pcloud_data);
        
        // send rgb and depth image
        cv::Mat rgb_image(
            image_response.at(0).height, image_response.at(0).width, CV_8UC3,
            (void*) image_response.at(0).image_data_uint8.data()
        );

        cv::Mat depth_image(
            image_response.at(1).height, image_response.at(1).width, CV_32FC1,
            (void*) image_response.at(1).image_data_float.data()
        );
        
        send_rgb_stream(0, rgb_image);

        cv::Mat depth_image_uint16;
        airsim_client.process_depth_data(depth_image, depth_image_uint16);
        send_depth_stream(1, depth_image_uint16);
        
        // send point cloud 
        pcl::PointCloud <pcl::PointXYZ>::Ptr pcloud(new pcl::PointCloud<pcl::PointXYZ>());
        for(int index=0; index<pcloud_data.size(); index++){
            pcloud->push_back(pcl::PointXYZ(pcloud_data[index].x, pcloud_data[index].y, pcloud_data[index].z));
        }
        send_pcloud_stream(2, pcloud);
    }

    return 0;
}