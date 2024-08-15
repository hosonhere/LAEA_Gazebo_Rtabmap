#include "Stream.h"

int Stream::send_data(struct Data *data){
    int ret = media_stream->push_frame(data->buffer, data->size, RTP_NO_FLAGS);
    if (ret != RTP_OK){
        std::cout << "RTP push_frame transmit failed" << std::endl;
        return -1;
    }
    return 0;
};

struct Data* Stream::get_data(){
    struct Data *data = stream_buffer.get();
    return data;
}
