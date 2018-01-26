#include "mainwidget.h"
#include "ui_mainwidget.h"

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->resize(640,530);
    bStopAnimation = true;
    initUI();
    connect(&randTimer,SIGNAL(timeout()),this,SLOT(onRandTimer()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initUI()
{
    cubeWidget = new NeHeWidget(this,0, false);
    cubeWidget->setGeometry(0,50,640,480);

    randBtn = new QPushButton(this);
    randBtn->setGeometry(this->width()-100,0,100,50);
    randBtn->setText("随机旋转");
    connect(randBtn,SIGNAL(clicked()),this,SLOT(onRandBtn()));

    autobackBtn = new QPushButton(this);
    autobackBtn->setGeometry(this->width()-250,0,100,50);
    autobackBtn->setText("自动还原");
    connect(autobackBtn,SIGNAL(clicked()),this,SLOT(onAutobackBtn()));

    QLabel *label = new QLabel(this);
    label->setGeometry(0,0,200,50);
    label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
    label->setText("x 绿色; y 红色; z 蓝色");
}

void MainWidget::rotateAngle(int angle,NeHeWidget::LAYER layer,NeHeWidget::XYZ xyz)
{
    int i = angle/90;
    while (i-->0) {
        RotateTask rt={layer,xyz,NeHeWidget::ANGLE(90)};
        listRotateTask.append(rt);
    }
}

void MainWidget::onAutobackBtn()
{
   // autoBackCenter();

    while(!calAutoBack1())
    {
        if(autoBack1zthird())break;
        sleep(10);
    }
    autobackBtn->setText("finish");
    qDebug()<<"---------finish------------";
}

bool MainWidget::calAutoBack1()
{
    int xy[] ={1,0, 0,1, -1,0, 0,-1};
    for(int i=0;i<4;i=i+2)
    {
        Cube *cube1 = cubeWidget->findCubeAll(xy[i],xy[i+1],NeHeWidget::zthird);
        if(cube1->color[1]!=Cube::white)return false;
    }
    return true;
}

//黄色面的白色十字

void MainWidget::autoBackCenter()
{
    Cube *cube1=NULL;
    //0前 1后 2下 3上 4右 5左
    cube1 = cubeWidget->findCubeAll(0,0,1);//前面
    if(cube1->color[0]==Cube::white)return;
    cube1 = cubeWidget->findCubeAll(0,0,-1);//后面
    if(cube1->color[1]==Cube::white)
    {
        rotateAngle(180,NeHeWidget::ysecond,NeHeWidget::Y);
        rotateOneTime();
        return;
    }
    cube1 = cubeWidget->findCubeAll(-1,0,0);//左面
    qDebug()<<cube1->color[5];
    if(cube1->color[5]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::ysecond,NeHeWidget::Y);
        rotateOneTime();
        return;
    }

    cube1 = cubeWidget->findCubeAll(1,0,0);//右面
    qDebug()<<cube1->color[4];
    if(cube1->color[4]==Cube::white)
    {
        rotateAngle(270,NeHeWidget::ysecond,NeHeWidget::Y);
        rotateOneTime();
        return;
    }
    cube1 = cubeWidget->findCubeAll(0,1,0);//上面
    if(cube1->color[3]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::xsecond,NeHeWidget::X);
        rotateOneTime();
        return;
    }
    cube1 = cubeWidget->findCubeAll(0,-1,0);//下面
    if(cube1->color[2]==Cube::white)
    {
        rotateAngle(270,NeHeWidget::xsecond,NeHeWidget::X);
        rotateOneTime();
        return;
    }
}

Cube *MainWidget::findCubebyXY(int x,int y)
{
    int i=0;
    while (i++<4) {
        Cube *cube1 = cubeWidget->findCubeAll((GLfloat)x,(GLfloat)y,NeHeWidget::zthird);
        if(cube1->color[1]==Cube::white)
        {
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateOneTime();
            continue;
        }
        return cube1;
    }
    return NULL;
}

