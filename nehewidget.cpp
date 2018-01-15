#include <qimage.h>
#include "nehewidget.h"

NeHeWidget::NeHeWidget( QWidget* parent, const char* name, bool fs )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;
    zoom = -12.0;
    xSpeed = ySpeed = 0.0;
    filter = 0;
    light = false;
    fullscreen = fs;
    rotateAngle = 0;
    rotateangle = A_0;
    ROTATE_LAYER = zfirst;
    xyz = Z;
    xRoll = yRoll = zRoll = 0;

    setGeometry( 100, 100, 640, 480 );
    initCube();
    getLayerCubeZ(rotatecube,othercube,ROTATE_LAYER);

    timeLine = new QTimeLine(1500, this);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(rotateCube(int)));
    connect(timeLine,SIGNAL(finished()),this,SLOT(rotateCubeFinished()));
}

NeHeWidget::~NeHeWidget()
{
}

void NeHeWidget::initCube()
{
    int index=0;
    GLfloat z=0.5+0.5,x=-1.5+0.5,y=-1.5+0.5;
    for(int k=0;k<3;k++)
        for(int j=0;j<3;j++)
            for(int i=0;i<3;i++)
            {
                cube[index++].setXYZ(x+i,y+j,z-k);
            }
}

//绘制一个立方体 魔方共27个
void NeHeWidget::drawOneCube(Cube *b, GLfloat x, GLfloat y, GLfloat z)//
{
    x=x-0.5;
    y=y-0.5;
    z=z-0.5;
    const Cube::Color *color = b->getColor();
    glBindTexture( GL_TEXTURE_2D, texture[color[0]] );
    glBegin( GL_QUADS );
    //后面 黄色
    glNormal3f( x, y, z );
    glTexCoord2f( 0,   0  );glVertex3f(x,   y, z);
    glTexCoord2f( 0+1, 0  );glVertex3f(x+1, y, z);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x+1, y+1, z);
    glTexCoord2f( 0,   0+1);glVertex3f(x, y+1, z);

    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[color[1]] );
    glBegin( GL_QUADS );
    //前面 白色
    glNormal3f( x, y, z+1 );
    glTexCoord2f( 0,   0  );glVertex3f(x,   y,   z+1);
    glTexCoord2f( 0+1, 0  );glVertex3f(x+1, y,   z+1);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x+1,   y+1, z+1);
    glTexCoord2f( 0,   0+1);glVertex3f(x, y+1, z+1);

    glEnd();
    glBindTexture( GL_TEXTURE_2D, texture[color[2]] );
    glBegin( GL_QUADS );

    //下面 蓝色
    glNormal3f( x, y, z);
    glTexCoord2f( 0,   0  );glVertex3f(x,   y,   z);
    glTexCoord2f( 0+1, 0  );glVertex3f(x+1, y,   z);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x+1,   y, z+1);
    glTexCoord2f( 0,   0+1);glVertex3f(x, y, z+1);

    glEnd();
    glBindTexture( GL_TEXTURE_2D, texture[color[3]] );
    glBegin( GL_QUADS );

    //上面 绿色
    glNormal3f( x, y+1, z);
    glTexCoord2f( 0,   0  );glVertex3f(x,   y+1,   z);
    glTexCoord2f( 0+1, 0  );glVertex3f(x+1, y+1,   z);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x+1,   y+1, z+1);
    glTexCoord2f( 0,   0+1);glVertex3f(x, y+1, z+1);

    glEnd();
    glBindTexture( GL_TEXTURE_2D, texture[color[5]] );
    glBegin( GL_QUADS );

    //左面 橙色
    glNormal3f( x, y, z);
    glTexCoord2f( 0,   0  );glVertex3f(x,   y,   z);
    glTexCoord2f( 0+1, 0  );glVertex3f(x, y+1,   z);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x,   y+1, z+1);
    glTexCoord2f( 0,   0+1);glVertex3f(x, y, z+1);

    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[color[4]] );
    glBegin( GL_QUADS );

    //右面 红色
    glNormal3f( x+1, y, z);
    glTexCoord2f( 0,   0  );glVertex3f(x+1,   y,   z);
    glTexCoord2f( 0+1, 0  );glVertex3f(x+1, y+1,   z);
    glTexCoord2f( 0+1, 0+1);glVertex3f(x+1,   y+1, z+1);
    glTexCoord2f( 0,   0+1);glVertex3f(x+1, y, z+1);

    glEnd();

}

