#ifndef CUBE_H
#define CUBE_H

#include <qgl.h>
#include <QDebug>

class Cube
{
public:
    enum Color{white,yellow,blue,green,orage,red};

public:
    Cube();
    Cube(GLfloat x,GLfloat y,GLfloat z);
    void initColor();
    void setColor(Color color[]);
    const Color* getColor()const;
    void draw();
    void setXYZ(GLfloat x, GLfloat y,GLfloat z);
    Cube *getCubeFromXYZ(GLfloat x,GLfloat y,GLfloat z);
    void copy(Cube *);
    void setAngle(GLfloat angle);
    void exchange2Color(int index1,int index2);
    void rotateColor2Angle90X();
    void rotateColor2Angle90Y();
    void rotateColor2Angle90Z();
public:
    GLfloat x,y,z;
    GLfloat angle;
    Color color[6];
};

#endif // CUBE_H
