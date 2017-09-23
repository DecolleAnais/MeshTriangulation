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

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    void drawSierpinski();
    Triangulation triangulation;
    unsigned int display_mode;

    float _angleX;
    float _angleY;
    QPoint _position;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