#define pi 3.141592653
//旋转完成后，计算哪些方块的颜色要变化
//参考http://blog.sina.com.cn/s/blog_3fd642cf0101cc8w.html
void NeHeWidget::rotateCubeFinished()
{
    double tmpcos = cos(rotateangle*pi/180)*100;
    double tmpsin = sin(rotateangle*pi/180)*100;
    tmpcos = qRound(tmpcos)/100;
    tmpsin = qRound(tmpsin)/100;
    //01 前后 45左右 23上下
    switch(xyz)
    {
    case X:
        for(int i=0;i<9;i++)
        {
            double y = rotatecube[i]->y*tmpcos + rotatecube[i]->z*tmpsin;
            double z = rotatecube[i]->z*tmpcos - rotatecube[i]->y*tmpsin;
            rotatecube[i]->setXYZ(rotatecube[i]->x,(GLfloat)y,(GLfloat)z);
            changeCubeColor(rotateangle,X,rotatecube[i]);
        };
        break;
    case Y:
        for(int i=0;i<9;i++)
        {
            double x = rotatecube[i]->x*tmpcos + rotatecube[i]->z*tmpsin;
            double z = rotatecube[i]->z*tmpcos - rotatecube[i]->x*tmpsin;
            rotatecube[i]->setXYZ((GLfloat)x,rotatecube[i]->y,(GLfloat)z);
            changeCubeColor(rotateangle,Y,rotatecube[i]);
        };
        break;
    case Z:
        for(int i=0;i<9;i++)
        {
            double x = rotatecube[i]->x*tmpcos + rotatecube[i]->y*tmpsin;
            double y = rotatecube[i]->y*tmpcos - rotatecube[i]->x*tmpsin;
            rotatecube[i]->setXYZ((GLfloat)x,(GLfloat)y,rotatecube[i]->z);
            changeCubeColor(rotateangle,Z,rotatecube[i]);
        };
        break;
    }
    rotateangle = A_0;
    rotateAngle = 0;
    //01 前后 45左右 23上下
}

//计算旋转颜色的变化
void NeHeWidget::changeCubeColor(ANGLE angle, XYZ xyz, Cube *cube)
{
    int index = angle/90;
    //每次计算旋转90度颜色变化，180则计算两次
    while (index-->0) {
        switch(xyz)
        {
            case X:
            cube->rotateColor2Angle90X();
            break;
            case Y:
            cube->rotateColor2Angle90Y();
            break;
            case Z:
            cube->rotateColor2Angle90Z();
            break;
        }
    }
}

//timeline 更新动画
void NeHeWidget::rotateCube(int angle)
{
    rotateAngle = angle;
    updateGL();
}

//获取layer绕Z轴要旋转的方块以及不动的方块 p旋转 other不旋转
void NeHeWidget::getLayerCubeZ(Cube **p,Cube **other, LAYER layer)
{
    xRoll = 0;
    yRoll = 0;
    zRoll = 1;
    int n1=0,n2=0;
    for(int i=0;i<27;i++)
    {
        if((int)cube[i].z == ((int)layer)){
            p[n1++]=&cube[i];

        }else{
            other[n2++]=&cube[i];
        }
    }
}

void NeHeWidget::getLayerCubeX(Cube **p,Cube **other, LAYER layer)
{
    xRoll = 1;
    yRoll = 0;
    zRoll = 0;
    int n1=0,n2=0;
    for(int i=0;i<27;i++)
    {
        qDebug()<<(int)cube[i].x<<layer;
        if((int)cube[i].x == ((int)layer)){
            p[n1++]=&cube[i];

        }else{
            other[n2++]=&cube[i];
        }
    }
    qDebug()<<n1<<n2;
}

void NeHeWidget::getLayerCubeY(Cube **p,Cube **other, LAYER layer)
{
    xRoll = 0;
    yRoll = 1;
    zRoll = 0;
    int n1=0,n2=0;
    for(int i=0;i<27;i++)
    {
        if((int)cube[i].y == ((int)layer)){
            p[n1++]=&cube[i];

        }else{
            other[n2++]=&cube[i];
        }
    }
}

void NeHeWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glTranslatef(  0.0,  0.0, zoom );

    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );

    //glBindTexture( GL_TEXTURE_2D, texture[filter] );

    // glBegin( GL_QUADS );

    //绘制坐标轴，方便调试
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,-6,0);
    glColor3f(1,0,0); glVertex3f(0,6,0);

    glColor3f(0,1,0); glVertex3f(-6,0,0);
    glColor3f(0,1,0); glVertex3f(6,0,0);

    glColor3f(0,0,1); glVertex3f(0,0,-6);
    glColor3f(0,0,1); glVertex3f(0,0,6);

    glEnd();

    glColor3f(1.0f,1.0f,1.0f);

   // getLayerCubeZ(rotatecube,othercube,ROTATE_LAYER);

    //绘制旋转要动的方块
    glRotatef( rotateAngle,  xRoll,  yRoll,  zRoll );
    for(int i=0;i<9;i++)
    {
        drawOneCube(rotatecube[i],rotatecube[i]->x,rotatecube[i]->y,rotatecube[i]->z);
    }
    glRotatef( -rotateAngle,  xRoll,  yRoll,  zRoll );

    //绘制不用旋转的方块
    for(int i=0;i<18;i++)
    {
        drawOneCube(othercube[i],othercube[i]->x,othercube[i]->y,othercube[i]->z);
    }
    //glEnd();

    //xRot += xSpeed;
    yRot += ySpeed;
}

void NeHeWidget::initializeGL()
{
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    /*glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );*/

    glEnable( GL_LIGHT1 );
}

void NeHeWidget::resizeGL( int width, int height )
{
    if ( height == 0 )
    {
        height = 1;
    }
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

//绕t坐标轴的layer层旋转angle度
void NeHeWidget::rotateCubef(ANGLE angle, LAYER layer, XYZ t)
{
    xyz=t;
    rotateangle = angle;
    ROTATE_LAYER = layer;
    switch (t) {
    case X:
        getLayerCubeX(rotatecube,othercube,ROTATE_LAYER);
        break;
    case Y:
        getLayerCubeY(rotatecube,othercube,ROTATE_LAYER);
        break;
    case Z:
        getLayerCubeZ(rotatecube,othercube,ROTATE_LAYER);
        break;
    default:
        break;
    }
    timeLine->setFrameRange(0, angle);
    timeLine->start();
}

//键盘的控制
void NeHeWidget::keyPressEvent( QKeyEvent *e )
{
    int angle = 0;
    switch ( e->key() )
    {
    /*-----------绕Z轴旋转----------*/
    case Qt::Key_W:
        rotateCubef(A_90,zfirst,Z);
        break;
    case Qt::Key_E:
        rotateCubef(A_90,zsecond,Z);
        break;
    case Qt::Key_R:
        rotateCubef(A_90,zthird,Z);
        break;
    /*-----------绕X轴旋转----------*/
    case Qt::Key_S:
        rotateCubef(A_90,xfirst,X);
        break;
    case Qt::Key_D:
        rotateCubef(A_90,xsecond,X);
        break;
    case Qt::Key_F:
        rotateCubef(A_90,xthird,X);
        break;
    /*-----------绕Y轴旋转----------*/
    case Qt::Key_X:
        rotateCubef(A_90,yfirst,Y);
        break;
    case Qt::Key_C:
        rotateCubef(A_90,ysecond,Y);
        break;
    case Qt::Key_V:
        rotateCubef(A_90,ythird,Y);
        break;

    /*---------------------------*/
    case Qt::Key_Minus:
        zoom -= 0.2;
        updateGL();
        break;
    case Qt::Key_Plus:
        zoom += 0.2;
        updateGL();
        break;
    case Qt::Key_Up:
        //rotateangle = 0;
        ySpeed -= 0.01;
        yRot += ySpeed;
        updateGL();
        break;
    case Qt::Key_Down:
        ySpeed += 0.01;
        yRot -= ySpeed;
        updateGL();
        break;
    case Qt::Key_Right:
        xSpeed += 0.01;
        xRot += xSpeed;
        updateGL();
        break;
    case Qt::Key_Left:
        xSpeed -= 0.01;
        xRot += xSpeed;
        updateGL();
        break;
    case Qt::Key_Escape:
        close();
    }
}

void NeHeWidget::loadGLTextures()
{
    for(int i=0;i<6;i++)
    {
        glGenTextures( 1, &texture[i] );
        QImage tex, buf;
        buf.load( "./data/"+QString::number(i+1)+".jpg");
        tex = QGLWidget::convertToGLFormat( buf );

        glBindTexture( GL_TEXTURE_2D, texture[i] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    }
}
