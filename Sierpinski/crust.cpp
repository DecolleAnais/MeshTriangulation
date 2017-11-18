#include "crust.h"
#include <iostream>

Crust::Crust()
{
    _t = NULL;
}

Crust::~Crust() {
    //if(_t != NULL)
      //  delete(_t);
}

Crust::Crust(QTextStream& file, bool doApply) {
    // triangulation de delaunay avec les points originaux
    _t = new Triangulation(file);
    _nb_base_vertices = _t->_nb_vertices;
    if(doApply)
        apply();
}

void initialize() {

}

bool Crust::isInitialized() {
    return _t != NULL;
}

bool Crust::apply() {
    // recuperation des sommets de voronoi
    if(_t->_delaunay_voronoi != NULL) {
        _t->_delaunay_voronoi->updateVertices();
        QVector<Point3D> voronoi_vertices = _t->_delaunay_voronoi->vertices();
        // ajout des sommets de voronoi a la triangulation
        unsigned int start =  _t->_nb_vertices + 1;
        unsigned int end = start + voronoi_vertices.length();
        unsigned int j = 0;
        _t->_vertices.resize(end);
        for(unsigned int i = start; i < end;i++) {
            if(_t->_faces[j].isVisible()) {
                _t->processPoint(i, voronoi_vertices[j]);
            }
            j++;
        }
        // on garde seulement les aretes
        // dont les sommets sont des points de base
        _edges = QVector<Edge>();
        // on parcourt les faces
        for(int i = 0;i < _t->_faces.length();i++) {
            // on parcourt les 3 aretes de chaque face
            unsigned int a = _t->_faces[i].v(0);
            unsigned int b = _t->_faces[i].v(1);
            unsigned int c = _t->_faces[i].v(2);
            Edge ab(_t, a, b);
            Edge bc(_t, b, c);
            Edge ca(_t, c, a);
            if(a != 0 && a <= _nb_base_vertices &&
               b != 0 && b <= _nb_base_vertices &&
               !_edges.contains(ab)) {
                _edges.push_back(ab);
            }
            if(b != 0 && b <= _nb_base_vertices &&
               c != 0 && c <= _nb_base_vertices &&
               !_edges.contains(bc)) {
                _edges.push_back(bc);
            }
            if(c != 0 && c <= _nb_base_vertices &&
               a != 0 && a <= _nb_base_vertices &&
               !_edges.contains(ca)) {
                _edges.push_back(ca);
            }
        }
    }
    return true;
}

void Crust::draw(bool display_voronoi_vertices, bool display_voronoi_cells) {
    // dessine la triangulation
    _t->draw(display_voronoi_vertices, display_voronoi_cells);

    if(!_edges.empty()) {
        // dessine les aretes conservees
        glLineWidth(1.0);
        glColor3f(1.0, 0.0, 1.0);
        glBegin(GL_LINES);
            for(Edge e : _edges) {
                Point3D p1 = _t->_vertices[e.first()];
                Point3D p2 = _t->_vertices[e.second()];
                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
            }
        glEnd();
        // dessine les cellules et les sommets de voronoi si souhaite
        _t->_delaunay_voronoi->updateVertices();
        if(display_voronoi_cells)
            _t->_delaunay_voronoi->drawVoronoi(1.0, 1.0, 1.0);
        if(display_voronoi_vertices)
            _t->_delaunay_voronoi->drawVertices(0.0, 0.0, 1.0, 8.0);
    }
}
