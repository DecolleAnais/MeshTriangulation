#ifndef POINT3D_H
#define POINT3D_H

#include "vec3.h"
#include <ostream>

class Vec3;

class Point3D{
public:
    /*** Constructors ***/

    Point3D();
    Point3D(const float a, const float b, const float c);


    /*** Operators overloading ***/

    bool operator==(Point3D const& p);
    Vec3 operator- (const Point3D& p);
    Point3D operator+ (const Vec3& v);
    friend std::ostream& operator<<(std::ostream& o, const Point3D& p);

    /*** Others ***/

    /*** Public fields ***/

    float x, y, z;
    int faceId;

private:

};

#endif // POINT3D_H
