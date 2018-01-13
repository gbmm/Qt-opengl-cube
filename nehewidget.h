/*

    Qt OpenGL Tutorial - Lesson 07

    nehewidget.h
    v 1.00
    2002/12/19

    Copyright (C) 2002 Cavendish
                       cavendish@qiliang.net
                       http://www.qiliang.net/nehe_qt

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

*/

#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H

#include <qgl.h>
#include<QKeyEvent>
#include<QDebug>
#include<QTimeLine>
#include<math.h>
#include "cube.h"

class NeHeWidget : public QGLWidget
{
    Q_OBJECT

public:

  NeHeWidget( QWidget* parent = 0, const char* name = 0, bool fs = false );
  ~NeHeWidget();
  //表示坐标轴上的每一层，比如xfirt表示x轴上第一层
  enum LAYER{zfirst=1,zsecond=0,zthird=-1,none=-2,
            xfirst=1,xsecond=0,xthird=-1,
            yfirst=1,ysecond=0,ythird=-1,};

 enum XYZ{X,Y,Z};//x y z 坐标轴
 enum ANGLE{A_0=0,A_90=90,A_180=180,A_270=270,A_360=360};//旋转的角度
protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
  
  void keyPressEvent( QKeyEvent *e );
  void loadGLTextures();
public:
  void initCube();
  void changeCubeColor(ANGLE angle,XYZ xyz,Cube *);
public:
  void drawOneCube(Cube *,GLfloat x,GLfloat y,GLfloat z);

  //获取要旋转的方块，p要旋转的9个方块 other不旋转的18个方块
  void getLayerCubeZ(Cube **p,Cube **other, LAYER layer);
  void getLayerCubeX(Cube **p,Cube **other, LAYER layer);
  void getLayerCubeY(Cube **p,Cube **other, LAYER layer);
public slots:
  void rotateCube(int);
  void rotateCubef(ANGLE angle, LAYER layer, XYZ t);//绕轴旋转 角度 层
  void rotateCubeFinished();
protected:
  Cube cube[27];
  bool fullscreen;
  GLfloat xRot, yRot, zRot;
  GLfloat zoom;
  GLfloat xSpeed, ySpeed;
  GLuint texture[6];
  GLuint filter;
  bool light;
  int rotateAngle;
  ANGLE rotateangle;//旋转的角度
  int totalRoateAngle;
  QTimeLine *timeLine;
  LAYER ROTATE_LAYER;
  XYZ xyz;
  GLfloat xRoll,yRoll,zRoll;//绕某一坐标轴旋转 比如xRoll=1时绕x轴旋转
  Cube *rotatecube[9],*othercube[18];
};

#endif//NEHEWIDGET_H
