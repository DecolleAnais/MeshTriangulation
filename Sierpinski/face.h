#ifndef FACE_H
#define FACE_H

#include <array>
#include <vector>

class Face {

public:
    /*** Constructors ***/
    Face();
    Face(unsigned int v0, unsigned int v1, unsigned int v2);

    bool operator==(Face const& face);

    /*** Get functions ***/
    /**
     * @brief v
     * @param i id
     * @return id of the vertex number i
     */
    unsigned int& v(const unsigned int i);

    /**
     * @brief f
     * @param i id
     * @return id of the face number i
     */
    int& f(const unsigned int i);

    /**
     * @brief vertices
     * @return an array of 3 vertices
     */
    std::array<unsigned int, 3> vertices();

    /**
     * @brief getOpposedFaceId
     * @param v0 id of the first vertex of the edge
     * @param v1 id of the second vertex of the edge
     * @return the opposed face's id of this edge
     */
    unsigned int getIdOpposedFace(const unsigned int v0, const unsigned int v1);

    int getIdOpposedVertex(const unsigned int v0, const unsigned int v1);

    /*** Others ***/

    std::vector<unsigned int> getIdBorderFaces();
    bool isVisible();
    bool containsVertex(unsigned int v);

    // No Point saved in this structure, todo in mesh
    /*bool isTrigo();
    Point3D getCenterCirconscriptedCircle();
    bool isInside(Point3D p);*/


private:
    std::array<unsigned int, 3> _vertices; // vector of vertices ids
    std::array<int, 3> _faces; // vector of neighbours faces ids
};

#endif // FACE_H
