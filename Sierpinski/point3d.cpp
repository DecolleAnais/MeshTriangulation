#include "point3d.h"

Point3D::Point3D() : x(0), y(0), z(0)
{

}

Point3D::Point3D(const float a = 0, const float b = 0, const float c = 0) : x(a), y(b), z(c), faceId(-1){

}

bool Point3D::operator==(Point3D const& p) {
    return x == p.x && y == p.y && z == p.z;
}

Vec3 Point3D::operator- (const Point3D& p) {
    return Vec3(x - p.x, y - p.y, z - p.z);
}

Point3D Point3D::operator+ (const Vec3& v) {
    return Point3D(x + v.x, y + v.y, z + v.z);
}

std::ostream& operator<<(std::ostream& o, const Point3D& p) {
    o<<"p("<<p.x<<","<<p.y<<","<<p.z<<")";
    return o;
}

