#
#
#   Qt OpenGL Tutorial - Lesson 07
#
#   lesson07.pro
#   v 1.00
#   2002/12/19
#
#   Copyright (C) 2002 Cavendish
#                      cavendish@qiliang.net
#                      http://www.qiliang.net/nehe_qt
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#
TEMPLATE	= app
QT+=opengl
CONFIG		+= qt opengl warn_on
HEADERS		= nehewidget.h \
    cube.h
SOURCES		= nehewidget.cpp \
		  main.cpp \
    cube.cpp
TARGET		= lesson07
