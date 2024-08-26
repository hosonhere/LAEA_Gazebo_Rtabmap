#include <ros/ros.h>
#include <ros/serialization.h>
#include <geometry_msgs/PoseStamped.h>
#include "Common.h"

class PoseCodec{
    public:
        PoseCodec();
        ~PoseCodec();

    public:
        int encode(geometry_msgs::PoseStamped &pose, struct Data *data);
        int decode(struct Data *data, geometry_msgs::PoseStamped &restore_pose);
};