#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H

#include <qgl.h>
#include<QKeyEvent>
#include<QDebug>
#include<QTimeLine>
#include<math.h>
#include "cube.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QPushButton>

class NeHeWidget : public QGLWidget
{
    Q_OBJECT

public:
  NeHeWidget( QWidget* parent = 0, const char* name = 0, bool fs = false );
  ~NeHeWidget();
  //表示坐标轴上的每一层，比如xfirt表示x轴上第一层
  enum LAYER{zfirst=1,zsecond=0,zthird=-1,
            xfirst=1,xsecond=0,xthird=-1,
            yfirst=1,ysecond=0,ythird=-1,};

 enum XYZ{X,Y,Z};//x y z 坐标轴
 enum ANGLE{A_0=0,A_90=90,A_180=180,A_270=270,A_360=360};//旋转的角度

protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);
  void loadGLTextures();
public:
  void initCube();
  void changeCubeColor(ANGLE angle,XYZ xyz,Cube *);
  void drawOneCube(Cube *,GLfloat x,GLfloat y,GLfloat z);

  //获取要旋转的方块，p要旋转的9个方块 other不旋转的18个方块
  void getLayerCubeZ(Cube **p,Cube **other, LAYER layer);
  void getLayerCubeX(Cube **p,Cube **other, LAYER layer);
  void getLayerCubeY(Cube **p,Cube **other, LAYER layer);

  //随机打乱
  void randRotateCube();
  void keyPressEvent( QKeyEvent *e );

  //根据坐标查找cube
  Cube *findCube(GLfloat x,GLfloat y,GLfloat z);

public slots:
  void rotateCube(int);
  void rotateCubef(ANGLE angle, LAYER layer, XYZ t);//绕轴旋转 角度 层
  void rotateCubeFinished();

protected:
  Cube cube[27];
  bool fullscreen;
  GLfloat xRot, yRot, zRot;//旋转的角度
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
  //鼠标操作
  QPoint pointStart;//鼠标移动开始与起始值
  bool bMousePress;//是否按下鼠标
  //旋转完毕 方块颜色临时存储
  Cube tmprotatecube[9];
};

#endif//NEHEWIDGET_H
