
#include "cartographer/sensor/point_cloud.h"

#include "cartographer/sensor/proto/sensor.pb.h"
#include "cartographer/transform/transform.h"

namespace cartographer {
namespace sensor {


/*. 根据三维网格参数转换点云,返回转换后的新点云结果

*/
PointCloud TransformPointCloud(const PointCloud& point_cloud,
                               const transform::Rigid3f& transform) {
  PointCloud result;                                //3*1f的vector
  result.reserve(point_cloud.size());               //分配内存
  for (const Eigen::Vector3f& point : point_cloud) { 
    result.emplace_back(transform * point);         //C=A*B //A是Rigid3f,B是3*1
  }
  return result;
}


/*剪裁,去掉区域外的点云*/
PointCloud Crop(const PointCloud& point_cloud, const float min_z,
                const float max_z) {
  PointCloud cropped_point_cloud;
  for (const auto& point : point_cloud) {
    if (min_z <= point.z() && point.z() <= max_z) {//只保留在minz~maxz之间的点云
      cropped_point_cloud.push_back(point);
    }
  }
  return cropped_point_cloud;
}

//序列化
proto::PointCloud ToProto(const PointCloud& point_cloud) {
  proto::PointCloud proto;
  for (const auto& point : point_cloud) {
    proto.add_x(point.x());
    proto.add_y(point.y());
    proto.add_z(point.z());
  }
  return proto;
}

PointCloud ToPointCloud(const proto::PointCloud& proto) {
  PointCloud point_cloud;
  const int size = std::min({proto.x_size(), proto.y_size(), proto.z_size()});
  point_cloud.reserve(size);
  for (int i = 0; i != size; ++i) {
    point_cloud.emplace_back(proto.x(i), proto.y(i), proto.z(i));
  }
  return point_cloud;
}

}  // namespace sensor
}  // namespace cartographer
