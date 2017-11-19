#include "gldisplay.h"
#include <iostream>

#define FRUSTUM_SIZE 1.0f

GLDisplay::GLDisplay(QWidget *parent) :
    QGLWidget(parent),
    _angleX(0.0f),
    _angleY(0.0f)
{
}

void GLDisplay::initializeGL()
{

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0, 1.0, 0.0);

    _display_mode = 1; glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    _display_voronoi_cells = false;
    _display_voronoi_vertices = false;
    _display_triangulation = true;
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glRotatef(_angleX, 0.0f, 1.0f, 0.0f);
    glRotatef(_angleY, 1.0f, 0.0f, 0.0f);

    if(_triangulation.isInitialized()) {
        _triangulation.draw(_display_voronoi_vertices, _display_voronoi_cells, _display_triangulation);
    }

    if(_crust.isInitialized()) {
        _crust.draw();
    }
}

void GLDisplay::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glViewport(0, 0, w, h);

    glOrtho(-FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE);

    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint position = event->pos();

        _angleX += (position.x() - _position.x());
        _angleY += (position.y() - _position.y());

        _position = position;

        updateGL();
    }
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _position = event->pos();
}

void GLDisplay::changeDisplayMode() {
    switch (_display_mode) {
    case 0 : _display_mode = 1; glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 1 : _display_mode = 2; glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    case 2 : _display_mode = 0; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    }
    updateGL();
}

void GLDisplay::setDisplayVoronoiCells(bool b) {
    _display_voronoi_cells = b;
    updateGL();
}

void GLDisplay::setDisplayVoronoiVertices(bool b) {
    _display_voronoi_vertices = b;
    updateGL();
}

void GLDisplay::setDisplayTriangulation(bool b) {
    _display_triangulation = b;
    updateGL();
}

void GLDisplay::loadFile(QTextStream& file) {
    _crust.initialize();
    _triangulation = Triangulation(file);
    updateGL();
}

void GLDisplay::applyCrust() {
    _crust.apply(&_triangulation);
    updateGL();
}

void GLDisplay::saveAs(QTextStream& out) {
    _triangulation.savePTS(out);
}

void GLDisplay::reset() {
    _triangulation = Triangulation();
    _crust.initialize();
    updateGL();
}

void GLDisplay::lawson() {
    _triangulation.lawson();
    updateGL();
}
