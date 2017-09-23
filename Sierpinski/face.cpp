#include "face.h"

#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range

Face::Face() {

}

Face::Face(unsigned int v0, unsigned int v1, unsigned int v2) {
    _vertices = {v0, v1, v2};

    _faces = {-1, -1, -1};
}

unsigned int& Face::v(unsigned int i) {
    try {
        return _vertices[i];
    }catch(const std::out_of_range & e) {
        std::cerr << e.what();
    }
}

int& Face::f(unsigned int i) {
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
    return _vertices[0] && _vertices[1] && _vertices[2];
}