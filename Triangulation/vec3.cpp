#include "vec3.h"
#include <math.h>

Vec3::Vec3() {

}

Vec3::Vec3(const double a=0, const double b=0, const double c=0) : x(a), y(b), z(c) {

}

Vec3::Vec3(const Point3D& p0, const Point3D& p1) {
    x = p1.x - p0.x;
    y = p1.y - p0.y;
    z = p1.z - p0.z;
}

Vec3 Vec3::operator/ (const double d) {
    return Vec3(x / d, y / d, z / d);
}

Vec3 Vec3::operator* (const double d) {
    return Vec3(x * d, y *d, z * d);
}

Vec3 Vec3::cross(const Vec3& v) {
    return Vec3(y*v.z - v.y*z,
                x*v.z - v.x*z,
                x*v.y - v.x*y);
}

double Vec3::dot(const Vec3& v) {
    return x*v.x + y*v.y + z*v.z;
}

double Vec3::cosinus(Vec3& v) {
    return ( this->dot(v) ) / ( this->length() * v.length() );
}

double Vec3::length() {
    return sqrt( x*x + y*y + z*z );
}
