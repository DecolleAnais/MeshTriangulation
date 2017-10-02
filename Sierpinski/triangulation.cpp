#include "triangulation.h"

#include <iostream>
#include <fstream> //ifstream, ofstream
#include <cstdlib> //exit
#include <QtGlobal> // qMin, qMax





Triangulation::Triangulation()
{
    _nb_vertices = 0;
    _nb_faces = 0;

    // creation sommet fictif infini
    _vertices.resize(1);
    _vertices[0] = Point3D(0, 0, -1);

    // chargement de la triangulation a partir d'un fichier

    //loadTriangulation("/home/ad/Documents/M2/GAM/queen.off");
    //loadTriangulation("/home/ad/Documents/M2/GAM/carre.off");
    //loadTriangulation("/home/ad/Documents/M2/GAM/cube.off");
    //loadTriangulation("/home/ad/Documents/M2/GAM/carre.pts");
    loadTriangulation("/home/ad/Documents/M2/GAM/test.pts");

}

Triangulation::Triangulation (const char* file) {
    _nb_vertices = 0;
    _nb_faces = 0;
    // creation sommet fictif infini
    _vertices.resize(1);
    _vertices[0] = Point3D(0, 0, -1);
    // chargement de la triangulation a partir d'un fichier
    loadTriangulation(file);
}

void Triangulation::draw()
{
    glBegin(GL_TRIANGLES);

         for(int i = 0;i < _faces.length();i++) {
             // dessine uniquement les faces visibles de la triangulation (n'utilisant pas le sommet fictif)
             if(_faces[i].isVisible()) {
                 glVertex3f(_vertices[_faces[i].v(0)].x, _vertices[_faces[i].v(0)].y, _vertices[_faces[i].v(0)].z);
                 glVertex3f(_vertices[_faces[i].v(1)].x, _vertices[_faces[i].v(1)].y, _vertices[_faces[i].v(1)].z);
                 glVertex3f(_vertices[_faces[i].v(2)].x, _vertices[_faces[i].v(2)].y, _vertices[_faces[i].v(2)].z);
             }
         }

    glEnd();

}


FaceIterator Triangulation::beginFace() {
    FaceIterator it(this, 0);
    return it;
}

FaceIterator Triangulation::endFace() {
    FaceIterator it(this, _nb_faces - 1);
    return it;
}

VertexIterator Triangulation::beginVertex() {
    VertexIterator it(this, 1);     // commence a 1 car 0 est le sommet virtuel
    return it;
}

VertexIterator Triangulation::endVertex() {
    VertexIterator it(this, _nb_vertices + 1);
    return it;
}

FaceCirculator Triangulation::getIncidentFace(const Point3D& vertex) {
    FaceCirculator fc(this, getId(vertex));
    return fc;
}

VertexCirculator Triangulation::getAdjacentVertex(const Point3D& vertex) {
    VertexCirculator vc(this, getId(vertex));
    return vc;
}

unsigned int Triangulation::getId(const Point3D& p) {
    return _vertices.indexOf(p);
}

void Triangulation::updateNeighbours(QMap<Edge, unsigned int> & map, Edge edge, unsigned int idFace) {

    // on verifie si l'arete est dans la map
    // si non -> on l'ajoute et on lui associe l'id de la face
    // si oui -> on associe les faces ensemble
    // la face a associer est en face du sommet non concerne par l'arete commune

    if(map.contains(edge)) {
        // recuperation voisin
        unsigned int neighbour = map.value(edge);

        // face opposee de la face actuelle
        unsigned int idOpposedFace = _faces[idFace].getIdOpposedFace(edge.first, edge.second);
        // maj du voisin pour la face actuelle, on connait le sommet oppose
        _faces[idFace].f(idOpposedFace) = neighbour;

        // face opposee du voisin
        idOpposedFace = _faces[neighbour].getIdOpposedFace(edge.first, edge.second);
        // maj du voisin
        _faces[neighbour].f(idOpposedFace) = idFace;

    }else {
        map.insert(edge, idFace);
    }
}


void Triangulation::loadTriangulation(const char* file) {
    // lecture fichier OFF
    std::ifstream ifs;
    ifs.open(file);
    if(ifs.bad())
      {std::cout<<"Impossible d'ouvrir le fichier "<<file<<" en lecture \n"; exit(1);}
    // on verifie les 3 premiers caracteres pour connaitre le type de fichier (OFF ou PTS)
    char TYPE[3];
    ifs >> TYPE;

    if(strcmp(TYPE, "OFF") == 0) {
        // fichier OFF (liste de points et de triangles)
        loadOFF(ifs);
    }else if(strcmp(TYPE, "PTS") == 0) {
        // fichier PTS (liste de points)
        loadPTS(ifs);
    }

    ifs.close();
}


