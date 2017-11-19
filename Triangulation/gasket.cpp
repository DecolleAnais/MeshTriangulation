#include "gasket.h"
#include <GL/gl.h>
#include <cstdlib>
#include <time.h>

#define CONTROL_VERTICES 4
#define SUBDIVISIONS 3

point colors[] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0}};

Gasket::Gasket()
{
}

void triangle(GLfloat *va,GLfloat *vb,GLfloat *vc)
{
        glVertex3fv(va);
        glVertex3fv(vb);
        glVertex3fv(vc);
}

void tetra(GLfloat *a,GLfloat *b,GLfloat *c,GLfloat *d)
{
    glColor3fv(&colors[0].x);
    triangle( a, b, c);
    glColor3fv(&colors[1].x);
    triangle( a, c, d);
    glColor3fv(&colors[2].x);
    triangle( a, d, b);
    glColor3fv(&colors[3].x);
    triangle( b, d, c);
}

void divide_tetra(GLfloat *a,GLfloat *b,GLfloat *c,GLfloat *d,int m)
{
    GLfloat mid[6][3];
    int j;
    if(m>0)
    {    /*compute six midpoints*/
        for(j=0;j<3;j++) mid[0][j]=(a[j]+b[j])/2;
        for(j=0;j<3;j++) mid[1][j]=(a[j]+c[j])/2;
        for(j=0;j<3;j++) mid[2][j]=(a[j]+d[j])/2;
        for(j=0;j<3;j++) mid[3][j]=(b[j]+c[j])/2;
        for(j=0;j<3;j++) mid[4][j]=(c[j]+d[j])/2;
        for(j=0;j<3;j++) mid[5][j]=(b[j]+d[j])/2;

        divide_tetra(a,mid[0],mid[1],mid[2],m-1);
        divide_tetra(mid[0],b,mid[3],mid[5],m-1);
        divide_tetra(mid[1],mid[3],c,mid[4],m-1);
        divide_tetra(mid[2],mid[5],mid[4],d,m-1);
    }
    else
        tetra( a, b, c, d);      //draw triangle at end of recursion//
}

void Gasket::draw()
{
    point vertices [] = {
        {0.0f,0.0f,1.0f},
        {0.0f,1.0f,0.0f},
        {-1.0f,-0.5f,0.0f},
        {1.0f,-0.5f,0.0f}
    };

    glBegin(GL_TRIANGLES);
        divide_tetra(&vertices[0].x, &vertices[1].x, &vertices[2].x, &vertices[3].x, SUBDIVISIONS);
    glEnd();
}
