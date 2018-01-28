#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "nehewidget.h"
#include <QTime>
#include <QTimer>
#include <QList>
#include <QLabel>
#include <QThread>

namespace Ui {
class MainWidget;
}

struct RotateTask{
    NeHeWidget::LAYER layer;
    NeHeWidget::XYZ   xyz;
    NeHeWidget::ANGLE angle;
} ;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void initUI();
    void rotateAngle(int angle,NeHeWidget::LAYER layer,NeHeWidget::XYZ xyz);
    //还原第一步 黄色面的白色十字
    bool  autoBack1zthird();
    bool  autoBack1xthird();
    bool  autoBack1xfirst();
    bool  autoBack1yfirst();
    bool  autoBack1ythird();
    bool  autoBack1Last();
    void  autoBackCenter();
    Cube *findCubebyXY(int x,int y);
    bool  calAutoBack1();

    bool autoBack2();
    bool autoBack3();
protected:
    void keyPressEvent( QKeyEvent *e );
    void closeEvent(QCloseEvent *);
public slots:
    //随机旋转操作
    void onRandBtn();
    //随机旋转定时器函数
    void onRandTimer();
    //一次性跑完动作
    void rotateOneTime();
    //自动还原函数
    void onAutobackBtn();
private:
    Ui::MainWidget *ui;
    //opengl widget
    NeHeWidget   *cubeWidget;
    //随机旋转按钮
    QPushButton  *randBtn;
    //随机旋转时的定时器
    QTimer randTimer;
    //随机旋转的所有操作任务
    QList<RotateTask> listRotateTask;
    //自动还原按钮
    QPushButton *autobackBtn;
    //动画停止
    bool bStopAnimation;
};

#endif // MAINWIDGET_H
