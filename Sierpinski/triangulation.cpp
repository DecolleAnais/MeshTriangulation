#include "triangulation.h"

#include <iostream>
#include <fstream>  // ifstream, ofstream
#include <cstdlib>  // exit
#include <QtGlobal> // qMin, qMax
#include <QSet>     // set
#include <QQueue>   // queue

Triangulation::Triangulation()
{
    _delaunay_voronoi = NULL;
    _nb_vertices = 0;
    _nb_faces = 0;

    // creation sommet fictif infini
    _vertices.resize(1);
    _vertices[0] = Point3D(0, 0, -1);
}


Triangulation::Triangulation(QTextStream& file) {
    _delaunay_voronoi = NULL;
    _nb_vertices = 0;
    _nb_faces = 0;
    // creation sommet fictif infini
    _vertices.resize(1);
    _vertices[0] = Point3D(0, 0, -1);
    // chargement de la triangulation a partir d'un fichier
    loadTriangulation(file);
}

Triangulation::~Triangulation() {
    if(_delaunay_voronoi != NULL)
        delete(_delaunay_voronoi);
}

void Triangulation::draw(bool display_voronoi_vertices, bool display_voronoi_cells)
{
    // dessine les triangles
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
         for(int i = 0;i < _faces.length();i++) {
             // dessine uniquement les faces visibles de la triangulation (n'utilisant pas le sommet fictif)
             if(_faces[i].isVisible()) {
                 glColor3f(1.0, 1.0, 0.0);
             }else {
                 glColor3f(1.0, 0.0, 0.0);
             }
             glVertex3f(_vertices[_faces[i].v(0)].x, _vertices[_faces[i].v(0)].y, _vertices[_faces[i].v(0)].z);
             glVertex3f(_vertices[_faces[i].v(1)].x, _vertices[_faces[i].v(1)].y, _vertices[_faces[i].v(1)].z);
             glVertex3f(_vertices[_faces[i].v(2)].x, _vertices[_faces[i].v(2)].y, _vertices[_faces[i].v(2)].z);
         }
    glEnd();
    /*
    Point3D p(0.65, 0.35, 0.0);
    //Point3D p(-0.7, 0.3, 0.0);
    _vertices.push_back(p);
    glPointSize(8.0);
    glBegin(GL_POINTS);
        glColor3f(1.0, 0.5, 0.0);
        glVertex3f(p.x, p.y, p.z);
    glEnd();

    std::vector<unsigned int> t = displayVisibilityLocalization(0, _vertices.length()-1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 1.0,0.0);
        for(unsigned int i : t) {
             // dessine uniquement les faces visibles de la triangulation (n'utilisant pas le sommet fictif)
             glVertex3f(_vertices[_faces[i].v(0)].x, _vertices[_faces[i].v(0)].y, _vertices[_faces[i].v(0)].z);
             glVertex3f(_vertices[_faces[i].v(1)].x, _vertices[_faces[i].v(1)].y, _vertices[_faces[i].v(1)].z);
             glVertex3f(_vertices[_faces[i].v(2)].x, _vertices[_faces[i].v(2)].y, _vertices[_faces[i].v(2)].z);
        }
    glEnd();
    */

    if(_delaunay_voronoi != NULL) {
        _delaunay_voronoi->updateVertices();
        if(display_voronoi_cells)
            _delaunay_voronoi->drawVoronoi(1.0, 1.0, 1.0);
        if(display_voronoi_vertices)
            _delaunay_voronoi->drawVertices(0.0, 0.0, 1.0, 8.0);

    }
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

unsigned int Triangulation::getId(const Face& f) {
    return _faces.indexOf(f);
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
        unsigned int idOpposedFace = _faces[idFace].getIdOpposedFace(edge.first(), edge.second());
        // maj du voisin pour la face actuelle, on connait le sommet oppose
        _faces[idFace].f(idOpposedFace) = neighbour;

        // face opposee du voisin
        idOpposedFace = _faces[neighbour].getIdOpposedFace(edge.first(), edge.second());
        // maj du voisin
        _faces[neighbour].f(idOpposedFace) = idFace;

    }else {
        map.insert(edge, idFace);
    }
}


void Triangulation::loadTriangulation(QTextStream& file) {
    // on verifie les 3 premiers caracteres pour connaitre le type de fichier (OFF ou PTS)
    char TYPE[3];
    file >> TYPE;

    if(strcmp(TYPE, "OFF") == 0) {
        // fichier OFF (liste de points et de triangles)
        loadOFF(file);
    }else if(strcmp(TYPE, "PTS") == 0) {
        // fichier PTS (liste de points)
        loadPTS(file);
    }
}


