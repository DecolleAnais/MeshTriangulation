#include "iterators.h"
#include <iostream>


/*** Face Iterator ***/

FaceIterator::FaceIterator() : _triangulation(NULL), _id(0) {

}

FaceIterator::FaceIterator(Triangulation * triangulation, unsigned int id) : _triangulation(triangulation), _id(id) {

}

FaceIterator& FaceIterator::operator++() {
    ++_id;
    while(! (_triangulation->_faces[_id]).isVisible()) {
        // si c'est une face virtuelle (liee au sommet virtuelle), on la passe
        ++_id;
    }
    return *this;
}

Face& FaceIterator::operator*() {
    return _triangulation->_faces[_id];
}


bool FaceIterator::operator==(const FaceIterator& fi) {
    return _triangulation == fi._triangulation &&
            _id == fi._id;
}

bool FaceIterator::operator!=(const FaceIterator& fi) {
    return _triangulation != fi._triangulation ||
            _id != fi._id;
}

/*** Vertex Iterator ***/

VertexIterator::VertexIterator() : _triangulation(NULL), _id(1) {

}

VertexIterator::VertexIterator(Triangulation * triangulation, unsigned int id) : _triangulation(triangulation), _id(id) {

}

VertexIterator& VertexIterator::operator++() {
    ++_id;
    if(!_id)    // si c'est le sommet virtuel d'id 0, on le passe
        _id++;
    return *this;
}

Point3D& VertexIterator::operator*() {
    return _triangulation->_vertices[_id];
}

bool VertexIterator::operator==(const VertexIterator& vi) {
    return _triangulation == vi._triangulation &&
            _id == vi._id;
}

bool VertexIterator::operator!=(const VertexIterator& vi) {
    return _triangulation != vi._triangulation ||
            _id != vi._id;
}


/*** Face Circulator ***/

FaceCirculator::FaceCirculator() : _triangulation(NULL), _id_vertex(1), _id_face(0) {

}

FaceCirculator::FaceCirculator(Triangulation * triangulation, unsigned int id_vertex) : _triangulation(triangulation), _id_vertex(id_vertex){
    _id_face = _triangulation->_vertices[_id_vertex].faceId;
}

FaceCirculator& FaceCirculator::operator++() {
    // pour avoir la face suivante, on regarde si _id_vertex == v_0, v_1 ou v_2 du triangle
    // on prend le vertex suivant (sens trigo) et la face opposee est la face suivante
    int idVertexInFace = getVertexIdInFace(_id_face, _id_vertex);
    int idNextFace = _triangulation->_faces[_id_face].f((idVertexInFace + 1)%3);

    while(! (_triangulation->_faces[idNextFace]).isVisible() ) {
        // ignore les faces virtuelles
        idVertexInFace = getVertexIdInFace(idNextFace, _id_vertex);
        idNextFace = _triangulation->_faces[idNextFace].f((idVertexInFace + 1)%3);
    }
    _id_face = idNextFace;

    return *this;
}

Face& FaceCirculator::operator*() {
    return _triangulation->_faces[_id_face];
}

unsigned int FaceCirculator::getNextVertex(const unsigned int & id_face, const unsigned int & id_vertex) {
    return _triangulation->_faces[id_face].v((id_vertex + 1)%3);
}

int FaceCirculator::getVertexIdInFace(const unsigned int & id_face, const unsigned int & id_vertex) {
    if(id_vertex == _triangulation->_faces[id_face].v(0)) {
        return 0;
    } else if(id_vertex == _triangulation->_faces[id_face].v(1)) {
        return 1;
    }else if(id_vertex == _triangulation->_faces[id_face].v(2)){
        return 2;
    }else {
        return -1;
    }
}

bool FaceCirculator::operator==(const FaceCirculator& fc) {
    return _triangulation == fc._triangulation &&
            _id_vertex == fc._id_vertex &&
            _id_face == fc._id_face;
}

bool FaceCirculator::operator!=(const FaceCirculator& fc) {
    return _triangulation != fc._triangulation ||
            _id_vertex != fc._id_vertex ||
            _id_face != fc._id_face;
}

/*** Vertex Circulator ***/

VertexCirculator::VertexCirculator() : _triangulation(NULL), _id_vertex_axis(0),_id_vertex(0), _id_face(0) {

}

VertexCirculator::VertexCirculator(Triangulation * triangulation, unsigned int id_vertex) : _triangulation(triangulation), _id_vertex_axis(id_vertex) {
    // recuperation id face liee a l'axe de rotation (_id_vertex_axis)
    _id_face = _triangulation->_vertices[id_vertex].faceId;
    // recuperation numero sommet dans le triangle
    int idVertexInFace = getVertexIdInFace(_id_face, _id_vertex_axis);
    // recuperation id prochain sommet du triangle
    _id_vertex = getNextVertex(_id_face, idVertexInFace);

}

VertexCirculator& VertexCirculator::operator++() {
    // recuperation face opposee a _id_vertex
    _id_face = _triangulation->_faces[_id_face].f(_id_vertex);
    // recuperation numero sommet (axe de rotation) dans le triangle
    int idVertexInFace = getVertexIdInFace(_id_face, _id_vertex_axis);

    // recuperation id prochain sommet du triangle
    int nextId = getNextVertex(_id_face, idVertexInFace);

    // on passe le sommet virtuel
    if(!nextId) {
        _id_face = _triangulation->_faces[_id_face].f(nextId);
        idVertexInFace = getVertexIdInFace(_id_face, _id_vertex_axis);
        nextId = getNextVertex(_id_face, idVertexInFace);
    }

    _id_vertex = nextId;
    return *this;
}

Point3D& VertexCirculator::operator*() {
    return _triangulation->_vertices[_id_vertex];
}

int VertexCirculator::getVertexIdInFace(const unsigned int & id_face, const unsigned int & id_vertex) {
    if(id_vertex == _triangulation->_faces[id_face].v(0)) {
        return 0;
    } else if(id_vertex == _triangulation->_faces[id_face].v(1)) {
        return 1;
    }else if(id_vertex == _triangulation->_faces[id_face].v(2)){
        return 2;
    }else {
        return -1;
    }
}

unsigned int VertexCirculator::getNextVertex(const unsigned int & id_face, const unsigned int & id_vertex) {
    return _triangulation->_faces[id_face].v((id_vertex + 1)%3);
}

bool VertexCirculator::operator==(const VertexCirculator& vc) {
    return _triangulation == vc._triangulation &&
            _id_vertex_axis == vc._id_vertex_axis &&
            _id_vertex == vc._id_vertex &&
            _id_face == vc._id_face;
}

bool VertexCirculator::operator!=(const VertexCirculator& vc) {
    return _triangulation != vc._triangulation ||
            _id_vertex_axis != vc._id_vertex_axis ||
            _id_vertex != vc._id_vertex ||
            _id_face != vc._id_face;
}
