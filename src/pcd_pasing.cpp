#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "pcl_conversions/pcl_conversions.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>

using namespace std::chrono_literals;

void pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr point_cloud_msg) {
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*point_cloud_msg, pcl_pc2);

    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromPCLPointCloud2(pcl_pc2, cloud);

    // Example processing: Apply a pass-through filter to remove points outside a specified range
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud.makeShared());
    pass.setFilterFieldName("z");
    pass.setFilterLimits(0.0, 1.0); // Example range limits
    pass.filter(cloud);

    RCLCPP_INFO(rclcpp::get_logger("pcl_listener"), "Received a PCL message with %lu points", cloud.size());
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("pcl_listener");

    auto point_cloud_sub = node->create_subscription<sensor_msgs::msg::PointCloud2>(
        "/your/pointcloud/topic",  // Replace with the actual PointCloud2 topic
        10,
        pointCloudCallback
    );

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
