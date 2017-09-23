#include "gldisplay.h"

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

    display_mode = 0;
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glRotatef(_angleX, 0.0f, 1.0f, 0.0f);
    glRotatef(_angleY, 1.0f, 0.0f, 0.0f);

    triangulation.draw();
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
    switch (display_mode) {
    case 0 : display_mode = 1; glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 1 : display_mode = 2; glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    case 2 : display_mode = 0; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    }
    updateGL();
}
