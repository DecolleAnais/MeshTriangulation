#ifndef DELAUNAY_VORONOI_H
#define DELAUNAY_VORONOI_H

#include <QVector>
#include <QQueue>
#include "triangulation.h"
#include "edge.h"

class Triangulation;
class Edge;

class Delaunay_Voronoi
{
public:
    Delaunay_Voronoi(Triangulation* t);

    void delaunay();

    void delaunay(QQueue<Edge> toProcess);

    void updateVertices();

protected:
    Triangulation* _t;
    QVector<unsigned int> _vertices;
};

#endif // DELAUNAY_VORONOI_H
