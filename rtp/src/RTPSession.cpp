#include "RTPSession.h"
#include <atomic>

// uvgrtp 有提供 polling api 可以自行 implement RTP RTCP frame 的接收

// 此處為使用 callback
// uvgrtp callbacks for processing RTP frame RTCP frame 
// uvgrtp callbacks
void rtp_frame_recv_callback(void* arg, uvg_rtp::frame::rtp_frame *frame){
    //std::cout << "Received RTP frame. Payload size: " << frame->payload_len << std::endl;
    RTPSession *session = (RTPSession*) (arg);
    session->copy_frame(frame);
    
    (void) uvgrtp::frame::dealloc_frame(frame);
};

void RTPSession::copy_frame(uvg_rtp::frame::rtp_frame *frame){
    // std::cout << "Payload Type: " << (int)(frame->header).payload<< std::endl;
    size_t size = frame->payload_len;
    int payload_type_id = (int)(frame->header).payload;
    
    
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    // stream 過多不確定會不會執行太慢
    for (int i = 0; i < _streams.size(); i++){
        if (_streams[i]->payload_type_id == payload_type_id){          

            uint8_t* buffer = (uint8_t*) malloc(sizeof(uint8_t) * size);
            memcpy(buffer, frame->payload, size);
            struct Data *data = (struct Data *)malloc(sizeof(struct Data));
            data->buffer = buffer;
            data->size = size;
            
            (_streams[i]->stream_buffer).put(data);
            return;
        }
    }
    std::cout << "unknown payload type: " << payload_type_id << ", discard frame" << std::endl;
};

int RTPSession::process_frame(){
    return 0;
};

void rtcp_recv_callback(uvgrtp::frame::rtcp_receiver_report *frame){
    std::cout << "-------- RTCP Receiver Report -------- "  << std::endl;
    for (auto& block : frame->report_blocks)
    {
        std::cout << "ssrc: "       << block.ssrc     << std::endl;
        std::cout << "fraction: "   << block.fraction << std::endl;
        std::cout << "lost: "       << block.lost     << std::endl;
        std::cout << "last_seq: "   << block.last_seq << std::endl;
        std::cout << "jitter: "     << block.jitter   << std::endl;
        std::cout << "lsr: "        << block.lsr      << std::endl;
        std::cout << "dlsr (jiffies): "  << uvgrtp::clock::jiffies_to_ms(block.dlsr)
                  << std::endl << std::endl;
    }
    std::cout << "-------- RTCP Receiver Report --------" << std::endl;
    delete frame;  
};

void rtcp_send_callback(uvgrtp::frame::rtcp_sender_report *frame){
    std::cout << "-------- RTCP sender report -------- " << std::endl;
    std::cout << "NTP msw: "        << frame->sender_info.ntp_msw   << std::endl;
    std::cout << "NTP lsw: "        << frame->sender_info.ntp_lsw   << std::endl;
    std::cout << "RTP timestamp: "  << frame->sender_info.rtp_ts    << std::endl;
    std::cout << "packet count: "   << frame->sender_info.pkt_cnt   << std::endl;
    std::cout << "byte count: "     << frame->sender_info.byte_cnt  << std::endl;

    for (auto& block : frame->report_blocks)
    {
        std::cout << "ssrc: "       << block.ssrc     << std::endl;
        std::cout << "fraction: "   << block.fraction << std::endl;
        std::cout << "lost: "       << block.lost     << std::endl;
        std::cout << "last_seq: "   << block.last_seq << std::endl;
        std::cout << "jitter: "     << block.jitter   << std::endl;
        std::cout << "lsr: "        << block.lsr      << std::endl;
        std::cout << "dlsr (jiffies): "  << uvgrtp::clock::jiffies_to_ms(block.dlsr)
                  << std::endl << std::endl;
    }
    std::cout << "-------- RTCP sender report --------"       << std::endl;
    delete frame;
};




RTPSession::RTPSession(){};
RTPSession::~RTPSession(){};

