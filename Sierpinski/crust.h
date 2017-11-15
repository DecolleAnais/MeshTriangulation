#ifndef CRUST_H
#define CRUST_H

#include "triangulation.h"

class Crust
{
public:
    Crust();
    Crust(const char* file);
    ~Crust();

    void draw();

protected:
    Triangulation* _t;
    unsigned int _nb_base_vertices;
    QVector<Edge> _edges;
};

#endif // CRUST_H
