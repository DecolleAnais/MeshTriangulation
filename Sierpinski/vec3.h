#ifndef VEC3_H
#define VEC3_H

#include "point3d.h"

class Point3D;

class Vec3 {
public:
    /*** Constructors ***/
    Vec3();
    Vec3(const float a, const float b, const float c);
    Vec3(const Point3D& p0, const Point3D& p1);

    /*** Operators overloading ***/
    Vec3 operator/ (const float f);

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
    float dot(const Vec3& v);

    float cosinus(Vec3& v);

    float length();

    /*** Public fields ***/
    float x,y,z;

private:

};

#endif // VEC3_H
