#include "cube.h"
#include <qmath.h>

Cube::Cube()
{
  x=y=z=0;
  initColor();
}

Cube::Cube(GLfloat x1, GLfloat y1, GLfloat z1)
{
    x=x1;
    y=y1;
    z=z1;
}

void Cube::setXYZ(GLfloat x1, GLfloat y1, GLfloat z1)
{
    x=x1;
    y=y1;
    z=z1;
}

Cube *Cube::getCubeFromXYZ(GLfloat x1, GLfloat y1, GLfloat z1)
{
    if(qAbs(x1-x)<0.009 && qAbs(y1-y)<0.009 && qAbs(z1-z)<0.009)
    {
        return this;
    }
    else
    {
        return NULL;
    }
}

void Cube::copy(Cube *p)
{
    p->setXYZ(this->x,this->y,this->z);
    for(int i=0;i<6;i++)
    {
        p->color[i] = color[i];
    }
}

void Cube::initColor()
{
    color[0] = Cube::white;
    color[1] = Cube::yellow;
    color[2] = Cube::blue;
    color[3] = Cube::green;
    color[4] = Cube::orage;
    color[5] = Cube::red;
}

void Cube::setColor(Color c[])
{
    for(int i=0;i<6;i++)
    {
        color[i] = c[i];
    }
}

 const Cube::Color *Cube::getColor() const
{
    return color;
}

 //绕X轴旋转90度颜色变化顺序
 void Cube::rotateColor2Angle90X()
 {
     //green->white->blue->yellow
      Color tmp = color[3];
      color[3] = color[0];
      color[0] = color[2];
      color[2] = color[1];
      color[1] = tmp;
 }

 //绕Y轴旋转90度颜色变化顺序
 void Cube::rotateColor2Angle90Y()
 {
     //white->orange->yellow->red
      Color tmp = color[0];
      color[0] = color[4];
      color[4] = color[1];
      color[1] = color[5];
      color[5] = tmp;
 }

  //绕Z轴旋转90度颜色变化顺序
 void Cube::rotateColor2Angle90Z()
 {
    //green->orange->blue->red
     Color tmp = color[3];
     color[3] = color[4];
     color[4] = color[2];
     color[2] = color[5];
     color[5] = tmp;


 }

void Cube::exchange2Color(int index1, int index2)
{

    Color tmp;
    tmp  = color[index1];
    color[index1] = color[index2];
    color[index2] = tmp;
}


void Cube::setAngle(GLfloat angle1)
{
    angle = angle1;
}

void Cube::draw()
{

}
