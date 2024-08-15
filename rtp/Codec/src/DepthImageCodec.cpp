#include "DepthImageCodec.h"

DepthImageCodec::DepthImageCodec(){
    width = 640;
    height = 480;
};
DepthImageCodec::DepthImageCodec(int _width, int _height): width(_width), height(_height){};

DepthImageCodec::~DepthImageCodec(){};

// 壓縮深度圖資料
int DepthImageCodec::encode(cv::Mat &depth_image, struct Data *data){

    const int height = depth_image.size().height;
    const int width = depth_image.size().width;

    assert(width % zdepth::kBlockSize == 0);
    assert(height % zdepth::kBlockSize == 0);

    std::vector<uint16_t> depth_data;
    if(depth_image.isContinuous()){
        depth_data.assign((uint16_t*) depth_image.data, (uint16_t*) depth_image.data + depth_image.total() * depth_image.channels());
    }else{
        for(int i = 0; i < depth_image.rows; i++){
            depth_data.insert(depth_data.end(), depth_image.ptr<uint16_t>(i), depth_image.ptr<uint16_t>(i) + depth_image.cols*depth_image.channels());
        }
    }

    //std::cout << "depth_data size: " << depth_data.size() << std::endl;
    bool key_frame = true;
    uint16_t *depth_data_ptr = &depth_data[0];
    
    std::vector<uint8_t> compressed_data;
    compressor.Compress(width, height, depth_data_ptr, compressed_data, true);
    
    data->size = compressed_data.size();
    data->buffer = (uint8_t *)malloc(sizeof(uint8_t) * (data->size));
    std::copy(compressed_data.begin(), compressed_data.end(), data->buffer);

    //std::cout << "compressed_size: " << compressed_data.size() << std::endl;
    
    return 0;
};

// 還原深度圖資料
int DepthImageCodec::decode(struct Data *data, cv::Mat &restore_depth_image){
    
    std::vector<uint8_t> compressed_data(data->buffer, (data->buffer) + data->size);
    std::vector<uint16_t> restore_depth_data;
    
    int restore_width, restore_height;
    zdepth::DepthResult result = decompressor.Decompress(compressed_data, restore_width, restore_height, restore_depth_data);
    
    if(result != zdepth::DepthResult::Success){
        std::cout << "Failed: decompressor.Decompress returned " << zdepth::DepthResultString(result) << std::endl;
        return -1;
    }

    if(restore_width != width || restore_height != height){
        std::cout << "Failed: decompressor resolution not matched" << std::endl;
        return -1;
    }

    cv::Mat restore_depth_image_uint16 = cv::Mat(height, width, CV_16UC1);
    restore_depth_image = cv::Mat(height, width, CV_32FC1);
    memcpy(restore_depth_image_uint16.data, restore_depth_data.data(), restore_depth_data.size() * sizeof(uint16_t));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            float value;
            value = (float)restore_depth_image_uint16.at<uint16_t>(i, j) / 1000;
            restore_depth_image.at<float>(i, j) = value;
        }
    }

    //std::cout << "restore_width: " << width << " restore_height: " << height << std::endl; 
    return 0;
};