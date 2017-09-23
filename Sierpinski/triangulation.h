#ifndef TRIANGULATION_H
#define TRIANGULATION_H


#include <GL/gl.h>
#include <QVector>
#include <QMap>
#include <QPair>
#include "point3d.h"
#include "face.h"
#include "iterators.h"

class FaceIterator;
class VertexIterator;
class FaceCirculator;
class VertexCirculator;

// ids of 2 vertex of the couple (ids dans l'ordre croissant)
typedef QPair<unsigned int, unsigned int> Edge;

class Triangulation
{

public:
    /*** Constructors ***/
    Triangulation();
    Triangulation (const char* file);

    /**
     * @brief draw the triangulation
     */
    void draw();

    /*** Functions for iterators/circulators ***/
    /**
     * @brief beginFace
     * @return a face iterator on the first face
     */
    FaceIterator beginFace();

    /**
     * @brief endFace
     * @return a face iterator on the last face
     */
    FaceIterator endFace();

    /**
     * @brief beginVertex
     * @return a vertex iterator on the first vertex
     */
    VertexIterator beginVertex();

    /**
     * @brief endVertex
     * @return a vertex iterator on the last vertex
     */
    VertexIterator endVertex();

    /**
     * @brief getIncidentFace
     * @param vertex
     * @return an incident face of the vertex
     */
    FaceCirculator getIncidentFace(const Point3D& vertex);

    /**
     * @brief getAdjacentVertex
     * @param vertex
     * @return an adjacent vertex to this vertex
     */
    VertexCirculator getAdjacentVertex(const Point3D& vertex);

    /*** Get functions ***/
    /**
     * @brief getId
     * @param p point
     * @return the id in the _vertices vector of this point p
     */
    unsigned int getId(const Point3D& p);

private:
    /**
     * @brief updateNeighbours
     * @param map this map contains edge's couples and their associated face
     * @param edge the edge to search or add in the map
     * @param idFace id of actual face
     */
    void updateNeighbours(QMap<Edge, unsigned int> & map, Edge edge, unsigned int idFace);

    /**
     * @brief loadTriangulation
     * @param file .off or .pts to load
     */
    void loadTriangulation(const char* file);

    void loadOFF(std::ifstream & ifs);

    void loadPTS(std::ifstream & ifs);

    void createFace(unsigned int a, unsigned int b, unsigned int c, bool visible = true);

    bool isSensTrigo(unsigned int a, unsigned int b, unsigned int c);

    int isInFace(unsigned int d);

    void subdivideFace(unsigned int idFace, unsigned int o);

private:    
    friend class FaceIterator;
    friend class VertexIterator;
    friend class FaceCirculator;
    friend class VertexCirculator;
    /**
     * @brief _faces
     * contains faces of the triangulation and the virtual faces
     * (faces created by the virtual vertex that links border faces)
     */
    QVector<Face> _faces;
    /**
     * @brief _vertices
     * contains vertices of the triangulation and the virtual vertex
     * (the virtual vertex links border faces)
     */
    QVector<Point3D> _vertices;
    /**
     * @brief _nb_vertices
     * number of vertices in the triangulation
     */
    unsigned int _nb_vertices;
    /**
     * @brief _nb_faces
     * number of faces in the triangulation
     */
    unsigned int _nb_faces;

};

#endif // TRIANGULATION_H