void Triangulation::loadOFF(std::ifstream & ifs) {
    // map pour associer les ids des faces voisines (qui possedent le meme couple de sommets)
    QMap< Edge, unsigned int> map;
    Edge edge;

    double v_x, v_y, v_z;
    unsigned int tmp, id_v0, id_v1, id_v2;

    // nb sommets
    ifs >> _nb_vertices;
    _vertices.resize(_nb_vertices+1); // +1 pour le sommet fictif
    // nb faces
    ifs >> _nb_faces;
    _faces.resize(_nb_faces);
    // nb aretes
    ifs >> tmp;

    // lecture coords sommets
    for(unsigned int i = 1;i <= _nb_vertices;++i) { // debut a 1 pour laisser le sommet fictif en position 0
        ifs >> v_x;
        ifs >> v_y;
        ifs >> v_z;
        _vertices[i] = Point3D(v_x, v_y, v_z);
    }

    // lecture indices faces
    for(unsigned int i = 0;i < _nb_faces;++i) {
        // nb indices
        ifs >> tmp;

        // lecture indices
        ifs >> id_v0;
        ifs >> id_v1;
        ifs >> id_v2;

        // +1 aux indices de sommets car le sommet fictif occupe la position 0
        id_v0 += 1;
        id_v1 += 1;
        id_v2 += 1;

        _faces[i] = Face(id_v0, id_v1, id_v2);
        //createFace(id_v0, id_v1, id_v2);

        // association de la face aux sommets s'ils ne sont pas deja lies a une face
        for(unsigned int v_id : _faces[i].vertices()) {
           if(_vertices[v_id].faceId == -1) {
               _vertices[v_id].faceId = i;
           }
        }

        // mise a jour des faces voisines
        // ajout des couples de sommets a la map
        // 3 couple d'indices de sommets
        // l'indice min en premier
        // fonction d'association des faces updateNeighbours()
        edge = { qMin(id_v0, id_v1), qMax(id_v0, id_v1) };
        updateNeighbours(map, edge, i);
        edge = { qMin(id_v1, id_v2), qMax(id_v1, id_v2) };
        updateNeighbours(map, edge, i);
        edge = { qMin(id_v0, id_v2), qMax(id_v0, id_v2) };
        updateNeighbours(map, edge, i);
    }

    // connexion des bordures avec le sommet ficitf
    for(unsigned int i = 0;i < _nb_faces;++i) {
        //Face* face = &_faces[i];
        std::vector<unsigned int> idBorderFaces = _faces[i].getIdBorderFaces();

        for(unsigned int id : idBorderFaces) {
            // pour chaque face en bordure, on cree une face avec le sommet fictif
            unsigned int a = _faces[i].v( (id+1)%3 );
            unsigned int b = _faces[i].v( (id+2)%3 );
            // creation de la face fictive (v0 = sommet fictif, v1 = b, v2 = a, car l'arete ab est inversee dans la face voisine
            // false pour ne pas incrementer _nb_faces qui represente le nb de faces reelles
            createFace(a, 0, b, false);

            // maj du voisin fictif
            unsigned int lastId = _faces.length()-1;
            _faces[i].f(id) = lastId; // maj voisin pour la face reelle (id de la face fictive venant d'etre creee)
            _faces[lastId].f(1) = i; // maj voisin pour la face fictive (f1 car c'est la face opposee au sommet fictif v1)

            // maj des voisins entre faces fictives
            edge = { 0, a };
            updateNeighbours(map, edge, lastId);
            edge = { 0, b };
            updateNeighbours(map, edge, lastId);
        }
    }

}

