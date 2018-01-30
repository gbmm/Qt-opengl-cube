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
    //第四部
    autoBack4();
    //第五步
    autoBack5();
    //第六步
    autoBack6();
    //第七步
    autoBack7();
    autobackBtn->setText("finish");
    qDebug()<<"---------finish------------";
}

//第七步
int MainWidget::getSameColorSide7(Cube *p1, Cube *p2, Cube *p3, Cube *p4,
                                  Cube *p11, Cube *p22, Cube *p33, Cube *p44)
{
    int num=0;
    if(p1->color[2] == p11->color[2])num++;
    if(p2->color[4] == p22->color[4])num++;
    if(p3->color[3] == p33->color[3])num++;
    if(p4->color[5] == p44->color[5])num++;
    return num;
}

void MainWidget::test1()
{
    Cube* cube5 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ythird,NeHeWidget::zthird);
    rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
    rotateOneTime();

    while(cube5->color[1] != Cube::yellow)
    {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
        rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
        rotateOneTime();
    }


    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
    rotateOneTime();
}

void MainWidget::test2()
{
    Cube* cube5 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ythird,NeHeWidget::zthird);
    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
    rotateOneTime();

    while(cube5->color[1] != Cube::yellow)
    {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
        rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
        rotateOneTime();
    }


    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
    rotateOneTime();
}

bool MainWidget::autoBack7()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1,*cube2,*cube3,*cube4;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zthird);
    cube3 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zthird);
    cube4 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zthird);

    Cube *cube11,*cube22,*cube33,*cube44;
    cube11 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zsecond);
    cube22 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zsecond);
    cube33 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zsecond);
    cube44 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zsecond);

    Cube *r1,*r2;
    r1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::yfirst,NeHeWidget::zthird);
    r2 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::yfirst,NeHeWidget::zthird);

    Cube* cube5 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ythird,NeHeWidget::zthird);
    while(true)
    {
        int num = getSameColorSide7(cube1,cube2,cube3,cube4,
                                    cube11,cube22,cube33,cube44);
        //qDebug()<<"-----7-----"<<num;
        if(num==4 )break;
        if(num==1)
        {
            if(cube3->color[3] == cube33->color[3] && r1->color[3]==r2->color[3] && r1->color[3]==cube3->color[3])
            {

                if(cube1->color[2] == cube22->color[4])
                {
                    rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xthird,NeHeWidget::X);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xthird,NeHeWidget::X);
                    rotateOneTime();

                    while(cube5->color[1] != Cube::yellow)
                    {
                        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                        rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
                        rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
                        rotateOneTime();
                    }

                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateOneTime();
                }
                else
                {
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateOneTime();

                    while(cube5->color[1] != Cube::yellow)
                    {
                        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                        rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
                        rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
                        rotateOneTime();
                    }

                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateOneTime();
                }
                continue;
            }
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
            rotateOneTime();
        }
        else if(num==0)
        {
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
            rotateOneTime();

            while(cube5->color[1] != Cube::yellow)
            {
                rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
                rotateOneTime();
            }

            rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
            rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateOneTime();
        }
    }
}

//第六步
int MainWidget::getSameColorSide(Cube *p1, Cube *p2, Cube *p3, Cube *p4)
{
    //0前 1后 2下 3上 4右 5左
    int num = 0;
    if(p1->color[2] == p2->color[2]){num++;}
    if(p2->color[4] == p3->color[4]){num++;}
    if(p3->color[3] == p4->color[3]){num++;}
    if(p1->color[4] == p4->color[4]){num++;}
    return num;
}

