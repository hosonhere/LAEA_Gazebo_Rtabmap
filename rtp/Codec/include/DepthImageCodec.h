#ifndef _DepthImageCodec_H_
#define _DepthImageCodec_H_

#include <opencv2/opencv.hpp>
#include "zdepth.hpp"
#include "Common.h"

// 深度圖壓縮
class DepthImageCodec{
    public:
        DepthImageCodec();
        DepthImageCodec(int _width, int _height);
        ~DepthImageCodec();
    
    public:
        int width;
        int height;

    public:
        zdepth::DepthCompressor compressor;
        zdepth::DepthCompressor decompressor;
    
    public:
        int encode(cv::Mat &depth_image, struct Data *data);
        int decode(struct Data *data, cv::Mat &restore_depth_image);
};

#endif