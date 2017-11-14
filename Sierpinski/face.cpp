#include "face.h"

#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range

Face::Face() {

}

Face::Face(unsigned int v0, unsigned int v1, unsigned int v2) {
    _vertices = {v0, v1, v2};

    _faces = {-1, -1, -1};
}

bool Face::operator==(Face const& face) {
    if(_vertices[0] == face._vertices[0] &&
       _vertices[1] == face._vertices[1] &&
       _vertices[2] == face._vertices[2] &&
       _faces[0] == _faces[0] &&
       _faces[1] == _faces[1] &&
       _faces[2] == _faces[2])
        return true;
    else
        return false;
}

unsigned int& Face::v(const unsigned int i){
    try {
        return _vertices[i];
    }catch(const std::out_of_range & e) {
        std::cerr << e.what();
    }
}

int& Face::f(const unsigned int i){
    try {
        return _faces[i];
    }catch(const std::out_of_range & e) {
        std::cerr << e.what();
    }
}

std::array<unsigned int, 3> Face::vertices() {
    return _vertices;
}

unsigned int Face::getIdOpposedFace(const unsigned int v0, const unsigned int v1) {
    if(_vertices[0] != v0 && _vertices[0] != v1) {
        return 0;
    }else if (_vertices[1] != v0 && _vertices[1] != v1) {
        return 1;
    }else {
        return 2;
    }
}

int Face::getIdOpposedVertex(const unsigned int a, const unsigned int b) {
    if(containsVertex(a) && containsVertex(b)) {
        if(_vertices[0] != a && _vertices[0] != b)
            return _vertices[0];
        if(_vertices[1] != a && _vertices[1] != b)
            return _vertices[1];
        if(_vertices[2] != a && _vertices[2] != b)
            return _vertices[2];
    }
    std::cerr << "The edge " << a << " " << b <<
                 " isn't in the face " <<
                 _vertices[0] << _vertices[1] << _vertices[2] <<
                 ". No opposed vertex found." << std::endl;
    return -1;
}

std::vector<unsigned int> Face::getIdBorderFaces() {
    std::vector<unsigned int> idBorderFaces;
    if(_faces[0] == -1)
        idBorderFaces.emplace_back(0);
    if(_faces[1] == -1)
        idBorderFaces.emplace_back(1);
    if(_faces[2] == -1)
        idBorderFaces.emplace_back(2);
    return idBorderFaces;
}

bool Face::isVisible() {
    // si aucun sommet ne possede l'id 0, alors la face est visible
    return _vertices[0] != 0 && _vertices[1] != 0 && _vertices[2] != 0;
}

bool Face::containsVertex(unsigned int v) {
    if(_vertices[0] == v || _vertices[1] == v || _vertices[2] == v)
        return true;
    return false;
}

