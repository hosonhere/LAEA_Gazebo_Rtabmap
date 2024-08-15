#ifndef _PointCloudCodec_H_
#define _PointCloudCodec_H_

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/compression/octree_pointcloud_compression.h>
#include <pcl/octree/octree2buf_base.h>
#include <pcl/octree/impl/octree2buf_base.hpp>

#include "Common.h"

// 點雲壓縮
class PointCloudCodec{
    public:
        PointCloudCodec();
        ~PointCloudCodec();

    public:
        // 可以設定輸出的 profile
        pcl::io::compression_Profiles_e compression_profile = pcl::io::MED_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;
        bool show_statistics = false;
        pcl::io::OctreePointCloudCompression<pcl::PointXYZ> *point_cloud_encoder;
        pcl::io::OctreePointCloudCompression<pcl::PointXYZ> *point_cloud_decoder;

    public:
        // XYZ encode
        int encode(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr &pcloud, struct Data *data);
        int decode(struct Data *data, pcl::PointCloud<pcl::PointXYZ>::Ptr &restore_pcloud);
        
        //XYZRGB encode
        int encode(const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr &pcloud, struct Data *data);
        int decode(struct Data *data, pcl::PointCloud<pcl::PointXYZRGB>::Ptr &restore_pcloud);
};

#endif