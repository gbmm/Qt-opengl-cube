/*

    Qt OpenGL cube

    2018/01/13

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

*/

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