bool MainWidget::autoBack6()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1,*cube2,*cube3,*cube4;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ythird,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ythird,NeHeWidget::zthird);
    cube3 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::yfirst,NeHeWidget::zthird);
    cube4 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::yfirst,NeHeWidget::zthird);

    Cube *cube5,*cube6,*cube7,*cube8;
    cube5 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zsecond);
    cube6 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zsecond);
    cube7 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zsecond);
    cube8 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zsecond);
    while (true)
    {
        int num = getSameColorSide(cube1,cube2,cube3,cube4);
        if(num==4)break;
        if(num==1)
        {
            if(cube2->color[4] == cube3->color[4] &&  cube3->color[4] == cube6->color[4])
            {
                rotateAngle(180,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
                rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(90,NeHeWidget::ythird,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateOneTime();
                continue;
            }
            if(cube2->color[4] != cube3->color[4] )
            {
                rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                rotateOneTime();
                continue;
            }
            if(cube3->color[4] != cube6->color[4])
            {
                rotateAngle(90,NeHeWidget::zsecond,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::zfirst,NeHeWidget::Z);
                rotateOneTime();
                continue;
            }
            /*else if(cube3->color[3] == cube4->color[3] && cube4->color[3]==cube7->color[3])
            {
                rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(180,NeHeWidget::xthird,NeHeWidget::X);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(180,NeHeWidget::xthird,NeHeWidget::X);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateOneTime();
                continue;
            }
            else if(cube4->color[5] == cube1->color[5] && cube4->color[5]==cube8->color[5])
            {

            }
            else if(cube1->color[2] == cube1->color[2] && cube1->color[2]==cube5->color[2])
            {

            }*/

        }
        else if(num ==0 )
        {
            rotateAngle(180,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(270,NeHeWidget::ythird,NeHeWidget::Y);
            rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(180,NeHeWidget::yfirst,NeHeWidget::Y);
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateAngle(90,NeHeWidget::ythird,NeHeWidget::Y);
            rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
            rotateOneTime();
            continue;
        }
    }

    while(!(cube2->color[4] == cube3->color[4] &&  cube3->color[4] == cube6->color[4]))
    {
        rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
        rotateOneTime();
    }
    return true;
}

//第五步

int MainWidget::getYellowNum(Cube *p1, Cube *p2, Cube *p3, Cube *p4)
{
    //0前 1后 2下 3上 4右 5左
    int num  = 0;
    if(p1->color[1]==Cube::yellow)num=num+1;
    if(p2->color[1]==Cube::yellow)num=num+1;
    if(p3->color[1]==Cube::yellow)num=num+1;
    if(p4->color[1]==Cube::yellow)num=num+1;
    return num;
}

bool MainWidget::autoBack5()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1,*cube2,*cube3,*cube4;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ythird,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ythird,NeHeWidget::zthird);
    cube3 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::yfirst,NeHeWidget::zthird);
    cube4 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::yfirst,NeHeWidget::zthird);
    while (true)
    {
       int num = getYellowNum(cube1,cube2,cube3,cube4);
       if(num==4)return true;
       if(num==1)
       {
            if(cube1->color[1]==Cube::yellow)
            {
                if(cube2->color[2]==Cube::yellow)//小鱼1
                {
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                    rotateOneTime();
                }
                else//小鱼2
                {
                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                    rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                    rotateOneTime();
                }
                continue;
            }
            rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
            rotateOneTime();
            continue;
       }
       else if(num==2 )
       {
             if(cube1->color[1]!=Cube::yellow && cube1->color[2]==Cube::yellow)
             {
                 rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                 rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                 rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                 rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                 rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                 rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
                 rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                 rotateOneTime();
                  continue;
             }
             rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
             rotateOneTime();
             continue;
       }
       else if(num==0)
       {
           if(cube1->color[1]!=Cube::yellow && cube1->color[5]==Cube::yellow)
           {
               rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
               rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
               rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
               rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
               rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
               rotateAngle(180,NeHeWidget::zthird,NeHeWidget::Z);
               rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
               rotateOneTime();
               continue;
           }
           rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
           rotateOneTime();
           continue;
       }
    }
    return true;
}

//第四步
bool MainWidget::autoBack4()
{
    //0前 1后 2下 3上 4右 5左
    Cube *cube1,*cube2,*cube3,*cube4,*cube5;
    cube1 = cubeWidget->findCubeAll(NeHeWidget::xfirst,NeHeWidget::ysecond,NeHeWidget::zthird);
    cube2 = cubeWidget->findCubeAll(NeHeWidget::xthird,NeHeWidget::ysecond,NeHeWidget::zthird);

    cube3 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ysecond,NeHeWidget::zsecond);

    cube4 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::yfirst,NeHeWidget::zthird);
    cube5 = cubeWidget->findCubeAll(NeHeWidget::xsecond,NeHeWidget::ythird,NeHeWidget::zthird);

    while ( !(  cube1->color[1]==Cube::yellow
           &&cube2->color[1]==Cube::yellow
           &&cube3->color[1]==Cube::yellow
           &&cube4->color[1]==Cube::yellow
           &&cube5->color[1]==Cube::yellow
              )
           )
    {
            if( (cube1->color[1] == Cube::yellow && cube2->color[1]==Cube::yellow)
                ||(cube1->color[1] == Cube::yellow && cube4->color[1]==Cube::yellow)
                ||(cube1->color[1]!=Cube::yellow
                   &&cube2->color[1]!=Cube::yellow
                   &&cube4->color[1]!=Cube::yellow
                   &&cube5->color[1]!=Cube::yellow)    )
            {
                rotateAngle(270,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateAngle(270,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::xfirst,NeHeWidget::X);
                rotateAngle(270,NeHeWidget::zthird,NeHeWidget::Z);
                rotateAngle(90,NeHeWidget::yfirst,NeHeWidget::Y);
                rotateOneTime();
                continue;
            }
            else// if(cube4->color[1] == Cube::yellow && cube5->color[1]==Cube::yellow)
            {
                rotateAngle(90,NeHeWidget::zthird,NeHeWidget::Z);
                rotateOneTime();
                continue;
            }

    }

    return true;
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
    randTimer.start(200);
}

void MainWidget::rotateOneTime()
{
    while (listRotateTask.size()>0) {
        cubeWidget->rotateCubef(listRotateTask[0].angle,listRotateTask[0].layer,listRotateTask[0].xyz);
        delete &listRotateTask[0];
        listRotateTask.pop_front();
        sleep(200);
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
    if(e->key()==Qt::Key_Q)
    {
        test1();
    }else if(e->key()==Qt::Key_A)
    {
        test2();
    }
    cubeWidget->keyPressEvent(e);
}

void MainWidget::closeEvent(QCloseEvent *e)
{
    exit(0);
}
