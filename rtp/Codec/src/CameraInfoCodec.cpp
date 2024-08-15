#include "CameraInfoCodec.h"
#include <iostream>

CameraInfoCodec::CameraInfoCodec(){
    width = 640;
    height = 480;
};

CameraInfoCodec::CameraInfoCodec(int _width, int _height){
    width = _width;
    height = _height;
};

CameraInfoCodec::~CameraInfoCodec(){};

int CameraInfoCodec::encode(sensor_msgs::CameraInfo &camera_info, struct Data *data) {
    // 计算序列化后的数据大小
    uint32_t data_size = ros::serialization::serializationLength(camera_info);
    
    // 分配缓冲区
    uint8_t *buffer = (uint8_t*)malloc(data_size);
    
    // 创建序列化流
    ros::serialization::OStream stream(buffer, data_size);
    
    // 序列化
    ros::serialization::serialize(stream, camera_info);
    
    // 将序列化数据保存到data结构体
    data->size = data_size;
    data->buffer = (uint8_t*)malloc(sizeof(uint8_t) * data_size);
    memcpy(data->buffer, buffer, data_size);
    
    // 释放临时缓冲区
    free(buffer);
    
    return 0;
};

int CameraInfoCodec::decode(struct Data *data, sensor_msgs::CameraInfo &restore_camera_info){
    // std::cout<<"Decoding camera info..."<<std::endl;
    uint8_t *buffer = reinterpret_cast<uint8_t*>(data->buffer);
    ros::serialization::IStream stream(buffer, data->size);

    ros::serialization::deserialize(stream, restore_camera_info);
    // std::cout<<"Successfully decoded camera info!"<<std::endl;
    return 0;
};

