#ifndef CRUST_H
#define CRUST_H

#include "triangulation.h"
#include <QTextStream>

class Crust
{
public:
    Crust();
    Crust(Triangulation* t);
    //~Crust();

    void initialize();
    bool isInitialized();
    bool apply(Triangulation* t);
    void draw();

protected:
    Triangulation* _t;
    unsigned int _nb_base_vertices;
    QVector<Edge> _edges;
};

#endif // CRUST_H
