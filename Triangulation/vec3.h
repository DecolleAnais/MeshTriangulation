#ifndef VEC3_H
#define VEC3_H

#include "point3d.h"

class Point3D;

class Vec3 {
public:
    /*** Constructors ***/
    Vec3();
    Vec3(const double a, const double b, const double c);
    Vec3(const Point3D& p0, const Point3D& p1);

    /*** Operators overloading ***/
    Vec3 operator/ (const double d);
    Vec3 operator* (const double d);

    /*** Others ***/

    /**
     * @brief cross
     * @param v Vec3
     * @return vectorial product between my vector and v
     */
    Vec3 cross(const Vec3& v);

    /**
     * @brief dot
     * @param v Vec3
     * @return scalar product between my vector and v
     */
    double dot(const Vec3& v);

    double cosinus(Vec3& v);

    double length();

    /*** Public fields ***/
    double x,y,z;

private:

};

#endif // VEC3_H
