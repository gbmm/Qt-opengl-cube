#include <qapplication.h>
#include <qmessagebox.h>
#include <QTextCodec>
//#include "nehewidget.h"
#include "mainwidget.h"

int main( int argc, char **argv )
{
    QApplication a(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));

    /*QWidget *widget = new QWidget();
    widget->setGeometry(100,100,800,800);

    NeHeWidget w( widget, 0, false );
    widget->show();*/

    MainWidget w;
    w.show();
    return a.exec();
}
