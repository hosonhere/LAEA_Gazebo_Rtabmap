#ifndef _RTPSession_H_
#define _RTPSession_H_

#include "Common.h"
#include "Stream.h"

void rtp_frame_recv_callback(void *arg, uvg_rtp::frame::rtp_frame *frame);
void rtcp_recv_callback(uvgrtp::frame::rtcp_receiver_report *frame);
void rtcp_send_callback(uvgrtp::frame::rtcp_sender_report *frame);



class RTPSession{
    public:
        RTPSession();
        ~RTPSession();

    private:
        uvg_rtp::context _context;
        uvg_rtp::session *_session;
    
    // RCE_RTCP flag 為設定是否啟用 RTCP
    // 目前 uvgrtp 在測試時似乎有 bug，RTP 與 RTCP 同時使用時 (設定 RCP_RTCP flag)，
    // 利用 RCE_FRAGEMNT_GENERTIC 接收 frame，第一個 frame 會收不到
    // 單獨使用 RTP (不設定 RCP_RTCP flag) 不會有此情況
    // 相關討論 https://github.com/ultravideo/uvgRTP/issues/67
    private:
        int no_flags = RCE_NO_FLAGS;
        bool enable_RTCP = false;
       
        int bidirection_flags = //RCE_RTCP |
                                  //RCE_SYSTEM_CALL_CLUSTERING |
                                  RCE_FRAGMENT_GENERIC;
                                  
        int send_flags = // RCE_RTCP |                      /* enable RTCP */
                         //RCE_SYSTEM_CALL_CLUSTERING |    /* Enable system call clustering (only Linux) */
                         RCE_SEND_ONLY |                 /* interpret address/port as destination address/port */
                         RCE_FRAGMENT_GENERIC;                  

        int recv_flags = // RCE_RTCP |                      /* enable RTCP */
                         //RCE_SYSTEM_CALL_CLUSTERING |
                         RCE_RECEIVE_ONLY |           /* interpret address/port as binding interface */
                         RCE_FRAGMENT_GENERIC;               
    
    private:
        std::map<int, Stream*> _streams;
    
    public:
        int send_data(int stream_id, struct Data *data);
        struct Data *get_data(int stream_id);

    public:
        int create_session(std::string local_ip, std::string remote_ip);
        int destroy_session();
    
    public:
        int create_stream(
            int id, uint16_t local_port, uint16_t remote_port,
            std::string name, std::string media_type, int payload_type_id,
            std::string direction
        );
        
        void destroy_stream();
    
    public:
        void copy_frame(uvg_rtp::frame::rtp_frame *frame);
        int process_frame();

};

#endif