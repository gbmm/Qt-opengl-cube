#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->resize(640,530);
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

    QLabel *label = new QLabel(this);
    label->setGeometry(0,0,200,50);
    label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
    label->setText("x 绿色; y 红色; z 蓝色");
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
    randTimer.start(2000);
}

void MainWidget::onRandTimer()
{
    if (listRotateTask.size()>0) {
        cubeWidget->rotateCubef(listRotateTask[0].angle,listRotateTask[0].layer,listRotateTask[0].xyz);
        listRotateTask.pop_front();
    }else{
        randTimer.stop();
        randBtn->setText("随机旋转");
    }
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
   cubeWidget->keyPressEvent(e);
}
