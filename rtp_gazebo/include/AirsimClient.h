#ifndef _AirsimClient_H_
#define _AirsimClient_H_

#include <opencv2/opencv.hpp>
#include <iomanip>

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/Common.hpp"
#include "common/common_utils/ProsumerQueue.hpp"
#include "common/common_utils/FileSystem.hpp"
#include "common/ClockFactory.hpp"

// 點雲 structure
struct point_cloud{
    float x, y, z, r;
    point_cloud(float x0, float y0, float z0, float r0){
        x = float(x0);
        y = float(y0);
        z = float(z0);
        r = float(r0);
    }
};

typedef msr::airlib::ImageCaptureBase::ImageRequest ImageRequest;
typedef msr::airlib::ImageCaptureBase::ImageResponse ImageResponse;
typedef msr::airlib::ImageCaptureBase::ImageType ImageType;

// Airsim Client API
class AirsimClient{

    public:
        AirsimClient();
        ~AirsimClient();
    
    public:
        msr::airlib::MultirotorRpcLibClient client;
    
    // 需要與 Airsim setting.json 設定相符
    private:
        std::vector<ImageRequest> image_request = {
            ImageRequest("RGB", ImageType::Scene, false, false),
            ImageRequest("DepthPlanar", ImageType::DepthPlanar, true, false)
        };

    public:
        void get_image_data(std::vector<ImageResponse> &image_data);
        void get_lidar_data(msr::airlib::LidarData& lidar_data, std::vector<point_cloud> &point_cloud_data);
        void process_depth_data(cv::Mat &depth_image, cv::Mat &depth_image_uint16);

    public:
        void start(){
            client.confirmConnection();
        }

    
};

#endif
