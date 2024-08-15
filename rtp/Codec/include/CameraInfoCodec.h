#include <ros/ros.h>
#include <sensor_msgs/CameraInfo.h>
#include <ros/serialization.h>
#include "Common.h"

class CameraInfoCodec{
    public:
        CameraInfoCodec();
        CameraInfoCodec(int _width, int _height);
        ~CameraInfoCodec();
    
    public:
        int width;
        int height;

    public:
        int encode(sensor_msgs::CameraInfo &camera_info, struct Data *data);
        int decode(struct Data *data, sensor_msgs::CameraInfo &restore_camera_info);
};