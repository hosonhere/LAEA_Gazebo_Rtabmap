#include <ros/ros.h>
#include "CommandCodec.h"

CommandCodec::CommandCodec(){};
CommandCodec::~CommandCodec(){};

int CommandCodec::encode(struct Data *data, quadrotor_msgs::PositionCommand &command){
    // 计算序列化后的数据大小
    uint32_t data_size = ros::serialization::serializationLength(command);
    
    // 分配缓冲区
    uint8_t *buffer = (uint8_t*)malloc(data_size);
    
    // 创建序列化流
    ros::serialization::OStream stream(buffer, data_size);
    
    // 序列化
    ros::serialization::serialize(stream, command);
    
    // 将序列化数据保存到data结构体
    data->size = data_size;
    data->buffer = (uint8_t*)malloc(sizeof(uint8_t) * data_size);
    memcpy(data->buffer, buffer, data_size);
    
    // 释放临时缓冲区
    free(buffer);
    
    return 0;
};

int CommandCodec::decode(quadrotor_msgs::PositionCommand &command, struct Data *data){
    // std::cout<<"Decoding command..."<<std::endl;
    uint8_t *buffer = reinterpret_cast<uint8_t*>(data->buffer);
    ros::serialization::IStream stream(buffer, data->size);

    ros::serialization::deserialize(stream, command);
    // std::cout<<"Successfully decoded command!"<<std::endl;
    return 0;
};
