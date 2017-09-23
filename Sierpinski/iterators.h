#ifndef ITERATORS_H
#define ITERATORS_H

#include "triangulation.h"

class Triangulation;

/*** Face Iterator ***/

class FaceIterator
{
public:
    FaceIterator();
    FaceIterator(Triangulation * triangulation, unsigned int id);
    FaceIterator& operator++();
    Face& operator*();
    bool operator==(const FaceIterator& fi);
    bool operator!=(const FaceIterator& fi);

private:
    Triangulation * _triangulation;
    unsigned int _id;
};


/*** Vertex Iterator ***/

class VertexIterator
{
public:
    VertexIterator();
    VertexIterator(Triangulation * triangulation, unsigned int id);
    VertexIterator& operator++();
    Point3D& operator*();
    bool operator==(const VertexIterator& vi);
    bool operator!=(const VertexIterator& vi);

private:
    Triangulation * _triangulation;
    unsigned int _id;
};


/*** Face Circulator ***/

class FaceCirculator
{
public:
    FaceCirculator();
    FaceCirculator(Triangulation * triangulation, unsigned int id_vertex);
    FaceCirculator& operator++();
    Face& operator*();
    bool operator==(const FaceCirculator& fc);
    bool operator!=(const FaceCirculator& fc);

private:
    int getVertexIdInFace(const unsigned int & id_face, const unsigned int & id_vertex);
    unsigned int getNextVertex(const unsigned int & id_face, const unsigned int & id_vertex);

    Triangulation * _triangulation;
    unsigned int _id_vertex;
    unsigned int _id_face;
};


/*** Vertex Circulator ***/

class VertexCirculator
{
public:
    VertexCirculator();
    VertexCirculator(Triangulation * triangulation, unsigned int id_vertex);
    VertexCirculator& operator++();
    Point3D& operator*();
    bool operator==(const VertexCirculator& vc);
    bool operator!=(const VertexCirculator& vc);

private:
    int getVertexIdInFace(const unsigned int & id_face, const unsigned int & id_vertex);
    unsigned int getNextVertex(const unsigned int & id_face, const unsigned int & id_vertex);

    Triangulation * _triangulation;
    unsigned int _id_vertex_axis;
    unsigned int _id_vertex;
    unsigned int _id_face;
};



#endif // ITERATORS_H