void Triangulation::loadPTS(std::ifstream & ifs) {
    // nb sommets
    ifs >> _nb_vertices;
    _vertices.resize(_nb_vertices + 1); // + 1 pour le sommet fictif
    double v_x, v_y, v_z;

    // lecture coords sommets
    for(unsigned int i = 1;i <= _nb_vertices;++i) {  // debut a 1 pour laisser le sommet fictif en position 0
        ifs >> v_x;
        ifs >> v_y;
        ifs >> v_z;
        _vertices[i] = Point3D(v_x, v_y, v_z);

        // POINT INVISIBLE POUR LES BORDS

        if(i == 3) {
            // si on est au troisieme sommet, on cree le premier triangle
            // en testant le sens trigo selon l'ordre des sommets
            createFace(1, 2, 3);
            // association de la face aux sommets
            _vertices[1].faceId = 0;
            _vertices[2].faceId = 0;
            _vertices[3].faceId = 0;
            // creation faces virtuelles
            _faces.push_back( Face(_faces[0].v(0), 0, _faces[0].v(1)) );
            _faces.push_back( Face(_faces[0].v(1), 0, _faces[0].v(2)) );
            _faces.push_back( Face(_faces[0].v(2), 0, _faces[0].v(0)) );
            // liaison des voisins
            _faces[0].f(0) = 2;
            _faces[0].f(1) = 3;
            _faces[0].f(2) = 1;

            _faces[1].f(0) = 2;
            _faces[1].f(1) = 0;
            _faces[1].f(2) = 3;

            _faces[2].f(0) = 3;
            _faces[2].f(1) = 0;
            _faces[2].f(2) = 1;

            _faces[3].f(0) = 1;
            _faces[3].f(1) = 0;
            _faces[3].f(2) = 2;

        }else if(i > 3) {
            // si on a deja cree notre premier triangle
            // on ajoute le nouveau sommet par rapport aux triangles existants

            int idFace = isInFace(i);
            if(idFace != -1) {
                // CAS 1 :  le nouveau sommet est dans un triangle existant
                // separation du triangle en 3
                subdivideFace(idFace, i);

            }else {
                // CAS 2 : le nouveau sommet est a l'exterieur des triangles existants
            }


        }
    }

}

void Triangulation::createFace(unsigned int a, unsigned int b, unsigned int c, bool visible) {
    if(isSensTrigo(a, b, c)) {
        // si les sommets a, b, c sont dans l'ordre trigo, on cree la face de sommets a, b, c
       _faces.push_back(Face(a, b, c));
    }else {
        // sinon on cree la face de sommets c, b, a
        _faces.push_back(Face(c, b, a));
    }
    if(visible)
        _nb_faces++;
}

bool Triangulation::isSensTrigo(unsigned int a, unsigned int b, unsigned int c) {
    // construction vecteurs ab et ac
    Vec3 ab(_vertices[a], _vertices[b]);
    Vec3 ac(_vertices[a], _vertices[c]);
    // si la composante z du produit vectoriel ab, ac est negative
    // alors le triangle est dans le sens trigo
    if(ab.cross(ac).z > 0) {
        return true;
    }else {
        return false;
    }
}

int Triangulation::isInFace(unsigned int d) {
    unsigned int i = 0;
    // parcours des faces
    for(Face face : _faces) {
        // si les triangles abd, bcd et cad sont dans le sens trigo
        // alors le point d est dans le triangle
        if(isSensTrigo(face.v(0), face.v(1), d) &&
                isSensTrigo(face.v(1), face.v(2), d) &&
                isSensTrigo(face.v(2), face.v(0), d)) {
            // on retourne l'indice de la face qui contient le sommet
            return i;
        }
        i++;
    }
    // on retourne -1 si aucune face ne contient le point d
    return -1;
}

void Triangulation::subdivideFace(unsigned int idFace, unsigned int o) {

    // association des faces aux sommets
    _vertices[o].faceId = idFace;
    _vertices[_faces[idFace].v(0)].faceId = _faces.size();

    // modification du triangle original abc
    // sauvegarde du sommet a de la face
    // sommet a -> o pour faire le triangle obc
    unsigned int a = _faces[idFace].v(0);
    _faces[idFace].v(0) = o;

    // creation de 2 nouveaux triangles
    // triangle abo
    createFace(a, _faces[idFace].v(1), o);
    // triangle aoc
    createFace(a, o, _faces[idFace].v(2));



    // maj des voisins

    // triangle aoc
    _faces[_faces.size() - 1].f(0)  = idFace;
    _faces[_faces.size() - 1].f(1)  = _faces[idFace].f(1);
    _faces[_faces.size() - 1].f(2)  = _faces.size() - 2;

    // triangle abo
    _faces[_faces.size() - 2].f(0)  = idFace;
    _faces[_faces.size() - 2].f(1)  = _faces.size() - 1;
    _faces[_faces.size() - 2].f(2)  = _faces[idFace].f(2);

    // maj voisins du triangle original abc
    unsigned int id = _faces[idFace].f(1);
    _faces[id].f(1) = _faces.size() - 1;
    id = _faces[idFace].f(2);
    _faces[id].f(1) = _faces.size() - 2;

    // triangle obc (ex triangle original)    
    _faces[idFace].f(1) = _faces.size() - 1;
    _faces[idFace].f(2) = _faces.size() - 2;

}








