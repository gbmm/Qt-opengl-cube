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
    autoBackCenter();
    while(!calAutoBack1())
    {
        if(autoBack1zthird())break;
        if(autoBack1xthird())break;
        if(autoBack1xfirst())break;
        if(autoBack1yfirst())break;
        if(autoBack1ythird())break;
        sleep(10);
    }
    autoBack1Last();

    //第二步
    autoBack2();
    //第三步
    autoBack3();

    autobackBtn->setText("finish");
    qDebug()<<"---------finish------------";
}

//第三步
bool MainWidget::autoBack3()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1,*cube2,*cube3;

    cube2 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zsecond);
    cube3 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zsecond);

FIRST:
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zthird);
    int i=0;
    while (i<4)
    {
       if(cube1->color[3] != Cube::yellow && cube1->color[1] != Cube::yellow)
       {
           i=0;
           if(cube1->color[3] == cube2->color[3])
           {
                if(cube1->color[1] == cube3->color[4])
                {
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateOneTime();
                }
                else
                {
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateOneTime();
                }
                continue;
           }
           rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
           rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
           rotateOneTime();
           continue;
       }
       rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
       rotateOneTime();
       i++;
    }

SECOND:
    i=0;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::yfirst,NeHeWidget::zsecond);
    while (i<4)
    {
       if(cube1->color[3] != cube2->color[3] || cube1->color[4] != cube3->color[4])
       {
           rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
           rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
           rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
           rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
           rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
           rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
           rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
           rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
           rotateOneTime();
           goto FIRST;
           break;
       }
       rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
       rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
       rotateOneTime();
       i++;
    }
    return true;

}

bool MainWidget::autoBack2()
{
    //0前 1后 2下 3上 4右 5左
    int i = 0;
    Cube *cube1,*cube2,*cube3;
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zsecond);
    cube3 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zsecond);

FIRST:
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::yfirst,NeHeWidget::zthird);
    i=0;
    while (i<4)
    {
        if(cube1->color[3]== Cube::white)
        {
            i=0;
            if(   (cube1->color[5] == cube2->color[5] && cube1->color[1] == cube3->color[3])
               || (cube1->color[5] == cube3->color[3] && cube1->color[1] == cube2->color[5]))
            {
                rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateOneTime();

                continue;
            }
            rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
            rotateOneTime();
        }
        else if(cube1->color[5]== Cube::white)
        {
            i=0;
            if(   (cube1->color[3] == cube2->color[5] && cube1->color[1] == cube3->color[3])
               || (cube1->color[3] == cube3->color[3] && cube1->color[1] == cube2->color[5]))
            {
                rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
                rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
                rotateOneTime();
                continue;
            }
            rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
            rotateOneTime();
        }
        else if(cube1->color[1]== Cube::white)//cube3 上
        {
            i=0;
            if(   (cube1->color[3] == cube2->color[5] && cube1->color[5] == cube3->color[3])
               || (cube1->color[3] == cube3->color[3] && cube1->color[5] == cube2->color[5]))
            {
                rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateOneTime();
            }
            rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
            rotateOneTime();
        }
        else
        {
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateOneTime();
        }
        sleep(10);
        i++;
    }

    i=0;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::yfirst,NeHeWidget::zfirst);
    while (i<4) {
       if(   (cube1->color[0]==Cube::white&&cube1->color[3] != cube3->color[3])
           || cube1->color[3]==Cube::white||cube1->color[5]==Cube::white )
       {
           rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
           rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
           rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
           rotateOneTime();
           goto FIRST;
           break;
       }
       rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
       rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
       rotateOneTime();
       i++;
    }

    return true;
}


bool MainWidget::calAutoBack1()
{
    int xy[] ={1,0, 0,1, -1,0, 0,-1};
    for(int i=0;i<8;i=i+2)
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

    if(cube1->color[5]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::ysecond,NeHeWidget::Y);
        rotateOneTime();
        return;
    }

    cube1 = cubeWidget->findCubeAll(1,0,0);//右面

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
    if(cube1->color[5]==Cube::white)
    {
        findCubebyXY(0,1);
        rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,0,-1);
    if(cube1->color[5]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
        goto FIRST;
    }
    if(calAutoBack1())return true;

    SECOND:
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,-1,0);
    if(cube1->color[5]==Cube::white)
    {
        findCubebyXY(0,-1);
        rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,0,1);
    if(cube1->color[5]==Cube::white)
    {
        findCubebyXY(-1,0);
        rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
        goto SECOND;
    }
    if(calAutoBack1())return true;
}

