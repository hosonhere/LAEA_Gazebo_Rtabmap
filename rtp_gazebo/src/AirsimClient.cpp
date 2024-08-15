#include "AirsimClient.h"

AirsimClient::AirsimClient(){};
AirsimClient:: ~AirsimClient(){};

void AirsimClient::get_image_data(std::vector<ImageResponse>& image_data){
    image_data = client.simGetImages(image_request);
};

// 需要與 Airsim setting.json 設定相符
void AirsimClient::get_lidar_data(msr::airlib::LidarData &lidar_data, 
                                  std::vector<point_cloud> &point_cloud_data){
    lidar_data = client.getLidarData("Lidar", "drone_1");
    point_cloud_data.clear();
    
    std::vector<float>::iterator iter = lidar_data.point_cloud.begin();

    // NED 數值轉 ENU x, y 對調, z值取負號
    // airsim 數組排列 (x,y,z,x,y,z......)
    for(iter; iter < lidar_data.point_cloud.end(); iter = iter+3){
        std::iter_swap(iter, iter + 1);
        *(iter + 2) =- (*(iter + 2));
        point_cloud_data.push_back(point_cloud(float(*iter), float(*(iter+1)), float(*(iter+2)), 0.0f));
    }
}

// airsim 的原始資料 max range 為 100 米
// 因實際的 kinect camera 的資料範圍並沒有那麼大 (約莫 5 米內)
// 此 function 為只取約 5 米內的資料範圍作為有效資料
void AirsimClient::process_depth_data(cv::Mat &depth_image, cv::Mat &depth_image_uint16){
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
            }else if((milli_value = value * 1000) > 65536){
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