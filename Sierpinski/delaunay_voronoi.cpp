#include "delaunay_voronoi.h"
#include "vec3.h"
#include <iostream>

Delaunay_Voronoi::Delaunay_Voronoi(Triangulation* t) : _t(t)
{
    _vertices = QVector<unsigned int>(_t->_nb_faces);
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
        if(!_t->isSensTrigo(a,d,c) &&
           _t->isSensTrigo(a,d,b)) {
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
