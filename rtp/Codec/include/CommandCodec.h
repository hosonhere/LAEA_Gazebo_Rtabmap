#include <ros/ros.h>
#include "quadrotor_msgs/PositionCommand.h"
#include <ros/serialization.h>
#include "Common.h"

class CommandCodec{
    public:
        CommandCodec();
        ~CommandCodec();
    
    public:
        int encode(struct Data *data, quadrotor_msgs::PositionCommand &command);
        int decode(quadrotor_msgs::PositionCommand &command, struct Data *data);
};