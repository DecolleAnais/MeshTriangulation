#ifndef EDGE_H
#define EDGE_H

#include <QQueue>     // queue
#include "triangulation.h"

class Triangulation;

class Edge
{
public:
    Edge();

    Edge(Triangulation* t, unsigned int a, unsigned int b);

    //bool operator==(const Edge& e) const;
    //bool equals(const Edge& e) const;
    bool operator<(const Edge& e) const;
    //uint qHash(const Edge& e);
    //uint qHash(const Edge &e, uint seed);

    unsigned int first() const;

    unsigned int second() const;

    void setFaceIds();

    int* getFaceIds();

    int getFaceId(unsigned int i) const;

    bool isContour();

    QQueue<Edge> getNeighboringEdgesInside();

protected:
    Triangulation* _triangulation;
    unsigned int _first;
    unsigned int _second;
    int _faceIds [2];
};

inline bool operator==(const Edge& e1, const Edge& e2) {
    if( (e1.first() == e2.first() && e1.second() == e2.second()) ||
        (e1.first() == e2.second() && e1.second() == e2.first())) {
        return true;
    }
    return false;
}

inline uint qHash(const Edge& e) {
    return e.first() + 3*e.second() + 5*e.getFaceId(0) + 10*e.getFaceId(1);
}

inline uint qHash(const Edge& e, uint seed) {
    return qHash(e) * seed;
}

#endif // EDGE_H
