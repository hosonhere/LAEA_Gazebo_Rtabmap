#ifndef _Stream_H_
#define _Stream_H_

#include "Common.h"
#include "SharedQueue.h"

class Stream{
    public:
        Stream(
            int _id, uint16_t _local_port, uint16_t _remote_port,
            std::string _name, std::string _media_type, int _payload_type_id,
            std::string _direction){
            
            local_port = _local_port;
            remote_port = _remote_port;
            name = _name;
            id = _id;
            media_type = _media_type;
            payload_type_id = _payload_type_id;
            direction = _direction;
        };
        ~Stream(){};
    
    public:
        int id;
        uint16_t local_port;
        uint16_t remote_port;
        std::string name;
        std::string media_type;
        int payload_type_id;
        std::string direction;

    public:
        uvg_rtp::media_stream *media_stream;
        SharedQueue<struct Data *> stream_buffer;

    public:
        
        int send_data(struct Data *data);
        struct Data * get_data();
    
};

#endif