#include "circle.h"

Circle::Circle() : _center(Point3D(0.0,0.0,0.0)), _radius(0.0)
{}

Circle::Circle(Point3D center, float radius) : _center(center), _radius(radius)
{}

Circle::Circle(Point3D center, Vec3 radius) : _center(center), _radius(radius.length())
{}

Point3D Circle::center() {
    return _center;
}

void Circle::circumscribed(Point3D a, Point3D b, Point3D c) {
    float d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    float x = (1/d) * ( (a.x*a.x + a.y*a.y) * (b.y - c.y) +
                         (b.x*b.x + b.y*b.y) * (c.y - a.y) +
                         (c.x*c.x + c.y*c.y) * (a.y - b.y) );
    float y = (1/d) * ( (a.x*a.x + a.y*a.y) * (c.x - b.x) +
                         (b.x*b.x + b.y*b.y) * (a.x - c.x) +
                         (c.x*c.x + c.y*c.y) * (b.x - a.x) );
    _center = Point3D(x, y, 0.0);
    _radius = (a - _center).length();
}