// Session Function
int RTPSession::create_session(std::string local_ip, std::string remote_ip){
    std::pair<std::string, std::string> bind_addresses(local_ip, remote_ip);
    std::cout << "Create Session!" << std::endl;
    _session = _context.create_session(bind_addresses);
    
    return _session ? 0 : -1;
};

int RTPSession::destroy_session(){
    for (std::map<int, Stream*>::iterator it = _streams.begin(); it!=_streams.end(); it++){
        _session->destroy_stream(((*it).second)->media_stream);
        std::cout << "Destroy Stream id: " << (*it).first << std::endl;
    }

    _context.destroy_session(_session);
    std::cout << "Destroy Session!" << std::endl;
    return 0;
};

// Stream Function
int RTPSession::create_stream(
        int id, uint16_t local_port, uint16_t remote_port, 
        std::string name, std::string media_type, int payload_type_id,
        std::string direction
    )
{   
    if(_session == NULL){
        std::cout << "Session not initialized!" << std::endl;
        return -1;
    }
    uvgrtp::media_stream *stream = NULL;
    if(direction.compare("recvonly") == 0){
        stream = _session->create_stream(local_port, remote_port, RTP_FORMAT_GENERIC, recv_flags);
        if (!stream || 
            //(stream->get_rtcp()->install_sender_hook(rtcp_send_callback)) != RTP_OK ||
            (stream->install_receive_hook(this, rtp_frame_recv_callback)) != RTP_OK
        )
        {
            std::cout << "Create Stream Failed: " << name << std::endl;
            return -1;
        }
        int BUFFER_SIZE_MB = 400 * 1000 * 1000;
        stream->configure_ctx(RCC_UDP_RCV_BUF_SIZE, BUFFER_SIZE_MB);
        //int MAX_PACKET_INTERVAL_MS = 2000;
        //stream->configure_ctx(RCC_PKT_MAX_DELAY, MAX_PACKET_INTERVAL_MS);
    }else if(direction.compare("sendonly") == 0){
        stream = _session->create_stream(local_port, remote_port, RTP_FORMAT_GENERIC, send_flags);
        if (!stream  //||
            //(stream->get_rtcp()->install_receiver_hook(rtcp_recv_callback)) != RTP_OK
        )
        {
            std::cout << "Create Stream Failed: " << name << std::endl;
            return -1;
        }
        int BUFFER_SIZE_MB = 400 * 1000 * 1000;
        stream->configure_ctx(RCC_UDP_SND_BUF_SIZE, BUFFER_SIZE_MB);

    }else if(direction.compare("sendrecv") == 0){
        stream = _session->create_stream(local_port, remote_port, RTP_FORMAT_GENERIC, bidirection_flags);
        if (!stream || 
            //(stream->get_rtcp()->install_sender_hook(rtcp_send_callback)) != RTP_OK
            (stream->install_receive_hook(this, rtp_frame_recv_callback)) != RTP_OK 
        )
        {
            std::cout << "Create Stream Failed: " << name << std::endl;
            return -1;
        }
        
    }else{
        std::cout << "Not supported stream direction type: " << direction << std::endl;
        return -1;
    }
    
    // set the payload type of rtp stream
    stream->configure_ctx(RCC_DYN_PAYLOAD_TYPE, payload_type_id);
    stream->configure_ctx(RCC_MTU_SIZE, 1300);

    Stream *_stream = new Stream(
        id, local_port, remote_port,
        name, media_type, payload_type_id,
        direction
    );
    
    
    _stream->media_stream = stream;
    _streams.insert(std::pair<int, Stream*>(id, _stream));

    return 0;
};

void RTPSession::destroy_stream(){
    
}

// get and send data api
int RTPSession::send_data(int stream_id, struct Data *data){
    return _streams[stream_id]->send_data(data);
};

struct Data* RTPSession::get_data(int stream_id){
    return _streams[stream_id]->get_data();
};