bool MainWidget::autoBack1xfirst()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,1,0);
    FIRST:
    if(cube1->color[4]==Cube::white)
    {
        findCubebyXY(0,1);
        rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,0,-1);
    if(cube1->color[4]==Cube::white)
    {
        rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
        rotateOneTime();
        goto FIRST;
    }
    if(calAutoBack1())return true;

    SECOND:
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,-1,0);
    if(cube1->color[4]==Cube::white)
    {
        findCubebyXY(0,-1);
        rotateAngle(90,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,0,1);
    if(cube1->color[4]==Cube::white)
    {
        findCubebyXY(1,0);
        rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
        rotateOneTime();
        goto SECOND;
    }
    if(calAutoBack1())return true;
}

bool MainWidget::autoBack1yfirst()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1 = cubeWidget->findCubeAll(1,NeHeWidget::yfirst,0);
    FIRST:
    if(cube1->color[3]==Cube::white)
    {
        findCubebyXY(1,0);
        rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,NeHeWidget::yfirst,-1);
    if(cube1->color[3]==Cube::white)
    {
        rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
        goto FIRST;
    }
    if(calAutoBack1())return true;

    SECOND:
    cube1 = cubeWidget->findCubeAll(-1,NeHeWidget::yfirst,0);
    if(cube1->color[3]==Cube::white)
    {
        findCubebyXY(-1,0);
        rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,NeHeWidget::yfirst,1);
    if(cube1->color[3]==Cube::white)
    {
        findCubebyXY(0,1);
        rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
        rotateOneTime();
        goto SECOND;
    }
    if(calAutoBack1())return true;
}

bool MainWidget::autoBack1ythird()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1 = cubeWidget->findCubeAll(1,NeHeWidget::ythird,0);
    FIRST:
    if(cube1->color[2]==Cube::white)
    {
        findCubebyXY(1,0);
        rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,NeHeWidget::ythird,-1);
    if(cube1->color[2]==Cube::white)
    {
        rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
        goto FIRST;
    }
    if(calAutoBack1())return true;

    SECOND:
    cube1 = cubeWidget->findCubeAll(-1,NeHeWidget::ythird,0);
    if(cube1->color[2]==Cube::white)
    {
        findCubebyXY(-1,0);
        rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
        rotateOneTime();
    }
    if(calAutoBack1())return true;

    cube1 = cubeWidget->findCubeAll(0,NeHeWidget::ythird,1);
    if(cube1->color[2]==Cube::white)
    {
        findCubebyXY(0,-1);
        rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
        rotateOneTime();
        goto SECOND;
    }
    if(calAutoBack1())return true;
}

bool MainWidget::autoBack1Last()
{
    //0前 1后 2下 3上 4右 5左
    //上
    Cube *cube1 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zthird);
    Cube *cube2 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zsecond);
    while (!(cube1->color[3] == cube2->color[3] && cube1->color[1]==Cube::white)) {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateOneTime();
    }
    rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
    rotateOneTime();
    //下
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zsecond);
    while (!(cube1->color[2] == cube2->color[2] && cube1->color[1]==Cube::white)) {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateOneTime();
    }
    rotateAngle(180,NeHeWidget::ythird,NeHeWidget::Y);
    rotateOneTime();
    //左
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zsecond);
    while (!(cube1->color[5] == cube2->color[5] && cube1->color[1]==Cube::white)) {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateOneTime();
    }
    rotateAngle(180,NeHeWidget::xthird,NeHeWidget::X);
    rotateOneTime();
    //右
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zsecond);
    while (!(cube1->color[4] == cube2->color[4] && cube1->color[1]==Cube::white)) {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateOneTime();
    }
    rotateAngle(180,NeHeWidget::xfirst,NeHeWidget::X);
    rotateOneTime();
    return true;
}

void MainWidget::onRandBtn()
{
    autobackBtn->setText("自动还原");
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
        delete &listRotateTask[0];
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
