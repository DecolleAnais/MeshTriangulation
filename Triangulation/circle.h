#ifndef CIRCLE_H
#define CIRCLE_H

#include "point3d.h"
#include "vec3.h"

class Circle
{
public:
    Circle();
    Circle(Point3D center, double _radius);
    Circle(Point3D center, Vec3 _radius);

    Point3D center();

    void circumscribed(Point3D a, Point3D b, Point3D c);

protected:
    Point3D _center;
    double _radius;
};

#endif // CIRCLE_H