void Triangulation::loadOFF(QTextStream& ts) {
    // map pour associer les ids des faces voisines (qui possedent le meme couple de sommets)
    QMap< Edge, unsigned int> map;
    Edge edge;

    double v_x, v_y, v_z;
    unsigned int tmp, id_v0, id_v1, id_v2;

    // nb sommets
    ts >> _nb_vertices;
    _vertices.resize(_nb_vertices+1); // +1 pour le sommet fictif
    // nb faces
    ts >> _nb_faces;
    _faces.resize(_nb_faces);
    // nb aretes
    ts >> tmp;

    // lecture coords sommets
    for(unsigned int i = 1;i <= _nb_vertices;++i) { // debut a 1 pour laisser le sommet fictif en position 0
        ts >> v_x;
        ts >> v_y;
        ts >> v_z;
        _vertices[i] = Point3D(v_x, v_y, v_z);
    }

    // lecture indices faces
    for(unsigned int i = 0;i < _nb_faces;++i) {
        // nb indices
        ts >> tmp;

        // lecture indices
        ts >> id_v0;
        ts >> id_v1;
        ts >> id_v2;

        // +1 aux indices de sommets car le sommet fictif occupe la position 0
        id_v0 += 1;
        id_v1 += 1;
        id_v2 += 1;

        _faces[i] = Face(id_v0, id_v1, id_v2);

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
        edge = Edge(this, id_v0, id_v1);
        updateNeighbours(map, edge, i);
        edge = Edge(this, id_v1, id_v2);
        updateNeighbours(map, edge, i);
        edge = Edge(this, id_v0, id_v2);
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
            // creation de la face fictive
            _faces.push_back(Face(a, 0, b));

            // maj du voisin fictif
            unsigned int lastId = _faces.length()-1;
            _faces[i].f(id) = lastId; // maj voisin pour la face reelle
            _faces[lastId].f(1) = i; // maj voisin pour la face fictive

            // maj des voisins entre faces fictives
            edge = Edge(this, 0, a);
            updateNeighbours(map, edge, lastId);
            edge = Edge(this, 0, b);
            updateNeighbours(map, edge, lastId);
        }
    }
}

void Triangulation::loadPTS(QTextStream& ts) {
    // nb sommets
    ts >> _nb_vertices;
    _vertices.resize(_nb_vertices + 1); // + 1 pour le sommet fictif
    double v_x, v_y, v_z;

    // lecture coords sommets
    for(unsigned int i = 1;i <= _nb_vertices;++i) {  // debut a 1 pour laisser le sommet fictif en position 0
        ts >> v_x;
        ts >> v_y;
        ts >> v_z;
        processPoint( i, Point3D(v_x, v_y, v_z) );
    }
}

