#ifndef _VideoCodec_H_
#define _VideoCodec_H_

#include "Common.h"
#include <opencv2/opencv.hpp>

extern "C"{
    #include <libavformat/avformat.h>
    #include <libavutil/rational.h>
    #include <libswscale/swscale.h>
    #include <libavutil/opt.h>
    #include <libavutil/error.h>
    #include <libavutil/imgutils.h> 
}

class VideoCodec{
    public:
        VideoCodec();
        ~VideoCodec();
    
    private:
        int image_width;
        int image_height;
        AVCodecID codec_id; 
        AVMediaType codec_type; 
        AVPixelFormat pix_fmt; 
        AVPixelFormat sws_pix_fmt;
    
    private:    
        AVDictionary *param;
        AVCodec *encoder;
        AVCodec *decoder;
        AVCodecContext *encoder_context;
        AVCodecContext *decoder_context;
        SwsContext *encoder_sws_context;
        SwsContext *decoder_sws_context;
        AVCodecParserContext *parser;

    public:
        int init(
            int _image_width, 
            int _image_height, 
            AVCodecID _codec_id, 
            AVMediaType _codec_type, 
            AVPixelFormat _pix_fmt, 
            AVPixelFormat _sws_pix_fmt
        );

        int encode(cv::Mat &, struct Data *data);
        int decode(struct Data *data, std::vector<cv::Mat *> &decode_images);
};

#endif