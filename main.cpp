#include <qapplication.h>
#include <qmessagebox.h>

#include "nehewidget.h"

int main( int argc, char **argv )
{
  bool fs = false;

  QApplication a(argc,argv);

  NeHeWidget w( 0, 0, fs );
  w.show();
  
  return a.exec();
}
