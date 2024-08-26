#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <ros/serialization.h>
#include "Common.h"

class MarkerCodec{
    public:
        MarkerCodec();
        ~MarkerCodec();
    
    public:
        int encode(visualization_msgs::Marker &marker, struct Data *data);
        int decode(struct Data *data, visualization_msgs::Marker &restore_marker);
};