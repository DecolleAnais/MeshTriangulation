#ifndef CRUST_H
#define CRUST_H

#include "triangulation.h"
#include <QTextStream>

class Crust
{
public:
    Crust();
    Crust(const char* file);
    Crust(QTextStream& file, bool doApply);
    ~Crust();

    void initialize();
    bool isInitialized();
    bool apply();
    void draw(bool display_voronoi_vertices, bool display_voronoi_cells);

protected:
    Triangulation* _t;
    unsigned int _nb_base_vertices;
    QVector<Edge> _edges;
};

#endif // CRUST_H
