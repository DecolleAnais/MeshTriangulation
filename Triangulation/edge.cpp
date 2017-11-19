#include "edge.h"
#include <QtGlobal> // qMin, qMax

Edge::Edge() {}

Edge::Edge(Triangulation* t, unsigned int a, unsigned int b) : _triangulation(t)
{
    _first = qMin(a, b);
    _second = qMax(a, b);
    setFaceIds();
}

bool Edge::operator<(const Edge& e) const {
    if(_first < e.first() ||
       _second < e.second()) {
        return true;
    }
    return false;
}

unsigned int Edge::first() const {
    return _first;
}

unsigned int Edge::second() const {
    return _second;
}

void Edge::setFaceIds() {
    unsigned int count = 0;
    int i = 0;
    // tant qu'on n'a pas trouve les 2 faces liees a l'arete
    // et qu'on n'a pas depasse le tableau de faces
    while(count < 2 && i < _triangulation->_faces.size()) {
        // si la face contient les 2 sommets de l'arete
        if(_triangulation->_faces[i].containsVertex(_first)
           && _triangulation->_faces[i].containsVertex(_second)) {
            // on ajoute l'id de la face
            _faceIds[count] = i;
            count++;
        }
        i++;
    }

}

int* Edge::getFaceIds() {
    setFaceIds();
    return _faceIds;
}

int Edge::getFaceId(unsigned int i) const {
    if(i == 0)
        return _faceIds[0];
    if(i == 1)
        return _faceIds[1];
    return -1;
}

bool Edge::isContour() {
    // on verifie que les 2 sommets opposes a l'arete
    // ne sont pas les sommets virtuels
    setFaceIds();
    // pour chaque face
    // recuperation des ids des sommets opposes a l'arete
    int a = _triangulation->_faces[ _faceIds[0] ].getIdOpposedVertex(_first, _second);
    int b = _triangulation->_faces[ _faceIds[1] ].getIdOpposedVertex(_first, _second);
    if(a != 0 && b != 0) {
        return false;
    }else {
        return true;
    }
}

QQueue<Edge> Edge::getNeighboringEdgesInside() {
    QQueue<Edge> neighboringEdges;
    // recuperation des 2 faces liees a l'arete
    setFaceIds();
    // pour chacune des 2 faces
    for(unsigned int i = 0;i <= 1;i++) {
        // id de la face traitee
        unsigned int faceId = _faceIds[i];
        // parcours des sommets de la face
        for(unsigned int j = 0;j <= 2;j++) {
            unsigned int id_vertex_0 = _triangulation->_faces[faceId].v(j);
            unsigned int id_vertex_1 = _triangulation->_faces[faceId].v( (j+1)%3 );
            // si l'arete n'est pas l'arete courante
            if( _first != qMin(id_vertex_0, id_vertex_1)  ||
                _second != qMax(id_vertex_0, id_vertex_1) ) {
                // creation arete
                Edge e = Edge(_triangulation, id_vertex_0, id_vertex_1);
                // verifie que l'arete n'est pas un contour
                if( !e.isContour() && !neighboringEdges.contains(e) ) {
                    // ajout aux aretes voisines interieures
                    neighboringEdges.enqueue(e);
                }
            }
        }
    }
    /*std::cout << "aretes voisines de " << edge.first << edge.second << " a traiter : " << std::endl;
    for(Edge e : neighboringEdges) {
        std::cout << e.first << e.second << " ";
    }
    std::cout << std::endl;*/
    return neighboringEdges;
}
