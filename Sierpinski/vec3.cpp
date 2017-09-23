#include "vec3.h"

Vec3::Vec3() {

}

Vec3::Vec3(const float a=0, const float b=0, const float c=0) : x(a), y(b), z(c) {

}

Vec3::Vec3(const Point3D& p0, const Point3D& p1) {
    x = p1.x - p0.x;
    y = p1.y - p0.y;
    z = p1.z - p0.z;
}

Vec3 Vec3::operator/ (const float f) {
    return Vec3(x / f, y / f, z / f);
}

Vec3 Vec3::cross(const Vec3& v) {
    return Vec3(y*v.z - v.y*z,
                x*v.z - v.x*z,
                x*v.y - v.x*y);
}

float Vec3::dot(const Vec3& v) {
    return x*v.x + y*v.y + z*v.z;
}
