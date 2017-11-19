#ifndef GASKET_H
#define GASKET_H

struct point {
    float x, y, z;
};

struct triangle {
    point p1, p2, p3;
};

class Gasket
{
public:
    Gasket();

    void draw();
};

#endif // GASKET_H
