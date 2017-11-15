#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <triangulation.h>
#include <QMouseEvent>

class GLDisplay : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLDisplay(QWidget *parent = 0);

    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL(int w, int h);

    void changeDisplayMode();

    void setDisplayVoronoiCells(bool b);

    void setDisplayVoronoiVertices(bool b);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    void drawSierpinski();
    Triangulation _triangulation;
    unsigned int _display_mode;
    bool _display_voronoi_vertices;
    bool _display_voronoi_cells;

    float _angleX;
    float _angleY;
    QPoint _position;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
