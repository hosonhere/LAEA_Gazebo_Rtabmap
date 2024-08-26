#include <ros/ros.h>
#include "OdomCodec.h"

OdomCodec::OdomCodec(){};
OdomCodec::~OdomCodec(){};

int OdomCodec::encode(nav_msgs::Odometry &odom, struct Data *data){
    // 计算序列化后的数据大小
    uint32_t data_size = ros::serialization::serializationLength(odom);
    
    // 分配缓冲区
    uint8_t *buffer = (uint8_t*)malloc(data_size);
    
    // 创建序列化流
    ros::serialization::OStream stream(buffer, data_size);
    
    // 序列化
    ros::serialization::serialize(stream, odom);
    
    // 将序列化数据保存到data结构体
    data->size = data_size;
    data->buffer = (uint8_t*)malloc(sizeof(uint8_t) * data_size);
    memcpy(data->buffer, buffer, data_size);
    
    // 释放临时缓冲区
    free(buffer);
    
    return 0;
};

int OdomCodec::decode(struct Data *data, nav_msgs::Odometry &restore_odom){
    // std::cout<<"Decoding odom..."<<std::endl;
    uint8_t *buffer = reinterpret_cast<uint8_t*>(data->buffer);
    ros::serialization::IStream stream(buffer, data->size);

    ros::serialization::deserialize(stream, restore_odom);
    // std::cout<<"Successfully decoded odom!"<<std::endl;
    return 0;
};