bool MainWidget::autoBack1zthird()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1 = cubeWidget->findCubeAll(-1,0,NeHeWidget::zfirst);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(-1,0);
        rotateAngle(180,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,-1,NeHeWidget::zfirst);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(0,-1);
        rotateAngle(180,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(1,0,NeHeWidget::zfirst);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(1,0);
        rotateAngle(180,NeHeWidget::xfirst,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,1,NeHeWidget::zfirst);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(0,1);
        rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

}

bool MainWidget::autoBack1xthird()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,1,0);
    FIRST:
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(0,1);
        rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,0,-1);
    if(cube1->color[0]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
        goto FIRST;
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,-1,0);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(0,-1);
        rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,0,1);
    if(cube1->color[0]==Cube::white)
    {
        findCubebyXY(-1,0);
        rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;
}

void MainWidget::autoBack1xfirst()
{
    int i = 0;
    //0前 1后 2下 3上 4右 5左
    while(i<4)
    {
        Cube *cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,1,0);
        //qDebug()<<"x third "<<cube1->color[4];
        if(cube1->color[5]!=Cube::white)
        {
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateOneTime();
        }
        else
        {
             rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
             rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
             rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
             rotateAngle(180,NeHeWidget::xthird,NeHeWidget::X);
             rotateOneTime();
             if(calAutoBack1()==NULL)return;
             i=0;
        }
        i++;
    }
}

void MainWidget::autoBack1yfirst()
{
    int i = 0;
    //0前 1后 2下 3上 4右 5左
    while(i<4)
    {
        Cube *cube1 = cubeWidget->findCubeAll(-1,NeHeWidget::yfirst,0);
        //qDebug()<<"y first "<<cube1->color[3];
        if(cube1->color[3]!=Cube::white )
        {
            rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateOneTime();
           // break;
        }
        else
        {
             rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
             rotateOneTime();
             if(calAutoBack1()==NULL)return;
             i=0;
        }
        i++;
    }
}

void MainWidget::autoBack1ythird()
{
    int i = 0;
    //0前 1后 2下 3上 4右 5左
    while(i<4)
    {
        Cube *cube1 = cubeWidget->findCubeAll(-1,NeHeWidget::ythird,0);
        //qDebug()<<"y third "<<cube1->color[2];
        if(cube1->color[2]!=Cube::white )
        {
            rotateAngle(90,NeHeWidget::ythird,NeHeWidget::Y);
            rotateOneTime();
            //break;
        }
        else
        {
             rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
             rotateOneTime();
             if(calAutoBack1()==NULL)return;
             i=0;
        }
        i++;
    }
}

void MainWidget::onRandBtn()
{
    randBtn->setText("旋转中...");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<10; i++)
    {
        int layer = qrand()%3-1;
        int xyz   = qrand()%3;
        int angle = 90;
        RotateTask rt;
        rt.angle = NeHeWidget::ANGLE(angle);
        rt.layer = NeHeWidget::LAYER(layer);
        rt.xyz   = NeHeWidget::XYZ(xyz);
        listRotateTask.append(rt);
    }
    randTimer.start(800);
}

void MainWidget::rotateOneTime()
{
    while (listRotateTask.size()>0) {
        cubeWidget->rotateCubef(listRotateTask[0].angle,listRotateTask[0].layer,listRotateTask[0].xyz);
        listRotateTask.pop_front();
         sleep(500);
    }
}

void MainWidget::onRandTimer()
{
    if (listRotateTask.size()>0) {
        cubeWidget->rotateCubef(listRotateTask[0].angle,listRotateTask[0].layer,listRotateTask[0].xyz);
        listRotateTask.pop_front();
    }else{
        randTimer.stop();
        randBtn->setText("随机旋转");
        bStopAnimation = true;
    }
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->text().isEmpty())return;
    cubeWidget->keyPressEvent(e);
}

void MainWidget::closeEvent(QCloseEvent *e)
{
    exit(0);
}
