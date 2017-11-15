#include "delaunay_voronoi.h"
#include "vec3.h"
#include "circle.h"
#include <iostream>

Delaunay_Voronoi::Delaunay_Voronoi() { _t = NULL; }

Delaunay_Voronoi::Delaunay_Voronoi(Triangulation* t) : _t(t)
{
}

void Delaunay_Voronoi::delaunay() {
    // aretes a traiter : aretes a l'interieur
    QQueue<Edge> toProcess = _t->getEdgesInside();
    delaunay(toProcess);

}

void Delaunay_Voronoi::delaunay(QQueue<Edge> toProcess) {
    //std::cout << "aretes a traiter " << toProcess.size() << std::endl;
    bool flip;

    // tant que la liste des aretes a traiter n'est pas vide
    while(!toProcess.empty()) {
        Edge e = toProcess.head();

        flip = false;
        // recuperation des 2 faces a tester,
        // de chaque cote de l'arete bc
        unsigned int b = e.first();
        unsigned int c = e.second();
        e.setFaceIds();
        int f0 = e.getFaceId(0);
        int f1 = e.getFaceId(1);
        // pour chaque face
        // recuperation des ids des sommets opposes a l'arete
        int a = _t->_faces[ f0 ].getIdOpposedVertex(e.first(), e.second());
        int d = _t->_faces[ f1 ].getIdOpposedVertex(e.first(), e.second());
        //std::cout << " a=" << a << " b=" << b <<" c=" << c << " d=" << d << std::endl;
        // si ce n'est pas une forme convexe
        if( !e.isContour() &&
            ( ( !_t->isSensTrigo(a,d,c) && _t->isSensTrigo(a,d,b) ) ||
              ( _t->isSensTrigo(a,d,c) && !_t->isSensTrigo(a,d,b) ) ) ){
            Vec3 v_ba(_t->_vertices[a] - _t->_vertices[b]);
            Vec3 v_bc(_t->_vertices[c] - _t->_vertices[b]);
            Vec3 v_da(_t->_vertices[a] - _t->_vertices[d]);
            Vec3 v_dc(_t->_vertices[c] - _t->_vertices[d]);
            Vec3 v_cd(_t->_vertices[d] - _t->_vertices[c]);
            Vec3 v_cb(_t->_vertices[b] - _t->_vertices[c]);
            Vec3 v_ad(_t->_vertices[d] - _t->_vertices[a]);
            Vec3 v_ab(_t->_vertices[b] - _t->_vertices[a]);
            //std::cout << v_ba.cosinus(v_bc) << std::endl;
            //std::cout << v_da.cosinus(v_dc) << std::endl;
            if( v_ba.cosinus(v_bc) > v_da.cosinus(v_dc) ) {
                // si l'angle abc < adc,
                // alors le triangle abc n'est pas localement de Delaunay
                //std::cout << a << b << c << " < " << a << d << c << std::endl;
                // flip de l'arete bc -> ad
                _t->flipEdge(f0, a);
                flip = true;

            }else if( v_cd.cosinus(v_cb) > v_ad.cosinus(v_ab) ) {
                // si l'angle dcb < dab,
                // alors le triangle bcd n'est pas localement de Delaunay
                //std::cout << d << c << b << " < " << d << a << b << std::endl;
                // flip de l'arete bc -> ad
                _t->flipEdge(f1, d);
                flip = true;
            }
            // s'il y a eu un flip, maj des aretes a traiter
            if(flip) {
                // recuperation des aretes voisines
                Edge flipped_edge(_t, a, d);
                // ajout des aretes voisines au flip a traiter
                QQueue<Edge> neighboring_edges = flipped_edge.getNeighboringEdgesInside();
                while(!neighboring_edges.isEmpty()) {
                    Edge n = neighboring_edges.head();
                    if( !toProcess.contains( n ) ) {
                        toProcess.enqueue(n);
                    }
                    neighboring_edges.dequeue();
                }
                //toProcess += flipped_edge.getNeighboringEdgesInside();
            }
        }
        // on supprime l'arete qui a ete traitee de la liste
        toProcess.dequeue();

        /*std::cout << "Edge : " << e.first << " " << e.second << " Faces : " << faceIds[0] << " " << faceIds[1] <<
        " Opposed Vertices : " << id_v0 << " " << id_v1 << std::endl;*/
    }
}

void Delaunay_Voronoi::updateVertices() {
    _vertices = QVector<Point3D>();
    for(int i = 0;i < _t->_faces.length();i++) {
        Point3D a = _t->_vertices[_t->_faces[i].v(0)];
        Point3D b = _t->_vertices[_t->_faces[i].v(1)];
        Point3D c = _t->_vertices[_t->_faces[i].v(2)];
        /*if(!_t->_faces[i].isVisible()) {
            if(_t->_faces[i].v(0) == 0) {
                a = b + (b-c)/2 +;
            }else if(_t->_faces[i].v(1) == 0) {
                b = a + (a-c)/2;
            }else if(_t->_faces[i].v(2) == 0) {
                c = a + (a-b)/2;
            }
        }*/
        Circle circle;
        circle.circumscribed(a, b, c);
        _vertices.push_back(circle.center());
    }
}

QVector<Point3D> Delaunay_Voronoi::vertices() {
    return _vertices;
}

Point3D Delaunay_Voronoi::vertice(int i) {
    return _vertices[i];
}

void Delaunay_Voronoi::drawVoronoi(GLfloat r, GLfloat g, GLfloat b) {
    // dessine les cellules de Voronoi
    glBegin(GL_LINES);
        glColor3f(r, g, b);
        for(int i = 0;i < _t->_faces.length();i++) {
            if( _t->_faces[i].isVisible() ){
                for(int j = 0;j <= 2;j++) {
                    int f = _t->_faces[i].f(j);
                    Point3D p1 = vertice(i);
                    Point3D p2;
                    if(!_t->_faces[f].isVisible()) {
                        unsigned int v1 = _t->_faces[i].v( (j+1)%3 );
                        unsigned int v2 = _t->_faces[i].v( (j+2)%3 );
                        // milieu de l'arete contour
                        Point3D m = _t->_vertices[v1] + (_t->_vertices[v2] - _t->_vertices[v1]) / 2;
                        Point3D out = p1 + (m-p1)*100;
                        if( _t->isSensTrigo(out, _t->_vertices[v2], _t->_vertices[v1]) )
                            p2 = p1 + (m-p1)*100;
                        else
                            p2 = p1 - (m-p1)*100;
                    }else {
                        p2 = vertice(f);
                    }
                    glVertex3f(p1.x, p1.y, p1.z);
                    glVertex3f(p2.x, p2.y, p2.z);
                }
            }
        }
    glEnd();
}

void Delaunay_Voronoi::drawVertices(GLfloat r, GLfloat g, GLfloat b, GLfloat size) {
    // dessine les centres des cercles circonscrits
    glPointSize(size);
    glBegin(GL_POINTS);
        glColor3f(r, g, b);
        for(int i = 0;i < _t->_faces.length();i++) {
            if(_t->_faces[i].isVisible()) {
                Point3D p = vertice(i);
                glVertex3f(p.x, p.y, p.z);
            }
        }
    glEnd();
}