void Triangulation::processPoint(unsigned int i, Point3D p) {
    _vertices[i] = p;

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
        // liaison du sommet virtuel à une face virtuelle
        _vertices[0].faceId = 3;
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
        int idFace = visibilityLocalization(0, i);
        if(idFace != -1) {
            // CAS 1 :  le nouveau sommet est dans un triangle existant
            // separation du triangle en 3
            subdivideFace(idFace, i);
        }else {
            // CAS 2 : le nouveau sommet est a l'exterieur des triangles existants
            addExternVertex(i);
        }
        // delaunay iteratif
        if(_delaunay_voronoi != NULL) {
            delete(_delaunay_voronoi);
            _delaunay_voronoi = NULL;
        }
        _delaunay_voronoi = new Delaunay_Voronoi(this);
        _delaunay_voronoi->delaunay(getOpposedEdges(i));
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

bool Triangulation::isSensTrigo(Point3D& a, Point3D& b, Point3D& c) {
    // construction vecteurs ab et ac
    Vec3 ab(a, b);
    Vec3 ac(a, c);
    // si la composante z du produit vectoriel ab, ac est negative
    // alors le triangle est dans le sens trigo
    return (ab.cross(ac).z > 0);
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

    // triangle obc (ex triangle original)    
    _faces[idFace].f(1) = _faces.size() - 1;
    _faces[idFace].f(2) = _faces.size() - 2;

    // utilisation de la fonction de voisinage
    unsigned int idFaceABO = _faces.size() - 2;
    unsigned int idFaceAOC = _faces.size() - 1;
    unsigned int idFaceOpposedB = _faces[idFaceAOC].f(1);
    unsigned int idFaceOpposedC = _faces[idFaceABO].f(2);
    unsigned int b = _faces[idFaceABO].v(1);
    unsigned int c = _faces[idFaceAOC].v(2);

    updateNeighbour(idFaceOpposedB, Edge(this, a, c), idFaceAOC);
    updateNeighbour(idFaceOpposedC, Edge(this, a, b), idFaceABO);
}

void Triangulation::addExternVertex(unsigned int i) {
    // parcours des faces virtuelles (contour du maillage)
    // verification pour chaque face virtuelle, si l'arete du contour est visible depuis le nouveau point (test de sens trigo)
    FaceCirculator fc = FaceCirculator(this, 0);    // circulateur de faces autour du sommet infini (contour)
    FaceCirculator fc_begin = fc;
    std::vector<unsigned int> near_faces;   // contient les ids des faces proches, contours visible pour le nouveau point

    bool wait = true;   // variable qui attend de tomber sur un contour non proche
    bool trigo;         // test trigo pour savoir si le point est visible depuis un contour
    // boucle sur tout le contour
    do{
        Face f = *fc;
        // verifie si le point est visible depuis ce contour
        trigo = isSensTrigo(i, f.v(2), f.v(0));

        // si on n'est plus en phase de recherche du premier contour non proche
        if( !wait && trigo) {
            // ajoute la face aux faces proches a traiter
            near_faces.emplace_back(getId(f));
        }
        // si un contour non proche a ete trouve pour la premiere fois
        if(wait && !trigo) {
            // on arrete de chercher
            wait = false;
            // actualisation de fc_begin pour ne faire qu'une boucle
            fc_begin = fc;
        }
        ++fc;
    }while(wait || (!wait && fc != fc_begin) );

    // maj face id du nouveau sommet
    _vertices[i].faceId = near_faces[0];

    // traiter les faces trouvees
    for(unsigned int id : near_faces) {
        // remplace le sommet infini par le nouveau point dans la face virtuelle
        _faces[id].v(1) = i;
    }
    // creer 2 nouvelles faces vituelles, une pour chaque extremite des faces a traiter
    unsigned int first_face = near_faces[0];
    unsigned int last_face = near_faces[near_faces.size() - 1];

    // face virtuelle a la 1ere face
    unsigned int a = i;
    unsigned int b = 0;
    unsigned int c = _faces[first_face].v(2);
    Face fa = Face(a, b, c);
    fa.f(0) = _faces[first_face].f(0);
    fa.f(1) = first_face;
    fa.f(2) = _faces.size() + 1;
    _faces.push_back(fa);
    _nb_faces++;
    // maj faceId sommet virtuel
    _vertices[0].faceId = _faces.size() - 1;

    // face virtuelle a la derniere face
    a = _faces[last_face].v(0);
    b = 0;
    c = i;
    fa = Face(a, b, c);
    fa.f(0) = _faces.size() - 1;
    fa.f(1) = last_face;
    fa.f(2) = _faces[last_face].f(2);
    _faces.push_back(fa);

    // maj voisins virtuels
    unsigned int left = _faces[first_face].f(0);
    unsigned int right = _faces[last_face].f(2);
    _faces[left].f(2) =  _faces.size() - 2;
    _faces[right].f(0) =  _faces.size() - 1;

    // maj voisins des faces de chaque extremite avec les nouvelles faces virtuelles
    _faces[first_face].f(0) = _faces.size() - 2;
    _faces[last_face].f(2) = _faces.size() - 1;
}

void Triangulation::flipEdge(unsigned int idFace, unsigned int idOpposedVertex) {
    int idInFace = getIdInFace(idFace, idOpposedVertex);
    if(idInFace != -1) {
        unsigned int idFace2 = _faces[idFace].f(idInFace);
        unsigned int i0, i1, j1, i2, j2;

        // recuperation ids sommets a modifier
        i0 = _faces[idFace].v((idInFace + 2)%3);
        unsigned int idInFace2 = getIdInFace(idFace2, i0);

        // attribution des nouveaux sommets
        i1 = idOpposedVertex;
        j1 = _faces[idFace2].v((idInFace2 + 2)%3);
        _faces[idFace].v((idInFace + 2)%3) = j1; // i0 -> j1
        _faces[idFace2].v((idInFace2 + 1)%3) = i1; // j0 -> i1

        // maj faceId des sommets i2 et j2
        i2 = _faces[idFace].v((idInFace + 1)%3);
        j2 = _faces[idFace2].v(idInFace2);
        _vertices[i2].faceId = idFace;
        _vertices[j2].faceId = idFace2;

        // maj des voisins des 2 faces flippees
        unsigned int id = getIdInFace(idFace, i1);
        _faces[idFace].f(id) = _faces[idFace2].f(getIdInFace(idFace2, j2));
        id = getIdInFace(idFace2, j1);
        _faces[idFace2].f(id) = _faces[idFace].f(getIdInFace(idFace, i2));
        id = getIdInFace(idFace, i2);
        _faces[idFace].f(id) = idFace2;
        id = getIdInFace(idFace2, j2);
        _faces[idFace2].f(id) = idFace;
        // maj des faces voisines aux faces flippees
        unsigned int idF = _faces[idFace].f( getIdInFace(idFace, i1) );
        id = getIdInFace(idF, i2);
        _faces[idF].f( (id + 1)%3) = idFace;
        idF = _faces[idFace2].f( getIdInFace(idFace2, j1) );
        id = getIdInFace(idF, j2);
        _faces[idF].f( (id + 1)%3) = idFace2;
    }
}


unsigned int Triangulation::getIdInFace(const unsigned int idFace, const unsigned int idVertex) {
    if(_faces[idFace].v(0) == idVertex)
        return 0;
    else if(_faces[idFace].v(1) == idVertex)
        return 1;
    else if(_faces[idFace].v(2) == idVertex)
        return 2;
    else
        return -1;
}


QQueue<Edge> Triangulation::getEdgesInside() {
    QQueue<Edge> edges_inside;

    // parcours des faces
    for(int i = 0;i < _faces.size();i++) {
        // parcours des sommets de la face
        for(unsigned int j = 0;j <= 2;j++) {
            unsigned int id_vertex_0 = _faces[i].v(j);
            unsigned int id_vertex_1 = _faces[i].v( (j+1)%3 );
            // si l'arete ne contient pas le sommet virtuel
            if( id_vertex_0 != 0 && id_vertex_1 != 0 ) {
                // creation arete
                Edge e = Edge(this, id_vertex_0, id_vertex_1);
                // si l'arete n'est pas un contour
                if( !e.isContour() && !edges_inside.contains(e) ) {
                    // ajout de l'arete aux aretes interieures
                    edges_inside.enqueue(e);
                }
            }
        }
    }
    return edges_inside;
}

Edge Triangulation::getEdge(unsigned int idFace, unsigned int idOpposedVertex) {
    Face f = _faces[idFace];
    unsigned int id = getIdInFace(idFace, idOpposedVertex);
    unsigned int a = f.v((id+1)%3);
    unsigned int b = f.v((id+2)%3);
    Edge e(this, a, b);
    return e;
}

QQueue<Edge> Triangulation::getOpposedEdges(unsigned int idVertex) {
    QQueue<Edge> opposedEdges;
    // on cherche les faces reelles qui contiennent ce sommet
    for(int i = 0;i < _faces.length();i++) {
        if(_faces[i].isVisible() && _faces[i].containsVertex(idVertex)) {
            // ajout de l'arete opposee au sommet dans cette face
            Edge e = getEdge(i, idVertex);
            opposedEdges.enqueue(e);
        }
    }
    return opposedEdges;
}

void Triangulation::updateNeighbour(unsigned int idFace, Edge edge, unsigned int idNeighbour) {
    unsigned int idOpposedVertex = _faces[idFace].getIdOpposedVertex(edge.first(), edge.second());
    idOpposedVertex = getIdInFace(idFace, idOpposedVertex);
    _faces[idFace].f(idOpposedVertex) = idNeighbour;
}

std::vector<unsigned int> Triangulation::displayVisibilityLocalization(unsigned int f, unsigned int p) {
    std::vector<unsigned int> walk;
    unsigned int currentFace = f;
    bool continueWalk = true;
    // tant qu'on peut continuer la marche
    // on se deplace de face en face vers le point
    while(continueWalk) {
        walk.push_back(currentFace);
        // si on trouve une face  virtuelle,
        // le point est a l'exterieur du maillage existant
        // et est localise dans cette face virtuelle
        if(!_faces[currentFace].isVisible()) {
            break;
        }
        continueWalk = false;
        for(unsigned int i = 0;i <= 2;i++) {
            unsigned int a = _faces[currentFace].v(i);
            unsigned int b = _faces[currentFace].v( (i+1)%3 );
            unsigned int c = p;
            // le point voit une arete de la face
            if(isSensTrigo(a, c, b)) {
                // deplacement sur la face suivante en se rapprochant du point
                currentFace = _faces[currentFace].f( (i+2)%3 );
                continueWalk = true;
                break;
            }
        }
    }
    return walk;
}

int Triangulation::visibilityLocalization(int f, unsigned int p) {
    int currentFace = f;
    bool continueWalk = true;
    // tant qu'on peut continuer la marche
    // on se deplace de face en face vers le point
    while(continueWalk) {
        // si on trouve une face  virtuelle,
        // le point est a l'exterieur du maillage existant
        // et est localise dans cette face virtuelle
        if(!_faces[currentFace].isVisible()) {
            return -1;
        }
        continueWalk = false;
        for(unsigned int i = 0;i <= 2;i++) {
            unsigned int a = _faces[currentFace].v(i);
            unsigned int b = _faces[currentFace].v( (i+1)%3 );
            unsigned int c = p;
            // le point voit une arete de la face
            if(isSensTrigo(a, c, b)) {
                // deplacement sur la face suivante en se rapprochant du point
                currentFace = _faces[currentFace].f( (i+2)%3 );
                continueWalk = true;
                break;
            }
        }
    }
    return currentFace;
}
