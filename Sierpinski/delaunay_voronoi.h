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
    Delaunay_Voronoi();

    Delaunay_Voronoi(Triangulation* t);

    void delaunay();

    void delaunay(QQueue<Edge> toProcess);

    void updateVertices(Triangulation* t);

    QVector<Point3D> vertices();

    Point3D vertice(int i);

    void drawVoronoi(GLfloat r, GLfloat g, GLfloat b);

    void drawVertices(GLfloat r, GLfloat g, GLfloat b, GLfloat size);

protected:
    Triangulation* _t;
    QVector<Point3D> _vertices;
};

#endif // DELAUNAY_VORONOI_H
