#include <ros/ros.h>
#include <ros/serialization.h>
#include <nav_msgs/Odometry.h>
#include "Common.h"

class OdomCodec{
    public:
        OdomCodec();
        ~OdomCodec();

    public:
        int encode(nav_msgs::Odometry &odom, struct Data *data);
        int decode(struct Data *data, nav_msgs::Odometry &restore_odom);
};